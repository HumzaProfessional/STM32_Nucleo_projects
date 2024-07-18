#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
extern void initialise_monitor_handles(void);


#define MAX_PUZZLES 5

typedef struct { // define the stururtre of the room to ease use when using the printroomscaneiro fucntion
    int x;
    int y;
    int money;

   bool puzzles[MAX_PUZZLES]; // array for the puzzles
   bool greeted;
} Room;

Room currentRoom = {0, 0}; // Starting room coordinates

void printRoomScenario(Room *room);
void displayMainMenu(void);

int main(void) {
    initialise_monitor_handles();
    displayMainMenu();

    printf("Welcome to the Game:\n");

    while (1) {
        printRoomScenario(&currentRoom);
        char input;
        printf("Enter direction (w/a/s/d) to move or q to quit: ");
        scanf(" %c", &input);

        // Restrict movement from room (0, 0) to only one direction
        if (currentRoom.x == 0 && currentRoom.y == 0 && input != 'd' && input != 'q') {
             printf("You can only move to the north from here.\n");
             continue;
                }
        switch (input) {
            case 'w': // Move up
                currentRoom.y -= 1;
                break;
            case 'a': // Move left
                currentRoom.x -= 1;
                break;
            case 's': // Move down
                currentRoom.y += 1;
                break;
            case 'd': // Move right
                currentRoom.x += 1;
                break;
            case 'q': // Quit
                return 0;
            default:
                printf("Invalid input! Please enter w/a/s/d or q.\n");
                break;

        }


    }

    return 0;
}

