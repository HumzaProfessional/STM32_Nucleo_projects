#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
extern void initialise_monitor_handles(void);

#define MAX_PUZZLES 4
#define MAX_TREASURES 1
#define MAX_ITEMS 10
#define ITEM_NAME_LENGTH 50
#define GREET 4
#define MAP_SIZE 5 // Define the size of the ASCII map

typedef struct { // define the structure of the room to ease use when using the printRoomScenario function
   signed int x;
   signed int y;
    bool puzzles[MAX_PUZZLES]; // array for the puzzles
    bool treasures[MAX_TREASURES]; // array for treasures
    bool greeted[GREET];
} Room;

Room currentRoom = {0, 0}; // Starting room coordinates
Room previousRoom; // To store the previous room coordinates

typedef struct {
    char items[MAX_ITEMS][ITEM_NAME_LENGTH];
    int itemCount;
} Inventory;

Inventory playerInventory = {.itemCount = 0}; // Initialize inventory with 0 items

int worldEssences = 0; // Global variable to track the number of world essences
bool isWizard = false; // Boolean to store character type: false for Warrior, true for Wizard
int difficultyMode = 0; // Integer to store difficulty mode: 0 for Normal, 1 for Hard
int money = 100;
int hp = 100;

void printRoomScenario(Room *room, Inventory *inventory);
void displayMenu();
void addItemToInventory(Inventory *inventory, const char *item);
void printInventory(const Inventory *inventory);
void solveWhirlpoolPuzzle(Room *room);
bool navigateMaze();
void gameOver();
void bossFight(const char *bossName);
void updateMap(char map[30][51], int x, int y);
void useItem();

