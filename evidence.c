#include "defs.h"
#include "helpers.h" 
#include "myheader.h"
#include <stdlib.h>

/*
	All function documentation is located in myheader.h
*/

void room_add_evidence(struct Room* room, enum EvidenceType evidence) {
	room->evidence = room->evidence | evidence;	//Bitwise OR
}

void room_remove_evidence(struct Room* room, enum EvidenceType evidence) {
	room->evidence = room->evidence & ~evidence;	//Bitwise AND with NOT
}

bool room_has_evidence(struct Room* room, enum EvidenceType evidence) {
	return (room->evidence & evidence) != 0;	//Bitwise AND
}

void casefile_add_evidence(struct CaseFile* casefile, enum EvidenceType evidence) {
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
