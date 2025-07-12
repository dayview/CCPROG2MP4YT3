#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "yHeader_COLCOLPAVINO.h"

const char *Idols[MAX_IDOLS] = {
    "Chika", "Riko", "You", "Hanamaru", 
    "Ruby", "Dia", "Kanan", "Mari"
};

char MainMenu(int saveGame){
	char choice;
	printf("\n****************************************\n");
	printf("*         Yohane The Parhelion!        *\n");
	printf("*      The Siren in the Mirror World!  *\n");
	printf("\n****************************************\n");
	
	if(saveGame)
		printf("[C]ontinue\n");
	else 
		printf("[N]ew Game\n");
	
	printf("[V]iew Achievements\n");
	printf("[Q]uit\n\n");
	printf("Choice: ");
	scanf(" %c", &choice);
	return choice;	
}

void setNewGame(GameState *state, const char *idolNames[]){
    int i, count = 0;
    srand((unsigned)time(NULL));

    while (count < SELECTED_IDOLS){
        i = rand() % MAX_IDOLS;
        if (duplicateCheck(state->selectedIdols, count, i) != 1){
            state->selectedIdols[count] = i;
            state->doneDungeons[count] = 0;
            count++;
        }
    }
    state->gold = 0;
    state->hp = 3;
    
    for (i = 0; i < INVENTORY_SLOTS; i++){
        state->inventory[i] = 0;
    }

    printf("\nRescue these idols in this playthrough:\n");
    for (i = 0; i < SELECTED_IDOLS; i++){
        printf("  - %s\n", idolNames[state->selectedIdols[i]]);
    }
}

void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]){
    FILE *fp = fopen("saveFile.dat", "wb");

    if (fp){
        fwrite(state, sizeof(GameState), 1, fp);
        fwrite(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fwrite(achievements, sizeof(int), 28, fp);
        fclose(fp);
        printf("Game saved!\n");
    } else {
        printf("Error saving...");
    }
}

int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]){
    FILE *fp = fopen("saveFile.dat", "rb");

    if (fp){
        fread(state, sizeof(GameState), 1, fp);
        fread(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fread(achievements, sizeof(int), 28, fp);
        fclose(fp);
        printf("Game loaded!\n");
    } else {
        printf("Error loading...");
    }

    return 0;
}

int duplicateCheck(int selected[], int count, int val){
    int i, found = 0;

    for (i = 0; i < count; i++){
        if (selected[i] == val)
            found = 1;
    }
    return found;
}

void displayDungeonHUD(int hp, int maxHp, int gold, const char *itemName, int itemQty){
    printf("HP: %d/%d           Total Gold: %d GP", hp, maxHp, gold);

    if (itemQty > 0){
        printf("Item on hand: %s (%d)\n", itemName, itemQty);
    } else {
        printf("Item on hand: N/A\n");
    }
}

void postDungeonFeedback(int idolID, const char *idolNames[]){
    printf("\n*******************************************************\n");
    printf("                 Dungeon Cleared!\n");
    printf("          %s rescued!\n", idolNames[idolID]);
    printf("\n*******************************************************\n");
}

void displayInventory(GameState *state){
    printf("Lailaps: These are the items you have, Yohane!\n");
    printf("HP: %d/5\tGold: %d GP\n", state->hp, state->gold);
    printf("Items Available:\n");
    printf("1) Tears of a Fallen Angel\t x %d\n", state->inventory[0]);
    printf("2) Noppo Bread\t\t\t x %d\n", state->inventory[1]);
    printf("3) Choco-Mint Ice Cream\t x %d\n", state->inventory[2]);
    printf("\n[R]eturn\n");
}

void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]){
    int i, count = 0;

    for (i = 0; i < totalAchievements; i++){
        if (earned[i])
            count++;
    }

    printf("**************************************************\n");
    printf("               Achievements Module\n");
    printf("               Obtained: %d / %d\n", count, totalAchievements);
    printf("**************************************************\n");

    for (i = 0; i < totalAchievements; i++){
        if (earned[i]){
            printf("[%d] %-30s %s\n", i+1, achievementNames[i], "EARNED!");
        } else {
            printf("[%d] %-30s %s\n", i+1, achievementNames[i], "NOT EARNED!");
        }
    }
}

