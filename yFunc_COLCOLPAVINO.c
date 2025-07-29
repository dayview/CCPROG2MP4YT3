// ONCE SIREN HAS BEEN KILLED NO MORE BATS, AND PRIORITIZE YOHANE'S POSITION (but it's better to prioritize both positions, if possible)
#include "yHeader_COLCOLPAVINO.h"

void startGameLoop(GameState *state, int rescuedIdols[], int achievements[], const char *dungeonNames[], int *finalBossVictories, const char *idolNames[]){
    int done = 0;
    char choice;

    while (!done && state->isGameOver == 0){
        if (checkAllDungeonsCleared(state)){
            printf("All dungeons cleared! Final dungeon unlocked.\n");
            
            startFinalDungeon(state, achievements, finalBossVictories);

            done = 1;
        } else {
            displayDungeonSelection(state, rescuedIdols, state->selectedIdols, dungeonNames);
            scanf(" %c", &choice);

            if (choice == 'I' || choice == 'i'){
                displayInventory(state);
            } else if (choice == 'S' || choice == 's'){
                saveGameFile(state, rescuedIdols, achievements);
                done = 1;
            } else if ((choice == 'H' || choice == 'h') && rescuedIdols[3]){
                hanamaruShop(state, rescuedIdols, achievements);
            } else if (choice >= '1' && choice <= '3'){
                int dungeonIndex = choice - '1';
                if (state->doneDungeons[dungeonIndex] == 0){
                    Dungeon dungeon;
                    startDungeon(&dungeon, dungeonIndex);
                    dungeonLoop(&dungeon, state, dungeonIndex, rescuedIdols, achievements, dungeonNames, idolNames);
                } else {
                    printf("That dungeon has already been cleared.\n");
                }
            } else {
                printf("Invalid input.\n");
            }

            if (state->isGameOver == 1){
                printf("You died. Returning to main menu.\n");
                saveGameFile(state, rescuedIdols, achievements);
                done = 1;
            }
        }
    }
} // L

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
} // L & M

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
} // L & M

int duplicateCheck(int selected[], int count, int val){

    int i, found = 0;
    for (i = 0; i < count; i++){
        if (selected[i] == val){
            found = 1;
        }
    }
    return found;
} // L & M

void setNewGame(GameState *state, const char *idolNames[]){
    int i, count = 0;
    srand((unsigned)time(NULL));
    while (count < SELECTED_IDOLS){
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
} // L

void checkChocoRevive(GameState *state)
{
	int i;
	
	if(state->hp <= 0 && state->currentItem == ITEM_CHOCO && state->usedChoco == 0){
		for(i = 0; i < MAX_ITEMS; i++){
			if(state->inventory[i] == ITEM_CHOCO){
				state->inventory[i] = 0;
				state->hp = (float) state->maxHP;
				state->usedChoco = 1;
				printf("Choco-Mint Ice Cream revived Yohane! HP fully restored. \n");

                checkChocoMintSaveAchievement(state->inventory);
			}
		}
	}
	if (state->hp <= 0)
		triggerDeath(state, "Killed by: Siren");
} // M

void triggerDeath(GameState *state, const char *cause){
    state->isGameOver = 1;
    strcpy(state->deathReason, cause);

    printf("\n****************************************\n");
    printf("*               You Died!              *\n");
    printf("*            Killed by: %-14s*\n", cause);
    printf("****************************************\n");
} // L

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
} // M

void movement(char input, Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], const char *idolNames[])
{
	int x = dungeon->yohaneX;
	int y = dungeon->yohaneY;
	char Tile;
	int move = 1;
	int reward, gold; //for treasure
	int inv, i;
	int found = 0;
    int pickupGold;
	
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
	
	//check for out of bounds
	if (x < 0 || x >= ROWS || y < 0 || y >= COLS)
	{
		printf("Out of bounds. Stay inside the dungeon\n");
		move = 0;
	}
	
	Tile = dungeon->map[x][y];
	
	if (move == 1)
	{
		switch(Tile)
		{
			case '*':
				printf("Movement blocked. You've reached the border.\n");
				move = 0;
				break;
			case 'w':
				if (hasPassiveItem(state, ITEM_AIR))
					printf("Walk through the water using Air Shoes.\n");
				else
				{		
					printf("You can't swim! The water is currently blocking the way.\n");
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
					state->hp -= 0.5f;	
					checkChocoRevive(state);
					
					if (state->hp <= 0)
                        triggerDeath(state, "Spike Trap");

                    checkChocoRevive(state);
				}
				dungeon->map[x][y] = '.';			
				move = 0;
				break;
			case 'T':
				printf("You found a treasure chest!\n");
				reward = rand()%2; //0 - gold 1 - item
				if (reward == 0){
					gold = (rand() % 91) + 10;
					printf("You gained %d gold\n",gold);
					state->gold += gold;
				} else {
                    printf("You found Noppo Bread!\n");

                    for (inv = 0; inv < MAX_ITEMS; inv++){
                        if (found == 0 && state->inventory[inv] == 0){
                            state->inventory[inv] = ITEM_NOPPO;
                            found = 1;
                        }
                    }

                    if (found == 0)
                        printf("Inventory full! Couldn't pick up the item.\n");
					
				}
				dungeon->map[x][y] = '.';
				break;
			case 'E':
				printf("You Found the Exit! Proceed to next. \n");
				//ADD CODE BELOW
				if(nextFloor(dungeon))
				{
                    state->doneDungeons[currentDungeon] = 1;
					rescuedIdols[state->selectedIdols[currentDungeon]]++;
					
                    checkRescueAchievements(rescuedIdols, state->inventory, state->selectedIdols[currentDungeon]);
                    postDungeonFeedback(state->selectedIdols[currentDungeon], idolNames);
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
				if(currentDungeon == 0){
			        pickupGold = 5;
                } else if(currentDungeon == 1) {
			        pickupGold = 10;
                } else {
			        pickupGold = 15;
                }

                state->gold += pickupGold;
				printf("You picked up %d gold! Total gold: %d\n", pickupGold, state->gold);
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
			state->hp -= 1.0f; 
			checkChocoRevive(state);
			
			if(state->hp <= 0)
                triggerDeath(state, "Heat Tile");
            checkChocoRevive(state);
			
		}
		else
			printf("You stand on heat with your air shoes. No damage taken\n");
			
	}
	
	if(currentDungeon == 0)
	{
		state->batMoveCounter++;
		
		if(state->batMoveCounter >= 2)
		{
			BatMovement(dungeon, state, currentDungeon);
			state->batMoveCounter = 0;
		}
	}
	else
		BatMovement(dungeon, state, currentDungeon);
} // M 

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
	
	//default position (we can change this)
	dungeon->yohaneX = 1;
	dungeon->yohaneY = 1;
	dungeon->floor = 1;
	
    randomTile(dungeon, TILE_WALL);
    randomTile(dungeon, TILE_HEAT);
    randomTile(dungeon, TILE_SPIKE);
    randomTile(dungeon, TILE_WATER);
	
} // M

