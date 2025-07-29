// #ifndef YHEADER_COLCOLPAVINO_H
// #define YHEADER_COLCOLPAVINO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_IDOLS 8
#define SELECTED_IDOLS 3
#define INVENTORY_SLOTS 3
#define MAX_BATS 15
#define ROWS 10
#define COLS 55
#define MAX_ITEMS 25

// Tile Definitions
#define TILE_BORDER '*'
#define TILE_PASSABLE '.'
#define TILE_SPIKE 'x'
#define TILE_WATER 'w'
#define TILE_HEAT 'h'
#define TILE_EXIT 'E'
#define TILE_BAT 'b'
#define TILE_YOHANE 'Y'
#define TILE_TREASURE 'T'
#define TILE_WALL 'v'

// Final Dungeon Definitions
#define MAX_SWITCHES 3

// Item IDs
#define ITEM_TEARS 1
#define ITEM_NOPPO 2
#define ITEM_SHOVEL 3
#define ITEM_BAT 4
#define ITEM_AIR 5
#define ITEM_STEWSHINE 6
#define ITEM_MIKAN 7
#define ITEM_KUROSAWA 8
#define ITEM_CHOCO 9 // M

typedef struct {
    int selectedIdols[SELECTED_IDOLS];
    int doneDungeons[SELECTED_IDOLS];
    int gold;
    float hp;
    int maxHP;
    int inventory[MAX_ITEMS];
    int currentItem;
    int usedChoco;
    int isGameOver;
    int batMoveCounter;
    char deathReason[30];
    int totalShopSpent;
    int povertyMode;
} GameState; // M & L

typedef struct {
    int x, y;
    int alive;
    int attack;
} bat; // M

typedef struct {
    char map[ROWS][COLS];
    int floor;
    int maxFloor;
    int yohaneX;
    int yohaneY;
    bat bats[MAX_BATS];
} Dungeon; // M

typedef struct {
    char name[50];
    int cost;
    int unlockIdol;
    int inventoryIndex;
    char description[100];
} ShopItem; // L

typedef struct {
    int x, y;
} Position;

// Game Loop & Save/Load

/* Starts the main game loop that controls the dungeon traversal, achievements, and idol rescue.
Precondition: A valid GameState pointer and properly initialized arrays for rescued idols, achievements, dungeon names, and idol names must be passed.
@param state Pointer to the current GameState structure.
@param rescuedIdols Array indicating which idols have been rescued.
@param achievements Array tracking earned achievements.
@param dungeonNames Array of strings representing dungeon names.
@param finalBossVictories Pointer to the number of final boss victories.
@param idolNames Array of strings representing idol names.
@return None */
void startGameLoop(GameState *state, int rescuedIdols[], int achievements[], const char *dungeonNames[], int *finalBossVictories, const char *idolNames[]);

/* Saves the current game state to a file for future continuation.
Precondition: A valid GameState and associated arrays must be passed.
@param state Pointer to the current GameState structure.
@param rescuedIdols Array indicating which idols have been rescued.
@param achievements Array tracking earned achievements.
@return None */
void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]);

/* Loads the game state from a previously saved file.
Precondition: Valid pointers must be passed to populate with loaded game state data.
@param state Pointer to the GameState structure to populate.
@param rescuedIdols Array to populate with rescued idols status.
@param achievements Array to populate with earned achievements.
@return 1 if loading is successful, 0 otherwise */
int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]);

// Game Initialization

/* Initializes a new game by resetting the GameState and selecting new idols to rescue.
Precondition: idolNames must contain valid names of idols; GameState must be a valid pointer.
@param state Pointer to the GameState structure to initialize.
@param idolNames Array of strings containing all idol names.
@return None */
void setNewGame(GameState *state, const char *idolNames[]);

/* Transfers progress data such as rescued idols, gold, and items into a new playthrough.
Precondition: A valid GameState pointer with values from the previous playthrough.
@param state Pointer to the GameState structure to carry over progress.
@return None */
void carryOverProgress(GameState *state);

/* Checks if a value is already present in a selection array to prevent duplicates.
Precondition: Array must contain only integer values, and count must reflect actual size.
@param selected Array of selected values to be checked.
@param count Number of current elements in the selected array.
@param val The value to check for duplication.
@return 1 if val is already in the array, 0 otherwise */
int duplicateCheck(int selected[], int count, int val);

