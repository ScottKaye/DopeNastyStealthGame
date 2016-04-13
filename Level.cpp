#include <iostream>
#include <fstream>

#include "Level.h"
#include "System.h"

Level::Level(const std::string& filename) {
	std::string fWalls = filename + "-walls.txt";

	std::fstream f(fWalls);

	if (!f.good()) {
		std::cerr << "*** Error: Failed to open " << fWalls << std::endl;
		return;
	}

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(f, line)) {
		lines.push_back(line);
	}

	int numRows = lines.size();
	int numCols = lines[0].length();

	// reverse the rows so that bottom is at row 0
	int tileWidth = 50;
	int tileHeight = 50;

	int row = 0, col = 0;
	for (std::string line : lines) {
		for (char c : line) {
			WallPlane p = WallPlane::Air;

			switch (c) {
			case '#':
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::Air));
				break;
			case ']':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::Top));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::Right));
				break;
			case '[':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::Top));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::Left));
				break;
			case '{':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::Bottom));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::Left));
				break;
			case '}':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::Bottom));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::Right));
				break;
			case '>':
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::Right));
				break;
			case '<':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::Left));
				break;
			case '|':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::Left));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::Right));
				break;
			case '-':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::Bottom));
				break;
			case '_':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::Top));
				break;
			case '=':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::Top));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::Bottom));
				break;
			default:
				std::cout << "Unknown character " << c << std::endl;
			}


			col += 50;
		}

		col = 0;
		row += 50;
	}
}

Level::~Level() {
	for (Wall* w : Walls) {
		delete w;
	}
}

void Level::Draw(SDL_Renderer* renderer) {
	// Level boundaries
	int w = System::GetWindowWidth();
	int h = System::GetWindowHeight();

	int bSize = 5;

	SDL_Rect bTop = { 0, 0, w, bSize };
	SDL_Rect bRight = { w - bSize, 0, bSize, h };
	SDL_Rect bBottom = { 0, h - bSize, w, bSize };
	SDL_Rect bLeft = { 0, 0, bSize, h };

	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderFillRect(renderer, &bTop);
	SDL_RenderFillRect(renderer, &bRight);
	SDL_RenderFillRect(renderer, &bBottom);
	SDL_RenderFillRect(renderer, &bLeft);

	// Level walls
	for (Wall* w : Walls) {
		w->Draw(renderer);
	}
}

void Level::Update(float dt, Player* player) {

}