void displayDungeon(Dungeon *dungeon, GameState *state, int dungeonNumber, const char *dungeonName[]){
	int i, j;
    char itemName[50] = "N/A";
    int itemQty = 0;

    getItemInfo(state, state->currentItem, itemName, &itemQty);

    printf("\nDungeon #%d: %s\n", dungeonNumber+1, dungeonName[dungeonNumber]);
    printf("Floor %d of %d\n\n", dungeon->floor, dungeon->maxFloor);
    printf("HP: %.1f / %d\t\tTotal Gold: %d GP\n", state->hp, state->maxHP, state->gold);
    printf("Item on hand: %s", itemName);

    if (itemQty > 0)
        printf(" (%d)", itemQty);
    printf("\n\n");
	
	for (i = 0; i < ROWS; i++){
		for (j = 0; j < COLS; j++){
			if (i == dungeon->yohaneX && j == dungeon->yohaneY)
				printf("%c", TILE_YOHANE);
			else 
				printf("%c", dungeon->map[i][j]);
		}
		printf("\n");
	}
} // L & M

void startDungeon(Dungeon *dungeon, int currentDungeon) //current is tracker for how many dungeon already cleared
{
	int batCount;
	dungeon->floor = 1;
	
	if(currentDungeon == 0)
		dungeon->maxFloor = 1;
	else 
		dungeon->maxFloor = (rand() % 2) + 2; // random 2 or 3
		
	generateEmptyDungeon(dungeon);
	if(currentDungeon == 0)
		batCount =5;
		
	else if(currentDungeon == 1)
		batCount =10;
		
	else
		batCount = 15;
		
	randomBat(dungeon, batCount);
		
	randomTile(dungeon, TILE_TREASURE);
	randomTile(dungeon, TILE_EXIT);
}

void startFinalDungeon(GameState *state, int achievements[], int *finalBossVictories){
    int yohanePos[2], lailapsPos[2], sirenPos[2];
    int switches[6];
    int grid[ROWS][COLS];
    int switchesActivated = 0;
    char input;
    int sirenDefeated = 0;
    int loop = 1;
    int turnCount = 0;
    int i, j;

    Dungeon finalDungeon;
    generateEmptyDungeon(&finalDungeon);

    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
            grid[i][j] = 0;

    bat bats[MAX_BATS];
    for (i = 0; i < MAX_BATS; i++)
        bats[i].alive = 0;

    initializeFinalDungeon(yohanePos, lailapsPos, switches, sirenPos);
    displayFinalDungeon(yohanePos, lailapsPos, sirenPos, grid);

    printf("\nLailaps: Yohane, the Siren awaits. Be careful!\n");

    while (loop == 1){
        printf("\nEnter command (WASD to move, space to use item, [ or ] to switch): ");
        scanf(" %c", &input);

        moveYohaneAndLailaps(input, yohanePos, lailapsPos, grid);
        moveSiren(sirenPos, yohanePos, lailapsPos, state);

        displayFinalDungeon(yohanePos, lailapsPos, sirenPos, grid);

        if (checkSwitchActivation(yohanePos, lailapsPos, switches, grid)){
            switchesActivated++;
            printf("Switch %d activated!\n", switchesActivated);
        }

        turnCount++;

        if (turnCount % 8 == 0 && sirenDefeated == 0){
            spawnBatFinalDungeon(grid, bats, switchesActivated, sirenPos, yohanePos, lailapsPos);
        }

        moveFinalDungeonBats(bats, grid, state, yohanePos, lailapsPos, switchesActivated);

        if (state->isGameOver == 1){
            printf("Yohane has fallen. Game over.\n");
            loop = 0;
        }

        if (checkSirenDefeat(yohanePos, sirenPos)){
            sirenDefeated = 1;
            handleSirenDefeat(state, achievements, NULL, finalBossVictories);
        }

        if (sirenDefeated == 1 && grid[yohanePos[0]][yohanePos[1]] == TILE_EXIT){
            printf("You escaped the Mirror World!\n");
            carryOverProgress(state);
            resetIdolSelection(achievements);
            loop = 0;
        }
    }
} // L

