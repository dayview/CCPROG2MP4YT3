#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


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
