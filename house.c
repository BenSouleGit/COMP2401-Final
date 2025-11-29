#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>

/*
	All function documentation is located in myheader.h
*/

struct House* create_house(void) {
	struct House* house = (struct House*) malloc(sizeof(struct House));	//Allocate memory for the house
	if (house == NULL) {
		return NULL;
	}
	
	//Initialize fields
	house->room_count = 0;
	house->num_hunters = 0;
	house->max_hunters = 4;
	house->hunters = malloc(4*sizeof(struct Hunter*));
	house->starting_room = NULL;
	house->casefile.collected = 0;
	house->casefile.solved = false;
	
	house_populate_rooms(house);
	
	return house;
}

bool house_add_hunter(struct House* house, struct Hunter* hunter) {
	if (house->num_hunters >= house->max_hunters) {	//Check if we need to grow the array
		int new_max = house->max_hunters * 2;	//Double the capacity
		
		struct Hunter** new_array = realloc(house->hunters, new_max * sizeof(struct Hunter*));
		if (new_array == NULL) {
			return false;
		}
		
		house->hunters = new_array;
		house->max_hunters = new_max;
	}
	
	//Add hunter to the array
	house->hunters[house->num_hunters] = hunter;
	house->num_hunters++;
	
	return true;
}

//For single-threaded use only
bool check_game_over(struct House* house, struct Ghost* ghost) { 
	if (ghost->exited) {	//Ghost got bored!!
		return true;
	}
	
	//Check if all hunters have exited
	bool all_hunters_exited = true;
	for (int i=0; i < house->num_hunters; i++) {
		if (!house->hunters[i]->exited) {
			all_hunters_exited = false;
			break;
		}
	}
	if (all_hunters_exited) {
		return true;
	}
	
	//Check if any hunter left with evidence (victory)
	for (int i=0; i < house->num_hunters; i++) {
		if (house->hunters[i]->exited && house->hunters[i]->log_reason == LR_EVIDENCE) {
			return true;
		}
	}
	
	return false;
}