int nextFloor(Dungeon *dungeon)
{
	int floors = -1;
	if(dungeon->floor < dungeon->maxFloor)
	{
		dungeon->floor++;
		generateEmptyDungeon(dungeon);
		randomTile(dungeon, TILE_TREASURE);
		randomTile(dungeon, TILE_EXIT);
		printf("Moved to next floor: %d/%d\n",dungeon->floor,dungeon->maxFloor);
		floors = 0; //more floors
	}
	else
	{
		printf("Dungeon Clear! Congrats! \n");
		floors = 1;
	}
	
	return floors;
} // M

void dungeonMenu(GameState *state, const char *dungeonName[])
{
	int i, index;
	printf("Lailaps: Yohane! Where should we go to now?\n");
	printf("HP: %.1f / %d \tTotal Gold: %d GP\n",state->hp, state->maxHP,state->gold);
	printf("Item on hand: ");

	if (state->currentItem == ITEM_TEARS)
		printf("Tears of a Fallen Angel\n");
	else if (state->currentItem == ITEM_NOPPO)
		printf("Noppo Bread\n");
	else if (state->currentItem == ITEM_CHOCO)
		printf("Choco-Mint Ice Cream\n");
	else
		printf("N/A\n");
	
	for (i = 0; i < SELECTED_IDOLS; i++)
	{
		index = state->selectedIdols[i];
		if (state->doneDungeons[i] == 0)
			printf("[%d] Visit %s\n", i + 1, dungeonName[index]);
		else 
			printf("[X] Visit %s\n", dungeonName[index]);
	}
	
	printf("\n[I]nventory\t[S]ave and Quit\n");
	printf("Choice: ");
} // M

void useItem(GameState *state, int item)
{
    switch(item)
    {
        case ITEM_TEARS:
            if(state->hp < state->maxHP)
            {
                state->hp += 0.5f;
                if(state->hp > state->maxHP)
                    state->hp = (float)state->maxHP;
                printf("Yohane used Tears of the fallen Angel. Heals +0.5 HP\n");
            }
            else
                printf("HP is already full!\n");
            break;

        case ITEM_NOPPO:
            if(state->hp < state->maxHP)
            {
                state->hp += 0.5f;
                if(state->hp > state->maxHP)
                    state->hp = (float) state->maxHP;
                printf("Yohane used Noppo Bread. Heals +0.5 HP\n");
            }
            else
                printf("HP is already full!\n");
            break;

    }
} // M 

void displayInventory(GameState *state){
    char itemName[50] = "N/A";
    int currentQty = 0;

    int tear = countItem(state, ITEM_TEARS);
    int noppo = countItem(state, ITEM_NOPPO);
    int choco = countItem(state, ITEM_CHOCO);

    getItemInfo(state, state->currentItem, itemName, &currentQty);

    printf("\nLailaps: These are the items you have, Yohane!\n");
    printf("HP: %.1f / %d\tTotal Gold: %d GP\n", state->hp, state->maxHP, state->gold);
    printf("Item on hand: %s", itemName);

    if (currentQty > 0){
        printf("(%d)", currentQty);
    }
    
    printf("\n\nItems Available:\n");
    printf("1. Tears of a Fallen Angel\t x %d\n", tear);
    printf("2. Noppo Bread\t\t\t x %d\n", noppo);
    printf("3. Choco-Mint Ice Cream\t x %d\n", choco);
    printf("\n[R]eturn\n");
    printf("Choice: ");
} // L & M

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
				printf("Tears of a Fallen Angel\n");
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
} // M

void useItemInHand(GameState *state)
{
    int i;
    int use = 0;

    if (state->currentItem == 0) {
        printf("No item in hand. Turn ended...\n");
    } else {
        for (i = 0; i < MAX_ITEMS; i++) {
            if (state->inventory[i] == state->currentItem && use == 0) {
                useItem(state, state->currentItem);
                state->inventory[i] = 0;
                use = 1; // used
            }
        }

        if (use == 0) {
            printf("Don't have that item anymore.\n");
        }
    }
} // M & L

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
} // M 

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
} // M 

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
	
	if(hasPassiveItem(state, ITEM_BAT))
		damage = 0.5;
		
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
					attacked = 1;
					dungeon->bats[i].attack = 1;
					dungeon->map[batX][batY] = 'B';

					if(state->hp <= 0)
                        triggerDeath(state, "Bat");
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
} // M

void postDungeonFeedback(int idolID, const char *idolNames[]){

    printf("\n*******************************************************\n");
    printf("                 Dungeon Cleared!\n");
    printf("          %s rescued!\n", idolNames[idolID]);
    printf("\n*******************************************************\n");
} // L

