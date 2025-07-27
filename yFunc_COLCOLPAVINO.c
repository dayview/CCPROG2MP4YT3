#include "yHeader_COLCOLPAVINO.h"

void saveGameFile(GameState *state, int rescuedIdols[], int achievements[]){

    FILE *fp = fopen("saveFile.dat", "wb");

    if (fp){
        fwrite(state, sizeof(GameState), 1, fp);
        fwrite(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fwrite(achievements, sizeof(int), 28, fp);
        fclose(fp);
        printf("Game saved!\n");
    } else {
        printf("Error saving...\n");
    }
}

int loadGameFile(GameState *state, int rescuedIdols[], int achievements[]){

    FILE *fp = fopen("saveFile.dat", "rb");

    int loaded = 0;

    if (fp){
        fread(state, sizeof(GameState), 1, fp);
        fread(rescuedIdols, sizeof(int), MAX_IDOLS, fp);
        fread(achievements, sizeof(int), 28, fp);
        fclose(fp);
        printf("Game loaded!\n");
        loaded = 1;
    } else {
        printf("Error loading...");
    }

    return loaded;
}

int duplicateCheck(int selected[], int count, int val){

    int i, found = 0;
    for (i = 0; i < count && found == 0; i++){
        if (selected[i] == val){
            found = 1;
        }
    }
    return found;
}

void setNewGame(GameState *state, const char *idolNames[]){
    int i, count = 0;
    srand((unsigned)time(NULL));
    while (!(count < SELECTED_IDOLS)){
        i = rand() % MAX_IDOLS;
        if (!duplicateCheck(state->selectedIdols, count, i)){
            state->selectedIdols[count] = i;
            state->doneDungeons[count] = 0;
            count++;
        }
    }
    state->gold = 0;
    state->hp = 3;
    state->maxHP = 3;
    state->usedChoco = 0;
    state->isGameOver = 0;
    state->currentItem = 0;

    for (i = 0; i < MAX_ITEMS; i++){
        state->inventory[i] = 0;
    }

    printf("\nRescue these idols in this playthrough:\n");
    for (i = 0; i < SELECTED_IDOLS; i++){
        printf("  - %s\n", idolNames[state->selectedIdols[i]]);
    }
}

void useItem(GameState *state, int item){
    if (item == ITEM_TEARS || item == ITEM_NOPPO){
        if (state->hp < state->maxHP){
            state->hp = state->hp + 0.5;
            if (state->hp > state->maxHP){
                state->hp = state->maxHP;
            }
            printf("Yohane healed +0.5 HP\n");
        } else {
            printf("HP already full!\n");
        }
    } else {
        if (item == ITEM_STEWSHINE || item == ITEM_MIKAN || item == ITEM_KUROSAWA){
            state->maxHP = state->maxHP + 1;
            printf("Max HP increased by 1!\n");
        }
    }
}

void useItemInHand(GameState *state){
    int i;
    int used;
    i = 0;
    used = 0;
    if (state->currentItem == 0){
        printf("No item in hand. Turn ends...\n");
    } else {
        while (i < MAX_ITEMS && used == 0){
            if (state->inventory[i] == state->currentItem){
                useItem(state, state->currentItem);
                state->inventory[i] = 0;
                used = 1;
            }
            i = i + 1;
        }
        if (used == 0){
            printf("Don't have that item anymore.\n");
        }
    }
}

void switchItem(GameState *state, char direction){
    int items[3] = {ITEM_TEARS, ITEM_NOPPO, ITEM_CHOCO};
    int owned[3] = {0, 0, 0};
    int held, total, i, j, check, next;
    held = -1;
    total = 0;
    i = 0;
    while (i < 3){
        j = 0;
        while (j < MAX_ITEMS){
            if (owned[i] == 0 && state->inventory[j] == items[i]){
                owned[i] = 1;
                total = total + 1;
            }
            j = j + 1;
        }
        if (state->currentItem == items[i]){
            held = i;
        }
        i = i + 1;
    }

    if (total <= 1){
        printf("Only one or no usable item. No switching.\n");
    } else {
        next = -1;
        i = 1;
        while (i <= 3 && next == -1){
            if (direction == ']'){
                check = (held + i) % 3;
            } else {
                check = (held - i + 3) % 3;
            }
            if (owned[check] == 1){
                next = check;
            }
            i = i + 1;
        }

        if (next != -1){
            state->currentItem = items[next];
            printf("Switched to: ");
            if (state->currentItem == ITEM_TEARS){
                printf("Tears of a fallen angel\n");
            } else {
                if (state->currentItem == ITEM_NOPPO){
                    printf("Noppo bread\n");
                } else {
                    if (state->currentItem == ITEM_CHOCO){
                        printf("Choco-mint ice cream\n");
                    }
                }
            }
        }
    }

    if (owned[0] == 0 && owned[1] == 0 && owned[2] == 0){
        printf("Item on hand: N/A\n");
        state->currentItem = 0;
    }
}

void checkChocoRevive(GameState *state){
    int i;
    i = 0;
    if (state->hp <= 0 && state->currentItem == ITEM_CHOCO && state->usedChoco == 0){
        while (i < MAX_ITEMS && state->usedChoco == 0){
            if (state->inventory[i] == ITEM_CHOCO){
                state->inventory[i] = 0;
                state->hp = state->maxHP;
                state->usedChoco = 1;
                printf("Choco-mint ice cream revived Yohane! HP fully restored.\n");
            }
            i = i + 1;
        }
    }
    if (state->hp <= 0){
        state->isGameOver = 1;
    }
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

void randomTile(Dungeon *dungeon, char tile)
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
}

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

void placeRandomTile(Dungeon *dungeon, char tile, int count)
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

void generateEmptyDungeon(Dungeon *dungeon)
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

void dungeonMenu(GameState *state)
{
	int i, Index;
	printf("Lailaps: Yohane! Where should we go to now?\n");
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

void checkRescueAchievements(int rescuedIdols[], int earned[], int currentIdol){

    if (!earned[currentIdol]){
        switch (currentIdol){
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

    if (rescuedIdols[0] >= 2 && !earned[13]){
        unlockAchievement(earned, 13, "One more sunshine story!");
    }
    if (rescuedIdols[1] >= 2 && !earned[14]){
        unlockAchievement(earned, 14, "Pianoforte Monologue!");
    }
    if (rescuedIdols[2] >= 2 && !earned[15]){
        unlockAchievement(earned, 15, "Beginner's Sailing!");
    }
    if (rescuedIdols[3] >= 2 && !earned[16]){
        unlockAchievement(earned, 16, "Oyasuminasan!");
    }
    if (rescuedIdols[4] >= 2 && !earned[17]){
        unlockAchievement(earned, 17, "Red Gem Wink!");
    }
    if (rescuedIdols[5] >= 2 && !earned[18]){
        unlockAchievement(earned, 18, "White First Love!");
    }
    if (rescuedIdols[6] >= 2 && !earned[19]){
        unlockAchievement(earned, 19, "Sakana ka Nandaka!");
    }
    if (rescuedIdols[7] >= 2 && !earned[20]){
        unlockAchievement(earned, 20, "New Winding Road!");
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

    if (rescuedIdols[0] && rescuedIdols[1] && rescuedIdols[2] && 
        rescuedIdols[3] && rescuedIdols[4] && rescuedIdols[5] && 
        rescuedIdols[6] && rescuedIdols[7] && !earned[21]){
        unlockAchievement(earned, 21, "Eikyuu Hours!");
    }
}

void checkMilestoneAchievements(GameState *state, int earned[], int totalDungeonsCleared){

    if (totalDungeonsCleared >= 10 && !earned[11]){
        unlockAchievement(earned, 11, "Dungeon Master");
    }

    if (state->gold <= 0 && !earned[12]){
        unlockAchievement(earned, 12, "Big Spender");
    }

    if (totalDungeonsCleared == 1 && !earned[22]){
        unlockAchievement(earned, 22, "Yohane Descends!");
    }

    if (totalDungeonsCleared >= 10 && !earned[23]){
        unlockAchievement(earned, 23, "No. 10!");
    }

    if (state->gold == 0 && !earned[25]){
        unlockAchievement(earned, 25, "Step! ZERO to ONE!");
    }
}

void checkFinalBossAchievement(int finalBossVictories, int earned[]){

    if (finalBossVictories >= 2 && !earned[24]){
        unlockAchievement(earned, 24, "Deep Resonance!");
    }
}

void checkDamageLessDungeonAchievement(int dungeonDamage, int earned[]){

    if (dungeonDamage == 0 && !earned[26]){
        unlockAchievement(earned, 26, "Aozora Jumping Heart!");
    }
}

void checkShopSpendingAchievement(int totalShopSpending, int earned[]){

    if (totalShopSpending >= 5000 && !earned[27]){
        unlockAchievement(earned, 27, "Mitaiken Horizon!");
    }
}

void checkChocoMintSaveAchievement(int earned[]){

    if (!earned[28]){
        unlockAchievement(earned, 28, "Ruby-chan! Hai? Nani ga suki?");
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
    int allRescued = 1;
    for (i = 0; i < MAX_IDOLS; i++){
        if (rescuedIdols[i] == 0){
            allRescued = 0;
        }
    }
    return allRescued;
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

void characterProfile(char Idols[], int choice){

    int i;

    printf("Character Profile:\n");
    for (i = 0; i < MAX_IDOLS; i++){
    printf("[%d] Name: %c", rescuedIdols[0]);
    
    switch (choice){
        case 1:
            printf("Yoshiko is a 1st year member of the idol group Aquors,
                    who claims she's a fallen angel who was cast out of heaven.
                    This persona enabled her alternate self in the mirror world
                    to possess magical powers, capable of protecting Numazu from supernatural threats.\n");
            printf("cv. Aika Kobayashi\n");
            break;
        case 2:
            printf("Lailaps exists in the mirror world as Yohane's loyal wolf
                    companion. She acts as the voice of reason and guides Yohane
                    throughout her mission.\n");
            printf("cv. Yoko Hikasa\n");
            break;
        case 3:
            printf("Chika is a 2nd year and the founding member of Aquors.
                    Her family runs a Ryokan inn, similarly to her mirror world self,
                    where she specializes in her family's signature Mikan Mochi!\n");
            printf("cv. Anju Inami\n");
            break;
        case 4:
            printf("You is a 2nd year member of Aquors, who is Chika's
                    childhood friend well-known for her athletic prowess.
                    In the mirror world, she's a post-lady with equipment that
                    allows her to travel far and wide through the land of Numazu.\n");
            printf("cv. Saito Shuka\n");
            break;
        case 5:
            printf("Riko is a 2nd year member of Aquors who transferred to Uchiura from Tokyo.
                    As the pianist and main composer of the group, her diligence carries over
                    to the mirror world where she acts as a zoologist capable of understanding
                    and manipulating beast behavior.\n");
            printf("cv. Rikako Aida\n");
            break;
        case 6:
            printf("Hanamaru is a 1st year member of Aquors, well-known for her fondness for
                    food, books, and her tendency to say "zura" at the end of every sentence.
                    She's a travelling merchant in the mirror world who specializes in selling sweets
                    and all sorts of goodies to help adventurers along their journey.\n");
            printf("cv. Kanako Takatsuki\n");
            break;
        case 7:
            printf("Ruby is a 1st year member of Aquors, who is Hanamaru's childhood friend.
                    This carries over to the mirror world where Ruby, now a fairy, helps Hanamaru
                    by supplying equipment to her shop. She also bestows magic in her sweet treats,
                    some of which is rumored to be "life-savingly" delicious.\n");
            printf("cv. Ai Furihata\n");
            break;
        case 8:
            printf("Dia is a 3rd year member of Aquors and is Ruby's older sister. As she's the
                    student council president of Uranohoshi high school, her mirro world self is the
                    chief of staff of the Numazu Administrative Bureau. However, her mirror self is known
                    to be highly fond of anything macha.\n");
            printf("cv. Arisa Komiya\n");
            break;
        case 9:
            printf("Kanan is a 3rd year member of Aquors, who runs a dive shop with her family. Her mirror
                    world self is a mechanic who creates powerful and useful gadgets to make working and
                    performing tasks in Numazu more convenient for citizens.\n");
            printf("cv. Nanaka Suwa\n");
            break;
        case 10:
            printf("Mari is a 3rd year member of Aquors, As part of an affluent and wealthy family,
                    she has been designated as the school director of Uranohoshi high school. Her
                    mirror self is no different, being a demon lord who commands a small horde
                    of friendly creatures in the island of Awashima.\n");
            printf("cv. Aina Suzuki\n");
            break;
        default:
            printf("Please select a character to read their description.\n");
            break;
        }
    }
}