#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>
#include <string.h>

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
	if (hunter->current_room->ghost != NULL) {	//Ghost is in the room
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
			if (evidence_is_valid_ghost(hunter->casefile->collected)) { //Hunter is returning because they've collected enough evidence to identify the ghost type
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
				log_swap(hunter->id, hunter->boredom, hunter->fear, old_device, new_device);	//Log the device swap
			}
		}
	}
	
	//Attempt to gather evidence
	if (!hunter->current_room->is_exit) {	//Not in the exit
	
		if (room_has_evidence(hunter->current_room, hunter->device)) {	//Evidence type match
		
			room_remove_evidence(hunter->current_room, hunter->device);		//Remove evidence from room
			casefile_add_evidence(hunter->casefile, hunter->device);		//Add evidence to case file
			hunter->returning_to_van = true;					//Update flag
			log_evidence(hunter->id, hunter->boredom, hunter->fear, hunter->current_room->name, hunter->device);	//Log the evidence
			
		} else { //No evidence type match
			
			if (rand_int_threadsafe(0, 100) < 10) {	//Small chance hunter returns to van to swap device
				hunter->returning_to_van = true;
			}
		}
	}
	
	//Movement
	if (hunter->returning_to_van) {
		if (hunter->path != NULL) {	//Has rooms to backtrack to
			struct Room* new_room = roomstack_pop(&hunter->path);	//Get previous room
			
			const char* from_room = hunter->current_room->name;
			const char* to_room = new_room->name;
			
			if (room_add_hunter(new_room, hunter)) {	//Try to add to new room
				room_remove_hunter(hunter->current_room, hunter);	//Remove from old room
				
				log_move(hunter->id, hunter->boredom, hunter->fear, from_room, to_room, hunter->device);
			}
		}
		
	} else { //Exploring
		struct Room* new_room = hunter->current_room->connected_rooms[rand_int_threadsafe(0, hunter->current_room->num_connections)];	//Random connected room
		
		const char* from_room = hunter->current_room->name;
		const char* to_room = new_room->name;
		struct Room* old_room = hunter->current_room;
		
		if (room_add_hunter(new_room, hunter)) { //Try to add to new room
			room_remove_hunter(old_room, hunter);	//Remove from old room
			roomstack_push(&hunter->path, old_room);	//Store old room on stack
			
			log_move(hunter->id, hunter->boredom, hunter->fear, from_room, to_room, hunter->device);
		}	
	}
}
