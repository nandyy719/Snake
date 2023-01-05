#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<vector>
#include<cstdlib>
#include<iostream>
#include<sstream>

const unsigned BLOCK_SIZE = 32;
const unsigned GAME_WIDTH = 30;
const unsigned GAME_HEIGHT = 20;
const unsigned WIN_WIDTH = BLOCK_SIZE * GAME_WIDTH;
const unsigned WIN_HEIGHT = BLOCK_SIZE * GAME_HEIGHT;
const unsigned SPEED = 1;
bool GAMEOVER = false;
unsigned streak = 1;


enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

unsigned genRandNum(unsigned max) {
	return rand() % max;
}
struct Position {
	unsigned x;
	unsigned y;
	Position(unsigned x = 0, unsigned y = 0) : x(x), y(y) {}

};
struct Food {
	Position p;
	Food() : p(genRandNum(GAME_WIDTH), genRandNum(GAME_HEIGHT)) {}
	void reset() {
		p.x = genRandNum(GAME_WIDTH);
		p.y = genRandNum(GAME_HEIGHT);
	}
	void draw(sf::RenderWindow& win, sf::Sprite& sp) {
		sp.setPosition(BLOCK_SIZE * p.x, BLOCK_SIZE * p.y);
		win.draw(sp);
	}
};

struct Snake {
	std::vector<Position> body;
	Direction dir;
	bool moving = false;
	Snake() : dir(Direction::RIGHT) {
		body.emplace_back(7, 10);
	}
	void tick() {
		if (moving) {
			for (size_t i = body.size() - 1; i > 0; i--) {
				body[i] = body[i - 1];
			}
			switch (dir) {
			case Direction::UP:
				body[0].y -= SPEED;
				break;
			case Direction::DOWN:
				body[0].y += SPEED;
				break;
			case Direction::LEFT:
				body[0].x -= SPEED;
				break;
			case Direction::RIGHT:
				body[0].x += SPEED;
				break;

			}
		}
	}
	void handleKeyBoardEvent(sf::Event& e) {
		switch (e.key.code) {
		case sf::Keyboard::W:
			if (dir != Direction::UP && dir != Direction::DOWN) {
				dir = Direction::UP;
			}
			break;
		case sf::Keyboard::A:
			if (dir != Direction::LEFT && dir != Direction::RIGHT) {
				dir = Direction::LEFT;
			}
			break;
		case sf::Keyboard::S:
			if (dir != Direction::UP && dir != Direction::DOWN) {
				dir = Direction::DOWN;
			}
			break;
		case sf::Keyboard::D:
			if (dir != Direction::LEFT && dir != Direction::RIGHT) {
				dir = Direction::RIGHT;
			}
			break;
		case sf::Keyboard::Up:
			if (dir != Direction::UP && dir != Direction::DOWN) {
				dir = Direction::UP;
			}
			break;
		case sf::Keyboard::Left:
			if (dir != Direction::LEFT && dir != Direction::RIGHT) {
				dir = Direction::LEFT;
			}
			break;
		case sf::Keyboard::Down:
			if (dir != Direction::UP && dir != Direction::DOWN) {
				dir = Direction::DOWN;
			}
			break;
		case sf::Keyboard::Right:
			if (dir != Direction::LEFT && dir != Direction::RIGHT) {
				dir = Direction::RIGHT;
			}
			break;
		}
		moving = true;
	}
	void draw(sf::RenderWindow& win, sf::Sprite& sp) {
		for (Position& p : body) {
			sp.setPosition(BLOCK_SIZE * p.x, BLOCK_SIZE * p.y);
			win.draw(sp);
		}
	}
	void handleCollisions(sf::RenderWindow& win, Food& food) {
		if (body[0].x < 0 || body[0].x > GAME_WIDTH || body[0].y < 0 || body[0].y > GAME_HEIGHT) {
			GAMEOVER = true;
		}
		if (body[0].x == food.p.x && body[0].y == food.p.y) {
			body.emplace_back(body.back().x, body.back().y);
			streak++;
			food.reset();
		}
		for (size_t i = 1; i < body.size(); i++) {
			switch (dir) {
			case Direction::UP:
				if (body[0].x == body[i].x && body[0].y == (body[i].y + 1)) {
					GAMEOVER = true;
				}
				break;
			case Direction::DOWN:
				if (body[0].x == body[i].x && body[0].y == (body[i].y - 1)) {
					GAMEOVER = true;
				}
				break;
			case Direction::LEFT:
				if (body[0].x == (body[i].x + 1) && body[0].y == body[i].y) {
					GAMEOVER = true;
				}
				break;
			case Direction::RIGHT:
				if (body[0].x == (body[i].x - 1) && body[0].y == body[i].y) {
					GAMEOVER = true;
				}
				break;

			}
		}
	}

};
int main() {
	srand(time(NULL));
	sf::RenderWindow win(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "NANDY SNAKE!!");
	win.setFramerateLimit(60);
	sf::Texture board_texture, snake_texture, food_texture;

	board_texture.loadFromFile("assets/images/white.png");
	snake_texture.loadFromFile("assets/images/green.png");
	food_texture.loadFromFile("assets/images/red.png");

	sf::Sprite tile_sprite(board_texture);
	sf::Sprite snake_sprite(snake_texture);
	sf::Sprite food_sprite(food_texture);

	tile_sprite.setScale(2.f, 2.f);
	snake_sprite.setScale(2.f, 2.f);
	food_sprite.setScale(2.f, 2.f);

	Snake snake;
	Food food;
	sf::Clock clock;
	float delay = 0.1;
	while (win.isOpen()) {
		if (clock.getElapsedTime().asSeconds() > delay) {
			snake.tick();
			snake.handleCollisions(win, food);
			clock.restart();
		}
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (e.type == sf::Event::KeyPressed) {
				snake.handleKeyBoardEvent(e);
			}
		}
		if (streak > 15) {
			streak = 0;
			delay /= 2;
		}
		if (!GAMEOVER) {
			win.clear();

			for (int i = 0; i < GAME_WIDTH; i++) {
				for (int j = 0; j < GAME_HEIGHT; j++) {
					tile_sprite.setPosition(BLOCK_SIZE * i, BLOCK_SIZE * j);
					win.draw(tile_sprite);
				}
			}
			snake.draw(win, snake_sprite);
			food.draw(win, food_sprite);
			win.display();
		}
		else {
			win.clear(sf::Color::White);
			sf::Text t;
			sf::Font f;
			f.loadFromFile("assets/fonts/arial.ttf");
			t.setFont(f);
			std::stringstream stream;
			stream << "GAME OVER\n\n\n\n" << "SCORE: " << snake.body.size() << '\n';
			t.setString(stream.str());
			t.setCharacterSize(40);
			t.setFillColor(sf::Color::Black);
			t.setPosition(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2 - 100);
			win.draw(t);
			win.display();


		}
	}

}


