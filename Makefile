OBJ = main.o house.o hunter.o ghost.o room.o evidence.o helpers.o

all: final_assignment

final_assignment: $(OBJ)
	gcc -o final_assignment $(OBJ)
	
main.o: main.c defs.h helpers.h myheader.h
	gcc -c main.c

house.o: house.c defs.h helpers.h myheader.h
	gcc -c house.c

hunter.o: hunter.c defs.h helpers.h myheader.h
	gcc -c hunter.c
	
ghost.o: ghost.c defs.h helpers.h myheader.h
	gcc -c ghost.c
	
room.o: room.c defs.h helpers.h myheader.h
	gcc -c room.c
	
evidence.o: evidence.c defs.h helpers.h myheader.h
	gcc -c evidence.c
	
helpers.o: helpers.c defs.h helpers.h myheader.h
	gcc -c helpers.c

clean:
	rm -f $(OBJ) final_assignment
