<h1> Test Plan for CCPROG2 Machine Project </h1>

**Yohane the Parhelion! The Siren in the Mirror World!**

Test Plan Design by Leon Pavino | Testing by Massi Colcol


| Function | # | Test Description | Test Input | Expected Output | Actual Output | Pass / Failure |
| --------- | -------- | -------- | -------- | -------- | -------- | -------- |
| mainMenu() | 1 | If *saveGame == 0*, does it display *[N]ew Game* instead of *[C]ontinue*? | Cell 4, Row 1 | Cell 5, Row 1 | Cell 6, Row 1 | Cell 7, Row 1 |
| mainMenu() | 2 | What happens with *invalid* inputs like *'Z'* or *5*? | Cell 4, Row 1 | Cell 5, Row 1 | Cell 6, Row 1 | Cell 7, Row 1 |
| mainMenu() | 3 | Does the menu return to prompt again after *invalid input*? | Cell 4, Row 1 | Cell 5, Row 1 | Cell 6, Row 1 | Cell 7, Row 1 |
| mainMenu() | 4 | Is the flow consistent with the *presence* or *absence* of a *save file*? | Cell 4, Row 1 | Cell 5, Row 1 | Cell 6, Row 1 | Cell 7, Row 1 |
| setNewGame() | 5 | Are 3 unique idols *randomly selected*? | Cell 4, Row 1 | Cell 5, Row 1 | Cell 6, Row 1 | Cell 7, Row 1 |
| setNewGame() | 6 | Are all *state->doneDungeons[]* & *state->inventory[]* initialized to 0? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| setNewGame() | 7 | Is *state->hp* correctly initialized to 3? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| setNewGame() | 8 | Does it reset *gold* to 0? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| saveGameFile() | 9 | Does it create *saveFile.dat* & are the contents correctly written in binary? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| saveGameFile() | 10 | Does the *"Game Saved!"* message show after saving? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| saveGameFile() | 11 | Does it handle large *inventory* or *gold* values correctly? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| saveGameFile() | 12 | What happens if the *file* cannot be created (e.g. no permission)? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| loadGameFile() | 13 | Does it load valid *save data* correctly? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| loadGameFile() | 14 | Is the *state*, *rescuedIdols*, and *achievements* restored accurately? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| loadGameFile() | 15 | Does it return 0 if *file* is not found? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| loadGameFile() | 16 | Does it show *"Error loading..."* if there's no save file? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| traverseDungeon() | 17 | Does Yohane *move* with *WASD* correctly on *each map tile*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| traverseDungeon() | 18 | Does she take *damage correctly* on *spike* and *heat* tiles? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| traverseDungeon() | 19 | Is *digging the wall tile 'v'* allowed but stationary? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| traverseDungeon() | 20 | Is gold *awarded* when *stepping on treasure tile*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| traverseDungeon() | 21 | What happens if *user enters invalid key* (e.g. 'Q' / 'Z, enter)? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| hanamaruShop() | 22 | Do *items locked behind idol rescue appear only when unlocked*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| hanamaruShop() | 23 | Is *inventory updated only if not maxed* (cap at 99)? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| hanamaruShop() | 24 | Are *one-time only items prevented from being repurchased*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| hanamaruShop() | 25 | Is *"Purchase cancelled."* shown when pressing *'N'*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| useItem() | 26 | Are *items removed from inventory correctly*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| useItem() | 27 | Does *HP increment correctly based on item*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| useItem() | 28 | Is *HP clamped at 5 (max)*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| useItem() | 29 | Is "*No items available!" shown if using item at 0 qty*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| initializeFinalDungeon() & displayFinalDungeon() | 30 | Are Yohane & Lailaps *initialized in correct positions*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| initializeFinalDungeon() & displayFinalDungeon() | 31 | Are *switches and Siren placed accurately*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| initializeFinalDungeon() & displayFinalDungeon() | 32 | Is the 10x10 grid displayed with *correct symbols* *(Y, L, S, 0, *, .)*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| moveCharacter() | 33 | Is *movement restricted when blocked by wall* (*)? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| moveCharacter() | 34 | Does *movement update position correctly*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| moveCharacter() | 35 | Does *character stay in place if move is invalid*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkSwitchActivation() | 36 | Are *switches removed only when both characters are on matching pair*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkSwitchActivation() | 37 | Is message *"Switch pair X activated!"* shown? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkSwitchActivation() | 38 | Do *grid values update correctly*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkSirenDefeat() | 39 | Does it *return 1 only* when *Yohane is adjacent to Siren*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| allIdolsRescued() & resetIdolSelection() | 40 | Is *return value 1 only* when *all rescuedIdols[] == 1*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| allIdolsRescued() & resetIdolSelection() | 41 | Does it *reset rescuedIdols[] to 0* if *all are rescued*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| carryOverProgress() | 42 | Is *gold* and *items* correctly *printed* and *retained* in *new playthrough*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| carryOverProgress() | 43 | Are *excluded items* (e.g., upgrades) *not retained*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |

<br>
<h1> Miscellaneous/Achievement Test Functions </h1>

| Function | # | Test Description | Test Input | Expected Output | Actual Output | Pass / Failure |
| --------- | -------- | -------- | -------- | -------- | -------- | -------- |
| checkRescueAchievement() | 1 | Are *achievements unlocked only the first time*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkRescueAchievement() | 2 | Is *group achievement unlocked only after required idols are rescued*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkMilestoneAchievements() | 3 | Are *dungeon count-based achievements triggered at the right time*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkMilestoneAchievements() | 4 | Is *"Step! ZERO to ONE!"* triggered if *gold == 0*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkDamageLessDungeonAchievement() | 5 | Is achievement triggered only *when damage taken is exactly 0*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkShopSpendingAchievement() | 6 | Is *cumulative spending tracked across playthroughs* & only triggered when *>= 5000G*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkChocoMintSaveAchievement() | 7 | Is achievement triggered after *saving Yohane with Choco-Mint Ice Cream*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |
| checkFinalBossAchievement() | 8 | Is achievement triggered *after 2 victories over Siren*? | Cell 4, Row 2 | Cell 5, Row 2 | Cell 6, Row 2 | Cell 7, Row 2 |