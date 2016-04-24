#include <iostream>
#include <fstream>
#include <regex>

#include "Level.h"
#include "System.h"
#include "Gameplay.h"

Level::Level(const std::string& filename)
	: mComplete(false)
{
	Entities.clear();
	Walls.clear();

	LoadWalls(filename);
	LoadPaths(filename);

	if (MainPlayer == NULL) {
		throw "## ERR: No player cell for level " + filename;
	}

	if (EndPortal == NULL) {
		throw "## ERR: No end portal cell for level " + filename;
	}
}

Level::~Level() {
	for (Wall* w : Walls) {
		delete w;
	}

	for (auto e : Entities) {
		delete e;
	}

	delete MainPlayer;
	delete EndPortal;
}

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
	int x = 0, y = 0;
	for (std::string line : lines) {
		for (char c : line) {
			switch (c) {
			case '#':
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Map[x][y] = true;
				break;
			case ']':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				Map[x][y] = true;
				break;
			case '[':
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Map[x][y] = true;
				break;
			case '{':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Map[x][y] = true;
				break;
			case '}':
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				Map[x][y] = true;
				break;
			case '>':
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Map[x][y] = true;
				break;
			case '<':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Walls.push_back(new Wall({ col, row, 50, 10 }, WallPlane::WP_Top));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col, row + 40, 50, 10 }, WallPlane::WP_Bottom));
				Map[x][y] = true;
				break;
			case '|':
				Walls.push_back(new Wall({ col, row, 10, 50 }, WallPlane::WP_Left));
				Walls.push_back(new Wall({ col, row, 50, 50 }, WallPlane::WP_None));
				Walls.push_back(new Wall({ col + 40, row, 10, 50 }, WallPlane::WP_Right));
				Map[x][y] = true;
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
				Map[x][y] = true;
				break;
			case 'l':
				Walls.push_back(new Wall({ col - 40, row, 90, 50 }, WallPlane::WP_Left));
				Map[x][y] = true;
				break;
			case 'r':
				Walls.push_back(new Wall({ col, row, 90, 50 }, WallPlane::WP_Right));
				Map[x][y] = true;
				break;
			case 'P':
			{
				Vec2 location = Vec2(
					(float)(col + Gameplay::PlayerTex->GetWidth() / 2),
					(float)(row + Gameplay::PlayerTex->GetHeight() / 2)
					);

				if (MainPlayer != NULL) {
					delete MainPlayer;
					MainPlayer = NULL;
				}

				MainPlayer = new Player(location, Gameplay::PlayerTex);
			}
			break;
			case 'X':
			{
				if (EndPortal != NULL) {
					delete EndPortal;
					EndPortal = NULL;
				}

				EndPortal = new Portal(Vec2(
					(float)(col + Gameplay::PortalTex->GetWidth() / 2),
					(float)(row + Gameplay::PortalTex->GetHeight() / 2)
					), Gameplay::PortalTex);
			}
			break;
			}

			++x;
			col += 50;

			// Don't load OOB
			if (x > 16) break;
		}

		x = 0;
		++y;
		col = 0;
		row += 50;

		// Don't load OOB
		if (y > 12) break;
	}

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
			float x = (float)(atof(matches[0].str().c_str()) * 50 + Gameplay::EnemyTex->GetWidth() / 2);
			float y = (float)(atof(matches[2].str().c_str()) * 50 + Gameplay::EnemyTex->GetWidth() / 2);

			Enemy* e = new Enemy(Vec2(x, y), Gameplay::EnemyTex);

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
			Entities.push_back(e);
		}
		else {
			std::cout << "Invalid path specified." << std::endl;
		}
	}
}

void Light(SDL_Renderer* renderer, Entity* ent, int r, int g, int b, int a) {
	int size = 50;
	int row = (int)std::floor(ent->Center.y / size);
	int col = (int)std::floor(ent->Center.x / size);

	// Light up surrounding squares a little bit as well
	for (int y = row - 1; y <= row + 1; ++y) {
		for (int x = col - 1; x <= col + 1; ++x) {
			SDL_Rect rect = { x * size, y * size, size, size };
			SDL_SetRenderDrawColor(renderer, r, g, b,
				x == col && y == row
				? a : (int)(a / 2));
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Hitbox(SDL_Renderer* renderer, Entity* ent) {
	// Raw box
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect = { (int)ent->Left(), (int)ent->Top(), (int)ent->Height(), (int)ent->Width() };
	SDL_RenderDrawRect(renderer, &rect);

	// Hit radius
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	int density = 25;
	for (int p = 0; p < density; p++) {
		float x = ent->Center.x + ent->HitRadius() * std::cos(2 * (float)M_PI * p / density);
		float y = ent->Center.y + ent->HitRadius() * std::sin(2 * (float)M_PI * p / density);

		SDL_RenderDrawPoint(renderer, (int)x, (int)y);
	}
}

void Level::Draw(SDL_Renderer* renderer) {
	if (mComplete) return;

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

	// Grid
	int size = 50;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 25);
	for (int col = 0; col < System::GetWindowWidth() / size; ++col) {
		for (int row = 0; row < System::GetWindowHeight() / size; ++row) {
			SDL_Rect rect = { col * size, row * size, size, size };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	// Light up squares with entities
	// Squares with more entities will be brighter
	// Also draw hitboxes
	for (auto e : Entities) {
		Light(renderer, e, 255, 255, 255, 25);

		if (Gameplay::DrawHitboxes) Hitbox(renderer, e);
	}

	Light(renderer, MainPlayer, 0, 246, 255, 35);
	if (Gameplay::DrawHitboxes) Hitbox(renderer, MainPlayer);

	// Portals
	EndPortal->Draw(renderer);

	// Entities
	for (auto e : Entities) {
		e->Draw(renderer);
	}

	// Player
	MainPlayer->Draw(renderer);

	// Level walls
	for (Wall* w : Walls) {
		w->Draw(renderer);
	}
}

bool Level::Update(float dt) {
	if (mComplete) return true;

	for (auto e : Entities) {
		bool safe = e->Update(dt);
		if (!safe) return false;

		if (Enemy* en = dynamic_cast<Enemy*>(e)) {
			bool canSeePlayer = Raycast(MainPlayer, e);
			if (!canSeePlayer) {
				en->UnSee();
				continue;
			}

			float dist = Vec2::Distance(MainPlayer->Center, e->Center);

			// Is the entity out of range?
			if (dist > en->VisionRange()) {
				en->UnSee();
				continue;
			}

			en->See();
		}
	}

	MainPlayer->Update(dt);

	if (MainPlayer->CollidesWith(EndPortal)) {
		mComplete = true;
	}

	return true;
}

bool Level::Raycast(const Entity* from, const Entity* to) const {
	// Don't check self-raycasts
	if (from == to) return false;

	float steps = 25;
	bool seen = true;

	Vec2 iterPoint = from->Center;
	float x = (to->Center.x - from->Center.x) / steps;
	float y = (to->Center.y - from->Center.y) / steps;

	for (int i = 0; i < steps; ++i) {
		iterPoint.x += x;
		iterPoint.y += y;

		int cellX = (int)std::floor(iterPoint.x / 50);
		int cellY = (int)std::floor(iterPoint.y / 50);

		if (Map[cellX][cellY]) {
			seen = false;
			break;
		}
	}

	return seen;
}