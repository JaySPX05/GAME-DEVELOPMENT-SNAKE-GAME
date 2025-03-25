#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

// Game Constants
const int WIDTH = 600, HEIGHT = 600;
const int CELL_SIZE = 20;
const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

// Movement directions
enum Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    std::vector<sf::Vector2i> body;
    Direction dir;
    bool grow;

    Snake() {
        body.push_back({GRID_WIDTH / 2, GRID_HEIGHT / 2}); // Start at the center
        dir = RIGHT;
        grow = false;
    }

    void move() {
        sf::Vector2i newHead = body.front(); // Copy the head position

        // Move the head in the current direction
        switch (dir) {
            case UP:    newHead.y--; break;
            case DOWN:  newHead.y++; break;
            case LEFT:  newHead.x--; break;
            case RIGHT: newHead.x++; break;
        }

        // Collision detection
        if (isCollidingWithWall(newHead) || isCollidingWithItself(newHead)) {
            std::cout << "Game Over! You crashed. Try again!\n";
            exit(0);
        }

        // Add new head to the front
        body.insert(body.begin(), newHead);

        // If not growing, remove the tail
        if (!grow) {
            body.pop_back();
        } else {
            grow = false; // Reset growth flag
        }
    }

    void changeDirection(Direction newDir) {
        // Prevent reversing direction (e.g., UP to DOWN)
        if ((dir == UP && newDir != DOWN) || (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) || (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

private:
    bool isCollidingWithWall(const sf::Vector2i& pos) {
        return (pos.x < 0 || pos.x >= GRID_WIDTH || pos.y < 0 || pos.y >= GRID_HEIGHT);
    }

    bool isCollidingWithItself(const sf::Vector2i& pos) {
        for (const auto& segment : body) {
            if (segment == pos) return true;
        }
        return false;
    }
};

class Food {
public:
    sf::Vector2i position;

    Food() {
        respawn();
    }

    void respawn(const std::vector<sf::Vector2i>& snakeBody = {}) {
        do {
            position.x = rand() % GRID_WIDTH;
            position.y = rand() % GRID_HEIGHT;
        } while (isOnSnake(snakeBody)); // Ensure food doesn't spawn inside the snake
    }

private:
    bool isOnSnake(const std::vector<sf::Vector2i>& snakeBody) {
        for (const auto& segment : snakeBody) {
            if (segment == position) return true;
        }
        return false;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random number generator

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");
    window.setFramerateLimit(10); // Control game speed

    Snake snake;
    Food food;

    // Visual representation of the snake
    sf::RectangleShape snakeBlock(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    snakeBlock.setFillColor(sf::Color::Green);

    // Visual representation of the food
    sf::RectangleShape foodBlock(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    foodBlock.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        // Handle input events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:    snake.changeDirection(UP); break;
                    case sf::Keyboard::Down:  snake.changeDirection(DOWN); break;
                    case sf::Keyboard::Left:  snake.changeDirection(LEFT); break;
                    case sf::Keyboard::Right: snake.changeDirection(RIGHT); break;
                    default: break;
                }
            }
        }

        // Move the snake
        snake.move();

        // Check if the snake eats food
        if (snake.body.front() == food.position) {
            snake.grow = true;
            food.respawn(snake.body); // Avoid food spawning inside the snake
        }

        // Rendering
        window.clear();

        // Draw food
        foodBlock.setPosition(food.position.x * CELL_SIZE, food.position.y * CELL_SIZE);
        window.draw(foodBlock);

        // Draw snake
        for (const auto& segment : snake.body) {
            snakeBlock.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
            window.draw(snakeBlock);
        }

        window.display();
    }

    return 0;
}
