#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
	All function documentation is located in myheader.h
*/

struct Hunter* create_hunter(const char* name, int id, struct Room* starting_room, enum EvidenceType device, struct CaseFile* casefile) {
	struct Hunter* hunter = malloc(sizeof(struct Hunter));	//Allocate memory for new hunter
	if (hunter == NULL) {
		return NULL;
	}
	
	//Initialize fields
	strncpy(hunter->name, name, MAX_HUNTER_NAME - 1);
	hunter->name[MAX_HUNTER_NAME - 1] = '\0';	//Ensure null termination
	hunter->id = id;
	hunter->fear = 0;
	hunter->boredom = 0;
	hunter->exited = false;				//Starting in van, but haven't "exited"
	hunter->current_room = starting_room;
	hunter->path = NULL;
	roomstack_push(&hunter->path, starting_room);	//Star with the van on the stack
	hunter->device = device;
	hunter->casefile = casefile;
	hunter->returning_to_van = false;
	hunter->log_reason = LR_BORED;			//Default log reason
	
	//Log the creation of the hunter
	log_hunter_init(id, starting_room->name, name, device);
	
	return hunter;
}

void hunter_take_turn(struct Hunter* hunter) {
	//Ghost check + update stats
	sem_wait(&hunter->current_room->mutex);
	bool ghost_present = (hunter->current_room->ghost != NULL);
	sem_post(&hunter->current_room->mutex);
	
	if (ghost_present) {	//Ghost is in the room
		hunter->boredom = 0;		
		hunter->fear++;			
	} else {		
		hunter->boredom++;		
	}
	
	//Condition check
	if (hunter->boredom > ENTITY_BOREDOM_MAX || hunter->fear > HUNTER_FEAR_MAX) {
		hunter->exited = true;	//Hunter has left the simulation
		
		if (hunter->boredom > ENTITY_BOREDOM_MAX) {	//Log reason is boredom
			hunter->log_reason = LR_BORED;
		} else {					//Log reason is fear
			hunter->log_reason = LR_AFRAID;
		}
		
		log_exit(hunter->id, hunter->boredom, hunter->fear, hunter->current_room->name, hunter->device, hunter->log_reason); //Log the exit
		
		room_remove_hunter(hunter->current_room, hunter);	//Remove hunter from room
		
		hunter->current_room = NULL;				//Clear the hunter's room pointer
		roomstack_cleanup(&hunter->path);
		
		return;	//End turn
	}
	
	//Exit room check
	if (hunter->current_room->is_exit) {		//Hunter is in the exit room
		if (hunter->returning_to_van) {
		
			sem_wait(&hunter->casefile->mutex);
			bool has_enough_evidence = (evidence_is_valid_ghost(hunter->casefile->collected));
			sem_post(&hunter->casefile->mutex);
			
			if (has_enough_evidence) { //Hunter is returning because they've collected enough evidence to identify the ghost type
				hunter->log_reason = LR_EVIDENCE;
				hunter->exited = true;
				
				log_exit(hunter->id, hunter->boredom, hunter->fear, hunter->current_room->name, hunter->device, hunter->log_reason); //Log the exit
				
				room_remove_hunter(hunter->current_room, hunter);
				hunter->current_room = NULL;
				roomstack_cleanup(&hunter->path);
				
				return;	//End turn
				
			} else { //Swap device
			
				const enum EvidenceType* all_evidence;
				int count = get_all_evidence_types(&all_evidence);	//Get all possible evidence types
				
				enum EvidenceType old_device = hunter->device;		//Track the old device
				enum EvidenceType new_device;
				
				do {
					new_device = all_evidence[rand_int_threadsafe(0, count)];
				} while (new_device == old_device);			//Ensure new device is different
				
				hunter->device = new_device;				//Update the hunter's device
				hunter->returning_to_van = false; 			//Update flag
				
				//Reset the stack
				roomstack_cleanup(&hunter->path);
				hunter->path = NULL;
				roomstack_push(&hunter->path, hunter->current_room);	//Push van back onto the stack
				
				log_swap(hunter->id, hunter->boredom, hunter->fear, old_device, new_device);	//Log the device swap
				
				return;
			}
		}
	}
	
	//Attempt to gather evidence
	if (!hunter->current_room->is_exit) {	//Not in the exit
	
		sem_wait(&hunter->current_room->mutex);
		if (room_has_evidence_locked(hunter->current_room, hunter->device)) {	//Evidence type match
			
			room_remove_evidence_locked(hunter->current_room, hunter->device);	//Remove evidence from room
			sem_post(&hunter->current_room->mutex);
			
			casefile_add_evidence(hunter->casefile, hunter->device);		//Add evidence to case file
			
			//Set flag if not already returning
			if (!hunter->returning_to_van) {
				hunter->returning_to_van = true;
			}
			
			log_evidence(hunter->id, hunter->boredom, hunter->fear, hunter->current_room->name, hunter->device);	//Log the evidence
			
		} else { //No evidence type match
			sem_post(&hunter->current_room->mutex);
			
			if (rand_int_threadsafe(0, 100) < 10) {	//Small chance hunter returns to van to swap device
				//Set flag if not already returning
				if (!hunter->returning_to_van) {
					hunter->returning_to_van = true;
				}
			}
		}
	}
	
	//Movement
	if (hunter->returning_to_van) {	//Returning to van
		if (hunter->path != NULL) {	//Has rooms to backtrack to
			struct Room* new_room = roomstack_pop(&hunter->path);	//Get previous room
			
			if (new_room == NULL) {	//Stack is empty
				hunter->returning_to_van = false;
				return;
			}
			
			struct Room* old_room = hunter->current_room;
			
			lock_rooms(old_room, new_room);
			
			const char* from_room = old_room->name;
			const char* to_room = new_room->name;
			
			if (room_add_hunter_locked(new_room, hunter)) {	//Try to add to new room
				room_remove_hunter_locked(old_room, hunter);	//Remove from old room
				hunter->current_room = new_room;
				
				log_move(hunter->id, hunter->boredom, hunter->fear, from_room, to_room, hunter->device);
			}
			
			unlock_rooms(old_room, new_room);
		} else {	//Stack is empty, so hunter can't return
			hunter->returning_to_van = false;
		}
		
	} else { //Exploring
		struct Room* old_room = hunter->current_room;
		
		sem_wait(&old_room->mutex);	//Lock old room first
		
		struct Room* new_room = old_room->connected_rooms[rand_int_threadsafe(0, old_room->num_connections)];	//Random connected room
		
		//Same room case (shouldn't happen)
		if (old_room == new_room) {
			sem_post(&old_room->mutex);
		} else {
		
			sem_wait(&new_room->mutex);	//Lock second room
		
			const char* from_room = old_room->name;
			const char* to_room = new_room->name;
		
			if (room_add_hunter_locked(new_room, hunter)) { //Try to add to new room
				room_remove_hunter_locked(old_room, hunter);	//Remove from old room
				hunter->current_room = new_room;		//Update room
				roomstack_push(&hunter->path, old_room);	//Push to stack
				
				log_move(hunter->id, hunter->boredom, hunter->fear, from_room, to_room, hunter->device);
			}
		
			sem_post(&new_room->mutex);
			sem_post(&old_room->mutex);
		}
	}
}

void* hunter_thread(void* arg) {
	struct Hunter* hunter = (struct Hunter*)arg;
	
	while (!hunter->exited) {
		hunter_take_turn(hunter);
		usleep(100000); //100ms delay
	}
	return NULL;
}
