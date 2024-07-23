#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
extern void initialise_monitor_handles(void);

#define MAX_PUZZLES 4
#define MAX_TREASURES 4
#define MAX_ITEMS 10
#define ITEM_NAME_LENGTH 50
#define GREET 4

typedef struct { // define the structure of the room to ease use when using the printRoomScenario function
   signed int x;
   signed int y;
    int money;

    bool puzzles[MAX_PUZZLES]; // array for the puzzles
    bool treasures[MAX_TREASURES]; // array for treasures
    bool greeted[GREET];
} Room;

Room currentRoom = {0, 0}; // Starting room coordinates

typedef struct {
    char items[MAX_ITEMS][ITEM_NAME_LENGTH];
    int itemCount;
} Inventory;

Inventory playerInventory = {.itemCount = 0}; // Initialize inventory with 0 items

int worldEssences = 0; // Global variable to track the number of world essences

void printRoomScenario(Room *room, Inventory *inventory);
void displayMenu();
void addItemToInventory(Inventory *inventory, const char *item);
void printInventory(const Inventory *inventory);

int main(void) {
    initialise_monitor_handles();
    displayMenu();

    printf("Welcome to the Game:\n");

    while (1) {
    	printf("Current Location: (%d, %d)\n", currentRoom.x, currentRoom.y); // Debug print
        printRoomScenario(&currentRoom, &playerInventory);

        char input;
        printf("Enter direction (w/a/s/d) to move, i to view inventory, u to use an item, m to check map, or q to quit: ");
        scanf(" %c", &input);

        switch (input) {
            case 'w': // Move up
                currentRoom.y += 1;
                break;
            case 'a': // Move left
                currentRoom.x -= 1;
                break;
            case 's': // Move down
                currentRoom.y -= 1;
                break;
            case 'd': // Move right
                currentRoom.x += 1;
                break;
            case 'i': // Check inventory
                printInventory(&playerInventory);
                break;
            case 'u': // Use item
                printf("Select an item number to use: ");
                int itemIndex;
                scanf("%d", &itemIndex);
                if (itemIndex > 0 && itemIndex <= playerInventory.itemCount) {
                    printf("You used %s.\n", playerInventory.items[itemIndex - 1]);
                    // Remove the item from the inventory
                    for (int i = itemIndex - 1; i < playerInventory.itemCount - 1; i++) {
                        strcpy(playerInventory.items[i], playerInventory.items[i + 1]);
                    }
                    playerInventory.itemCount--;
                } else {
                    printf("Invalid item number.\n");
                }
                break;
            case 'm': // Check map
                printf("You check your map. Current location: (%d, %d)\n", currentRoom.x, currentRoom.y);
                break;
            case 'q': // Quit
                return 0;
            default:
                printf("Invalid input! Please enter w/a/s/d/i/u/m or q.\n");
                break;
        }
    }

    return 0;
}

