#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "helpers.h"
#include "myheader.h"

int main() {

    /*
    1. Initialize a House structure.
    2. Populate the House with rooms using the provided helper function.
    3. Initialize all of the ghost data and hunters.
    4. Create threads for the ghost and each hunter.
    5. Wait for all threads to complete.
    6. Print final results to the console:
         - Type of ghost encountered.
         - The reason that each hunter exited
         - The evidence collected by each hunter and which ghost is represented by that evidence.
    7. Clean up all dynamically allocated resources and call sem_destroy() on all semaphores.
    */

	//Initialize house
	struct House* house = create_house();
	
	//Initialize ghost
	struct Ghost* ghost = create_ghost(house);
	

	//Hunter creation loop
	printf("Enter hunters one at a time. Type 'done' as the name to finish.\n");

	char name[MAX_HUNTER_NAME];
	int id;
	
	while (1) {
		printf("Enter hunter name (max 63 characters) or 'done' to finish: ");
		scanf("%63s", name);
		
		if (strcmp(name, "done") == 0) {
			break;
		}
	
		printf("Enter hunter ID: ");
		scanf("%d", &id);
	
		//Get random evidence type for starting device
		const enum EvidenceType* all_evidence;
		int count = get_all_evidence_types(&all_evidence);
		enum EvidenceType random_device = all_evidence[rand_int_threadsafe(0, count)];
		
	
		//Create hunter
		struct Hunter* hunter = create_hunter(name, id, house->starting_room, random_device, &house->casefile);
		
		//Append hunter
		house_add_hunter(house, hunter);
	}



	printf("\n=== SIMULATION STARTING ===\n");

	bool game_over = false;
	
	while (!game_over) {
		if (!ghost->exited) {
			ghost_take_turn(ghost);
		}
		
		for (int i=0; i < house->num_hunters; i++) {
			if (!house->hunters[i]->exited) {
				hunter_take_turn(house->hunters[i]);
			}
		}
		
		game_over = check_game_over(house, ghost);
	}
	printf("\n=== SIMULATION OVER ===\n");

	return 0;
}
