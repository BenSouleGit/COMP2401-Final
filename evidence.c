#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>

/*
	All function documentation is located in myheader.h
*/

//Public wrapper
void room_add_evidence(struct Room* room, enum EvidenceType evidence) {
	sem_wait(&room->mutex);
	room_add_evidence_locked(room, evidence);
	sem_post(&room->mutex);
}

//For use if room is already locked
void room_add_evidence_locked(struct Room* room, enum EvidenceType evidence) {
	room->evidence = room->evidence | evidence;	//Bitwise OR
}

//Public wrapper
void room_remove_evidence(struct Room* room, enum EvidenceType evidence) {
	sem_wait(&room->mutex);
	room_remove_evidence_locked(room, evidence);
	sem_post(&room->mutex);
}

//For use if room is already locked
void room_remove_evidence_locked(struct Room* room, enum EvidenceType evidence) {
	room->evidence = room->evidence & ~evidence;	//Bitwise AND with NOT
}

//Public wrapper
bool room_has_evidence(struct Room* room, enum EvidenceType evidence) {
	sem_wait(&room->mutex);
	bool result = room_has_evidence_locked(room, evidence);
	sem_post(&room->mutex);
	
	return result;
}

//For use if room is already locked
bool room_has_evidence_locked(struct Room* room, enum EvidenceType evidence) {
	return (room->evidence & evidence) != 0;	//Bitwise AND
}

//Public wrapper
void casefile_add_evidence(struct CaseFile* casefile, enum EvidenceType evidence) {
	sem_wait(&casefile->mutex);
	casefile_add_evidence_locked(casefile, evidence);
	sem_post(&casefile->mutex);
}

//For use if room is already locked
void casefile_add_evidence_locked(struct CaseFile* casefile, enum EvidenceType evidence) {
	casefile->collected |= evidence;		//Bitwise OR to add the evidence
}

const char* evidence_to_ghost_type(EvidenceByte evidence) {
	const enum GhostType* all_ghosts;
	int count = get_all_ghost_types(&all_ghosts);
	
	for (int i=0; i < count; i++) {
		if (all_ghosts[i] == evidence) {	//Match
			return ghost_to_string(all_ghosts[i]);
		}
	}
	return "N/A";	//No match found
}
