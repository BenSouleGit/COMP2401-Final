#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>


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
	
	house_populate_rooms(house);
	
	return house;
}