int main(void) {
    initialise_monitor_handles();
    displayMenu();

    printf("Welcome to the Game:\n");

    // Character selection
    int characterChoice;
    printf("Choose your character:\n");
    printf("[0] Warrior\n");
    printf("[1] Wizard\n");
    scanf("%d", &characterChoice);
    isWizard = (characterChoice == 1);

    // Difficulty selection
    printf("Choose your difficulty mode:\n");
    printf("[0] Normal\n");
    printf("[1] Hard\n");
    scanf("%d", &difficultyMode);

    while (1) {
        // Check if HP is zero
        if (hp <= 0) {
            gameOver();
        }

        printf("Current HP: %d\n", hp); // Display current HP
        printf("Current Money: %d\n", money); // Display current money
        printf("Current Location: (%d, %d)\n", currentRoom.x, currentRoom.y); // Debug print
        printRoomScenario(&currentRoom, &playerInventory);

        char input;
        printf("Enter direction (w/a/s/d) to move, i to view inventory, u to use an item, m to check map, q to quit: ");
        scanf(" %c", &input);

        // Store previous location
        previousRoom = currentRoom;

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
                useItem();
                break;
            case 'm': // Check map
    printf("You check your map. Current location: (%d, %d)\n", currentRoom.x, currentRoom.y);
    char map[30][51] = {
        "                ",
        "                                                  ",
        "                                                  ",
        "                                                  ",
        "                      ||                          ",
        "                      --                          ",
        "                      ##                          ",
        "                      --                          ",
        "                      ||                          ",
        "          --          --                          ",
        "          ||    --    ##                          ",
        "          --    ||    --                          ",
        "          ##          ||                          ",
        "    --*--*--*--*--*--*--*--*--*--*--*--*--*       ",
        "    ||#||#||#||#||#||#||#||#||#||#||#||#||#       ",
        "    --*--*--*--*--*--*--*--*--*--*--*--*--*       ",
        "                   ## || ##                       ",
        "                   ## -- ##                       ",
        "                    ##  ##                        ",
        "                     ####                         ",
        "                      --                          ",
        "                      ||                          ",
        "                      --                          ",
        "                      ##                          ",
        "                      --                          ",
        "                      ||                          ",
        "                      --                          ",
        "                                                  "
    };

    updateMap(map, currentRoom.x, currentRoom.y);

    for (int i = 0; i < 30; i++) {
        printf("%s\n", map[i]);
    }
    break;
            case 'q': // Quit
                return 0;
            case 'g': // Debug mode: go to specific location
                printf("Enter x coordinate: ");
                scanf("%d", &currentRoom.x);
                printf("Enter y coordinate: ");
                scanf("%d", &currentRoom.y);
                break;
            default:
                printf("Invalid input! Please enter w/a/s/d/i/u/m/q or g.\n");
                break;
        }
    }
}
void printRoomScenario(Room *room, Inventory *inventory) {
    if (room->x == 0 && room->y == 0) {
        printf("You are at the entrance of the great tree. The towering size gives a presence of grandeur and omininimity.\n");
    } else if (room->x == 0 && room->y == 1) { // room that begins journey
        printf("You are at the foot of the tree. It feels as if the way has shut out behind you.\n");
        if (!room->greeted[0]) {
            printf("You see something shining with light moving toward you...\n");
            printf("Greetings adventurer, I was expecting you. I am a guiding spirit, here to help guide heroes to save Yggdrasil: the mainspring of the universe.\n");
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
                 } else if (worldEssences == 3) {
                    printf ("[Guiding Spirit]: You have collected 3 world essences! Now you just need to go north from here to restore Yggdrasil! /n");
                } else {
                    printf("[Guiding Spirit]: You have %d world essences. Keep seeking more essences to save Yggdrasil.\n", worldEssences);
                }
            }
        }
    } else if (room->x == 0 && room->y == 2) {
        printf("You are further up the tree. The branches seem to be closing in the distance.\n");
    } else if (room->x == 0 && room->y == 3) {
    printf("You move even further, revealing a complex pattern of thorns on the wall blocking your way.\n");
    if (worldEssences == 3) {
        printf("The walls seem to start glowing from your presence. Would you like to reveal the World Essences?\n");
        printf("[Y]/[N]\n");
        char choice;
        scanf(" %c", &choice); // Reading the player's choice

        if (choice == 'Y' || choice == 'y') {
            printf("The region around you starts to warp and vibrate...\n");
            printf("The guiding spirit appears before you, radiating with light.\n");
            printf("[Guiding Spirit]: You have done it, brave adventurer! The World Essences are reunited.\n");
            printf("[Guiding Spirit]: Yggdrasil will now start to heal, and with it, the universe will begin to restore its balance.\n");
            printf("[Guiding Spirit]: Your journey has saved countless lives and worlds. Thank you.\n");

            // Display a winning message
            const char *youWonArt[] = {
                "*************************************************",
                "*                                               *",
                "*                   YOU WON                     *",
                "*                                               *",
                "*       Yggdrasil will start to heal now.       *",
                "*                                               *",
                "*         The universe will restore balance.    *",
                "*                                               *",
                "*        Your journey has saved countless       *",
                "*           lives and worlds. Thank you.        *",
                "*                                               *",
                "*            _______                            *",
                "*           /       \\                           *",
                "*          /  You    \\                          *",
                "*         /   Won!    \\                         *",
                "*        /_____________\\                        *",
                "*       |               |                       *",
                "*       |               |                       *",
                "*       |_              *",
                "*                                               *",
                "*************************************************"
            };

            const int artSize = sizeof(youWonArt) / sizeof(youWonArt[0]);

            for (int i = 0; i < artSize; i++) {
                printf("%s\n", youWonArt[i]);
            }

            // End the game
            exit(0);
        } else {
            printf("You decide to hold on to the World Essences for now.\n");
        }
    }
} else if (room->x == 1 && room->y == 1) { // right side
        printf("You travelled to the right; you sense the movement of quiet, yet whimsical water.\n");
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
    } else if (room->x == -4 && room->y == 2) { // shop
        printf("You have entered a shop run by a trickster elf.\n");
        printf("The shop is filled with various potions and items. The elf greets you with a mischievous smile.\n");
        printf("Welcome to my shop, traveler! I have some items that might interest you.\n");
        printf("[1] Healing Herb - 10 gold\n");
        printf("[2] Elixir - 20 gold\n");
        printf("[3] Leave the shop\n");

     const char *elfArt[] = {
    "                  ,     ,",
    "    ____     (\\____/)",
    "     --   |  (_o/o_) ",
    "   _/--_\\-|_   (~)",
    "          |   __||__    \\)",
    "          | []/______\\[] /",
    "  --------|/ \\______/ \\/",
    "   Shop    /    /__\\",
    "         (\\   /____\\"
};


        const int elfArtSize = sizeof(elfArt) / sizeof(elfArt[0]);

        for (int i = 0; i < elfArtSize; i++) {
            printf("%s\n", elfArt[i]);
        }

         int shopChoice;
            scanf("%d", &shopChoice);

            switch (shopChoice) {
                case 1:
                    if (money >= 10) {
                        printf("You bought a Healing Herb.\n");
                        addItemToInventory(inventory, "Healing Herb");
                        money -= 10;
                    } else {
                        printf("You don't have enough gold to buy a Healing Herb.\n");
                    }
                    break;
                case 2:
                    if (money >= 20) {
                        printf("You bought an Elixir.\n");
                        addItemToInventory(inventory, "Elixir");
                        money -= 20;
                    } else {
                        printf("You don't have enough gold to buy an Elixir.\n");
                    }
                    break;
                case 3:
                    printf("You leave the shop.\n");
                    break;
                default:
                    printf("Invalid choice! You leave the shop.\n");
                    break;
            }
    } else if (currentRoom.x == -5 && currentRoom.y == 1) {
        if (!currentRoom.puzzles[0]) { // Assuming the first slot in the puzzles array is for this room
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
                hp = hp - 10;
                // Add logic for taking damage
            }
            currentRoom.puzzles[0] = true; // Mark puzzle as solved

            // For hard mode, solve an additional room
            if (difficultyMode == 1 && !currentRoom.puzzles[1]) {
                printf("You must choose another path.\n");
                printf("Choose left or right (l/r): ");
                scanf(" %c", &choice);
                leftSafe = rand() % 2 == 0;
                if ((choice == 'l' && leftSafe) || (choice == 'r' && !leftSafe)) {
                    printf("You chose the safe room.\n");
                } else {
                    printf("You chose the thorn room! You take some damage.\n");
                    hp = hp - 10;
                    // Add logic for taking damage
                }
                currentRoom.puzzles[1] = true; // Mark the second puzzle as solved
            }
        }
    }  else if (currentRoom.x == -6 && currentRoom.y == 1) { // boss fight
            bossFight("The Great Thorns");
    }else if (room->x == -2 && room->y == 2) {
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
    } else if (room->x == 2 && room->y == 1) { // right side
        printf("You sense the presence of raging waters. You might be in a different place if you keep going...\n");
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
            printf("[Wandering Mermaid]: Hello traveller, I can sense that you are here for a reason?\n");
            printf("[Wandering Mermaid]: The seas have gone rampant; a land creature has no business being here.\n");
            printf("[Wandering Mermaid]: Ahead are whirlpools which will force you change your path.\n");
            room->greeted[1] = true; // Mark as greeted
        } else {
            printf("What would you like to do?\n");
        }
    } else if (currentRoom.x == 5 && currentRoom.y == 1) {
        if (!currentRoom.puzzles[2]) {
            solveWhirlpoolPuzzle(&currentRoom);
            currentRoom.puzzles[2] = true;
        } else {
            printf("You have already solved the whirlpool puzzle in this room.\n");
        }
    } else if (currentRoom.x == 6 && currentRoom.y == 1) { // another boss fight
            bossFight("The Great Serpent");
    } else if (room->x == 1 && room->y == 0) { // south
            printf("You travel south sensing a cold presence.\n");
    } else if (room->x == -1 && room->y == 0) { // south
            printf("You travel south sensing a cold presence.\n");
    } else if ((room->x == 1 && room->y == -1) || (room->x == -1 && room->y == -1)) {
            printf("You have entered the Underworld. It's cold and a fog swallows all you can see. You sense a calm stream afar. \n");
            printf("You don't sense any message from the guiding spirit... \n");
    } else if ((room->x == 1 && room->y == -2) || (room->x == -1 && room->y == -2)) {
            printf("You approach the calm stream and see a shadowy figure standing by a boat.\n");
            if (!room->greeted[2]) {
                printf("[Ferryman of the Dead]: Welcome to the Underworld, traveler. I am the Ferryman of the Dead.\n");
                printf("[Ferryman of the Dead]: My task is to ferry souls across this river to their final resting place.\n");
                printf("[Ferryman of the Dead]: This place is at the very roots of Yggdrasil, the spirit contacted me about a traveler...\n");
                printf("[Ferryman of the Dead]: I presume that is you. The rivers of the dead is very hostile at this moment, due to the decay. Not an easy task to navigate for a mortal.\n");
                printf("[Ferryman of the Dead]: I can help you find the world essence of the Underworld through my ferry.\n");
                room->greeted[2] = true; // Mark as greeted
            }
                printf("The Ferryman of the Dead waits silently by his boat.\n");
                printf("[Ferryman of the Dead]: Are you ready to navigate the maze of the Underworld? [y/n]. \n");
                char choice;
                scanf(" %c", &choice);
                if (choice == 'y' || 'Y') {
                    // Engage the maze puzzle
                    if (!room->puzzles[3]) {
                        printf("You step onto the ferry and begin to navigate the maze of the Underworld.\n");
                        bool mazeSolved = navigateMaze(); // Function to handle the maze puzzle
                        if (mazeSolved) {
                            printf("You have successfully navigated the maze and found the World Essence of the Underworld!\n");
                            worldEssences++;
                            printf("The Ferryman returns you to the entrance of the Underworld.\n");
                            currentRoom.x = 1;
                            currentRoom.y = -1;
                            room->puzzles[3] = true; // Mark puzzle as solved
                        } else {
                            printf("You failed to navigate the maze. Try again.\n");
                        }
                    } else {
                        printf("You have already solved the maze puzzle in this room.\n");
                    }
                }
            }
         else { // void locations
            printf("You are in an unknown room. It seems like it's a dead end. You turn back.\n");
            *room = previousRoom; // Return to the original position
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

void solveWhirlpoolPuzzle(Room *room) {
    // Define the ASCII maps for the whirlpool puzzle
    char easyMap[MAP_SIZE][MAP_SIZE] = {
        {'S', '.', '.', 'W', '.'},
        {'.', 'W', '.', 'W', '.'},
        {'W', '.', '.', '.', '.'},
        {'.', 'W', '.', 'W', '.'},
        {'.', '.', 'W', '.', 'E'}
    };

    char hardMap[MAP_SIZE][MAP_SIZE] = {
        {'S', 'W', '.', 'W', '.'},
        {'.', 'W', '.', 'W', 'W'},
        {'W', '.', 'W', '.', '.'},
        {'.', 'W', '.', 'W', '.'},
        {'W', '.', 'W', 'W', 'E'}
    };

    // Select the appropriate map based on difficulty mode
    char (*map)[MAP_SIZE];
    if (difficultyMode == 0) {
        map = easyMap;
    } else {
        map = hardMap;
    }

    // Player's initial position
    int playerX = 0;
    int playerY = 0;

    // Display the map
    printf("Whirlpool Puzzle:\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (i == playerX && j == playerY) {
                printf("P "); // Player's position
            } else {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }

    // Puzzle loop
    while (1) {
        printf("Move (w/a/s/d): ");
        char move;
        scanf(" %c", &move);

        // Update player position based on input
        int newX = playerX;
        int newY = playerY;
        if (move == 'w' && newX > 0) newX--;
        if (move == 'a' && newY > 0) newY--;
        if (move == 's' && newX < MAP_SIZE - 1) newX++;
        if (move == 'd' && newY < MAP_SIZE - 1) newY++;

        // Check for whirlpools
        if (map[newX][newY] == 'W') {
            printf("You are caught in a whirlpool!\n");
            // Move player based on whirlpool rules (example: move to a random location)
            newX = rand() % MAP_SIZE;
            newY = rand() % MAP_SIZE;
        }

        playerX = newX;
        playerY = newY;

        // Display the map
        printf("Whirlpool Puzzle:\n");
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                if (i == playerX && j == playerY) {
                    printf("P "); // Player's position
                } else {
                    printf("%c ", map[i][j]);
                }
            }
            printf("\n");
        }

        // Check if player has reached the end
        if (map[playerX][playerY] == 'E') {
            printf("You have reached the end of the map! Puzzle solved.\n");
            break;
        }
    }
}
bool navigateMaze() {
    // Implement a simple text-based maze navigation
    // Example maze representation (1 = wall, 0 = path)
    int maze[5][5] = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1}
    };

    int x = 1, y = 1; // Starting position in the maze
    int endX = 3, endY = 3; // Ending position in the maze

    char viewMapChoice;
    printf("[Ferryman of the dead]: Would you like to see my estimation of the river path before starting? \n"); // There is no hard verison, since I think the maze would be too hard
    printf("[Y/N] \n");
    scanf(" %c", &viewMapChoice);

    if (viewMapChoice == 'Y' || viewMapChoice == 'y') {
        printf("Maze Map:\n");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (i == y && j == x) {
                    printf("S "); // Starting position
                } else if (i == endY && j == endX) {
                    printf("E "); // Ending position
                } else {
                    printf("%d ", maze[i][j]);
                }
            }
            printf("\n");
        }
        printf("[Ferryman of the dead]: According to my vision, the different numbers represent either a path or wall. \n");
    }

    while (x != endX || y != endY) {
        printf("You are in the maze. Choose a direction to move (w/a/s/d): ");
        char move;
        scanf(" %c", &move);

        switch (move) {
            case 'w':
                if (maze[y - 1][x] == 0) y--;
                else printf("You hit a wall. Try another direction.\n");
                break;
            case 'a':
                if (maze[y][x - 1] == 0) x--;
                else printf("You hit a wall. Try another direction.\n");
                break;
            case 's':
                if (maze[y + 1][x] == 0) y++;
                else printf("You hit a wall. Try another direction.\n");
                break;
            case 'd':
                if (maze[y][x + 1] == 0) x++;
                else printf("You hit a wall. Try another direction.\n");
                break;
            default:
                printf("Invalid input! Please enter w/a/s/d.\n");
                break;
        }

        printf("Current position: (%d, %d)\n", x, y);
    }

    printf("Congratulations! You have navigated the maze successfully.\n");
    return true;
}
void bossFight(const char *bossName) {
    printf("You encounter %s!\n", bossName);

    int damageTaken = isWizard ? 20 : 10; // Damage taken by character class
    int firestormCount = 0, thunderboltCount = 0, slashCount = 0, pierceCount = 0;
    int requiredFirestorm, requiredThunderbolt, requiredSlash, requiredPierce;

    // Set the required number of attacks based on the boss and difficulty mode
    if (strcmp(bossName, "The Great Thorns") == 0) {
        requiredFirestorm = (difficultyMode == 0) ? 1 : 2;
        requiredThunderbolt = (difficultyMode == 0) ? 5 : 9;
        requiredSlash = (difficultyMode == 0) ? 2 : 3;
        requiredPierce = (difficultyMode == 0) ? 2 : 3;
    } else if (strcmp(bossName, "The Great Serpent") == 0) {
        requiredFirestorm = (difficultyMode == 0) ? 3 : 6;
        requiredThunderbolt = (difficultyMode == 0) ? 2 : 2;
        requiredSlash = (difficultyMode == 0) ? 1 : 2;
        requiredPierce = (difficultyMode == 0) ? 3 : 4;
    }

    bool bossDefeated = false;

    while (!bossDefeated) {
        printf("Choose your action:\n");
        printf("[1] Attack\n");
        printf("[2] Use Item\n");
        int actionChoice;
        scanf("%d", &actionChoice);

        if (actionChoice == 2) {
            printf("Select an item number to use: ");
            int itemIndex;
            scanf("%d", &itemIndex);
            if (itemIndex > 0 && itemIndex <= playerInventory.itemCount) {
                printf("You used %s.\n", playerInventory.items[itemIndex - 1]);
                // Apply item effects (e.g., healing)
                if (strcmp(playerInventory.items[itemIndex - 1], "Healing Herb") == 0) {
                    hp += 20; // Example healing amount
                    printf("You restored 20 HP. Current HP: %d\n", hp);
                } else if (strcmp(playerInventory.items[itemIndex - 1], "Elixir") == 0) {
                    hp += 40; // Example healing amount
                    printf("You restored 50 HP. Current HP: %d\n", hp);
                }

                // Remove the item from the inventory
                for (int i = itemIndex - 1; i < playerInventory.itemCount - 1; i++) {
                    strcpy(playerInventory.items[i], playerInventory.items[i + 1]);
                }
                playerInventory.itemCount--;
            } else {
                printf("Invalid item number.\n");
            }
            continue;
        }

        if (isWizard) {
            printf("As a Wizard, you decide to use your spells to fight %s.\n", bossName);
            printf("Choose your spell:\n");
            printf("[1] Firestorm\n");
            printf("[2] Thunderbolt\n");
            int spellChoice;
            scanf("%d", &spellChoice);

            switch (spellChoice) {
                case 1:
                    printf("You cast Firestorm!\n");
                    firestormCount++;
                    if (strcmp(bossName, "The Great Thorns") == 0) {
                        printf("%s is burned by the Firestorm and is weak!\n", bossName);
                    } else {
                        printf("%s takes some damage from the Firestorm.\n", bossName);
                    }
                    break;
                case 2:
                    printf("You cast Thunderbolt!\n");
                    thunderboltCount++;
                    if (strcmp(bossName, "The Great Serpent") == 0) {
                        printf("%s is shocked by the Thunderbolt and is weak!\n", bossName);
                    } else {
                        printf("%s takes some damage from the Thunderbolt.\n", bossName);
                    }
                    break;
                default:
                    printf("Invalid spell choice. The boss attacks you!\n");
                    hp -= damageTaken;
                    if (hp <= 0) {
                        printf("You have been defeated by %s...\n", bossName);
                        gameOver();
                        return;
                    }
                    continue;
            }
        } else {
            printf("As a Warrior, you decide to use your strength to fight %s.\n", bossName);
            printf("Choose your attack:\n");
            printf("[1] Slash\n");
            printf("[2] Pierce\n");
            int attackChoice;
            scanf("%d", &attackChoice);

            switch (attackChoice) {
                case 1:
                    printf("You swing your sword with a Slash attack!\n");
                    slashCount++;
                    if (strcmp(bossName, "The Great Serpent") == 0) {
                        printf("%s is cut by your Slash and is weak!\n", bossName);
                    } else {
                        printf("%s takes some damage from the Slash.\n", bossName);
                    }
                    break;
                case 2:
                    printf("You thrust your sword with a Pierce attack!\n");
                    pierceCount++;
                    if (strcmp(bossName, "The Great Thorns") == 0) {
                        printf("%s is pierced by your attack and is weak!\n", bossName);
                    } else {
                        printf("%s takes some damage from the Pierce.\n", bossName);
                    }
                    break;
                default:
                    printf("Invalid attack choice. The boss attacks you!\n");
                    hp -= damageTaken;
                    if (hp <= 0) {
                        printf("You have been defeated by %s...\n", bossName);
                        gameOver();
                        return;
                    }
                    continue;
            }
        }

        printf("%s attacks you back!\n", bossName);
        hp -= damageTaken;
        if (hp <= 0) {
            printf("You have been defeated by %s...\n", bossName);
            gameOver();
            return;
        }

        // Check if the boss is defeated
        if ((firestormCount >= requiredFirestorm || thunderboltCount >= requiredThunderbolt) ||
            (slashCount >= requiredSlash || pierceCount >= requiredPierce)) {
            bossDefeated = true;
        }
    }

    printf("You have defeated %s and found the World Essence!\n", bossName);
    worldEssences++;
    printf("You are transported back to the foot of the tree.\n");
    currentRoom.x = 0;
    currentRoom.y = 1;
}

