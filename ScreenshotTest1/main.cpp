#include <Windows.h>
#include <iostream>
#include <vector>

#include "ScreenshotAssist.h"
#include "MinesweeperCheat.h"

int main() {
	SetConsoleTitle("lol");

	// 682, 392 is starting position for minesweeper.
    // each box is 30x30. 18x14 boxes (medium)

    system("pause");

    MinesweeperCheat* mscheat = new MinesweeperCheat(1);

    std::vector<MinesweeperCheat::cell_data> data = mscheat->get_cell_data();

    MinesweeperCheat::cell_data cell;
    if (!mscheat->get_cell(data, 1, 2, cell)) printf("failed getting cell lol");

    std::vector<MinesweeperCheat::cell_data> whatdoesthisfuncdoagain = mscheat->get_mineland_in_range(data, cell);

    for (MinesweeperCheat::cell_data cell : whatdoesthisfuncdoagain) {
        printf("[ %d  %d ] - %d\n", cell.x, cell.y, cell.value);
    }

    /*for (MinesweeperCheat::cell_data cell : data) {
        const char* val = "";

        switch (cell.value) {
        case -2: case -1: break; // ground
        case 0: val = "Flag"; break;
        case 1: val = "One"; break;
        case 2: val = "Two"; break;
        case 3: val = "Three"; break;
        case 4: val = "Four"; break;
        default: printf("bro wtf = %d [ %d  %d ]\n", cell.value, cell.y, cell.x); break;
        }

        if (val != "")
            printf("[ %d  %d ] = %s\n", cell.y, cell.x, val);
    }*/

	std::cin.get();
	return 1;
}