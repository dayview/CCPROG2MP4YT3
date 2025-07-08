#ifndef yHeader_COLCOLPAVINO.h
#define yHeader_COLCOLPAVINO.h

#include <stdio.h>

#define MAX_IDOLS 8
#define SELECTED_IDOLS 3

typedef struct GameState{
    int selectedIdols[SELECTED_IDOLS];
    int DoneDungeons[SELECTED_IDOLS];
    int gold;
    int hp;
    int inventory[3];
} GameState;

/**
 * Displays the in-dungeon HUD with HP, Gold, and currently equipped item.
 * Precondition: hp, maxHp >= 0; itemName can be NULL if itemQty is 0
 * @param hp current HP
 * @param maxHp maximum HP
 * @param gold current gold
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

#endif