void updateMap(char map[30][51], int x, int y) {
    // Convert x, y coordinates to map index and place 'X' in the map
    // Adjust according to your game's coordinate system and map layout
    int mapX = x + 25; // Example conversion, adjust as needed
    int mapY = 15 - y; // Example conversion, adjust as needed

    if (mapY >= 0 && mapY < 30 && mapX >= 0 && mapX < 50) {
        if (map[mapY][mapX] == '|' && map[mapY][mapX + 2] == '|') {
            map[mapY][mapX + 1] = 'X';
        }
    }
}
void useItem() {
    printf("Select an item number to use: ");
    int itemIndex;
    scanf("%d", &itemIndex);
    if (itemIndex > 0 && itemIndex <= playerInventory.itemCount) {
        printf("You used %s.\n", playerInventory.items[itemIndex - 1]);
        // Apply item effects (e.g., healing)
        if (strcmp(playerInventory.items[itemIndex - 1], "Healing Herb") == 0) {
            hp += 20; // Example healing amount
            printf("You restored 20 HP. Current HP: %d\n", hp);
        } else if (strcmp(playerInventory.items[itemIndex - 1], "Elixir") == 0) {
            hp += 40; // Example healing amount
            printf("You restored 50 HP. Current HP: %d\n", hp);
        }

        // Remove the item from the inventory
        for (int i = itemIndex - 1; i < playerInventory.itemCount - 1; i++) {
            strcpy(playerInventory.items[i], playerInventory.items[i + 1]);
        }
        playerInventory.itemCount--;
    } else {
        printf("Invalid item number.\n");
    }
}

