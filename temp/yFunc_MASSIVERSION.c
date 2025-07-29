#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_IDOLS 8
#define SELECTED_IDOLS 3
#define MAX_BATS 15

#define ROWS 10
#define COLS 55
#define TILE_BORDER '*'
#define TILE_PASSABLE '.'
#define TILE_SPIKE 'x'
#define TILE_WATER 'w'
#define TILE_HEAT 'h'
#define TILE_EXIT 'E'
#define TILE_BAT 'b'
#define TILE_YOHANE 'Y'
#define TILE_TREASURE 'T'
#define MAX_ITEMS 10
#define TILE_WALL 'v'

#define ITEM_TEARS 1
#define ITEM_NOPPO 2
#define ITEM_SHOVEL 3
#define ITEM_BAT 4
#define ITEM_AIR 5
#define ITEM_STEWSHINE 6
#define ITEM_MIKAN 7
#define ITEM_KUROSAWA 8
#define ITEM_CHOCO 9

typedef struct 
{
	int selectedIdols[SELECTED_IDOLS];
	int DoneDungeons[SELECTED_IDOLS]; //if 0 not yet done, 1 if done
	int gold;
	float hp;
	//NEW
	int maxHP;
	int inventory[MAX_ITEMS];
	int currentItem; //ID num 0 if none
	int usedChoco;  
	int isGameOver; //
} GameState;

typedef struct
{
	int x;
	int y;
	int alive; //1 alive, 0 dead
	int attack; //1- damaged the player, 0=normal
} bat;

//New for dungeon struct
typedef struct 
{
	char map[ROWS][COLS];
	int floor;
	int maxFloor; //added for stages
	int yohaneX; //player position x
	int yohaneY; //player position y
	bat bats[MAX_BATS];
} Dungeon;

//New for bat
const char *Idols[MAX_IDOLS] = {"Chika", "Riko", "You", "Hanamaru", "Ruby", "Dia", "Kanan", "Mari"};

//added new const
const char *DungeonNames[MAX_IDOLS] = {"Yasudaya Ryokan", "Numazu Deep Sea Aquarium", "Izu-Mito Sea Paradise", "Shougetsu Confectionary", "Nagahama Castle Ruins", "Numazugoyotei", "Uchiura Bay Pier", "Awashima Marine Park",};


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

// removed save & load function (refactored by leon)

// removed duplicate function (refactored by leon)

// removed setNewGame (refactored by leon)

void checkChocoRevive(GameState *state)
{
	int i;
	
	if(state->hp <= 0 && state->currentItem == ITEM_CHOCO && state->usedChoco == 0)
	{
		for(i = 0; i < MAX_ITEMS; i++)
		{
			if(state->inventory[i] == ITEM_CHOCO && state->usedChoco == 0)
			{
				state->inventory[i] = 0;
				state->hp = state->maxHP;
				state->usedChoco = 1;
				printf("Choco-mint ice cream revived Yohane! HP fully restored. \n");
				
			}
		}
	}
	
	if(state->hp <= 0)
		state->isGameOver = 1;
}

