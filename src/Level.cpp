#include <iostream>
#include <fstream>
#include <regex>

#include "Level.h"
#include "System.h"
#include "Game.h"

void Level::LoadWalls(const std::string& filename) {
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

	int tileWidth = 50;
	int tileHeight = 50;

	int row = 0, col = 0;
	for (std::string line : lines) {
		for (char c : line) {
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
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case '<':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case '|':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				break;
			case '-':
				Walls.push_back(new Wall({ col, row - 10, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case '_':
				Walls.push_back(new Wall({ col, row + 50, 50, 10 }, WallPlane::WP_Top));
				break;
			case '=':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				break;
			case 'l':
				Walls.push_back(new Wall({ col - 40, row, 90, 50 }, WallPlane::WP_Left));
				break;
			case 'r':
				Walls.push_back(new Wall({ col, row, 90, 50 }, WallPlane::WP_Right));
				break;
			case 'P':
				Game::MainPlayer->Center = Vec2(
					col + Game::MainPlayer->Width() / 2,
					row + Game::MainPlayer->Height() / 2
				);
				break;
			}

			col += 50;
		}

		col = 0;
		row += 50;
	}

	// TODO don't hardcode these numbers
	Walls.push_back(new Wall({ 0, 0, 50 * 16, 5 }, WallPlane::WP_Bottom));
	Walls.push_back(new Wall({ 0, 50 * 12 - 5, 50 * 16, 5 }, WallPlane::WP_Top));
	Walls.push_back(new Wall({ 0, 0, 5, 50 * 12 }, WallPlane::WP_Right));
	Walls.push_back(new Wall({ 50 * 16 - 5, 0, 5, 50 * 12 }, WallPlane::WP_Left));
}

void Level::LoadPaths(const std::string& filename) {
	std::string fPaths = filename + "-paths.txt";
	std::fstream f(fPaths);

	if (!f.good()) {
		std::cerr << "*** Error: Failed to open " << fPaths << std::endl;
		return;
	}

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(f, line)) {
		lines.push_back(line);
	}

	std::regex rgx("(\\d+),(\\d+):([A-Z]):([A-Z]+)");

	for (std::string line : lines) {
		std::smatch matches;

		if (std::regex_search(line, matches, rgx)) {
			float x = atof(matches[0].str().c_str()) * 50 + Game::EnemyTex->GetWidth() / 2;
			float y = atof(matches[2].str().c_str()) * 50 + Game::EnemyTex->GetWidth() / 2;

			Enemy * e = new Enemy(Vec2(x, y), Game::EnemyTex);

			EnemyDirection initialDir;

			switch (matches[3].str()[0]) {
			default:
			case 'U':
				initialDir = EnemyDirection::ED_Up;
				break;
			case 'R':
				initialDir = EnemyDirection::ED_Right;
				break;
			case 'D':
				initialDir = EnemyDirection::ED_Down;
				break;
			case 'L':
				initialDir = EnemyDirection::ED_Left;
				break;
			}

			e->SetDirection(initialDir);

			auto actions = std::vector<EnemyAction>();
			for (char a : matches[4].str()) {
				switch (a) {
				case 'F':
					actions.push_back(EnemyAction::EA_Forward);
					break;
				case 'R':
					actions.push_back(EnemyAction::EA_TurnRight);
					break;
				case 'L':
					actions.push_back(EnemyAction::EA_TurnLeft);
					break;
				}
			}

			e->SetPath(actions);
			Enemies.push_back(e);
		}
		else {
			std::cout << "Invalid path specified." << std::endl;
		}
	}
}

Level::Level(const std::string& filename) {
	LoadWalls(filename);
	LoadPaths(filename);
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