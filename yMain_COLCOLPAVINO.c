#include "yHeader_COLCOLPAVINO.h"

int main(){
    GameState game;
    int rescuedIdols[MAX_IDOLS] = {0};
    int achievements[28] = {0};
    int finalBossVictories = 0;

    const char *Idols[MAX_IDOLS] = {
    "Chika", "Riko", "You", "Hanamaru", 
    "Ruby", "Dia", "Kanan", "Mari"
    }; // M & L

    const char *dungeonNames[MAX_IDOLS] = {
    "Yasudaya Ryokan", "Numazu Deep Sea Aquarium", "Izu-Mito Sea Paradise", "Shougetsu Confectionary", 
    "Nagahama Castle Ruins", "Numazugoyotei", "Uchiura Bay Pier", "Awashima Marine Park"
    }; // M

    const char *achievementNames[28] = {
    "Yohane Descends!", "Mikan Power!", "Riko-chan BEAM!", "Yousoro!",
    "It’s the future, zura!", "Ganbaruby!", "Buu-buu desu wa!", "Hug!!!",
    "Shiny!", "In This Unstable World!", "One more sunshine story!",
    "Pianoforte Monologue!", "Beginner’s Sailing!", "Oyasuminasan!",
    "Red Gem Wink!", "White First Love!", "Sakana ka Nandaka!",
    "New Winding Road!", "Deep Resonance!", "No. 10!", "CYaRon!",
    "AZALEA!", "Guilty Kiss!", "Eikyuu Hours!", "Aozora Jumping Heart!",
    "Mitaiken Horizon!", "Ruby-chan! Hai? Nani ga suki?", "Step! ZERO to ONE!"
    }; // L

    int saveExists = 0;
    FILE *test = fopen("saveFile.dat", "rb");
    if (test != NULL){
        fclose(test);
        saveExists = (loadGameFile(&game, rescuedIdols, achievements) == 1);
    }
    int keepRunning = 1;
    char input;

    while (keepRunning == 1){
        printf("\n************************************************\n");
        printf("*            Yohane The Parhelion!            *\n");
        printf("*       The Siren in the Mirror World!        *\n");
        printf("************************************************\n");

        if (saveExists){
            printf("    [C]ontinue\n");
        } else {
            printf("    [N]ew Game\n");
        }

        printf("    [V]iew Achievements\n");
        printf("    [P]rofile Viewer\n");
        printf("    [Q]uit\n");
        printf("Your choice: ");
        scanf(" %c", &input);

        switch (input){
            case 'C': case 'c':
            if (saveExists){
                printf("Continuing game...\n");
                startGameLoop(&game, rescuedIdols, achievements, dungeonNames, &finalBossVictories, Idols);
            } else {
                printf("No saved game found. Please start a new game.\n");
            }
            break;

            case 'N': case 'n':
                printf("Starting new game...\n");
                setNewGame(&game, Idols);
                startGameLoop(&game, rescuedIdols, achievements, dungeonNames, &finalBossVictories, Idols);
                saveExists = 1;
                break;

            case 'V': case 'v':
                printf("Viewing achievements...\n");
                displayAchievements(achievements, 28, achievementNames);
                break;

            case 'P': case 'p': {
                char profileChoice;
                int viewProfiles = 1;

                while (viewProfiles == 1) {
                    printf("\n=== Profile Viewer ===\n");
                    printf("Select a character to view:\n");
                    printf("[1] Yohane\n");
                    printf("[2] Lailaps\n");
                    printf("[3] Chika\n");
                    printf("[4] You\n");
                    printf("[5] Riko\n");
                    printf("[6] Hanamaru\n");
                    printf("[7] Ruby\n");
                    printf("[8] Dia\n");
                    printf("[9] Kanan\n");
                    printf("[0] Mari\n");
                    printf("[R]eturn\n");
                    printf("Choice: ");
                    scanf(" %c", &profileChoice);

                    switch (profileChoice){
                        case '1':
                            characterProfile(1);
                            break;
                        case '2':
                            characterProfile(2);
                            break;
                        case '3':
                            characterProfile(3);
                            break;
                        case '4':
                            characterProfile(4);
                            break;
                        case '5':
                            characterProfile(5);
                            break;
                        case '6':
                            characterProfile(6);
                            break;
                        case '7':
                            characterProfile(7);
                            break;
                        case '8':
                            characterProfile(8);
                            break;
                        case '9':
                            characterProfile(9);
                            break;
                        case '0':
                            characterProfile(10);
                            break;
                        case 'R':
                        case 'r':
                            viewProfiles = 0;
                            break;
                        default:
                            printf("Invalid input.\n");
                            break;
                    }
                } while (profileChoice != '0');
                break;
            }

            case 'Q':
            case 'q':
                printf("Quitting the game. Goodbye!\n");
                keepRunning = 0;
                break;
            
            default:
                printf("Invalid input. Please try again.\n");
                break;
        }
    }
    return 0;
}