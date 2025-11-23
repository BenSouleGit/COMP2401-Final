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