// Dungeon

/* Initializes a dungeon layout, populating it with traps, enemies, and the player’s starting position.
Precondition: The Dungeon pointer must be valid; currentDungeon must be a valid index (0 to 2).
@param dungeon Pointer to the Dungeon structure to initialize.
@param currentDungeon Index of the currently selected dungeon.
@return None */
void startDungeon(Dungeon *dungeon, int currentDungeon);

/* Displays the dungeon selection menu and handles user input for dungeon traversal, inventory, shop, or save.
Precondition: GameState and dungeonNames must be initialized properly.
@param state Pointer to the current GameState.
@param dungeonNames Array of dungeon names based on idol mapping.
@return None */
void dungeonMenu(GameState *state, const char *dungeonNames[]);

/* Executes player movement or action based on input and updates the dungeon and game state.
Precondition: Dungeon and GameState pointers must be valid. Input should be a movement/action key.
@param input Character input ('W', 'A', 'S', 'D', 'X', '[', ']', or ' ').
@param dungeon Pointer to the current Dungeon structure.
@param state Pointer to the current GameState.
@param currentDungeon Index of the current dungeon.
@param rescuedIdols Array indicating which idols have been rescued.
@param idolNames Array of idol names.
@return None */
void movement(char input, Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], const char *idolNames[]);

/* Displays the current dungeon floor layout along with player stats.
Precondition: All parameters must be initialized and valid.
@param dungeon Pointer to the current Dungeon.
@param state Pointer to the GameState.
@param dungeonNumber Index of the current dungeon.
@param dungeonName Array of dungeon name strings.
@return None */
void displayDungeon(Dungeon *dungeon, GameState *state, int dungeonNumber, const char *dungeonName[]);

/* Checks if the current floor is the last floor and updates to the next if not.
Precondition: Dungeon structure must be valid and floor must not exceed maxFloor.
@param dungeon Pointer to the Dungeon structure.
@return 1 if dungeon is finished, 0 otherwise */
int nextFloor(Dungeon *dungeon);

/* Clears the dungeon map and sets up the dungeon border and default floor layout.
Precondition: Dungeon pointer must be valid.
@param dungeon Pointer to the Dungeon structure to generate.
@return None */
void generateEmptyDungeon(Dungeon *dungeon);

/* Randomly places a single tile of a given type in the dungeon.
Precondition: Dungeon pointer and tile character must be valid.
@param dungeon Pointer to the Dungeon structure.
@param tile Character representing the tile to place (e.g., 'T', 'x', etc.).
@return None */
void randomTile(Dungeon *dungeon, char tile);

/* Places a given number of tiles of a specified type at random locations in the dungeon.
Precondition: Dungeon pointer must be valid; tile must be a valid character.
@param dungeon Pointer to the Dungeon structure.
@param tile Character representing the tile to place.
@param count Number of tiles to place.
@return None */
void placeRandomTile(Dungeon *dungeon, char tile, int count);

/* Main gameplay loop for dungeon traversal, handling movement, item usage, and battle events.
Precondition: All pointers must be valid; currentDungeon must be within the selected idol count.
@param dungeon Pointer to the Dungeon being traversed.
@param state Pointer to the GameState.
@param currentDungeon Index of the current dungeon.
@param rescuedIdols Array indicating which idols have been rescued.
@param achievements Array tracking earned achievements.
@param dungeonName Array of dungeon name strings.
@param idolNames Array of idol names.
@return None */
void dungeonLoop(Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], int achievements[], const char *dungeonName[], const char *idolNames[]);

// Final Dungeon

/* Starts the final dungeon sequence, initializing the grid and starting the boss battle.
Precondition: GameState and achievement arrays must be initialized. finalBossVictories must be valid.
@param state Pointer to the current GameState.
@param achievements Array for tracking achievements.
@param finalBossVictories Pointer to counter of how many times the final boss was defeated.
@return None */
void startFinalDungeon(GameState *state, int achievements[], int *finalBossVictories);

