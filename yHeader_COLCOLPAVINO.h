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
void startGameLoop(GameState *state, int rescuedIdols[], int achievements[], const char *dungeonNames[], int *finalBossVictories, const char *idolNames[]);
void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]);
int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]);

// Game Initialization
void setNewGame(GameState *state, const char *idolNames[]);
void carryOverProgress(GameState *state);
int duplicateCheck(int selected[], int count, int val);

// Dungeon
void startDungeon(Dungeon *dungeon, int currentDungeon);
void dungeonMenu(GameState *state, const char *dungeonNames[]);
void movement(char input, Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], const char *idolNames[]);
void displayDungeon(Dungeon *dungeon, GameState *state, int dungeonNumber, const char *dungeonName[]);
int nextFloor(Dungeon *dungeon);
void generateEmptyDungeon(Dungeon *dungeon);
void randomTile(Dungeon *dungeon, char tile);
void placeRandomTile(Dungeon *dungeon, char tile, int count);
void dungeonLoop(Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], int achievements[], const char *dungeonName[], const char *idolNames[]);

// Final Dungeon
void startFinalDungeon(GameState *state, int achievements[], int *finalBossVictories);
void initializeFinalDungeon(int yohanePos[], int lailapsPos[], int switches[], int sirenPos[]);
void displayFinalDungeon(int yohanePos[], int lailapsPos[], int sirenPos[], int grid[ROWS][COLS]);
void moveYohaneAndLailaps(char input, int yohanePos[], int lailapsPos[], int grid[ROWS][COLS]);
void moveSiren(int sirenPos[], int yohanePos[], int lailapsPos[], GameState *state);
int checkSwitchActivation(int yohanePos[], int lailapsPos[], int switches[], int grid[ROWS][COLS]);
int checkSirenDefeat(int yohanePos[], int sirenPos[]);
void handleSirenDefeat(GameState *state, int earned[], Dungeon *finalDungeon, int *finalBossVictories);
void spawnBatFinalDungeon(int grid[ROWS][COLS], bat bats[], int switchesActivated, int sirenPos[], int yohanePos[], int lailapsPos[]);
void moveFinalDungeonBats(bat bats[], int grid[ROWS][COLS], GameState *state, int yohanePos[], int lailapsPos[], int switchesActivated);

// Bat Functions
void randomBat(Dungeon *dungeon, int batCount);
void resetBat(Dungeon *dungeon);
void BatMovement(Dungeon *dungeon, GameState *state, int currentDungeon);

// Inventory
void displayInventory(GameState *state);
void switchItem(GameState *state, char direction);
void useItemInHand(GameState *state);
void useItem(GameState *state, int item);
int hasPassiveItem(GameState *state, int itemID);
int countItem(GameState *state, int itemID);
void getItemInfo(GameState *state, int itemID, char *name, int *qty);

// Shop
void hanamaruShop(GameState *state, int rescuedIdols[], int achievements[]);

// Death & Revive
void checkChocoRevive(GameState *state);
void triggerDeath(GameState *state, const char *cause);

// Achievements
void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]);
void unlockAchievement(int earned[], int index, const char *message);
void checkRescueAchievements(int rescuedIdols[], int earned[], int currentIdol);
void checkMilestoneAchievements(GameState *state, int earned[], int totalDungeonsCleared);
void checkDamageLessDungeonAchievement(int dungeonDamage, int earned[]);
void checkShopSpendingAchievement(int totalShopSpending, int earned[]);
void checkChocoMintSaveAchievement(int earned[]);
void checkFinalBossAchievement(int finalBossVictories, int earned[]);

// Utility
void displayDungeonSelection(GameState *state, int rescuedIdols[], int currentIdols[], const char* dungeonName[]);
int checkAllDungeonsCleared(GameState *state);
void markDungeonClear(GameState *state, int dungeonIndex);
void characterProfile(int choice);
int allIdolsRescued(int rescuedIdols[]);
void resetIdolSelection(int rescuedIdols[]);

// Post Dungeon
void postDungeonFeedback(int idolID, const char *idolNames[]);

// #endif