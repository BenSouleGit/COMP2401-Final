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


// Temporary test - remove later
#include <stdio.h>

int main() {
    struct House* house = create_house();
    if (house) {
        printf("House created successfully!\n");
        printf("Room count: %d\n", house->room_count);
        printf("Starting room: %s\n", house->starting_room->name);
        
        // Test a connection
        printf("Van connects to: ");
        for (int i = 0; i < house->starting_room->num_connections; i++) {
            printf("%s ", house->starting_room->connected_rooms[i]->name);
        }
        printf("\n");
        
        // Don't forget to free everything later!
        free(house->hunters);
        free(house);
    }
    return 0;
}