int hasPassiveItem(GameState *state, int itemID)
{
	int i;
	int found = 0;
	for(i = 0; i < MAX_ITEMS; i++)
	{
		if(state->inventory[i] == itemID)
			found = 1;
	}
	
	return found;
}
//New movement player
void movement(char input, Dungeon *dungeon, GameState *state, int currentDungeon)
{
	int x = dungeon->yohaneX;
	int y = dungeon->yohaneY;
	char Tile;
	int move = 1;
	int reward,gold; //for treasure
	int inv ,i;
	int found = 0;
	
	switch(input)
	{
		case 'W': 
			x--;
			break;
		case 'w':
			x--;
			break;
		case 'S': 
			x++;
			break;
		case 's':
			x++;
			break;	
		case 'A': 
			y--;
			break;
		case 'a':
			y--;
			break;
		case 'D': 
			y++;
			break;
		case 'd':
			y++;
			break;
		case 'X': 
			printf("Yohane stay in place.\n");
			move = 1;
			break;
		case 'x':
			printf("Yohane stay in place.\n");
			move = 1;
			break;
		case '[':
			switchItem(state, input);  
	    	move = 1; 
    		break;
		case ']':
    		switchItem(state, input);  
	    	move = 1; 
    		break;
    	case ' ':
		    useItemInHand(state);
	    	move = 1;  // Counts as a turn even if nothing happened
    		break;
		default:
			printf("Invalid input! Yohane will not move and it is considered as a turn\n");
			move = 1;
			break;
	}
	
	//check for out of bounce
	if(x < 0 || x >=ROWS || y < 0 || y >= COLS)
	{
		printf("Out of bounce. Stay inside the dungeon\n");
		move = 0;
	}
	
	Tile = dungeon->map[x][y];
	
	if(move == 1)
	{
		switch(Tile)
		{
			case '*':
				printf("You've reached the border which blocks the movement\n");
				move = 0;
				break;
			case 'w':
				if (hasPassiveItem(state, ITEM_AIR))
					printf("Walk through the water using Air Shoes.\n");
				else
				{		
					printf("You can't Swim! Water blocks the way \n");
					move = 0;
				}
				break;
			case 'v':
				printf("You dig through a wall.\n");
				dungeon->map[x][y] = '.';
				move = 0;
				break;
			case 'x':
				if (hasPassiveItem(state,ITEM_SHOVEL))
					printf("You dig through a spike trap without taking damage!\n");
				else
				{
					printf("You break through a spike trap! (-0.5 HP)\n");
					state->hp -= 0.5;	
					checkChocoRevive(state);
				}
				dungeon->map[x][y] = '.';			
				move = 0;
				break;
			case 'T':
				printf("You found a treasure chest!\n");
				reward = rand()%2; //0 -gold 1-item
				if(reward == 0)
				{
					gold = (rand() % 91) + 10;
					printf("You gained %d gold\n",gold);
					state->gold += gold;
				}
				else
				{
					printf("You found a Noppo Bread! \n");
					//ADD CODE FOR ITEM BELOW
					for(inv = 0; inv < MAX_ITEMS; inv++)
					{
						if(found == 0 && state->inventory[inv] == 0)
						{
							state->inventory[inv] = ITEM_NOPPO;
							found = 1;
						}
							
					}
					
				}
				dungeon->map[x][y] = '.';
				break;
			case 'E':
				printf("You Found the Exit! Proceed to next. \n");
				//ADD CODE BELOW
				if(nextFloor(dungeon))
				{
					state->DoneDungeons[currentDungeon] = 1;
					printf("Cleared! You saved %s!\n",Idols[state->selectedIdols[currentDungeon]]);
				}
				move = 0;
				
				break;
			case '.':
				break;
			case 'h':
				//PASSABLE BUT IF SHES STANDING ON H REDUCE 1 HP//
				break;
			case 'b':  // player moved onto a bat tile
			    printf("You defeated a bat!\n");
				move = 0;
				
			    for(i = 0; i < MAX_BATS; i++)
			    {
			        if(dungeon->bats[i].alive == 1 &&
			           dungeon->bats[i].x == x &&
			           dungeon->bats[i].y == y)
			        {
			            dungeon->bats[i].alive = 0;
			
			            // Drop gold on the tile
			            dungeon->map[x][y] = 'g'; // 'G' for gold
			        }
			    }
				break;
				
			case 'g':
				if(currentDungeon == 0)
			        state->gold += 5;
			    else if(currentDungeon == 1)
			        state->gold += 10;
			    else
			        state->gold += 15;
				
				printf("You picked up %d gold! Total gold: %d\n", state->gold);
		    
			    dungeon->map[x][y] = '.'; // Remove gold icon after pickup
			    break;	
		}
	}
	//finish move
	if(move == 1)
	{
		dungeon->yohaneX = x;
		dungeon->yohaneY = y;
	}
	
	//check if player is stepping on h and moves
	if(move == 0 && dungeon->map[dungeon->yohaneX][dungeon->yohaneY] == 'h')
	{
		if(!hasPassiveItem(state, ITEM_AIR))
		{
			printf("You stood on a heat tile! (-1 HP)\n");
			state->hp -= 1; 
			checkChocoRevive(state);
		}
		else
			printf("You stand on heat with your air shoes. No damage taken\n");
	}
}