void displayAchievements(int earned[], int totalAchievements, const char *achievementNames[]) {
    int i;
    int currentPage = 0;
    int achievementsPerPage = 8;
    int totalPages = (totalAchievements + achievementsPerPage - 1) / achievementsPerPage;
    int done = 0;
    char choice[10];

    const char *descriptions[28] = {
        "Cleared first dungeon",                   // 1
        "Rescued Chika for the first time",        // 2
        "Rescued Riko for the first time",         // 3
        "Rescued You for the first time",          // 4
        "Rescued Hanamaru for the first time",     // 5
        "Rescued Ruby for the first time",         // 6
        "Rescued Dia for the first time",          // 7
        "Rescued Kanan for the first time",        // 8
        "Rescued Mari for the first time",         // 9
        "Beat the Final boss for the first time",  // 10
        "Rescued Chika twice",                     // 11
        "Rescued Riko twice",                      // 12
        "Rescued You twice",                       // 13
        "Rescued Hanamaru twice",                  // 14
        "Rescued Ruby twice",                      // 15
        "Rescued Dia twice",                       // 16
        "Rescued Kanan twice",                     // 17
        "Rescued Mari twice",                      // 18
        "Beat the Final boss twice",               // 19
        "Clear 10 dungeons",                       // 20
        "Rescued Chika, You, and Ruby",            // 21
        "Rescued Hanamaru, Dia, and Kanan",        // 22
        "Rescued Riko and Mari",                   // 23
        "Rescue all Aqours members",               // 24
        "Clear a dungeon without damage",          // 25
        "Spent 5000G at Hanamaru’s shop",          // 26
        "Saved by Ruby’s choco-mint ice cream",    // 27
        "Complete a run with 0G remaining"         // 28
    };

    while (done == 0) {
        int earnedCount = 0;
        for (i = 0; i < totalAchievements; i++) {
            if (earned[i]) {
                earnedCount++;
            }
        }

        printf("**************************************************\n");
        printf("               Achievements Module\n");
        printf("               Obtained: %d / %d\n", earnedCount, totalAchievements);
        printf("**************************************************\n");

        int startIndex = currentPage * achievementsPerPage;
        int endIndex = startIndex + achievementsPerPage;
        if (endIndex > totalAchievements) {
            endIndex = totalAchievements;
        }

        for (i = startIndex; i < endIndex; i++) {
            if (earned[i]) {
                printf("[%2d] %-35s %s\n", i - startIndex + 1, achievementNames[i], "EARNED!");
            } else {
                printf("[%2d] %-35s %s\n", i - startIndex + 1, achievementNames[i], "NOT EARNED!");
            }
        }

        printf("\nPage %d of %d\n", currentPage + 1, totalPages);
        printf("[N]ext Page\t[P]revious Page\t[R]eturn to Main Menu\n");
        printf("Choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "N") == 0 || strcmp(choice, "n") == 0) {
            if (currentPage < totalPages - 1) {
                currentPage++;
            } else {
                printf("Already on last page.\n");
            }
        } else if (strcmp(choice, "P") == 0 || strcmp(choice, "p") == 0) {
            if (currentPage > 0) {
                currentPage--;
            } else {
                printf("Already on first page.\n");
            }
        } else if (strcmp(choice, "R") == 0 || strcmp(choice, "r") == 0) {
            done = 1;
        } else {
            int num = atoi(choice);
            if (num >= 1 && num <= achievementsPerPage && startIndex + num - 1 < totalAchievements) {
                int actualIndex = startIndex + num - 1;
                printf("\n************************************************************\n");
                printf("Achievement Name: %s\n", achievementNames[actualIndex]);
                if (earned[actualIndex]) {
                    printf("STATUS: EARNED!\n");
                } else {
                    printf("STATUS: NOT EARNED!\n");
                }
                printf("\nDescription:\n%s\n", descriptions[actualIndex]);
                printf("\n[R]eturn to Achievements Module\n");
                printf("Choice: ");
                scanf("%s", choice);
            } else {
                printf("Invalid choice.\n");
            }
        }
    }
} // L 

void unlockAchievement(int earned[], int index, const char *message){

    if (!earned[index]){
        earned[index] = 1;
        printf("Achievement Unlocked: %s\n", message);
    }
} // L

void displayDungeonSelection(GameState *state, int rescuedIdols[], int currentIdols[], const char* dungeonName[]){
    int i;
    char itemName[50] = "N/A";
    int currentQty = 0;

    getItemInfo(state, state->currentItem, itemName, &currentQty);

    printf("\n=== Dungeon Selection ===\n");
    printf("Yohane's HP: %.1f / %d\n", state->hp, state->maxHP);
    printf("Total Gold: %d GP\n", state->gold);
    printf("Item on hand: %s", itemName);

    if (currentQty > 0){
        printf(" (%d)", currentQty);
    } else if (state->currentItem != 0){
        printf(" (0)");
    }
    printf("\n\n");

    printf("Available Dungeons:\n");
    for (i = 0; i < SELECTED_IDOLS; i++){
        if (state->doneDungeons[i] == 0){
            printf("[%d] %s's Dungeon\n", i+1, dungeonName[currentIdols[i]]);
        } else {
            printf("[X] %s's Dungeon (Cleared)\n", dungeonName[currentIdols[i]]);
        }
    }

    printf("\n[I]nventory\t[S]ave and Quit");

    if (rescuedIdols[3]){
        printf("\t[H]anamaru Shop");
    }

    printf("\nChoice: ");
} // L