/* Initializes the starting positions of Yohane, Lailaps, the Siren, and switches for the final dungeon.
Precondition: All arrays must be valid and able to hold position data.
@param yohanePos Array of size 2 to store Yohane's x and y position.
@param lailapsPos Array of size 2 to store Lailaps' x and y position.
@param switches Array of size 6 (3 switch pairs) to store switch coordinates.
@param sirenPos Array of size 2 to store the Siren’s x and y position.
@return None */
void initializeFinalDungeon(int yohanePos[], int lailapsPos[], int switches[], int sirenPos[]);

/* Displays the grid of the final dungeon including positions of Siren, players, and switches.
Precondition: All input arrays and grid must be initialized.
@param yohanePos Array containing Yohane’s position.
@param lailapsPos Array containing Lailaps’ position.
@param sirenPos Array containing Siren’s position.
@param grid 2D grid representing the final dungeon layout.
@return None */
void displayFinalDungeon(int yohanePos[], int lailapsPos[], int sirenPos[], int grid[ROWS][COLS]);

/* Moves both Yohane and Lailaps simultaneously based on player input, considering obstacles.
Precondition: Position arrays and grid must be initialized properly.
@param input Character representing direction ('W', 'A', 'S', 'D', or 'X').
@param yohanePos Array containing Yohane’s position.
@param lailapsPos Array containing Lailaps’ position.
@param grid 2D array representing dungeon layout.
@return None */
void moveYohaneAndLailaps(char input, int yohanePos[], int lailapsPos[], int grid[ROWS][COLS]);

/* Moves the Siren in a direction towards Yohane or Lailaps after switch sequence is complete.
Precondition: Position arrays must be valid; GameState must be active.
@param sirenPos Array containing Siren’s position.
@param yohanePos Array containing Yohane’s position.
@param lailapsPos Array containing Lailaps’ position.
@param state Pointer to the current GameState.
@return None */
void moveSiren(int sirenPos[], int yohanePos[], int lailapsPos[], GameState *state);

/* Checks whether the current switch pair is activated by Yohane and Lailaps.
Precondition: All arrays and grid must be initialized and contain valid positions.
@param yohanePos Array containing Yohane’s position.
@param lailapsPos Array containing Lailaps’ position.
@param switches Array of switch coordinates.
@param grid Final dungeon grid.
@return 1 if the switch pair is activated, 0 otherwise */
int checkSwitchActivation(int yohanePos[], int lailapsPos[], int switches[], int grid[ROWS][COLS]);

/* Checks whether the Siren has been defeated by being attacked by Yohane.
Precondition: Positions must be valid and adjacent.
@param yohanePos Array containing Yohane’s position.
@param sirenPos Array containing Siren’s position.
@return 1 if Siren is defeated, 0 otherwise */
int checkSirenDefeat(int yohanePos[], int sirenPos[]);

/* Handles victory conditions after the Siren is defeated and updates achievements.
Precondition: All pointers must be valid; dungeon should be cleared.
@param state Pointer to the GameState.
@param earned Array of earned achievements.
@param finalDungeon Pointer to the final Dungeon.
@param finalBossVictories Pointer to count of final boss victories.
@return None */
void handleSirenDefeat(GameState *state, int earned[], Dungeon *finalDungeon, int *finalBossVictories);

/* Spawns a bat on a passable tile in the final dungeon every 8 moves based on switch progress.
Precondition: Grid and bat array must be valid; positions must not overlap with switches.
@param grid Final dungeon grid.
@param bats Array of bat structs.
@param switchesActivated Number of switch pairs activated.
@param sirenPos Position of the Siren.
@param yohanePos Position of Yohane.
@param lailapsPos Position of Lailaps.
@return None */
void spawnBatFinalDungeon(int grid[ROWS][COLS], bat bats[], int sirenPos[], int yohanePos[], int lailapsPos[]);

/* Controls the behavior and movement of bats in the final dungeon based on switch progress.
Precondition: All pointers and arrays must be valid; bats must be initialized.
@param bats Array of bat structs.
@param grid Final dungeon grid.
@param state Pointer to the current GameState.
@param yohanePos Position of Yohane.
@param lailapsPos Position of Lailaps.
@param switchesActivated Number of switches activated.
@return None */
void moveFinalDungeonBats(bat bats[], int grid[ROWS][COLS], GameState *state, int yohanePos[], int lailapsPos[], int switchesActivated);

// Bat Functions

