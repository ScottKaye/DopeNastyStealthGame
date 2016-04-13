#include <iostream>
#include <fstream>

#include "Level.h"
#include "System.h"
#include "Game.h"

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
			WallPlane p = WallPlane::WP_None;

			switch (c) {
			case '#':
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				break;
			case ']':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				break;
			case '[':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				break;
			case '{':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				break;
			case '}':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				break;
			case '>':
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				break;
			case '<':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				break;
			case '|':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				break;
			case '-':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case '_':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Top));
				break;
			case '=':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case 'l':
				Walls.push_back(new Wall({ col - 40, row, 90, 50 }, WallPlane::WP_Left));
				break;
			case 'r':
				Walls.push_back(new Wall({ col, row, 90, 50 }, WallPlane::WP_Right));
				break;
			default:
				std::cout << "Unknown character " << c << std::endl;
			}


			col += 50;
		}

		col = 0;
		row += 50;
	}

	// Test path
	std::vector<EnemyAction> tp = std::vector<EnemyAction>();

	tp.push_back(EnemyAction::EA_Forward);
	tp.push_back(EnemyAction::EA_Forward);
	tp.push_back(EnemyAction::EA_TurnRight);
	tp.push_back(EnemyAction::EA_TurnRight);
	tp.push_back(EnemyAction::EA_Forward);
	tp.push_back(EnemyAction::EA_Forward);
	tp.push_back(EnemyAction::EA_TurnRight);
	tp.push_back(EnemyAction::EA_TurnRight);

	Enemy* e = new Enemy(Vec2((50 * 2) - 25, (50 * 3) - 25), Game::EnemyTex);
	e->SetPath(tp);
	Enemies.push_back(e);
}

Level::~Level() {
	for (Wall* w : Walls) {
		delete w;
	}

	for (Enemy* e : Enemies) {
		delete e;
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

	// Enemies
	for (Enemy* e : Enemies) {
		e->Draw(renderer);
	}
}

void Level::Update(float dt, Player* player) {
	for (Enemy* e : Enemies) {
		e->Update(dt);
	}
}