#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "yHeader_COLCOLPAVINO.h"

#define MAX_IDOLS 8
#define SELECTED_IDOLS 3

typedef struct GameState
{
	int selectedIdols[SELECTED_IDOLS];
	int DoneDungeons[SELECTED_IDOLS]; //if 0 not yet done, 1 if done
	int gold;
	int hp;
}

const char *Idols[MAX_IDOLS] = {"Chika", "Riko", "You", "Hanamaru", "Ruby", "Dia", "Kanan", "Mari"};

//SavedGame if 1 a saved game exist, 0 if none
char MainMenu(int SavedGame)
{
	char choice;
	
	printf("\n****************************************\n");
	printf("*         Yohane The Parhelion!        *\n");
	printf("*      The Siren in the Mirror World!  *\n");
	printf("\n****************************************\n");
	

	
	if(SavedGame == 1)
		printf("[C]ontinue\n");
	else 
		printf(" [N]ew Game\n");
	
	printf("[V]iew Achievements\n");
	printf("[Q]uit\n");
	printf("\n Enter: ");
	scanf(" %c", &choice);
	
	return choice;	
}

void saveGame(GameState *state)
{
	FILE *file = fopen("saveFile.dat","wb");
	
	if(file !=NULL)
	{
		fwrite(state, sizeof(GameState), 1, file);
		fclose(file);
		printf("Game Saved! \n");
	}
	else
		printf("Error Saving the game :( \n");
}

int loadGame(GameState *state)
{
	int load = -1;
	FILE *file = fopen("saveFile.dat","rb");
	
	if(file !=NULL)
	{
		fread(state, sizeof(GameState), 1, file);
		fclose(file);
		load = 1;
	}
	else
		load = 0;
	
	return load;
}

//return fount = 1 if idol is already chosen
int Duplicate(int selected[], int count, int val)
{
	int i;
	int found = 0;
	
	for(i = 0; i < count; i++)
	{
		if(selected[i] == value)
			found = 1;	
	}
	
	return found;
}


//using -> since im using pointers so need idereference
void setNewGame(GameState *state)
{
	srand(time(NULL));
	int count = 0;
	int random;
	int i;
	
	while (count < SELECTED_IDOLS)
	{
		random = rand() % MAX_IDOLS;
		
		if(Duplicate(state->selectedIdols, count, random) != 1)
		{
			state->selectedIdols[count] = random;
			state->DoneDungeons[count] = 0; // Since new game not clear yet
			count++;
		}
	}
	
	state->gold = 0;
	state->hp = 3;
	
	
	
	printf("\nNew Game! Please Rescue these Aqours members: \n");
	for(i = 0; i < SELECTED_IDOLS; i++)
		printf(" %s\n",Idols[state->selectedIdols[i]]);
			
}

void displayDungeonHUD(int hp, int maxHp, int gold, char* itemName, int itemQty){
    printf("HP: %d/%d\tTotal Gold: %d GP", hp, maxHp, gold);

    if (itemQty > 0)
        printf("Item on hand: %s (%d)\n", itemName, itemQty);
    else
        printf("Item on hand: N/A\n");
}

void postDungeonFeedback(int idolID, const char *Idols[]){
    printf("\n*******************************************************\n");
    printf("                 Dungeon Cleared!\n");
    printf("          %s rescued!\n", Idols[idolID]);
    printf("********************************************************\n");
}

void displayInventory(GameState *state){
    printf("Lailaps: These are the items you have, Yohane!\n");
    printf("HP: %d / 5\tTotal Gold: %d GP\n", state->hp, state->gold);
    printf("Items Available:\n");
    printf("1. Tears of a Fallen Angel\t x %d\n", state->inventory[0]);
    printf("2. Noppo Bread\t\t\t x %d\n", state->inventory[1]);
    printf("3. Choco-Mint Ice Cream\t x %d\n", state->inventory[2]);
    printf("[R]eturn\n");
}

void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]){
    int i, count = 0;

    printf("**************************************************\n");
    printf("               Achievements Module\n");
    
    for (i = 0; i < totalAchievements; i++){
        if (earned[i])
            count++;
    }

    printf("               Obtained: %d / %d\n", count, totalAchievements);
    printf("**************************************************\n");

    for (i = 0; i < totalAchievements; i++){
        if(earned[i]){
            printf("[%d] %s\t\t\t\tEARNED!\n", i+1, achievementNames[i]);
        } else {
            printf("[%d] %s\t\t\t\tNOT EARNED!\n", i+1, achievementNames[i]);
        }
    }
}

void hanamaruShop(GameState *state, int rescuedIdols[]){
    int choice;

    printf("Hanamaru: Yohane-chan, zura! What can I do for you today?\n");
    printf("Total Gold: %d GP\n", state->gold);
    printf("[1] Tears of a Fallen Angel - 30GP\n");
    printf("[2] Noppo Bread - 100GP\n");

    if (rescuedIdols[6])
        printf("[3] Shovel Upgrade - 300GP\n");

    if (rescuedIdols[1])
        printf("[4] Bat Tamer - 400GP\n");
    
    if (rescuedIdols[2])
        printf("[5] Air Shoes - 500GP\n");
    
    if (rescuedIdols[7])
        printf("[6] Stewshine - 1000GP\n");

    if (rescuedIdols[0])
        printf("[7] Mikan Mochi - 1000GP\n");
    
    if (rescuedIdols[5])
        printf("[8] Kurosawa Macha - 1000GP\n");
    
    if (rescuedIdols[4])
        printf("[9] Choco-Mint Ice Cream - 2000GP\n");
    
    printf("[R]eturn\n");
    printf("Choice: ");

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

void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]){
    FILE *fp = fopen("saveFile.dat", "wb");

    if (fp != NULL){
        fwrite(state, sizeof(GameState), 1, fp);
        fwrite(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fwrite(achievements, sizeof(int), 28, fp);
        fclose(fp);
        printf("Game Saved!\n");
    }
}

int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]){
    FILE *fp = fopen("saveFile.dat", "rb");

    if (fp != NULL){
        fread(state, sizeof(GameState), 1, fp);
        fread(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fread(achievements, sizeof(int), 28, fp);
        fclose(fp);
        return 1;
    }
    return 0;
}

void unlockAchievement(int earned[], int index, const char *message){
    if (!earned[index]){
        earned[index] = 1;
        printf("Achievement Unlocked: %s\n", message);
    }
}