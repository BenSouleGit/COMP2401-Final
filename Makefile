OBJ = main.o house.o hunter.o ghost.o room.o evidence.o helpers.o

all: final_assignment

final_assignment: $(OBJ)
	gcc -Wall -pthread -o final_assignment $(OBJ)
	
main.o: main.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c main.c

house.o: house.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c house.c

hunter.o: hunter.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c hunter.c
	
ghost.o: ghost.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c ghost.c
	
room.o: room.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c room.c
	
evidence.o: evidence.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c evidence.c
	
helpers.o: helpers.c defs.h helpers.h myheader.h
	gcc -Wall -pthread -c helpers.c

clean:
	rm -f $(OBJ) final_assignment
