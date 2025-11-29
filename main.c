#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "helpers.h"
#include "myheader.h"
#include <pthread.h>
#include <unistd.h>

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
	

	/*
	Hunter creation loop
	*/
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
		
		printf("\n");
	}



	/*
	Threads
	*/
	printf("\n=== GAME STARTING ===\n");

	pthread_t ghost_thread_id;
	pthread_t hunter_threads[house->num_hunters];
	
	//Ghost thread creation
	pthread_create(&ghost_thread_id, NULL, ghost_thread, ghost);
	
	//Hunter thread creations
	for (int i=0; i < house->num_hunters; i++) {
		pthread_create(&hunter_threads[i], NULL, hunter_thread, house->hunters[i]);
	}
	
	//Wait for all threads to finish
	pthread_join(ghost_thread_id, NULL);
	for (int i=0; i < house->num_hunters; i++) {
		pthread_join(hunter_threads[i], NULL);
	}

	printf("\n=== GAME OVER ===\n");
	
	
	
	/*
	Display results
	*/
	printf("\n=== RESULTS ===\n");
	
	//Hunters
	printf("Hunters:\n");
	for (int i=0; i < house->num_hunters; i++) {
		struct Hunter* h = house->hunters[i];
		printf("    - %s (ID: %d) exited because of [%s] (bored=%d, fear=%d)\n", h->name, h->id, exit_reason_to_string(h->log_reason), h->boredom, h->fear);
	}
	
	//Case file checklist
	printf("\nShared Case File Checklist:\n");
	const enum EvidenceType* all_evidence;
	int evidence_count = get_all_evidence_types(&all_evidence);
	
	for (int i=0; i < evidence_count; i++) {
		char check = (house->casefile.collected & all_evidence[i]) ? 'X' : ' ';	//Check off each collected evidence type
		printf("    [%c] %s\n", check, evidence_to_string(all_evidence[i]));
	}
	
	//Victory results
	printf("\nVictory Results:\n");
	int evidence_exits = 0;
	for (int i=0; i < house->num_hunters; i++) {
		if (house->hunters[i]->log_reason == LR_EVIDENCE) {
			evidence_exits++;
		}
	}
	printf("    - Hunters who exited after identifying the ghost : %d/%d\n", evidence_exits, house->num_hunters);
	printf("    - Ghost Guess: ");
	if (evidence_is_valid_ghost(house->casefile.collected)) {
		printf("%s\n", evidence_to_ghost_type(house->casefile.collected));	
	} else {
		printf("N/A\n");
	}
	printf("    - Actual Ghost Type: %s\n", ghost_to_string(ghost->type));
	printf("\nWinner: ");
	if (evidence_exits > 0) {
		printf("Hunters\n");
	} else if (ghost->exited) {
		printf("Ghost\n");
	} else {
		printf("N/A\n");
	}
	
	
	
	/*
	Cleanup
	*/
	for (int i=0; i <house->num_hunters; i++) {	//Free all hunters and their RoomStacks
		roomstack_cleanup(&house->hunters[i]->path);
		free(house->hunters[i]);
	}
	free(house->hunters);	//Free hunter array
	free(ghost);		//Free ghost
	free(house);		//Free house

	return 0;
}
