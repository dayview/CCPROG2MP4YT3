<h1> Test Plan for CCPROG2 Machine Project </h1>

**Yohane the Parhelion! The Siren in the Mirror World!**

Test Plan Design by Leon Pavino | Testing by Massi Colcol

| Function                                         | #   | Test Description                                                               | Test Input    | Expected Output | Actual Output | Pass / Failure |
| ------------------------------------------------ | --- | ------------------------------------------------------------------------------ | ------------- | --------------- | ------------- | -------------- |
| mainMenu()                                       | 1   | If _saveGame == 0_, does it display _[N]ew Game_ instead of _[C]ontinue_?      | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| mainMenu()                                       | 2   | What happens with _invalid_ inputs like _'Z'_ or _5_?                          | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| setNewGame()                                     | 3   | Are 3 unique idols _randomly selected_?                                        | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| setNewGame()                                     | 4   | Are all _state->doneDungeons[]_ & _state->inventory[]_ initialized to 0?       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 5   | Does it create _saveFile.dat_ & are the contents correctly written in binary?  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 6   | Does the _"Game Saved!"_ message show after saving?                            | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 7   | Does it load valid _save data_ correctly?                                      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 8   | Is the _state_, _rescuedIdols_, and _achievements_ restored accurately?        | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 9   | Does Yohane _move_ with _WASD_ correctly on _each map tile_?                   | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 10  | Does she take _damage correctly_ on _spike_ and _heat_ tiles?                  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 11  | Do _items locked behind idol rescue appear only when unlocked_?                | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 12  | Are _one-time only items prevented from being repurchased_?                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 13  | Are _items removed from inventory correctly_?                                  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 14  | Is "_No items available!" shown if using item at 0 qty_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| initializeFinalDungeon() & displayFinalDungeon() | 15  | Are Yohane & Lailaps _initialized in correct positions_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| initializeFinalDungeon() & displayFinalDungeon() | 16  | Is the 10x10 grid displayed with _correct symbols_ _(Y, L, S, 0, _, .)\*?      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| movement()                                       | 17  | Is _movement restricted when blocked by wall_ (\*)?                            | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| movement()                                       | 18  | Does _movement update position correctly_?                                     | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSwitchActivation()                          | 19  | Are _switches removed only when both characters are on matching pair_?         | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSwitchActivation()                          | 20  | Is message _"Switch pair X activated!"_ shown?                                 | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSirenDefeat()                               | 21  | Does it _return 1 only_ when _Yohane is adjacent to Siren_?                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| allIdolsRescued() & resetIdolSelection()         | 22  | Is _return value 1 only_ when _all rescuedIdols[] == 1_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| allIdolsRescued() & resetIdolSelection()         | 23  | Does it _reset rescuedIdols[] to 0_ if _all are rescued_?                      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| carryOverProgress()                              | 24  | Is _gold_ and _items_ correctly _printed_ and _retained_ in _new playthrough_? | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| carryOverProgress()                              | 25  | Are _excluded items_ (e.g., upgrades) _not retained_?                          | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |

<br>
<h1> Miscellaneous/Achievement Test Functions </h1>

| Function                            | #   | Test Description                                                                       | Test Input    | Expected Output | Actual Output | Pass / Failure |
| ----------------------------------- | --- | -------------------------------------------------------------------------------------- | ------------- | --------------- | ------------- | -------------- |
| checkRescueAchievement()            | 1   | Are _achievements unlocked only the first time_?                                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkRescueAchievement()            | 2   | Is _group achievement unlocked only after required idols are rescued_?                 | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkMilestoneAchievements()        | 3   | Are _dungeon count-based achievements triggered at the right time_?                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkMilestoneAchievements()        | 4   | Is _"Step! ZERO to ONE!"_ triggered if _gold == 0_?                                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkDamageLessDungeonAchievement() | 5   | Is achievement triggered only _when damage taken is exactly 0_?                        | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkShopSpendingAchievement()      | 6   | Is _cumulative spending tracked across playthroughs_ & only triggered when _>= 5000G_? | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkChocoMintSaveAchievement()     | 7   | Is achievement triggered after _saving Yohane with Choco-Mint Ice Cream_?              | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkFinalBossAchievement()         | 8   | Is achievement triggered _after 2 victories over Siren_?                               | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
