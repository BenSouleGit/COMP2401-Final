#ifndef MYHEADER_H
#define MYHEADER_H

#include "defs.h"


/**
* @brief	Creates and initializes a house structure.
* @return	Pointer to the new house, or NULL on failure.
*/
struct House* create_house(void);

/**
* @brief	Checks if the simulation should end. (For single-threaded use only)
* @param[in]	house: The house with hunters.
* @param[in]	ghost: The ghost.
* @return	true if simulation should end, false otherwise.
*/
bool check_game_over(struct House* house, struct Ghost* ghost);

/**
* @brief		Adds a hunter to the house's hunter array.
* @param[in, out]	house: The house to add the hunter to.
* @param[in]		hunter: The hunter to add.
* @return		true if successful, false on failure.
*/
bool house_add_hunter(struct House* house, struct Hunter* hunter);

/**
* @brief		Adds evidence to the shared case file.
* @param[in, out]	casefile: The shared case file to update.
* @param[in]		evidence: The evidence type to add.
* @return
*/
void casefile_add_evidence(struct CaseFile* casefile, enum EvidenceType evidence);

/**
* @brief	Connects two rooms.
* @param[in]	room1: Pointer to first room.
* @param[in]	room2: Pointer to second room.
* @return	An integer. 1 if rooms were succesfully connected. 0 otherwise.
*/
int room_connect(struct Room* room1, struct Room* room2);

/**
* @brief	Locks two rooms in consistent order to prevent deadlock.
* @param[in]	room1: First room to lock.
* @param[in]	room2: Second room to lock.
*/
void lock_rooms(struct Room* room1, struct Room* room2);

/**
* @brief	Unlocks two rooms.
* @param[in]	room1: First room to unlock.
* @param[in]	room2: Second room to unlock.
*/
void unlock_rooms(struct Room* room1, struct Room* room2);

/**
* @brief		Adds evidence to a room. (Used by ghost)
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
* @brief	Finds the ghost type that matches the given evidence.
* @param[in]	evidence: The collected evidence bitmask.
* @return	The matching ghost type.
*/
const char* evidence_to_ghost_type(EvidenceByte evidence);

/**
* @brief		Removes a hunter from a room's hunter list.
* @param[in, out]	room: The room to remove the hunter from.
* @param[in]		hunter: The hunter to remove.
*/
void room_remove_hunter(struct Room* room, struct Hunter* hunter);

/**
* @brief		Adds a hunter to a room's hunter list.
* @param[in,out]	room: The room to add the hunter to.
* @param[in]		hunter: The hunter to add.
* @return		true if successful, false if room is full.
*/
bool room_add_hunter(struct Room* room, struct Hunter* hunter);

/**
* @brief		Pushes a room onto a RoomStack.
* @param[in, out]	stack: Pointer to the stack head.
* @param[in]		room: Room to push onto the stack.
*/
void roomstack_push(RoomStack* stack, struct Room* room);

/**
* @brief		Pops a room from a RoomStack.
* @param[in, out]	stack: Pointer to the stack head.
* @return		The room popped from the stack, or NULL if empty.
*/
struct Room* roomstack_pop(RoomStack* stack);

/**
* @brief		Frees all nodes in a RoomStack.
* @param[in,out]	stack: Pointer to the stack to clean up.
*/
void roomstack_cleanup(RoomStack* stack);

/**
* @brief	Creates and initializes a hunter.
* @param[in]	name: The hunter's name.
* @param[in]	id: The hunter's unique id.
* @param[in]	starting_room: The room where the hunter begins.
* @param[in]	device: The evidence device the hunter carries.
* @param[in]	casefile: Pointer to the shared case file.
* @return	Pointer to the new hunter, or NULL on failure.
*/
struct Hunter* create_hunter(const char* name, int id, struct Room* starting_room, enum EvidenceType device, struct CaseFile* casefile);

/**
* @brief		Simulates one turn of hunter behaviour.
* @param[in, out]	hunter: The hunter taking their turn.
*/
void hunter_take_turn(struct Hunter* hunter);

/**
* @brief	Hunter thread function. Runs hunter behavior in a loop.
* @param[in]	arg Pointer to the hunter structure (cast from void*).
* @return	NULL.
*/
void* hunter_thread(void* arg);

/**
* @brief		Creates and initializes a ghost.
* @param[in, out]	house: The house where the ghost will be placed.
* @return		Pointer to the new ghost, or NULL on failure.
*/
struct Ghost* create_ghost(struct House* house);

/**
* @brief		Executes one turn of ghost behaviour.
* @param[in, out]	ghost: The ghost taking its turn.
*/
void ghost_take_turn(struct Ghost* ghost);

/**
* @brief	Ghost thread function. Runs ghost behavior in a loop.
* @param[in]	arg: Pointer to the ghost structure (cast from void*).
* @return	NULL.
*/
void* ghost_thread(void* arg);

#endif //MYHEADER_H