void displayMenu(){
    const char *art[] = {
        "*************************************************",
        "*                 Remnant of Yggdrasil          *",
        "*  _____                                       *",
        "* |  __ \\                                                      *",
        "* | |__) |  ___     |~~~~~~    _____ ____       __     _         |__  _ _|*",
        "* |  _  /  / _ \\   |  || |   | / \\   | ||    / _ \\   || \\   __    |  | *",
        "* | | \\ \\ |  __/  | |  | | | \\   / /  ||  /  | - || || | | ||    |  | *",
        "* |_|  \\_\\|_____ | |   | | | /   \\/   |\\ / | / \\ |  ||  \\ |_|    |  | *",
        "*                                                              *",
        "*  ____       ______                                   *",
        "* |  _ \\   |   ___\\                                   *",
        "* | | | |   |  |__                                      *",
        "* | | | |  | _____|                                       *",
        "* | |_| || | |                                           *",
        "* |_   _|  |_|                                                   *",
        "*  -_ _-                                                       *",
        "*  __     __                                                    *",
        "*  \\ \\   / /                                                          *",
        "*   \\ \\_/ /    ___     __ _    ___ __ _                 _____    ___      _   *",
        "*    \\   /     / _` |  / _` |  | __| )  )     / _ \\    / ____|    \\__/    | |*",
        "*     | |      / ( | |  /( | |  |   __  |     / /_\\ \\  | |____      _      | | *",
        "*     |_|      \\__,   \\__, |  |  |  \\ \\    /   _   \\  \\____||    | |     | | *",
        "*               __/ |   __/ |   | |   \\  \\  / _ /  \\_\\   ____/|   |   |    |  |__ *",
        "*               |___/   |___/                           _____/   |_____|   |_____|     *",
        "*                                                                                     *",
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
        "*                                                           \\/ ._\\\\//_/__/  ,\\\\_//__\\/.  \\\\_//__/ *",
        "*************************************************"
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

void gameOver() {
    const char *art[] = {
        "*************************************************",
        "*                                               *",
        "*                 GAME OVER                     *",
        "*                                               *",
        "*       Yggdrasil has succumbed to decay.       *",
        "*                                               *",
        "*          -    --.                             *",
        "*           \\- - - ,---.'. .--.                  *",
        "*            /  Y    Y    \\                      *",
        "*            |  \\--. |   |                       *",
        "*             \\ '\\   \\`    /    /            *",
        "*              `-._ \\ `--'           /           *",
        "*                  `- Y.            /            *",
        "*                    `.           /-             *",
        "*                     \\   ,--,    -              *",
        "*                      \\_/   \\_ -               *",
        "*                       |     |                 *",
        "*                       |     |                 *",
        "*                       |     |                 *",
        "*                      /       \\                *",
        "*                     /         \\               *",
        "*                    |           |              *",
        "*                    |           |              *",
        "*                    |           |              *",
        "*                   /             \\             *",
        "*                 _/               \\            *",
        "*               /                   \\_          *",
        "*              /                      \\_        *",
        "*            _/                         \\       *",
        "*          _/                            \\      *",
        "*         /                               \\     *",
        "*        /                                 \\    *",
        "*                                               *",
        "*************************************************"
    };

    const int artSize = sizeof(art) / sizeof(art[0]);

    for (int i = 0; i < artSize; i++) {
        printf("%s\n", art[i]);
    }

    printf("Press any key to exit...\n");
    getchar();
    exit(0);
}