void unlockAchievement(int earned[], int index, const char *message){
    if (!earned[index]){
        earned[index] = 1;
        printf("Achievement Unlocked: %s\n", message);
    }
}

void displayDungeonSelection(GameState *state, int rescuedIdols[], int currentIdols[]){
    int i;
    
    printf("\n=== Dungeon Selection ===\n");
    printf("Yohane's HP: %d/5\n", state->hp);
    printf("Total Gold: %d GP\n", state->gold);
    printf("Item on hand: ");

    int hasItem = 0;
    for (i = 0; i < INVENTORY_SLOTS; i++){
        if (state->inventory[i] > 0){
            switch(i){
                case 0:
                    printf("Tears of a Fallen Angel (%d)", state->inventory[i]);
                    break;
                case 1:
                    printf("Noppo Bread (%d)", state->inventory[i]);
                    break;
                case 2:
                    printf("Choco-Mint Ice Cream (%d)", state->inventory[i]);
                    break;
            }
            hasItem = 1;
            break; // refactor since if-break
        }
    }

    if (!hasItem)
        printf("N/A");
    printf("\n\n");

    printf("Available Dungeons:\n");
    for (i = 0; i < SELECTED_IDOLS; i++){
        if (state->doneDungeons[i] == 0){
            printf("[%d] %s's Dungeon", i+1, Idols[currentIdols[i]]);
        } else {
            printf("[X] %s's Dungeon (Cleared)\n", Idols[currentIdols[i]]);
        }
    }

    printf("\n[I]nventory [S]ave and Quit");
    
    if (rescuedIdols[3])
        printf("[H]anamaru Shop");
        printf("\nChoice: ");
}

int checkAllDungeonsCleared(GameState *state){
    int i;

    for (i = 0; i < SELECTED_IDOLS; i++){
        if (state->doneDungeons[i] == 0)
            return 0;
    }
    return 1;
}

void markDungeonClear(GameState *state, int dungeonIndex){
    if (dungeonIndex >= 0 && dungeonIndex < SELECTED_IDOLS){
        state->doneDungeons[dungeonIndex] = 1;
    }
}

void initializeFinalDungeon(int yohanePos[], int lailapsPos[], int switches[], int sirenPos[]){
    yohanePos[0] = 0;
    yohanePos[1] = 0;
    lailapsPos[0] = 0;
    lailapsPos[1] = 1;

    switches[0] = 1;
    switches[1] = 1;
    switches[2] = 1;
    switches[3] = 1;
    switches[4] = 1;
    switches[5] = 1;

    sirenPos[0] = 4;
    sirenPos[1] = 4;
}

