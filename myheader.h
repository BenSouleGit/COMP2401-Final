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
* @return	Pointer to the new house, or NULL on failure.
*/
struct House* create_house(void);

/**
* @brief		Adds evidence to a room.
* @param[in, out]	room: The room to add evidence to.
* @param[in]		evidence: The type of evidence to add.
*/
void room_add_evidence(struct Room* room, enum EvidenceType evidence);

/**
* @brief		Removes evidence from a room.
* @param[in, out]	room: The room to remove evidence from.
* @param[in]		evidence: The type of evidence to remove.
*/
void room_remove_evidence(struct Room* room, enum EvidenceType evidence);

/**
* @brief	Checks if a room has specific evidence.
* @param[in]	room: The room to check.
* @param[in]	evidence: The evidence type to look for.
* @return	true if the room has the evidence, false otherwise.
*/
bool room_has_evidence(struct Room* room, enum EvidenceType evidence);

/**
* @brief	Creates and initializes a hunter.
* @param[]	name: The hunter's name.
* @param[]	id: The hunter's unique id.
* @param[]	starting_room: The room where the hunter begins.
* @param[]	device: The evidence device the hunter carries.
* @param[]	casefile: Pointer to the shared case file.
* @return	Pointer to the new hunter, or NULL on failure.
*/
struct Hunter* create_hunter(const char* name, int id, struct Room* starting_room, enum EvidenceType device, struct CaseFile* casefile);



#endif //MYHEADER_H