int checkAllDungeonsCleared(GameState *state){

    int i;
    int allCleared = 1;
    for (i = 0; i < SELECTED_IDOLS; i++){
        if (state->doneDungeons[i] == 0){
            allCleared = 0;
        }
    }
    return allCleared;
} // L

void markDungeonClear(GameState *state, int dungeonIndex){

    if (dungeonIndex >= 0 && dungeonIndex < SELECTED_IDOLS){
        state->doneDungeons[dungeonIndex] = 1;
    }
} // L

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
} // L

void displayFinalDungeon(int yohanePos[], int lailapsPos[], int sirenPos[], int grid[ROWS][COLS]){

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
} // L

int checkSwitchActivation(int yohanePos[], int lailapsPos[], int switches[], int grid[ROWS][COLS]){

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
} // L

void moveYohaneAndLailaps(char input, int yohanePos[], int lailapsPos[], int grid[ROWS][COLS]){
    int dx = 0, dy = 0;

    switch (input){
        case 'W':
        case 'w':
            dx = -1;
            break;
        case 'S':
        case 's':
            dx = 1;
            break;
        case 'A':
        case 'a':
            dy = -1;
            break;
        case 'D':
        case 'd':
            dy = 1;
            break;
        case 'X':
        case 'x':
            printf("Yohane and Lailaps stay in place.\n");
            break;
        default:
            printf("Invalid input. Turn continues.\n"); // unsure if this continues
            break;
    }

    int newYohaneX = yohanePos[0] + dx;
    int newYohaneY = yohanePos[1] + dy;
    int newLailapsX = lailapsPos[0] + dx;
    int newLailapsY = lailapsPos[1] + dy;

    if (newYohaneX >= 0 && newYohaneX < 10 && newYohaneY >= 0 && newYohaneY < 10){
        if (grid[newYohaneX][newYohaneY] != 2){
            yohanePos[0] = newYohaneX;
            yohanePos[1] = newYohaneY;
        }
    }

    if (newLailapsX >= 0 && newLailapsX < 10 && newLailapsY >= 0 && newLailapsY < 10){
        if (grid[newLailapsX][newLailapsY] != 2){
            lailapsPos[0] = newLailapsX;
            lailapsPos[1] = newLailapsY;
        }
    }
} // L 

int checkSirenDefeat(int yohanePos[], int sirenPos[]){

    int rowDiff = abs(yohanePos[0] - sirenPos[0]);
    int colDiff = abs(yohanePos[1] - sirenPos[1]);
    int canDefeat = 0;

    if ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1)){
        canDefeat = 1;
    }
    return canDefeat;
} // L

void handleSirenDefeat(GameState *state, int earned[], Dungeon *finalDungeon, int *finalBossVictories){
    int placed = 0;
    int x, y;

    printf("\nYou defeated the Siren of the Mirror World!\n");

    printf("Lailaps: We've killed the Siren, Yohane!\n");
    printf("You have gained 750 gold.\n");
    state->gold += 750;

    if (!earned[9]){
        unlockAchievement(earned, 9, "In This Unstable World!");
        printf("Achievement Unlocked: In This Unstable World!\n");
    }

    *finalBossVictories += 1;
    checkFinalBossAchievement(*finalBossVictories, earned);

    if (finalDungeon != NULL){
        while (!placed){
            x = rand() % (ROWS-2) + 1;
            y = rand() % (COLS-2) + 1;
            if (finalDungeon->map[x][y] == TILE_PASSABLE){
                finalDungeon->map[x][y] = TILE_EXIT;
                placed = 1;
                printf("A glowing portal appears at (%d, %d)...\n", x, y);
            }
        }
        printf("Proceed to the exit to leave the mirror world...\n");
    } else {
        printf("Proceed to the exit to leave the mirror world...\n");
        printf("A glowing portal appears where the Siren once stood.\n");
    }
} // L 

void moveSiren(int sirenPos[], int yohanePos[], int lailapsPos[], GameState *state){

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

    if ((abs(sirenPos[0] - yohanePos[0]) + abs(sirenPos[1] - yohanePos[1])) == 1){
        printf("The Siren attacked Yohane!\n");
        checkChocoRevive(state);
        if (state->hp <= 0){
            triggerDeath(state, "Siren (Yohane)");
        }
    }
    
    if ((abs(sirenPos[0] - lailapsPos[0]) + abs(sirenPos[1] - lailapsPos[1])) == 1){
        printf("The Siren attacked Lailaps! Instant Game Over.\n");
        triggerDeath(state, "Siren (Lailaps)");
    }
} // L 

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
} // l

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
} // l

void checkDamageLessDungeonAchievement(int dungeonDamage, int earned[]){

    if (dungeonDamage == 0 && !earned[26]){
        unlockAchievement(earned, 26, "Aozora Jumping Heart!");
    }
} // l

void checkShopSpendingAchievement(int totalShopSpending, int earned[]){

    if (totalShopSpending >= 5000 && !earned[27]){
        unlockAchievement(earned, 27, "Mitaiken Horizon!");
    }
} // l 

void checkChocoMintSaveAchievement(int earned[]){

    if (!earned[28]){
        unlockAchievement(earned, 28, "Ruby-chan! Hai? Nani ga suki?");
    }
} // l

