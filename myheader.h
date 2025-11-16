#ifndef MYHEADER_H
#define MYHEADER_H

#include "defs.h"



/**
* @brief
* @param[]
* @return
*/


/**
* @brief	Creates and initializes a room structure.
* @param[in] 	name: Name of the new room.
* @return	Pointer to the new room, or NULL on failure.
*/
struct Room* create_room(const char* name);

/**
* @brief	Connects two rooms.
* @param[in]	room1: Pointer to first room.
* @param[in]	room2: Pointer to second room.
* @return	An integer. 1 if rooms were succesfully connected. 0 otherwise.
*/
int room_connect(struct Room* room1, struct Room* room2);

/**
* @brief	Creates and initializes a house structure.
* @param[]	void
* @return	Pointer to the new house, or NULL on failure.
*/
struct House* create_house(void);


#endif //MYHEADER_H