/* Spawns a specified number of bats at random valid positions within the dungeon.
Precondition: Dungeon pointer must be valid; batCount must be within MAX_BATS.
@param dungeon Pointer to the Dungeon structure.
@param batCount Number of bats to place in the dungeon.
@return None */
void randomBat(Dungeon *dungeon, int batCount);

/* Resets all bat positions and statuses in the dungeon.
Precondition: Dungeon pointer must be valid and bats array must be initialized.
@param dungeon Pointer to the Dungeon structure.
@return None */
void resetBat(Dungeon *dungeon);

/* Updates the movement and behavior of all bats within the dungeon based on dungeon level.
Precondition: All pointers must be valid; batMoveCounter must reflect bat behavior rules.
@param dungeon Pointer to the Dungeon structure containing the bats.
@param state Pointer to the current GameState.
@param currentDungeon Index of the current dungeon being explored (affects bat behavior).
@return None */
void BatMovement(Dungeon *dungeon, GameState *state, int currentDungeon);

// Inventory

/* Displays the player’s current inventory including item names and quantities.
Precondition: GameState pointer must be valid and inventory must be populated.
@param state Pointer to the current GameState.
@return None */
void displayInventory(GameState *state);

/* Switches the currently selected item in the inventory based on direction.
Precondition: GameState pointer must be valid; direction must be either '[' or ']'.
@param state Pointer to the current GameState.
@param direction Character representing the direction to switch items ('[' for previous, ']' for next).
@return None */
void switchItem(GameState *state, char direction);

/* Uses the currently selected item in hand and applies its effect.
Precondition: GameState must be valid and currentItem must point to an item slot.
@param state Pointer to the current GameState.
@return None */
void useItemInHand(GameState *state);

/* Uses a specific item from inventory by its item ID and applies its effect accordingly.
Precondition: GameState must be valid; item must exist in inventory.
@param state Pointer to the current GameState.
@param item Item ID to be used.
@return None */
void useItem(GameState *state, int item);

/* Checks whether a specific passive item is currently held by the player.
Precondition: GameState and itemID must be valid.
@param state Pointer to the current GameState.
@param itemID ID of the passive item to check.
@return 1 if item is present, 0 otherwise */
int hasPassiveItem(GameState *state, int itemID);

/* Counts the quantity of a specific item in the player’s inventory.
Precondition: GameState pointer must be valid.
@param state Pointer to the current GameState.
@param itemID ID of the item to count.
@return Total number of the item in inventory */
int countItem(GameState *state, int itemID);

/* Retrieves the name and quantity of an item in the inventory for display purposes.
Precondition: GameState pointer must be valid. itemID must be within range.
@param state Pointer to the current GameState.
@param itemID ID of the item to get info about.
@param name Output parameter for the item name.
@param qty Output parameter for item quantity.
@return None */
void getItemInfo(GameState *state, int itemID, char *name, int *qty);

// Shop

/* Opens Hanamaru’s shop interface, allowing item purchases if available and unlocked.
Precondition: GameState, rescuedIdols, and achievements arrays must be valid; Hanamaru must be rescued.
@param state Pointer to the current GameState.
@param rescuedIdols Array indicating which idols have been rescued.
@param achievements Array tracking earned achievements.
@return None */
void hanamaruShop(GameState *state, int rescuedIdols[], int achievements[]);

// Death & Revive

/* Checks if Yohane has a Choco-Mint Ice Cream in inventory and revives her if fatal damage is taken.
Precondition: GameState pointer must be valid and inventory must be checked for revival item.
@param state Pointer to the current GameState.
@return None */
void checkChocoRevive(GameState *state);

/* Triggers a game over state, setting Yohane's HP to 0 and storing the reason for death.
Precondition: GameState pointer must be valid; cause must be a valid string.
@param state Pointer to the current GameState.
@param cause String describing the reason for death (e.g., "Bat", "Spike", etc.).
@return None */
void triggerDeath(GameState *state, const char *cause);

// Achievements

/* Displays a list of all achievements with their earned status.
Precondition: All arrays must be valid and properly sized.
@param earned Array indicating which achievements have been earned.
@param totalAchievements Total number of achievements in the game.
@param achievementNames Array of achievement names.
@return None */
void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]);