void checkFinalBossAchievement(int finalBossVictories, int earned[]){

    if (finalBossVictories >= 2 && !earned[24]){
        unlockAchievement(earned, 24, "Deep Resonance!");
    }
} // l 

void characterProfile(int choice){
    printf("\n=== Character Profile ===\n");
    
    switch (choice){
        case 1:
            printf("Yoshiko is a 1st year member of the idol group Aquors,\n");
            printf("who claims she's a fallen angel who was cast out of heaven.\n");
            printf("This persona enabled her alternate self in the mirror world\n");
            printf("to possess magical powers, capable of protecting Numazu from supernatural threats.\n");
            printf("cv. Aika Kobayashi\n");
            break;
        case 2:
            printf("Lailaps exists in the mirror world as Yohane's loyal wolf\n");
            printf("companion. She acts as the voice of reason and guides Yohane\n");
            printf("throughout her mission.\n");
            printf("cv. Yoko Hikasa\n");
            break;
        case 3:
            printf("Chika is a 2nd year and the founding member of Aquors.\n");
            printf("Her family runs a Ryokan inn, similarly to her mirror world self,\n");
            printf("where she specializes in her family's signature Mikan Mochi!\n");
            printf("cv. Anju Inami\n");
            break;
        case 4:
            printf("You is a 2nd year member of Aquors, who is Chika's\n");
            printf("childhood friend well-known for her athletic prowess.\n");
            printf("In the mirror world, she's a post-lady with equipment that\n");
            printf("allows her to travel far and wide through the land of Numazu.\n");
            printf("cv. Saito Shuka\n");
            break;
        case 5:
            printf("Riko is a 2nd year member of Aquors who transferred to Uchiura from Tokyo.\n");
            printf("As the pianist and main composer of the group, her diligence carries over\n");
            printf("to the mirror world where she acts as a zoologist capable of understanding\n");
            printf("and manipulating beast behavior.\n");
            printf("cv. Rikako Aida\n");
            break;
        case 6:
            printf("Hanamaru is a 1st year member of Aquors, well-known for her fondness for\n");
            printf("food, books, and her tendency to say 'zura' at the end of every sentence.\n");
            printf("She's a travelling merchant in the mirror world who specializes in selling sweets");
            printf("and all sorts of goodies to help adventurers along their journey.\n");
            printf("cv. Kanako Takatsuki\n");
            break;
        case 7:
            printf("Ruby is a 1st year member of Aquors, who is Hanamaru's childhood friend.\n");
            printf("This carries over to the mirror world where Ruby, now a fairy, helps Hanamaru\n");
            printf("by supplying equipment to her shop. She also bestows magic in her sweet treats,\n");
            printf("some of which is rumored to be 'life-savingly' delicious.\n");
            printf("cv. Ai Furihata\n");
            break;
        case 8:
            printf("Dia is a 3rd year member of Aquors and is Ruby's older sister. As she's the\n");
            printf("student council president of Uranohoshi high school, her mirro world self is the\n");
            printf("chief of staff of the Numazu Administrative Bureau. However, her mirror self is known\n");
            printf("to be highly fond of anything macha.\n");
            printf("cv. Arisa Komiya\n");
            break;
        case 9:
            printf("Kanan is a 3rd year member of Aquors, who runs a dive shop with her family. Her mirror\n");
            printf("world self is a mechanic who creates powerful and useful gadgets to make working and\n");
            printf("performing tasks in Numazu more convenient for citizens.\n");
            printf("cv. Nanaka Suwa\n");
            break;
        case 10:
            printf("Mari is a 3rd year member of Aquors, As part of an affluent and wealthy family,\n");
            printf("she has been designated as the school director of Uranohoshi high school. Her\n");
            printf("mirror self is no different, being a demon lord who commands a small horde\n");
            printf("of friendly creatures in the island of Awashima.\n");
            printf("cv. Aina Suzuki\n");
            break;
        default:
            printf("Invalid character number. Please choose 1 to 10.\n");
            break;
        }
} // L

void carryOverProgress(GameState *state){

    printf("Progress carried over to new playthrough!\n");

    state->maxHP = 3;
    state->hp = 3.0;
    state->usedChoco = 0;
    state->currentItem = 0;

    printf("Gold: %d GP\n", state->gold);

    int i;
    for (i = 0; i < MAX_ITEMS; i++){
        if (state->inventory[i] != ITEM_TEARS && state->inventory[i] != ITEM_NOPPO)
            state->inventory[i] = 0;
    }

    for (i = 0; i < SELECTED_IDOLS; i++){
        state->doneDungeons[i] = 0;
    }

    printf("Gold: %d GP\n", state->gold);
    printf("Items carried over:\n");

    int tearCount = 0, noppoCount = 0;

    for (i = 0; i < MAX_ITEMS; i++){
        if (state->inventory[i] == ITEM_TEARS)
            tearCount++;
        else if (state->inventory[i] == ITEM_NOPPO)
            noppoCount++;
    }

    if (tearCount > 0)
        printf("- Tears of a Fallen Angel: %d\n", tearCount);
    if (noppoCount > 0)
        printf("- Noppo Bread: %d\n", noppoCount);
} // L

