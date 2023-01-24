#include <SFML/Graphics.hpp>

#include <vector>

#include <random>

#include <ctime>

#include <thread>

using namespace sf;
using namespace std;

constexpr int WEIGHT = 1280;
constexpr int HEIGHT = 736;

constexpr int Size = 40;

namespace random {
	mt19937 mt{random_device{}()};

	Vector2f randomPosition() {
		uniform_int_distribution rand_x{ 2, 38 };
		uniform_int_distribution rand_y{ 2, 21 };
		return Vector2f{ float(32 * rand_x(mt)), float(32 * rand_y(mt)) };
	}
}

class Map {
private:

	const String map[Size]{
	"#######################",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#                     #",
	"#######################",
	};

	RectangleShape rectangle;

public:

	Map() = default;

	void draw(RenderTarget& window) {
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 23; j++) {
				if (map[i][j] == '#') {
					rectangle.setFillColor(Color::Black);
				}
				if (map[i][j] == ' ') {
					rectangle.setFillColor(Color::Yellow);
				}
				rectangle.setSize(Vector2f{ 32, 32 });
				rectangle.setPosition(32 * i, 32 * j);
				window.draw(rectangle);
			}
		}
	}
};

class Game: public Drawable {
public:
	
	Game() {
		snake.resize(1);

		snake[0].position = Vector2f(320, 320);

		snake[0].part.setPosition(snake[0].position);
		snake[0].part.setFillColor(Color::Green);
		snake[0].part.setOutlineThickness(3);
		snake[0].part.setOutlineColor(Color::Black);
		snake[0].part.setSize(Vector2f{29, 29});

		direction = Move::Right;

		apple.setPosition(random::randomPosition());
		apple.setFillColor(Color::Red);
		apple.setOutlineThickness(3);
		apple.setOutlineColor(Color::Black);
		apple.setSize(Vector2f{ 29, 29 });

		font.loadFromFile("Shriftes/arial.ttf");

		text.setFont(font);
		text.setString("Score: 0");
		text.setCharacterSize(24);
		text.setFillColor(Color::White);
		text.setPosition(1140, 705);

		Score = 0;
	}

	void draw(RenderTarget& target, RenderStates states) const{
		target.draw(apple);
		target.draw(text);
		for (int i = 0; i < snake.size(); i++) {
			target.draw(snake[i].part);
		}
	}

	void addPart() {
		Part temp;
		temp.position = Vector2f(snake[snake.size() - 1].position.x - 32, snake[snake.size() - 1].position.y);
		temp.part.setPosition(temp.position);
		temp.part.setFillColor(Color::Green);
		temp.part.setOutlineThickness(3);
		temp.part.setOutlineColor(Color::Black);
		temp.part.setSize(Vector2f{ 29, 29 });
		snake.push_back(temp);

	}

	void createnewApple() {
		apple.setPosition(random::randomPosition());
	}

	void updateScore() {
		++Score;
		text.setString("Score: " + to_string(Score));
	}

	bool isGameOver() {
		if (snake[0].position.x > 1247 || snake[0].position.x < 32 || snake[0].position.y > 700 || snake[0].position.y < 32) {
			return true;
		}

		if (snake.size() > 3) {
			bool end{ false };
			for (int i = 3; i < snake.size(); i++) {
				if (snake[0].position == snake[i].position) {
					end = true;
					break;
				}
			}
			if (end) {
				return true;
			}
		}

		return false;
	}

	void GameOver(Font& font, Text& text, RenderWindow& window) {
		font.loadFromFile("Shriftes/arial.ttf");
		text.setFont(font);
		text.setString("Game over");
		text.setCharacterSize(24);
		text.setFillColor(Color::Black);
		text.setPosition(250, 250);
		window.draw(text);

	}

	void update(RenderWindow& window) {
		
		static Font font;
		static Text text;

		if (snake[0].position == apple.getPosition()) {
			addPart();
			createnewApple();
			updateScore();
		}

		if (isGameOver()) {
			//GameOver(font, text, window);
			return;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			direction = Move::Up;
		}

		if (Keyboard::isKeyPressed(Keyboard::S)) {
			direction = Move::Down;
		}

		if (Keyboard::isKeyPressed(Keyboard::D)) {
			direction = Move::Right;
		}

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			direction = Move::Left;
		}

		Vector2f position = snake[0].position;

		switch (direction)
		{
		case Move::Up:
			snake[0].position.y -= 32;
			break;

		case Move::Down:
			snake[0].position.y += 32;
			break;

		case Move::Right:
			snake[0].position.x += 32;
			break;

		case Move::Left:
			snake[0].position.x -= 32;
			break;
		}
		
		for (int i = snake.size() - 1; i > 0; i--) {
			position = snake[i - 1].position;
			snake[i].position = Vector2f(position);
		}

		for (int i = 0; i < snake.size(); i++) {
			snake[i].part.setPosition(snake[i].position);
		}

		this_thread::sleep_for((0.1s));
	}

private:

	enum class Move {
		Left,
		Right,
		Down,
		Up,
	};

	struct Part {
		RectangleShape part;
		Vector2f position;
	};

	vector<Part> snake;
	Move direction;
	RectangleShape apple;
	Font font;
	Text text;
	int Score;
};

void reDrawFrame(RenderWindow& window, Map& map, Game& snake) {
	window.clear(Color::White);
	map.draw(window);
	window.draw(snake);
	window.display();
}

void pollEvent(RenderWindow& window) {

	Event event;

	while (window.pollEvent(event)) {

		if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
	}
}

int main() {
	RenderWindow window(VideoMode(WEIGHT, HEIGHT), "Snake");

	Map map;
	Game snake_game;
	snake_game.addPart();

	while (window.isOpen()) {

		pollEvent(window);
		snake_game.update(window);
		reDrawFrame(window, map, snake_game);
	}

	return 0;
}