void RandomTile(Dungeon *dungeon, char tile)
{
	int x,y;
	do
	{
		x = rand() % (ROWS - 2) + 1;
		y = rand() % (COLS - 2) + 1;
	} while (dungeon->map[x][y] != '.');
	
	dungeon->map[x][y] = tile;
}

void randomBat(Dungeon *dungeon, int batCount)
{
	int i, x, y;

	for(i = 0; i < batCount; i++)
	{
		do
		{
			x = rand() % (ROWS - 2) + 1;
			y = rand() % (COLS - 2) + 1;
		} while(dungeon->map[x][y] != TILE_PASSABLE);

		dungeon->bats[i].x = x;
		dungeon->bats[i].y = y;
		dungeon->bats[i].attack = 0;
		dungeon->bats[i].alive = 1;
		

		dungeon->map[x][y] = TILE_BAT;
	}
}

void BatMovement(Dungeon *dungeon, GameState *state, int currentDungeon)
{
	int i, index, randDirection, batX, batY, newX, newY, adjX, adjY, attacked, countDirection;
	float damage;
	char tile, origTile;
	
	int directions[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
	
	damage = 0.0;
	countDirection = 4;
	
	if(currentDungeon == 0)
	{
		damage = 0.5;
		countDirection = 4;
	}
	else if(currentDungeon == 1)
	{
		damage = 1.0;
		countDirection = 4;
	}
	else
	{
		damage = 1.5;
		countDirection = 8;
	}
		
	for(i = 0; i < MAX_BATS; i++)
	{
		if(dungeon->bats[i].alive == 1)
		{
			batX = dungeon->bats[i].x;
			batY = dungeon->bats[i].y;
			attacked = 0;
			
			for(index = 0; index < countDirection; index++)
			{
				adjX = batX + directions[index][0];
				adjY = batY + directions[index][1];
				
				if(adjX == dungeon->yohaneX && adjY ==dungeon->yohaneY)
				{
					printf("A bat attacked Yohane! (-%.1f HP)\n",damage);
					state->hp -= damage;
					checkChocoRevive(state);
					attacked =1;
					dungeon->bats[i].attack = 1;
					
					dungeon->map[batX][batY] = 'B';
				}
			}
			
			if(attacked == 0)
			{
				origTile = TILE_PASSABLE;
                
				for(index = 0; index < countDirection; index++)
				{
					randDirection = rand() % countDirection;
					newX = batX + directions[randDirection][0];
					newY = batY + directions[randDirection][1];
					
					if(newX >= 1 && newX < ROWS - 1 && newY >= 1 && newY < COLS - 1)
					{
						tile = dungeon->map[newX][newY];
						
						if(tile == TILE_PASSABLE || tile == TILE_WATER || tile == TILE_HEAT)
						{
							dungeon->map[batX][batY] = origTile;
							
							dungeon->map[newX][newY] = TILE_BAT;
							
							//bat moves
							dungeon->bats[i].x =newX;
							dungeon->bats[i].y =newY;
							
							index = countDirection;
						}
					}
				}
		}
	}
}

//makes B to b if bat attacked
void resetBat(Dungeon *dungeon)
{
	int i,x,y;
	
	for(i = 0; i < MAX_BATS; i++)
	{
		if(dungeon->bats[i].alive == 1 && dungeon->bats[i].attack == 1)
		{
			x = dungeon->bats[i].x;
			y = dungeon->bats[i].y;
			
			if(dungeon->map[x][y] == 'B')
				dungeon->map[x][y] = 'b';
				
			dungeon->bats[i].attack = 0;
		}
	}
}
//for walls,water, etc
void PlaceRandomTile(Dungeon *dungeon, char tile, int count)
{
	int i,x,y;
	for(i = 0; i < count; i++)
	{
		do
		{
			x = rand() % (ROWS - 2) + 1;
			y = rand() % (COLS - 2) + 1;
		} while (dungeon->map[x][y] != '.');
		
		dungeon->map[x][y] = tile;
	}
}

void GenerateEmptyDungeon(Dungeon *dungeon)
{
	int i,j;
	
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			if(i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1)
				dungeon->map[i][j] = TILE_BORDER;
			else
				dungeon->map[i][j] = TILE_PASSABLE;	
		}
	}
	
	//default position (we can change this)
	dungeon->yohaneX = 1;
	dungeon->yohaneY = 1;
	dungeon->floor = 1;
	
	PlaceRandomTile(dungeon, TILE_WALL, 25);
	PlaceRandomTile(dungeon, TILE_HEAT, 25);
	PlaceRandomTile(dungeon, TILE_SPIKE, 25);
	PlaceRandomTile(dungeon, TILE_WATER, 25);
	
}


