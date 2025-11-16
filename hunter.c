#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>
#include <string.h>


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
	hunter->exited = false;			//Starting in van, but haven't "exited"
	hunter->current_room = starting_room;
	hunter->path = NULL;			//Room stack starts empty
	hunter->device = device;
	hunter->casefile = casefile;
	
	//Log the creation of the hunter
	log_hunter_init(id, starting_room->name, name, device);
	
	return hunter;
}


#include <stdio.h>

// Temporary test main
int main() {
    // Create a test house and room first
    struct House* house = create_house();
    if (!house) {
        printf("Failed to create house\n");
        return 1;
    }
    
    // Create a hunter
    struct Hunter* hunter = create_hunter("Test Hunter", 123, house->starting_room, EV_EMF, &house->casefile);
    if (hunter) {
        printf("Hunter created successfully!\n");
        printf("Name: %s, ID: %d, Device: %d\n", hunter->name, hunter->id, hunter->device);
        printf("Current room: %s, Fear: %d, Boredom: %d\n", hunter->current_room->name, hunter->fear, hunter->boredom);
        
        // Clean up
        free(hunter);
    } else {
        printf("Failed to create hunter\n");
    }
    
    // Clean up house (you'll need cleanup functions later)
    free(house->hunters);
    free(house);
    
    return 0;
}
