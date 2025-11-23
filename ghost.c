#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>
#include <string.h>

/*
	All function documentation is located in myheader.h
*/

struct Ghost* create_ghost(struct House* house) {
	struct Ghost* ghost = malloc(sizeof(struct Ghost));	//Allocate memeory for new ghost
	if (ghost == NULL) {
		return NULL;
	}
	
	//Get random ghost type
	const enum GhostType* ghost_types;
	int num_ghosts = get_all_ghost_types(&ghost_types);
	
	//Initialize fields
	ghost->id = DEFAULT_GHOST_ID;
	ghost->boredom = 0;
	ghost->exited = false;
	ghost->current_room = &house->rooms[rand_int_threadsafe(0, house->room_count)];	//Set random starting room
	ghost->type = ghost_types[rand_int_threadsafe(1, num_ghosts)];	//lower_inclusive = 1, since ghost can't be in the van (0)
	
	//Log the creation of the ghost
	log_ghost_init(ghost->id, ghost->current_room->name, ghost->type);
	
	return ghost;
}

void ghost_take_turn(struct Ghost* ghost) {
	//Hunter check
	if (ghost->current_room->num_hunters > 0) {
		ghost->boredom = 0;	//Reset boredom
	} else {
		ghost->boredom++;	//Increase boredom
	}
	
	//Exit conditions
	if (ghost->boredom > ENTITY_BOREDOM_MAX) {
		ghost->exited = true;
		log_ghost_exit(ghost->id, ghost->boredom, ghost->current_room->name);	//Log the ghost exiting
		return;
	}
	
	//Take action
	int action = rand_int_threadsafe(0, 3);
	switch (action) {
		case 0:	//Idling
			
			log_ghost_idle(ghost->id, ghost->boredom, ghost->current_room->name);	//Log the ghost idling
			break;
			
		case 1:	//Haunting
			
			//Get all possible evidence types
			const enum EvidenceType* all_evidence;
			int num_evidence_types = get_all_evidence_types(&all_evidence);
			
			//Find which evidence types this ghost can leave
			enum EvidenceType ghost_evidence[3];		//Array to store the ghost's 3 evidence types
			int count = 0;
			
			for (int i=0; i < num_evidence_types; i++) {
				if (ghost->type & all_evidence[i]) {	//Check if ghost has this evidence type
					ghost_evidence[count] = all_evidence[i];
					count++;
				}
			}
			
			
			//Pick one of the three possible evidence types randomly
			enum EvidenceType chosen_evidence = ghost_evidence[rand_int_threadsafe(0, 3)];
			
			room_add_evidence(ghost->current_room, chosen_evidence);					//Add the evidence
			log_ghost_evidence(ghost->id, ghost->boredom, ghost->current_room->name, chosen_evidence);	//Log the added evidence
			
			break;
			
		case 2: //Moving
			
			if (ghost->current_room->num_hunters == 0) {	//Movement allowed
				
				const char* old_room_name = ghost->current_room->name;	//Track old room name for logging
				
				//Select a random connected room	
				int new_room_int = rand_int_threadsafe(0, ghost->current_room->num_connections);
				struct Room* new_room = ghost->current_room->connected_rooms[new_room_int];
				
				//Update pointers
				ghost->current_room->ghost = NULL;	
				new_room->ghost = ghost;		
				ghost->current_room = new_room;		
				
				log_ghost_move(ghost->id, ghost->boredom, old_room_name, ghost->current_room->name);	//Log the ghost movement
			}
			break;
	}
	
}