void displayDungeon(Dungeon *dungeon)
{
	int i,j;
	
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			if(i == dungeon->yohaneX && j == dungeon->yohaneY)
				printf("%c", TILE_YOHANE);
			else 
				printf("%c", dungeon->map[i][j]);
		}
		printf("\n");
	}
}

void startDungeon(GameState *state, Dungeon *dungeon, int currentDungeon) //current is tracker for how many dungeon already cleared
{
	int batCount;
	dungeon->floor = 1;
	
	if(currentDungeon == 0)
		dungeon->maxFloor = 1;
	else 
		dungeon->maxFloor = (rand() % 2) + 2; // random 2 or 3
		
	GenerateEmptyDungeon(dungeon);
	if(currentDungeon == 0)
		batCount =5;
		
	else if(currentDungeon == 1)
		batCount =10;
		
	else
		batCount = 15;
		
	randomBat(dungeon, batCount);
		
	RandomTile(dungeon, TILE_TREASURE);
	RandomTile(dungeon, TILE_EXIT);
}

int nextFloor(Dungeon *dungeon)
{
	int floors = -1;
	if(dungeon->floor < dungeon->maxFloor)
	{
		dungeon->floor++;
		GenerateEmptyDungeon(dungeon);
		RandomTile(dungeon, TILE_TREASURE);
		RandomTile(dungeon, TILE_EXIT);
		printf("Moved to next floor: %d/%d\n",dungeon->floor,dungeon->maxFloor);
		floors = 0; //more floors
	}
	else
	{
		printf("Dungeon Clear! Congrats! \n");
		floors = 1;
	}
	
	return floors;
}

void DungeonMenu(GameState *state)
{
	int i, Index;
	printf("Lilaps: Yohane! Where should we go to now?\n");
	printf("HP: %.1f / %d \tTotal Gold: %d GP\n",state->hp, state->maxHP,state->gold);
	printf("Item on hand: ");
	if(state->currentItem == ITEM_TEARS)
		printf("Tears of a fallen angel\n");
		
	else if(state->currentItem == ITEM_NOPPO)
		printf("Noppo Bread\n");
		
	else if(state->currentItem == ITEM_CHOCO)
		printf("Choco-mint ice cream\n");
		
	else
		printf("N/A\n");
	
	for(i = 0; i < SELECTED_IDOLS; i++)
	{
		Index = state->selectedIdols[i];
		if(state->DoneDungeons[i] == 0)
			printf("[%d] Visit %s\n", i + 1, DungeonNames[Index]);
		else 
			printf("[X] Visit %s\n", DungeonNames[Index]);
	}
	
	printf("\n[I]nventory\t[S]ave and Quit\n");
	printf("Choice: ");
}
//no bat air and shovel
void useItem(GameState *state, int item)
{
	switch(item)
	{
		case ITEM_TEARS:
			if(state->hp < state->maxHP)
			{
				state->hp += 0.5;
				if(state->hp > state->maxHP)
					state->hp = state->maxHP;
				printf("Yohane used Tears of the fallen Angel. Heals +0.5 HP\n");
			}
			else
				printf("HP is already full!\n");
			break;
			
		case ITEM_NOPPO:
			if(state->hp < state->maxHP)
			{
				state->hp += 0.5;
				if(state->hp > state->maxHP)
					state->hp = state->maxHP;
				printf("Yohane used Noppo Bread. Heals +0.5 HP\n");
			}
			else
				printf("HP is already full!\n");
			break;
			
		case ITEM_STEWSHINE:
			state->maxHP += 1;
			printf("Used Stewshine. Max HP permanently increased by 1!\n");
			break;
			
		case ITEM_MIKAN:
			state->maxHP += 1;
			printf("Used Mikan Mochi. Max HP permanently increased by 1!\n");
			break;
		
		case ITEM_KUROSAWA:
			state->maxHP += 1;
			printf("Used Kurosawa Macha. Max HP permanently increased by 1!\n");
			break;
			
	}
}