void printRoomScenario(Room *room) {
    if (room->x == 0 && room->y == 0) {
        printf("You are at the entrance of the great tree. The towering size gives a presence of gradeur and ominmouty.\n");
    } else if (room->x == 0 && room->y == 1) {
        printf("You are at the foot of the tree. It feels as if the the way has shut out behind you. \n ");
         if(!room->greeted){
        	 printf("You see a something shining with light moving toward you... \n");
        	 printf("Greetings adventurer, you must be insert_name. I am a guiding spirit. Here to help guide heros to save Yggdrasil: the mainspring of the universe. \n");
        	 printf("The great tree is dying due to unknown causes. That what awakened me, and I have sensed evil and rot in the nearby worlds \n");
             printf("In order to save Yggdrasil, you must seek out the world essence of the corrupted lands and bring them to the heart of the tree. \n ");
                           }
         else if (room->x == 0 && room->y == 1) {
        printf("Y\n");
    } else if (room->x == -1 && room->y == 0) {
        printf("You are in a dark room. You can hear water dripping somewhere.\n");
    } else if (room->x == 0 && room->y == -1) {
        printf("You have entered a room with a mysterious locked chest.\n");

void displayMenu(void)
       {
            const char *art[] = {
                "*************************************************",
                "*                                               *",
                "*          Remnants of Yggdrasil                *",
                "*                                               *",
                "*     ██▀███  ▓█████ ███▄ ▄███▓ ███▄    █  ▄▄▄      ███▄    █ ▄▄▄█████▓ ██████     ▒█████    █████▒   ▓██   ██▓  ▄████   ▄████ ▓█████▄  ██▀███  ▄▄▄        ██████  ██▓ ██▓       *",
                "*    ▓██ ▒ ██▒▓█   ▀▓██▒▀█▀ ██▒ ██ ▀█   █ ▒████▄    ██ ▀█   █ ▓  ██▒ ▓▒██    ▒    ▒██▒  ██▒▓██   ▒     ▒██  ██▒ ██▒ ▀█▒ ██▒ ▀█▒▒██▀ ██▌▓██ ▒ ██▒████▄    ▒██    ▒ ▓██▒▓██▒       *",
                "*    ▓██ ░▄█ ▒▒███  ▓██    ▓██░▓██  ▀█ ██▒▒██  ▀█▄ ▓██  ▀█ ██▒▒ ▓██░ ▒░ ▓██▄      ▒██░  ██▒▒████ ░      ▒██ ██░▒██░▄▄▄░▒██░▄▄▄░░██   █▌▓██ ░▄█ ▒██  ▀█▄  ░ ▓██▄   ▒██▒▒██░       *",
                "*    ▒██▀▀█▄  ▒▓█  ▄▒██    ▒██ ▓██▒  ▐▌██▒░██▄▄▄▄██▓██▒  ▐▌██▒░ ▓██▓ ░  ▒   ██▒   ▒██   ██░░▓█▒  ░      ░ ▐██▓░░▓█  ██▓░▓█  ██▓░▓█▄   ▌▒██▀▀█▄ ░██▄▄▄▄██   ▒   ██▒░██░▒██░       *",
                "*    ░██▓ ▒██▒░▒████▒██▒   ░██▒▒██░   ▓██░ ▓█   ▓██▒██░   ▓██░  ▒██▒ ░▒██████▒▒   ░ ████▓▒░░▒█░         ░ ██▒▓░░▒▓███▀▒░▒▓███▀▒░▒████▓ ░██▓ ▒██▒▓█   ▓██▒▒██████▒▒░██░░██████▒   *",
                "*    ░ ▒▓ ░▒▓░░░ ▒░ ░ ▒░   ░  ░░ ▒░   ▒ ▒  ▒▒   ▓▒█░ ▒░   ▒ ▒   ▒ ░░  ▒ ▒▓▒ ▒ ░   ░ ▒░▒░▒░  ▒ ░          ██▒▒▒  ░▒   ▒  ░▒   ▒  ▒▒▓  ▒ ░ ▒▓ ░▒▓░▒▒   ▓▒█░▒ ▒▓▒ ▒ ░░▓  ░ ▒░▓  ░   *",
                "*      ░▒ ░ ▒░ ░ ░  ░  ░      ░░ ░░   ░ ▒░  ▒   ▒▒ ░ ░░   ░ ▒░    ░   ░ ░▒  ░ ░     ░ ▒ ▒░  ░          ▓██ ░▒░   ░   ░   ░   ░  ░ ▒  ▒   ░▒ ░ ▒░ ▒   ▒▒ ░░ ░▒  ░ ░ ▒ ░░ ░ ▒  ░   *",
                "*      ░░   ░    ░  ░      ░      ░   ░ ░   ░   ▒     ░   ░ ░   ░     ░  ░  ░     ░ ░ ░ ▒   ░ ░        ▒ ▒ ░░  ░ ░   ░ ░ ░   ░  ░ ░  ░   ░░   ░  ░   ▒   ░  ░  ░   ▒ ░  ░ ░      *",
                "*       ░        ░  ░      ░            ░       ░  ░        ░               ░         ░ ░              ░ ░           ░       ░    ░       ░          ░  ░      ░   ░      ░  ░   *",
                "*                                                                                                      ░ ░                      ░                                                *",
                "*                                                                                                                       *",
                "*                                                                                       [1] Start Game                  *",
                "*                                                                                          [2] Quit                        *",
                "*                                                                                                                        *",
                "*                                                                                         ,@@@@/                 *",
                "*                                                                                     ,@@@@@/@@,                  *",
                "*                                                          \\   \\       ,,,.   ,@@@@@@/@@/  .oo8888o.          *",
                "*                                                            \\   \\  \\  ,&%%&%&&%,@@@@@/@@,/888\\\\88/8o        *",
                "*                                                              \\,%&\\\\%&&%&&%,@@@\\\\@@@/@@/@88\\\\88888/88'      *",
                "*                                                              %\\&&%&/%&&%@@\\\\@@/ /@/8888\\\\88888'      *",
                "*                                                             %&&%/ %&%%&&@@\\\\ V /@@' `88\\\\8 `/88'       *",
                "*                                                             `&%\\\\ ` /%&'    |.|        \\ '|8'         *",
                "*                                                                  |o|        | |         | |            *",
                "*                                                                  |.|        | |         | |            *",
                "*                                                           \\/ ._\\\\//_/__/  ,\\\\_//__\\/.  \\\\_//__/ *"
            };

            const int artSize = sizeof(art) / sizeof(art[0]);

            for (int i = 0; i < artSize; i++) {
                printf("%s\n", art[i]);
            }

            int choice;
            do {
                printf("Enter your choice: ");
                scanf("%d", &choice);

                if (choice == 2) {
                    printf("Exiting game...\n");
                    exit(0);
                } else if (choice != 1) {
                    printf("Invalid choice! Please enter 1 or 2.\n");
                }
            } while (choice != 1);
        }







