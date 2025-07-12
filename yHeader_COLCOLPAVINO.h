#ifndef yHeader_COLCOLPAVINO_h
#define yHeader_COLCOLPAVINO_h

#include <stdio.h>

#define MAX_IDOLS 8
#define SELECTED_IDOLS 3
#define INVENTORY_SLOTS 3

typedef struct GameState{
    int selectedIdols[SELECTED_IDOLS];
    int doneDungeons[SELECTED_IDOLS];
    int gold;
    int hp;
    int inventory[INVENTORY_SLOTS];
} GameState;

/**
 * Displays the in-dungeon HUD with HP, Gold, and currently equipped item.
 * Precondition: hp, maxHp >= 0; itemName can be NULL if itemQty is 0
 * @param hp current HP
 * @param maxHp maximum HP
 * @param gold Current gold
 * @param itemName name of current item
 * @param itemQty quantity of current item
 * @return none 
 */
 void displayDungeonHUD(int hp, int maxHp, int gold, char* itemName, int itemQty);

 /**
 * Displays post-dungeon rescue and clear feedback
 * Precondition: idolID is within bounds of Idols array
 * @param idolID index of rescued idol
 * @param Idols array of idol names
 * @return none
 */
void postDungeonFeedback(int idolID, const char *Idols[]);

/**
 * Displays the inventory screen
 * Precondition: GameState must be initialized with inventory and gold
 * @param state pointer to current GameState
 * @return none
 */
void displayInventory(GameState *state);

/**
 * Displays achievement status
 * Precondition: earned is an array of size totalAchievements; achievementNames is same length
 * @param earned array of 0 or 1 values
 * @param totalAchievements number of achievements
 * @param achievementNames array of achievement strings
 * @return none
 */
void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]);

/**
 * Hanamaru's shop logic including conditional availability and purchase validation
 * Precondition: state and rescuedIdols must be valid arrays
 * @param state pointer to current GameState
 * @param rescuedIdols array indicating rescued idols
 * @return none
 */
void hanamaruShop(GameState *state, int rescuedIdols[]);

/**
 * Saves game to file
 * Precondition: state, rescuedIdols, achievements are valid pointers
 * @param state pointer to current GameState
 * @param rescuedIdols array of rescued idol flags
 * @param achievements array of earned achievements
 * @return none
 */
void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]);

/**
 * Loads game from file
 * Precondition: pointers are allocated
 * @param state pointer to GameState
 * @param rescuedIdols array of rescued idols
 * @param achievements array of earned achievements
 * @return 1 if successful, 0 if file not found
 */
int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]);

/**
 * Unlocks an achievement and prints message
 * Precondition: earned must be a valid array of size >= index
 * @param earned array of earned achievement flags
 * @param index index of achievement to unlock
 * @param message message to display when unlocked
 * @return none
 */
void unlockAchievement(int earned[], int index, const char *message);

/**
 * Displays dungeon selection screen with available dungeons
 * Precondition: state, rescuedIdols, currentIdols must be valid
 * @param state pointer to current GameState
 * @param rescuedIdols array indicating rescued idols
 * @param currentIdols array of current playthrough idols
 * @return none
 */
void displayDungeonSelection(GameState *state, int rescuedIdols[], int currentIdols[]);

/**
 * Marks a dungeon as cleared
 * Precondition: state must be valid, dungeonIndex must be within bounds
 * @param state pointer to current GameState
 * @param dungeonIndex index of dungeon to mark as cleared
 * @return none 
 */
void markDungeonClear(GameState *state, int dungeonIndex);

/**
 * Initializes final dungeon with character position and switches
 * Precondition: all arrays must be allocated
 * @param yohanePos array for Yohane's position [row, col]
 * @param lailapsPos array for Lailaps' position [row, col]
 * @param switches array for switch states
 * @param sirenPos array for Siren's position [row, col]
 * @return none 
 */
void initializeFinalDungeon(int yohanePos[], int lailapsPos[], int switches[], int sirenPos[]);

/**
 * Displays the final dungeon grid
 * Precondition: all arrays must be valid
 * @param yohanePos Yohane's position
 * @param lailapsPos Lailaps' position
 * @param switches switch states 
 * @param sirenPos siren's position
 * @param grid dungeon grid
 * @return none
 */
void displayFinalDungeon(int yohanePos[], int lailapsPos[], int switches[], int sirenPos[], int grid[][10]);

/**
 * Moves a character based on input direction
 * Precondition: pos must be a valid position array
 * @param pos character position [row, col]
 * @param direction movement direction
 * @param grid dungeon grid
 * @return 1 if move successful, 0 otherwise 
 */
int moveCharacter(int pos[], char direction, int grid[][10]);

/**
 * Checks if switches are activated by character positions
 * Precondition: all arrays must be valid
 * @param yohanePos Yohane's position
 * @param lailapsPos Lailaps' position
 * @param switches switch states
 * @param grid dungeon grid
 * @return 1 if switch activated, 0 otherwise 
 */
int checkSwitchActivation(int yohanePos[], int lailapsPos[], int switches[], int grid[][10]);

/**
 * Checks if Siren can be defeated by Yohane
 * Precondition: position arrays must be valid
 * @param yohanePos Yohane's position
 * @param sirenPos Siren's position
 * @return 1 if defeat possible, 0 otherwise 
 */
int checkSirenDefeat(int yohanePos[], int sirenPos[]);

/**
 * Checks and unlocks rescue-related achievements
 * Precondition: arrays must be valid
 * @param rescuedIdols array of rescuedIdols
 * @param earned array of earned achievements
 * @param currentIdol index of current rescued idol
 * @return none 
 */
void checkRescueAchievements(int rescuedIdols[], int earned[], int currentIdol);

/**
 * Checks and unlocks milestone achievements
 * Precondition: state and earned must be valid
 * @param state pointer to GameState
 * @param earned array of earned achievements
 * @param totalDungeonsCleared total dungeons cleared
 * @return none 
 */
void checkMilestoneAchievements(GameState *state, int earned[], int totalDungeonsCleared);

/**
 * Uses an item from inventory
 * Precondition: state must be valid, itemIndex must be 0-2
 * @param state pointer to GameState
 * @param itemIndex index of item to use
 * @return none 
 */
void useItem(GameState *state, int itemIndex);

/**
 * Carries over progress to new playthrough
 * Precondition: arrays must be valid
 * @param rescuedIdols array of rescued idols
 * @param state pointer to GameState
 * @return none 
 */
void carryOverProgress(int rescuedIdols[], GameState *state);

/**
 * Checks if all idols have been rescued
 * Precondition: rescuedIdols must be a valid array
 * @param rescuedIdols array of rescuedIdols
 * @return 1 if all rescued, 0 otherwise 
 */
int allIdolsRescued(int rescuedIdols[]);

/**
 * Resets idol selection if all idols rescued
 * Precondition: rescuedIdols must be valid array
 * @param rescuedIdols array of rescued idols
 * @return none 
 */
void resetIdolSelectrion(int rescuedIdols[]);

/**
 * Main menu
 */
char MainMenu(int saveGame);

/**
 * New Game / Continue
 */
void setNewGame(GameState *state, const char *idolNames[]);

/**
 * Helpers
 */
int duplicateCheck(int selected[], int count, int val);

#endif