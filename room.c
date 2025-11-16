#include "defs.h"
#include "helpers.h"
#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Oh wait. I need to use room_init instead?
struct Room* create_room(const char* name) {
	struct Room* room = (struct Room*) malloc(sizeof(struct Room));	//Allocate memory for the room
	if (room == NULL) {
		return NULL;
	}
	
	//Initialize fields
	room->num_connections = 0;
	room->num_hunters = 0;
	room->ghost = NULL;
	room->is_exit = false;
	strncpy(room->name, name, MAX_ROOM_NAME - 1);
	room->name[MAX_ROOM_NAME - 1] = '\0';		//Ensure null termination
	
	return room;
}

void room_init(struct Room* room, const char* name, bool is_exit) {
	room->is_exit = is_exit;
	room->num_connections = 0;
	room->num_hunters = 0;
	room->ghost = NULL;
	strncpy(room->name, name, MAX_ROOM_NAME - 1);
	room->name[MAX_ROOM_NAME - 1] = '\0';		//Ensure null termination
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


