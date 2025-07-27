<h1> Test Plan for CCPROG2 Machine Project </h1>

**Yohane the Parhelion! The Siren in the Mirror World!**

Test Plan Design by Leon Pavino | Testing by Massi Colcol

| Function                                         | #   | Test Description                                                               | Test Input    | Expected Output | Actual Output | Pass / Failure |
| ------------------------------------------------ | --- | ------------------------------------------------------------------------------ | ------------- | --------------- | ------------- | -------------- |
| mainMenu()                                       | 1   | If _saveGame == 0_, does it display _[N]ew Game_ instead of _[C]ontinue_?      | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| mainMenu()                                       | 2   | What happens with _invalid_ inputs like _'Z'_ or _5_?                          | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| mainMenu()                                       | 3   | Does the menu return to prompt again after _invalid input_?                    | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| mainMenu()                                       | 4   | Is the flow consistent with the _presence_ or _absence_ of a _save file_?      | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| setNewGame()                                     | 5   | Are 3 unique idols _randomly selected_?                                        | Cell 4, Row 1 | Cell 5, Row 1   | Cell 6, Row 1 | Cell 7, Row 1  |
| setNewGame()                                     | 6   | Are all _state->doneDungeons[]_ & _state->inventory[]_ initialized to 0?       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| setNewGame()                                     | 7   | Is _state->hp_ correctly initialized to 3?                                     | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| setNewGame()                                     | 8   | Does it reset _gold_ to 0?                                                     | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 9   | Does it create _saveFile.dat_ & are the contents correctly written in binary?  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 10  | Does the _"Game Saved!"_ message show after saving?                            | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 11  | Does it handle large _inventory_ or _gold_ values correctly?                   | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| saveGameFile()                                   | 12  | What happens if the _file_ cannot be created (e.g. no permission)?             | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 13  | Does it load valid _save data_ correctly?                                      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 14  | Is the _state_, _rescuedIdols_, and _achievements_ restored accurately?        | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 15  | Does it return 0 if _file_ is not found?                                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| loadGameFile()                                   | 16  | Does it show _"Error loading..."_ if there's no save file?                     | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 17  | Does Yohane _move_ with _WASD_ correctly on _each map tile_?                   | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 18  | Does she take _damage correctly_ on _spike_ and _heat_ tiles?                  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 19  | Is _digging the wall tile 'v'_ allowed but stationary?                         | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 20  | Is gold _awarded_ when _stepping on treasure tile_?                            | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| traverseDungeon()                                | 21  | What happens if _user enters invalid key_ (e.g. 'Q' / 'Z, enter)?              | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 22  | Do _items locked behind idol rescue appear only when unlocked_?                | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 23  | Is _inventory updated only if not maxed_ (cap at 99)?                          | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 24  | Are _one-time only items prevented from being repurchased_?                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| hanamaruShop()                                   | 25  | Is _"Purchase cancelled."_ shown when pressing _'N'_?                          | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 26  | Are _items removed from inventory correctly_?                                  | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 27  | Does _HP increment correctly based on item_?                                   | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 28  | Is _HP clamped at 5 (max)_?                                                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| useItem()                                        | 29  | Is "_No items available!" shown if using item at 0 qty_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| initializeFinalDungeon() & displayFinalDungeon() | 30  | Are Yohane & Lailaps _initialized in correct positions_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| initializeFinalDungeon() & displayFinalDungeon() | 31  | Are _switches and Siren placed accurately_?                                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| initializeFinalDungeon() & displayFinalDungeon() | 32  | Is the 10x10 grid displayed with _correct symbols_ _(Y, L, S, 0, _, .)\*?      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| moveCharacter()                                  | 33  | Is _movement restricted when blocked by wall_ (\*)?                            | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| moveCharacter()                                  | 34  | Does _movement update position correctly_?                                     | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| moveCharacter()                                  | 35  | Does _character stay in place if move is invalid_?                             | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSwitchActivation()                          | 36  | Are _switches removed only when both characters are on matching pair_?         | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSwitchActivation()                          | 37  | Is message _"Switch pair X activated!"_ shown?                                 | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSwitchActivation()                          | 38  | Do _grid values update correctly_?                                             | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| checkSirenDefeat()                               | 39  | Does it _return 1 only_ when _Yohane is adjacent to Siren_?                    | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| allIdolsRescued() & resetIdolSelection()         | 40  | Is _return value 1 only_ when _all rescuedIdols[] == 1_?                       | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| allIdolsRescued() & resetIdolSelection()         | 41  | Does it _reset rescuedIdols[] to 0_ if _all are rescued_?                      | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| carryOverProgress()                              | 42  | Is _gold_ and _items_ correctly _printed_ and _retained_ in _new playthrough_? | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |
| carryOverProgress()                              | 43  | Are _excluded items_ (e.g., upgrades) _not retained_?                          | Cell 4, Row 2 | Cell 5, Row 2   | Cell 6, Row 2 | Cell 7, Row 2  |

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