void printRoomScenario(Room *room, Inventory *inventory) {
    if (room->x == 0 && room->y == 0) {
        printf("You are at the entrance of the great tree. The towering size gives a presence of grandeur and ominousness.\n");
    } else if (room->x == 0 && room->y == 1) { // room that begins journey
        printf("You are at the foot of the tree. It feels as if the way has shut out behind you.\n");
        if (!room->greeted[0]) {
            printf("You see something shining with light moving toward you...\n");
            printf("Greetings adventurer, you must be insert_name. I am a guiding spirit, here to help guide heroes to save Yggdrasil: the mainspring of the universe.\n");
            printf("The great tree is dying due to unknown causes. That is what awakened me, and I have sensed evil and rot in the nearby worlds.\n");
            printf("In order to save Yggdrasil, you must seek out the world essence of the corrupted lands and bring them to the heart of the tree.\n");
            room->greeted[0] = true; // Set greeted to true to avoid repeated greetings
        } else {
            printf("What would you like to do?\n");
            printf("[1] Talk to the guiding spirit\n");
            printf("[2] Move\n");
            char choice;
            scanf(" %c", &choice);
            if (choice == '1') {
                if (worldEssences == 0) {
                    printf("[Guiding Spirit]: You have no world essences. You should explore to the east, west, or south.\n");
                } else {
                    printf("[Guiding Spirit]: You have %d world essences. Keep seeking more essences to save Yggdrasil.\n", worldEssences);
                }
            }
        }
    } else if (room->x == 0 && room->y == 2) {
        printf("You are further up the tree. The branches seem to be closing in in the distance.\n");
    } else if (room->x == -1 && room->y == 1) { // left side
        printf("You travelled to the left; you sense the ruffling of trees far from you.\n");
    } else if (room->x == -2 && room->y == 1) {
        printf("You feel the roots of the tree uprooting around you, you might be close to someplace...\n");
    } else if (room->x == -3 && room->y == 1) {
        printf("You arrive at the great Canopies.\n");
        printf("You sense the words of the guiding spirits. Hear the message [y/n]? ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y') {
            printf("[Guiding Spirit]: The great canopies is one of the worlds that is corrupted.\n");
            printf("[Guiding Spirit]: It is the heart of vegetation of the many worlds.\n");
            printf("[Guiding Spirit]: Without it, the worlds can't regrow and rebuild themselves.\n");
        } else {
            printf("What would you like to do?\n");
        }
    } else if (room->x == -4 && room->y == 1) {
        printf("You sense the presence of vibrating thorns to the left, and purveying laughter to the north.\n");
    } else if (room->x == -5 && room->y == 1) {
        if (!room->puzzles[0]) { // Assuming the first slot in the puzzles array is for this room
            printf("You are in a mysterious room. You must choose a path.\n");
            printf("Choose left or right (l/r): ");
            char choice;
            scanf(" %c", &choice);
            // Randomly determine which path is safe
            bool leftSafe = rand() % 2 == 0;

            if ((choice == 'l' && leftSafe) || (choice == 'r' && !leftSafe)) {
                printf("You chose the safe room.\n");

            } else {
                printf("You chose the thorn room! You take some damage.\n");
                // Add logic for taking damage
            }
            room->puzzles[0] = true; // Mark puzzle as solved
        } else {
            printf("You have already solved the puzzle in this room.\n");
        }
    } else if (room->x == -6 && room->y == 1) { // boss fight

    } else if (room->x == -2 && room->y == 2) {
        printf("You appear in a long winded room.\n");
        if (!room->treasures[0]) { // Assuming the first slot in the treasure array is for this room
            printf("There is a treasure chest at the end of this room: open it[y/n]? ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'y') {
                printf("You found 3 Healing Herbs!\n");
                room->treasures[0] = true; // Mark the treasure as found
                addItemToInventory(inventory, "Healing Herb");
                addItemToInventory(inventory, "Healing Herb");
                addItemToInventory(inventory, "Healing Herb");
            } else {
                printf("You decided to not to open the chest.\n");
            }
        }
    } else if (room->x == 1 && room->y == 1) { // right side
        printf("You travelled to the right; you sense the movement of quiet, yet whimsical water.\n");
    } else if (room->x == 2 && room->y == 1) {
        // Add more rooms and descriptions as needed
    } else if (room->x == 3 && room->y == 1) {
        printf("You arrive at the Great Deep.\n");
        printf("You sense the words of the guiding spirits. Hear the message [y/n]? ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y') {
            printf("[Guiding Spirit]: The great deep is one of the worlds that is corrupted.\n");
            printf("[Guiding Spirit]: It is a source of water for the worlds.\n");
            printf("[Guiding Spirit]: Without it, the worlds can't replenish nutrients and repel waste.\n");
        } else {
            printf("What would you like to do?\n");
        }
    } else if (room->x == 4 && room->y == 1) {
        if (!room->greeted[1]) {
            printf("You see slithering across the calming raging waters.\n");
            printf("[Wandering Mermaid] Hello traveller, I can sense that you are here for a reason?\n");
            printf("The seas have gone rampant; a land creature has no business being here.\n");
            printf("Ahead are whirlpools which will force you to move certain paths. I can help you find the right path.\n");
            room->greeted[1] = true; // Mark as greeted
        } else {
            printf("What would you like to do?\n");
        }
    } else { // void locations
        printf("You are in an unknown room. It seems like it's a dead end.\n");
    }
}

void addItemToInventory(Inventory *inventory, const char *item) {
    if (inventory->itemCount < MAX_ITEMS) {
        strcpy(inventory->items[inventory->itemCount], item);
        inventory->itemCount++;
        printf("Added %s to your inventory.\n", item);
    } else {
        printf("Inventory is full! Cannot add %s.\n", item);
    }
}

void printInventory(const Inventory *inventory) {
    printf("Inventory:\n");
    for (int i = 0; i < inventory->itemCount; i++) {
        printf("%d. %s\n", i + 1, inventory->items[i]);
    }
}
void displayMenu(){
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