void displayFinalDungeon(int yohanePos[], int lailapsPos[], int switches, int sirenPos[], int grid[][10]){
    int i, j;
    
    printf("\n=== FINAL DUNGEON ===\n");
    printf("Yohane (Y) and Lailaps (L) must activate all switches (0) to unlock the Siren (S)!\n\n");

    for (i = 0; i < 10; i++){
        for (j = 0; j < 10; j++){
            if (i == yohanePos[0] && j == yohanePos[1])
                printf("Y ");
            else if (i == lailapsPos[0] && j == lailapsPos[1])
                printf("L ");
            else if (i == sirenPos[0] && j == sirenPos[1])
                printf("S ");
            else if (grid[i][j] == 1)
                printf("0 ");
            else if (grid[i][j] == 2)
                printf("* ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\nControls: WASD\n");
}

int moveCharacter(int pos[], char direction, int grid[][10]){
    int newRow = pos[0];
    int newCol = pos[1];

    switch (direction){
        case 'W':
        case 'w':
            newRow--;
            break;
        case 'S':
        case 's':
            newRow++;
            break;
        case 'A':
        case 'a':
            newCol--;
            break;
        case 'D':
        case 'd':
            newCol++;
            break;
    }

    if (newRow >= 0 && newRow < 10 && newCol >= 0 && newCol < 10 && grid[newRow][newCol] != 2){
        pos[0] = newRow;
        pos[1] = newCol;
        return 1; // refactor if-return
    }
    return 0;
}

int checkSwitchActivation(int yohanePos[], int lailapsPos[], int switches, int grid[][10]){
    int i, active = 0;

    for (i = 0; i < 6; i += 2){
        if (switches[i] == 1 && switches[i+1] == 1){
            int firstSwitchRow = 2 + (i/2) * 2;
            int firstSwitchCol = 2;
            int secondSwitchRow = 2 + (i/2) * 2;
            int secondSwitchCol = 7;

            if ((yohanePos[0] == firstSwitchRow && yohanePos[1] == firstSwitchCol &&
                lailapsPos[0] == secondSwitchRow && lailapsPos[1] == secondSwitchCol) ||
                (lailapsPos[0] == firstSwitchRow && lailapsPos[1] == firstSwitchCol &&
                yohanePos[0] == secondSwitchRow && yohanePos[1] == secondSwitchCol)){
                    switches[i] = 0;
                    switches[i+1] = 0;
                    grid[firstSwitchRow][firstSwitchCol] = 0;
                    grid[secondSwitchRow][secondSwitchCol] = 0;
                    active = 1;
                    printf("Switch pair %d activated!\n", (i/2) + 1);
                }
        }
    }
    return active;
}

int checkSirenDefeat(int yohanePos[], int sirenPos[]){
    int rowDiff = abs(yohanePos[0] - sirenPos[0]);
    int colDiff = abs(yohanePos[1] - sirenPos[1]);

    if ((rowDiff == 1 && colDiff = 0) || (rowDiff == 0 && colDiff = 1)){
        return 1; // refactor if-return
    }
    return 0;
}

void moveSiren(int sirenPos[], int yohanePos[], int lailapsPos[]){
    int targetRow, targetCol;

    int yohaneDist = abs(sirenPos[0] - yohanePos[0]) + abs(sirenPos[1] - yohanePos[1]);
    int lailapsDist = abs(sirenPos[0] - lailapsPos[0]) + abs(sirenPos[1] - lailapsPos[1]);

    if (yohaneDist <= lailapsDist){
        targetRow = yohanePos[0];
        targetCol = yohanePos[1];
    } else {
        targetRow = lailapsPos[0];
        targetCol = lailapsPos[1];
    }

    if (sirenPos[0] < targetRow)
        sirenPos[0]++;
    if (sirenPos[0] > targetRow)
        sirenPos[0]--;
    if (sirenPos[1] < targetCol)
        sirenPos[1]++;
    if (sirenPos[1] > targetCol)
        sirenPos[1]--;
}

void checkRescueAchievements(int rescuedIdols[], int earned[], int currentIdol){
    if (!earned[currentIdol]){
        switch (currentIdol){
            // no input case for yohane descends (yet)
            case 0:
                unlockAchievement(earned, currentIdol, "Mikan Power!");
                printf("\nRescued Chika for the first time!");
                break;
            case 1:
                unlockAchievement(earned, currentIdol, "Riko-chan BEAM!");
                printf("\nRescued Riko for the first time!");
                break;
            case 2:
                unlockAchievement(earned, currentIdol, "Yousoro!");
                printf("\nRescued You for the first time!");
                break;
            case 3:
                unlockAchievement(earned, currentIdol, "It's the future, zura!");
                printf("\nRescued Hanamaru for the first time!");
                break;
            case 4:
                unlockAchievement(earned, currentIdol, "Ganbaruby!");
                printf("\nRescued Ruby for the first time!");
                break;
            case 5:
                unlockAchievement(earned, currentIdol, "Buu-buu desu wa!");
                printf("\nRescued Dia for the first time!");
                break;
            case 6:
                unlockAchievement(earned, currentIdol, "Hug!!!");
                printf("\nRescued Kanan for the first time!");
                break;
            case 7:
                unlockAchievement(earned, currentIdol, "Shiny!");
                printf("\nRescued Mari for the first time!");
                break;
        }
    }

    if (rescuedIdols[3] && rescuedIdols[5] && rescuedIdols[6]){
        unlockAchievement(earned, 8, "AZALEA!");
    }

    if (rescuedIdols[0] && rescuedIdols[1] && rescuedIdols[2]){
        unlockAchievement(earned, 9, "CYaRon!");
    }

    if (rescuedIdols[4] && rescuedIdols[5] && rescuedIdols[6]){
        unlockAchievement(earned, 10, "Guilty Kiss!");
    }
}

void checkMilestoneAchievements(GameState *state, int earned[], int totalDungeonsCleared){
    if (totalDungeonsCleared >= 10 && learned[11]){
        unlockAchievement(earned, 11, "Dungeon Master");
    }

    if (state->gold <= 0 && !earned[12]){
        unlockAchievement(earned, 12, "Big Spender");
    }
}

void useItem(GameState *state, int itemIndex){
    if (state->inventory[itemIndex] > 0){
        state->inventory[itemIndex]--;

        switch(itemIndex){
            case 0:
            case 1:
                if (state->hp < 5)
                    state->hp += 1;
                printf("HP restored!\n");
                break;
            case 2:
                state->hp = 5;
                printf("Full health restored!\n");
                break;
        }
    } else {
        printf("No items available!\n");
    }
}

void carryOverProgress(int rescuedIdols[], GameState *state){
    printf("Progress carried over to new playthrough!\n");
    printf("Gold: %d GP\n", state->gold);
    printf("Items carried over:\n");
    if (state->inventory[0] > 0)
        printf("- Tears of a Fallen Angel: %d\n", state->inventory[0]);
    if (state->inventory[1] > 0)
        printf("- Noppo Bread: %d\n", state->inventory[1]);
    if (state->inventory[2] > 0)
        printf("- Choco-Mint Ice Cream: %d\n", state->inventory[2]);
}

int allIdolsRescued(int rescuedIdols[]){
    int i;

    for (i = 0; i < MAX_IDOLS; i++){
        if (rescuedIdols[i] == 0)
            return 0; // refactor if-return
    }    

    return 1;
}

void resetIdolSelection(int rescuedIdols[]){
    int i;

    if (allIdolsRescued(rescuedIdols)){
        for (i = 0; i < MAX_IDOLS; i++){
            rescuedIdols[i] = 0;
        }
        printf("All idols rescued! Selection reset for next playthrough.\n");
    }
}

void traverseDungeon(GameState *state, int dungeonIndex, const char *idolNames[]){
    /* to be implemented */
    postDungeonFeedback(dungeonIndex, idolNames);
}

void hanamaruShop(GameState *state, int rescuedIdols[]){
    int choice;

    printf("\nHanamaru's Shop - Gold: %d GP\n", state->gold);
    printf("1) Tears of a Fallen Angel - 30 GP\n");
    printf("2) Noppo Bread             - 100 GP\n");

    if (rescuedIdols[6])
    printf("[3] Shovel Upgrade         - 300 GP\n");

    if (rescuedIdols[1])
    printf("[4] Bat Tamer              - 400 GP\n");

    if (rescuedIdols[2])
    printf("[5] Air Shoes              - 500 GP\n");

    if (rescuedIdols[7])
    printf("[6] Stewshine              - 1000 GP\n");

    if (rescuedIdols[0])
    printf("[7] Mikan Mochi            - 1000 GP\n");

    if (rescuedIdols[5])
    printf("[8] Kurosawa Macha         - 1000 GP\n");

    if (rescuedIdols[4])
    printf("[9] Choco-Mint Ice Cream   - 2000 GP\n");

    printf("[R]eturn\n");
    printf("Choice: ");
    scanf("%d", &choice);

    int cost = 0, itemIndex = -1;
    switch (choice){
        case 1:
            cost = 30;
            itemIndex = 0;
            break;
        case 2:
            cost = 100;
            itemIndex = 1;
            break;
        case 9:
            if (rescuedIdols[4]){
                cost = 2000;
                itemIndex = 2;
            }
            break;
        default:
            break;
    }

    if (itemIndex >= 0 && state->gold >= cost){
        state->inventory[itemIndex]++;
        state->gold -= cost;
        printf("Purchase successful! Remaining Gold: %d GP\n", state->gold);
    } else {
        printf("Not enough gold or item locked!\n");
    }
}