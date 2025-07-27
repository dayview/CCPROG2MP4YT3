#include "yHeader_COLCOLPAVINO.h"
#include "yFunc_COLCOLPAVINO.c"

int main(){
    int choice;
    int keepRunning = 1;
    GameState game;
    int rescuedIdols[MAX_IDOLS] = {0};
    int achievements[30] = {0};

    int saveExists = (loadGameFile(&game, rescuedIdols, achievements) == 1);

    while (keepRunning){
        MainMenu(saveExists, &choice);

        switch (choice){
            case 1:
                printf("Continuing game...\n");
                // continue game logic
                keepRunning = 0;
                break;
            case 2:
                // new game logic
                printf("Starting new game...\n");
                setNewGame(&game, idolNames);
                break;
            case 3:
                // view achievements
                printf("Viewing achievements...\n");
                displayAchievements(achievements, 28, NULL);
                break;
            case 4:
                // quit game
                printf("Quitting the game. Goodbye!\n");
                keepRunning = 0;
                break;
            default:
                printf("Invalid input. Please try again.\n");
                break;
        }
        return 0;
    }
}