/* Unlocks a specific achievement and displays an in-game message.
Precondition: Index must be within valid range; message must be a valid string.
@param earned Array indicating earned achievements.
@param index Index of the achievement to unlock.
@param message Description to display when unlocked.
@return None */
void unlockAchievement(int earned[], int index, const char *message);

/* Checks and unlocks the achievement for rescuing a specific idol.
Precondition: rescuedIdols and earned arrays must be valid.
@param rescuedIdols Array of rescued idols.
@param earned Array of earned achievements.
@param currentIdol Index of the idol that was just rescued.
@return None */
void checkRescueAchievements(int rescuedIdols[], int earned[], int currentIdol);

/* Checks and unlocks milestone achievements based on number of dungeons cleared.
Precondition: GameState and achievement arrays must be valid.
@param state Pointer to the current GameState.
@param earned Array of earned achievements.
@param totalDungeonsCleared Total number of dungeons the player has cleared.
@return None */
void checkMilestoneAchievements(GameState *state, int earned[], int totalDungeonsCleared);

/* Unlocks achievement for completing a dungeon without taking any damage.
Precondition: dungeonDamage must be valid (e.g., 0 for flawless run).
@param dungeonDamage Damage taken during the dungeon.
@param earned Array of earned achievements.
@return None */
void checkDamageLessDungeonAchievement(int dungeonDamage, int earned[]);

/* Checks and unlocks achievement for cumulative spending in Hanamaru’s shop.
Precondition: totalShopSpending must reflect all playthroughs.
@param totalShopSpending Total GP spent in shop.
@param earned Array of earned achievements.
@return None */
void checkShopSpendingAchievement(int totalShopSpending, int earned[]);

/* Checks if the player has ever been revived by Choco-Mint Ice Cream and unlocks the related achievement.
Precondition: earned array must be valid.
@param earned Array of earned achievements.
@return None */
void checkChocoMintSaveAchievement(int earned[]);

/* Checks and unlocks achievement for defeating the final boss a certain number of times.
Precondition: finalBossVictories must reflect all completed playthroughs.
@param finalBossVictories Total number of final boss victories.
@param earned Array of earned achievements.
@return None */
void checkFinalBossAchievement(int finalBossVictories, int earned[]);

// Utility

/* Displays the dungeon selection menu, including cleared and available dungeons.
Precondition: All arrays and pointers must be valid.
@param state Pointer to the current GameState.
@param rescuedIdols Array indicating which idols have been rescued.
@param currentIdols Array of currently selected idols for this playthrough.
@param dungeonName Array of strings representing dungeon names.
@return None */
void displayDungeonSelection(GameState *state, int rescuedIdols[], int currentIdols[], const char* dungeonName[]);

/* Checks if all dungeons for the current playthrough have been cleared.
Precondition: GameState must be valid and dungeon progress must be tracked correctly.
@param state Pointer to the current GameState.
@return 1 if all dungeons are cleared, 0 otherwise */
int checkAllDungeonsCleared(GameState *state);

/* Marks a specific dungeon as cleared in the GameState.
Precondition: GameState must be valid; dungeonIndex must be within valid range.
@param state Pointer to the current GameState.
@param dungeonIndex Index of the dungeon to mark as completed.
@return None */
void markDungeonClear(GameState *state, int dungeonIndex);

/* Displays a character profile based on selected idol.
Precondition: choice must be within the index of the idol list.
@param choice Index of the idol to display.
@return None */
void characterProfile(int choice);

/* Checks whether all 8 Aqours idols have already been rescued.
Precondition: rescuedIdols array must be valid and properly tracked.
@param rescuedIdols Array indicating rescued idol status.
@return 1 if all idols have been rescued at least once, 0 otherwise */
int allIdolsRescued(int rescuedIdols[]);

/* Resets all idol selection flags to allow re-selection on the next playthrough.
Precondition: rescuedIdols array must be valid.
@param rescuedIdols Array to reset to zero.
@return None */
void resetIdolSelection(int rescuedIdols[]);

// Post Dungeon

/* Displays feedback after a dungeon is cleared and an idol is rescued.
Precondition: idolID must be valid and correspond to an index in idolNames.
@param idolID ID of the rescued idol.
@param idolNames Array of idol name strings.
@return None */
void postDungeonFeedback(int idolID, const char *idolNames[]);

// #endif