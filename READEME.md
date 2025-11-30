# Final Assignment, Simulating the Hunts
This program simulates a ghost hunting scenario (very similar to Phasmaphobia) where one or more hunters explore a house, collect evidence, and attempt to identify a ghost. The ghost wanders around the house, leaving evidence, and trying to scare the hunters. If the hunters get too scared, or the ghost or hunters get too bored, they will leave the game. If the hunters are able to collect enough evidence and correctly identify the ghost they win. If not, the ghost wins.

## Building and Running
1. Open a terminal and navigate to the project directory
2. Type 'make' to compile the program using the provided Makefile
3. Run the program with './final_assignment'
(If for some reason the make doesn't work, run 'gcc -Wall -pthread -o final_assignment *.c'
4. To test for race conditions, compile with 'gcc -fsanitize=thread -pthread -o final_assignment_san *.c' and run './final_assignment_san'
5. To check for memory leaks, run 'valgrind --leak-check=full ./final_assignment'

## Usage
- When prompted, enter the hunter names and IDs
- Type 'done' as the name to finish creating hunters
- The game will run automatically with hunters and the ghost taking turns
- The results of the game will display once it finishes

## Files
- 'main.c' - Initialization, user input, thread creation and management
- 'house.c' - House structure and room management
- 'hunter.c' - Hunter behaviour
- 'ghost.c' - Ghost behaviour
- 'room.c' - Room operations and RoomStack implementation
- 'evidence.c' - Evidence bitmask operations
- 'helpers.c' - Provided logging and helper function implementation
- 'helpers.h' - Provided logging and helper function documentation and declarations
- 'defs.h' - Structure definitions and constants
- 'myheader.h' - Custom helper function documentation and declarations
- 'Makefile' - Build configuration
- 'validate_logs.py' - Provided Python function for log validation

## Bonuses
- Documentation (myheader.h)

## Assumptions
- Error handling is not require (via "We are NOT concerned about error tolerance and clean error messages in this project, but you may include some if you wish.")
- Hunters continue to take turns even after the ghost has exited
- Hunters who are still exploring aren't required to return if another hunter identifies the ghost

## Credits and Sources
- Developed individually by Ben Soule (101358745)
- Used course lecture slides and the textbook for pthread and semaphore usage
- Used Monster and (prescription) Adderall to stay awake and focused :)