int allIdolsRescued(int rescuedIdols[]){
    int i;
    int allRescued = 1;
    for (i = 0; i < MAX_IDOLS; i++){
        if (rescuedIdols[i] == 0){
            allRescued = 0;
        }
    }
    return allRescued;
} // L

void resetIdolSelection(int rescuedIdols[]){
    int i;

    if (allIdolsRescued(rescuedIdols)){
        for (i = 0; i < MAX_IDOLS; i++){
            rescuedIdols[i] = 0;
        }
        printf("All idols rescued! Selection reset for next playthrough.\n");
    }
} // L

void hanamaruShop(GameState *state, int rescuedIdols[], int achievements[]){
    ShopItem shopItems[] = {
        {"Tears of a Fallen Angel", 30, -1, ITEM_TEARS, "Heals Yohane .5 HP"},
        {"Noppo Bread", 100, -1, ITEM_NOPPO, "Heals Yohane .5 HP"},
        {"Shovel Upgrade", 300, 6, ITEM_SHOVEL, "Allows Yohane to dig spike walls without damage"},
        {"Bat Tamer", 400, 1, ITEM_BAT, "Turns damage from all bats to a constant .5 HP damage"},
        {"Air Shoes", 500, 2, ITEM_AIR, "Allows Yohane to walk over water tiles. Prevents damage from standing on heat tiles."},
        {"Stewshine", 1000, 7, ITEM_STEWSHINE, "Increases Yohane’s HP by 1 permanently"},
        {"Mikan Mochi", 1000, 0, ITEM_MIKAN, "Increases Yohane’s HP by 1 permanently"},
        {"Kurosawa Macha", 1000, 5, ITEM_KUROSAWA, "Increases Yohane’s HP by 1 permanently"},
        {"Choco-Mint Ice Cream", 2000, 4, ITEM_CHOCO, "Saves Yohane from a fatal hit when on hand. Heals Yohane to full health afterwards"}
    };

    int numItems = sizeof(shopItems) / sizeof(shopItems[0]);
    int exitShop = 0;

    while (!exitShop) {
        int menuMap[numItems];
        int visibleItemCount = 0;
        char input[10], confirm[10];
        int choice = -1, i, invSlot, foundSlot = 0, c;

        printf("\n========================================\n");
        printf("         HANAMARU'S SHOP\n");
        printf("========================================\n");
        printf("Gold: %d GP\n", state->gold);
        printf("Current Inventory:\n");
        printf("  Tears of a Fallen Angel: %d\n", state->inventory[0]);
            for (i = 0; i < MAX_ITEMS; i++)
                if (state->inventory[i] == ITEM_TEARS)
                    printf("+");
        printf("  Noppo Bread: %d\n", state->inventory[1]);
            for (i = 0; i < MAX_ITEMS; i++)
                if (state->inventory[i] == ITEM_NOPPO)
                    printf("+");
        printf("  Choco-Mint Ice Cream: %d\n", state->inventory[2]);
            for (i = 0; i < MAX_ITEMS; i++)
                if (state->inventory[i] == ITEM_CHOCO)
                    printf("+");
        printf("========================================\n");
        printf("Available Items:\n");

        for (i = 0; i < numItems; i++) {
            if (shopItems[i].unlockIdol == -1 || rescuedIdols[shopItems[i].unlockIdol]) {
                menuMap[visibleItemCount] = i;
                printf("%d) %-22s - %4d GP\n", visibleItemCount+1, shopItems[i].name, shopItems[i].cost);
                visibleItemCount++;
            }
        }

        printf("[R]eturn\n");
        printf("Choice: ");
        scanf("%s", input);
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (strcmp(input, "R") == 0 || strcmp(input, "r") == 0) {
            printf("Returning to main menu...\n");
            exitShop = 1;
        } else {
            choice = atoi(input);
            if (choice < 1 || choice > visibleItemCount){
                printf("Invalid choice!\n");
            } else {
                int selectedIndex = menuMap[choice-1];
                ShopItem *item = &shopItems[selectedIndex];

                printf("\n%s\n", item->description);
                printf("Cost: %d GP\n", item->cost);
                printf("Purchase? (Y/N): ");
                scanf("%s", confirm);
                while ((c = getchar()) != '\n' && c != EOF) {}

                if (strcmp(confirm, "Y") == 0 || strcmp(confirm, "y") == 0){
                    if (state->gold >= item->cost){
                        foundSlot = 0;
                        for (invSlot = 0; invSlot < MAX_ITEMS; invSlot++){
                            if (state->inventory[invSlot] == 0 && foundSlot == 0){
                                state->inventory[invSlot] = item->inventoryIndex;

                                if (item->inventoryIndex == ITEM_STEWSHINE ||
                                    item->inventoryIndex == ITEM_MIKAN ||
                                    item->inventoryIndex == ITEM_KUROSAWA){
                                        state->maxHP++;
                                    printf("Yohane's Max HP increased by 1! HP: %.1f / %d\n", state->hp, state->maxHP);
                                }
                                foundSlot = 1;
                            }
                        }

                        if (foundSlot){
                            state->gold -= item->cost;
                            state->totalShopSpent += item->cost;

                            checkShopSpendingAchievement(state->totalShopSpent, achievements);

                            printf("Purchase successful! %s added to inventory.\n", item->name);
                            printf("Remaining Gold: %d GP\n", state->gold);
                        } else {
                            printf("Inventory full! Could not add item.\n");
                            printf("Purchased cancelled and gold refunded.\n");
                        }
                    } else {
                        printf("Not enough gold! You need %d GP more.\n", item->cost - state->gold);
                    }
                } else {
                        printf("Purchase cancelled.\n");
                }
            }
        }
    }
} // L & M

