#pragma once
#include "ScreenshotAssist.h"

#define check_avg(a, r, g, b) (a[0] == r && a[1] == g && a[2] == b)
#define valid_col(a,r2,g2,b2,t) (((a[0]-r2) * (a[0]-r2) + (a[1]-g2) * (a[1]-g2) + (a[2]-b2) * (a[2]-b2)) <= t * t)

class MinesweeperCheat {
public:
	struct cell_data {
		int x, y;
		int value;
	};

	MinesweeperCheat(int difficulty) { // 0 = easy, 1 = medium, 2 = hard
		switch (difficulty)
		{
		case 1:
			this->ms_data.x = 682;
			this->ms_data.y = 392;

			this->ms_data.x_box = 18;
			this->ms_data.y_box = 14;

			this->ms_data.box_size = 30;
			
			break;
		}
	}

	int* get_avg_rgb(HBITMAP hBitmap) {
		std::vector<BYTE> pixels = ssa->ToPixels(hBitmap);
		int rTotal = 0;
		int gTotal = 0;
		int bTotal = 0;
		int total = 0;

		for (int i = 0; i < pixels.size(); i += 4) {
			BYTE b = pixels[i];
			BYTE g = pixels[i + 1];
			BYTE r = pixels[i + 2];

			// exclude background 135, 175, 58
			if (!((r == 229 && g == 194 && b == 159) || (r == 215 && g == 184 && b == 153) || (r == 135 && g == 175 && b == 58))) {
				rTotal += r;
				gTotal += g;
				bTotal += b;
				total++;
			}
		}

		int ret[3] = {};
		if (total != 0) {
			ret[0] = rTotal / total;
			ret[1] = gTotal / total;
			ret[2] = bTotal / total;
		}
		return ret;
	}

	int determine_box_value(cell_data cell) { // could prob make this better by just taking one screenshot of whole game when needed instead of just box position but fuck u
		HBITMAP hb = ssa->Screenshot(ms_data.x + ms_data.box_size * cell.x, ms_data.y + ms_data.box_size * cell.y, ms_data.box_size, ms_data.box_size);
		std::vector<BYTE> pixels = ssa->ToPixels(hb);

		int* avg = get_avg_rgb(hb);

		//printf("[ %d  %d ] %d %d %d\n", cell.x, cell.y, avg[0], avg[1], avg[2]);

		if (check_avg(avg, 0, 0, 0))
			return -2; // empty value - no mines in range

		if (check_avg(avg, 170, 215, 81) || check_avg(avg, 162, 209, 73))
			return -1; // no known value

		if (valid_col(avg, 177, 177, 59, 50)) // flag
			return 0;

		if (valid_col(avg, 25, 118, 210, 100)) 
			return 1;

		if (valid_col(avg, 56, 142, 60, 100))
			return 2;

		if (valid_col(avg, 211, 47, 47, 100))
			return 3;

		if (valid_col(avg, 123, 31, 162, 100))
			return 4;

		if (valid_col(avg, 255, 143, 0, 100))
			return 5;

		if (valid_col(avg, 0, 151, 167, 100))
			return 6;

		if (valid_col(avg, 66, 66, 66, 100))
			return 7;

		return -3; // not sure
	}

	bool get_cell(std::vector<cell_data> cells, int x, int y, cell_data& outcell) {
		for (cell_data cell : cells)
			if (cell.x == x && cell.y == y) { outcell = cell; return true; }
		return false;
	}

	std::vector<cell_data> get_cells_in_range(std::vector<cell_data> cells, cell_data cell) {
		std::vector<cell_data> cells_in_range;

		for (cell_data c : cells) {
			int xo = c.x - cell.x;
			int yo = c.y - cell.y;

			if (xo <= 1 && xo >= -1)
				if (yo <= 1 && yo >= -1)
					if (!(xo == 0 && yo == 0))
						cells_in_range.push_back(c);
		}

		return cells_in_range;
	} // lord forgive me for this function

	std::vector<cell_data> get_value_in_range(std::vector<cell_data> cells, cell_data cell, int value) {
		std::vector<cell_data> grass_in_range; // formerly known as get_grass_in_range

		for (cell_data c : get_cells_in_range(cells, cell)) {
			if (c.value == value)
				grass_in_range.push_back(c);
		}

		return grass_in_range;
	}

	std::vector<cell_data> get_values_in_range(std::vector<cell_data> cells, cell_data cell, std::vector<int> values) {
		std::vector<cell_data> grass_in_range; // formerly known as get_grass_in_range

		for (cell_data c : get_cells_in_range(cells, cell)) {
			for (int value : values) {
				if (c.value == value)
					grass_in_range.push_back(c);
			}
		}

		return grass_in_range;
	}

	std::vector<cell_data> get_mineland_in_range(std::vector<cell_data> cells, cell_data cell) {
		return get_values_in_range(cells, cell, { -1, 0 });
	}

	std::vector<cell_data> get_cell_data() {
		std::vector<cell_data> data;
		for (int y = 0; y < this->ms_data.y_box; y++) {
			for (int x = 0; x < this->ms_data.x_box; x++) {
				cell_data cell;
				cell.x = x; cell.y = y;
				
				cell.value = determine_box_value(cell);

				data.push_back(cell);
			}
		}
		return data;
	}

	void FlagCell(cell_data& cell) {
		cell.value = 0;
		int x = ms_data.x + cell.x * ms_data.box_size;
		int y = ms_data.y + cell.y * ms_data.box_size;

		POINT p;
		GetCursorPos(&p);

		SetCursorPos(x + ms_data.box_size / 2, y + ms_data.box_size / 2);

		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		Sleep(1);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

		SetCursorPos(p.x, p.y);
	}

	void ClearCell(cell_data& cell) {
		if (cell.value == 0) printf("nooo\n");
		int x = ms_data.x + cell.x * ms_data.box_size;
		int y = ms_data.y + cell.y * ms_data.box_size;

		POINT p;
		GetCursorPos(&p);

		SetCursorPos(x + ms_data.box_size / 2, y + ms_data.box_size / 2);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(1);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		SetCursorPos(p.x, p.y);
	}
private:
	ScreenshotAssist* ssa = new ScreenshotAssist();

	struct {
		int x, y;
		int x_box, y_box;
		int box_size; // assuming all boxes have equal length sides
	}ms_data;
};