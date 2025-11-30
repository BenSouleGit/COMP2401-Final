#include "defs.h"
#include "helpers.h"
#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	All function documentation is located in myheader.h
*/

void room_init(struct Room* room, const char* name, bool is_exit) {
	//Initialize
	room->is_exit = is_exit;
	room->num_connections = 0;
	room->num_hunters = 0;
	room->ghost = NULL;
	room->evidence = 0;
	strncpy(room->name, name, MAX_ROOM_NAME - 1);
	room->name[MAX_ROOM_NAME - 1] = '\0';		//Ensure null termination
	
	//Initialize hunters array to NULL
	for (int i=0; i < MAX_ROOM_OCCUPANCY; i++) {
		room->hunters[i] = NULL;
	}	
	
	sem_init(&room->mutex, 0, 1); //1 = unlocked
}

int room_connect(struct Room* room1, struct Room* room2) {
	if (room1->num_connections >= MAX_CONNECTIONS || room2->num_connections >= MAX_CONNECTIONS) { //Check connection capacity
		return 0;
	}
	
	//Add room2 to room1's connections
	room1->connected_rooms[room1->num_connections] = room2;
	room1->num_connections++;
	
	//Add room1 to room2's connections
	room2->connected_rooms[room2->num_connections] = room1;
	room2->num_connections++;
	
	return 1;
}

//For use if room is already locked
void room_remove_hunter_locked(struct Room* room, struct Hunter* hunter) {
	//Remove hunter from current room
	for (int i=0; i < MAX_ROOM_OCCUPANCY; i++) {
		if (room->hunters[i] == hunter) {
			room->hunters[i] = NULL;
			if (room->num_hunters > 0) {
				room->num_hunters--;
			}
			return;
		}
	}
}

//Public wrapper
void room_remove_hunter(struct Room* room, struct Hunter* hunter) {
	sem_wait(&room->mutex);
	room_remove_hunter_locked(room, hunter);
	sem_post(&room->mutex);
}

//For use if room is already locked
bool room_add_hunter_locked(struct Room* room, struct Hunter* hunter) {
	if (room->num_hunters >= MAX_ROOM_OCCUPANCY) {
		return false;
	}
    
	//Find empty slot and add hunter
	for (int i = 0; i < MAX_ROOM_OCCUPANCY; i++) {
		if (room->hunters[i] == NULL) {
			room->hunters[i] = hunter;
			room->num_hunters++;
			return true;
		}
	}
	return false;
}

//Public wrapper
bool room_add_hunter(struct Room* room, struct Hunter* hunter) {
	sem_wait(&room->mutex);
	bool ok = room_add_hunter_locked(room, hunter);
	sem_post(&room->mutex);
	return ok;
}

void roomstack_push(RoomStack* stack, struct Room* room) {
	struct RoomNode* new_node = malloc(sizeof(struct RoomNode));	//Allocate memory for a new node
	
	//Initialize and link new node
	new_node->data = room;
	new_node->next = *stack;	//Points to previous stack head
	*stack = new_node;		//New node is the new stack head
}

struct Room* roomstack_pop(RoomStack* stack) {
	if (*stack == NULL) {	//Check if stack is empty
		return NULL;
	}
	
	struct RoomNode* old_top = *stack;	//Save current stack head
	struct Room* room = old_top->data;	//Save the room pointer
	*stack = old_top->next;			//Update stack head
	free(old_top);				//Free old stack head
	
	return room;
}

void roomstack_cleanup(RoomStack* stack) {
	while (*stack != NULL) {
		roomstack_pop(stack);
	}
}

void lock_rooms(struct Room* room1, struct Room* room2) {
	//Handle same room case
	if (room1 == room2) {
		sem_wait(&room1->mutex);
		return;
	}
	
	//Lock lower memory address first
	if (room1 < room2) {
		sem_wait(&room1->mutex);
		sem_wait(&room2->mutex);
	} else {
		sem_wait(&room2->mutex);
		sem_wait(&room1->mutex);
	}
}

void unlock_rooms(struct Room* room1, struct Room* room2) {
	//Handle same room case
	if (room1 == room2) {
		sem_post(&room1->mutex);
		return;
	}
	
	//Unlock in reverse order of locking
	if (room1 < room2) {
		sem_post(&room2->mutex);
		sem_post(&room1->mutex);
	} else {
		sem_post(&room1->mutex);
		sem_post(&room2->mutex);
	}
}

void room_cleanup(struct Room* room) {
	sem_destroy(&room->mutex);	//Destroy all semaphores.
}