int countItem(GameState *state, int itemID){
    int i, count = 0;
    for (i = 0; i < MAX_ITEMS; i++){
        if (state->inventory[i] == itemID)
            count++;
    }
    return count;
}

void getItemInfo(GameState *state, int itemID, char *name, int *qty){
    *qty = countItem(state, itemID);

    switch (itemID){
        case ITEM_TEARS:
            strcpy(name, "Tears of a Fallen Angel");
            break;
        case ITEM_NOPPO:
            strcpy(name, "Noppo Bread");
            break;
        case ITEM_CHOCO:
            strcpy(name, "Choco-mint Ice Cream");
            break;
        default:
            strcpy(name, "N/A");
            *qty = 0;
            break;
    }
}

void dungeonLoop(Dungeon *dungeon, GameState *state, int currentDungeon, int rescuedIdols[], int achievements[], const char *dungeonName[], const char *idolNames[]){
    int done = 0;
    char input;

    while (done == 0 && state->isGameOver == 0){
        displayDungeon(dungeon, state, currentDungeon, dungeonName);

        printf("Enter move (WASD, [ or ] to switch items, space to use, X to stay): ");
        scanf(" %c", &input);

        movement(input, dungeon, state, currentDungeon, rescuedIdols, idolNames);

        if (state->isGameOver == 1){
            printf("\nYohane has fallen! Returning to main menu...\n");
            saveGameFile(state, rescuedIdols, achievements);
            done = 1;
        }

        if (state->doneDungeons[currentDungeon] == 1){
            done = 1;
        }
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

void spawnBatFinalDungeon(int grid[ROWS][COLS], bat bats[], int switchesActivated, int sirenPos[], int yohanePos[], int lailapsPos[]){
    int i, x, y;
    int placed = 0;

    for (i = 0; i < MAX_BATS && placed == 0; i++){
        if (bats[i].alive == 0){
            int attempts = 0;
            do {
                x = rand() % ROWS;
                y = rand() % COLS;
                attempts++;

                if (grid[x][y] == 0 &&
                    !(x == sirenPos[0] && y == sirenPos[1]) &&
                    !(x == yohanePos[0] && y == yohanePos[1]) &&
                    !(x == lailapsPos[0] && y == lailapsPos[1])){
                        
                    bats[i].x = x;
                    bats[i].y = y;
                    bats[i].alive = 1;
                    bats[i].attack = 0;

                    grid[x][y] = 3;
                    printf("A bat has spawned in the mirror world...\n");

                    placed = 1;
                }
            } while (!placed && attempts < 100);
        }
    }
}

void moveFinalDungeonBats(bat bats[], int grid[ROWS][COLS], GameState *state, int yohanePos[], int lailapsPos[], int switchesActivated){
    int i, j;
    int directionCount = 0;
    float damage = 0.0;
    int attacked = 0;

    if (switchesActivated == 0){
        directionCount = 4;
        damage = 0.5;
    } else if (switchesActivated == 1){
        directionCount = 4;
        damage = 1.0;
    } else {
        directionCount = 8;
        damage = 1.5;
    }

    if (hasPassiveItem(state, ITEM_BAT)){
        damage = 0.5;
    }

    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (i = 0; i < MAX_BATS; i++){
        if (bats[i].alive == 1){
            int batX = bats[i].x;
            int batY = bats[i].y;
            int moved = 0;

            for (j = 0; j < directionCount && attacked == 0; j++){
                int adjX = batX + directions[j][0];
                int adjY = batY + directions[j][1];

                if ((adjX == yohanePos[0] && adjY == yohanePos[1]) ||
                    (adjX == lailapsPos[0] && adjY == lailapsPos[1])){
                        if (adjX == yohanePos[0] && adjY == yohanePos[1]){
                            printf("A bat attacked Yohane! (-%.1f HP)\n", damage);
                            state->hp -= damage;
                            checkChocoRevive(state);
                            if (state->hp <= 0){
                                triggerDeath(state, "Bat (Final Dungeon)");
                            }
                        } else {
                            printf("A bat attacked Lailaps! Game over.\n");
                            triggerDeath(state, "Bat (Lailaps)");
                        }

                        bats[i].attack = 1;
                        attacked = 1;
                        grid[batX][batY] = 3;
                    }
                }

                if (attacked == 0){
                    for (j = 0; j < directionCount && moved == 0; j++){
                        int dir = rand() % directionCount;
                        int newX = batX + directions[dir][0];
                        int newY = batY + directions[dir][1];

                        if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS){
                            if (grid[newX][newY] == 0 &&
                                !(newX == yohanePos[0] && newY == yohanePos[1]) &&
                                !(newX == lailapsPos[0] && newY == lailapsPos[1])) {
                                    grid[batX][batY] = 0;
                                    grid[newX][newY] = 3;

                                    bats[i].x = newX;
                                    bats[i].y = newY;
                                    moved = 1;
                                }
                        }
                    }
                }
        }
    }
}