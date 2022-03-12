#include <Windows.h>
#include <iostream>
#include <vector>

#include "ScreenshotAssist.h"
#include "MinesweeperCheat.h"

int main() {
	SetConsoleTitle("lol");

	// 682, 392 is starting position for minesweeper.
    // each box is 30x30. 18x14 boxes (medium)

    // how to optimise : 
    // * cache cells that arent gonna change (e.g. flags, number cells) and just input the cache data into get_cell_data, so it can then skip evaluating that cell. will be faster as game proceeds
    // * calculate cells in sections: e,g 3x3 - then calculate average from there. should only really be used in areas which are just grass 
    // * rewrite program and allow cells to be referenced so real value can be changed of cell for example when flagged, no need to determine box value
    // * take ONE screenshot of whole game, then work out cells from pixels from there (only reason i havent done this is because its complicated and needs its own dedicated sesh, once i get everything else working I will 8) )
    // * if one sweep has ONLY flagged cells ( not cleared any ) no need to get all cell data again, just re-execute sweep

    // detect when flag count = 0, then proceed to click every empty box (and pray)

    MinesweeperCheat* mscheat = new MinesweeperCheat(1);

    while (true) {
        system("pause");
        std::vector<MinesweeperCheat::cell_data> data = mscheat->get_cell_data();

        for (MinesweeperCheat::cell_data cell : data) {
            if (cell.value > 0) {
                std::vector<MinesweeperCheat::cell_data> mineland = mscheat->get_mineland_in_range(data, cell);
                if (mineland.size() == cell.value) {
                    for (MinesweeperCheat::cell_data cell : mineland) {
                        if (mscheat->determine_box_value(cell) == -1) {
                            printf("we got a bomb guys [ %d  %d ]\n", cell.x, cell.y);
                            mscheat->FlagCell(cell);
                            Sleep(50);
                        }
                    }
                }
            }
        }

        for (MinesweeperCheat::cell_data cell : data) {
            if (cell.value > 0) {
                std::vector<MinesweeperCheat::cell_data> mines = mscheat->get_value_in_range(data, cell, 0);
                if (mines.size() == cell.value) {
                    for (MinesweeperCheat::cell_data cc : mscheat->get_value_in_range(data, cell, -1)) {
                        int val = mscheat->determine_box_value(cc);
                        if (val != 0)
                            mscheat->ClearCell(cc);
                    }
                }
            }
        }

        // now all i need to do is add multi-cell flag resolver
    }

	std::cin.get();
	return 1;
}