void showInventory(GameState *state)
{
	char choice;
	int tear = 0, noppo = 0, choco = 0;
	int i;
	
	for(i = 0; i < MAX_ITEMS; i++)
	{
		switch(state->inventory[i])
		{
			case ITEM_TEARS:
				tear++;
				break;
			
			case ITEM_NOPPO:
				noppo++;
				break;
				
			case ITEM_CHOCO:
				choco++;
				break;
		}
	}
	
	printf("\nLailaps: These are the items you have, Yohane!\n");
	printf("HP: %.1f / %d\tTotal Gold: %d GP\n",state->hp,state->maxHP,state->gold);
	printf("Items available\n");
	printf("1. Tears of a fallen angel x %d\n", tear);
	printf("2. Noppo Bread             x %d\n", noppo);
	printf("3. Choco-mint ice cream    x %d\n", choco);
	printf("[R]eturn\n");
	printf("Choice: ");
	scanf(" %c", &choice);
	
	//ADD MORE LATER
}

void switchItem(GameState *state, char direction)
{
	int items[3] = {ITEM_TEARS, ITEM_NOPPO, ITEM_CHOCO};
	int owned[3] = {0, 0, 0};
	int i,j;
	int heldIndex = -1;
	int next = -1;
	int check,total = 0;
	

    //items player has
    for(i = 0; i < 3; i++) 
	{
		for(j = 0; j < MAX_ITEMS; j++) 
		{
            if(state->inventory[j] == items[i]) 
                owned[i] = 1;
            
        }
        
        if(owned[i] == 1)
        	total++;
       
    }
	
	for(i = 0; i < 3; i++)
	{
		if(state->currentItem == items[i])
			heldIndex = i;
	}
	
	if(total <= 1)
		printf("Only 1 usable item in hand. No switching.\n");
		
	else
	{
		for(i = 1; i<=3; i++)
		{
			if(direction == ']')
				check = (heldIndex + i) % 3;
			else if(direction == '[')
				check = (heldIndex - i + 3) % 3;
				
			if(next == -1 && owned[check] == 1)
				next = check;
		}
		
		if(next != -1)
		{
			state->currentItem = items[next];
			
			printf("Switched to: ");
			if(state->currentItem == ITEM_TEARS)
				printf("Tears of a fallen angel\n");
			else if(state->currentItem == ITEM_NOPPO)
				printf("Noppo bread\n");
			else if(state->currentItem == ITEM_CHOCO)
				printf("Choco-mint ice cream\n");
		}
	}
	
	if(owned[0] == 0 && owned[1] == 0 && owned[2] == 0)
	{
		printf("Item on hand: N/A\n");
		state->currentItem =0;
	}
}

void useItemInHand(GameState *state)
{
	int i;
	int use = 0;
	
	if(state->currentItem ==0)
		printf("No item in hand. Turn ended...\n");
	else
	{
		for(i = 0; i < MAX_ITEMS; i++)
		{
			if(state->inventory[i] == state->currentItem && use == 0)
			{
				useItem(state, state->currentItem);
				state->inventory[i] = 0;
				use = 1; //used
			}
		}
		
		if(use == 0)
			printf("Don't have that item anymore.\n");
	}
}