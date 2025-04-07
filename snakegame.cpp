#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Screen and grid settings
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;

// Enumeration for snake direction
enum Direction { UP, DOWN, LEFT, RIGHT };

// Utility function: Create an SDL_Rect of fixed grid size at (x, y)
SDL_Rect createRect(int x, int y) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = GRID_SIZE;
    rect.h = GRID_SIZE;
    return rect;
}

// Check if two rectangles collide (simple equality of grid positions)
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return a.x == b.x && a.y == b.y;
}

// Generate food at a random location not on the snake
SDL_Rect generateFood(const std::vector<SDL_Rect>& snake) {
    SDL_Rect food;
    bool onSnake;
    do {
        onSnake = false;
        food.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
        food.y = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
        // Check if food position overlaps any snake segment
        for (const auto& segment : snake) {
            if (checkCollision(food, segment)) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
    food.w = GRID_SIZE;
    food.h = GRID_SIZE;
    return food;
}

int SDL_main(int argc, char* argv[]) {
    // Seed random generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " 
                  << SDL_GetError() << "\n";
        return 1;
    }

    // Create the game window
    SDL_Window* window = SDL_CreateWindow("Snake Game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " 
                  << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Create the renderer for drawing
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " 
                  << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set up the snake starting position 
    std::vector<SDL_Rect> snake = { createRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) };
    // Generate the first food location
    SDL_Rect food = generateFood(snake);
    // Initial direction is RIGHT
    Direction dir = RIGHT;

    bool quit = false;
    SDL_Event e;
    int delay = 100;          // current delay between moves in milliseconds
    const int initialDelay = 100; // starting delay

    // Main game loop
    while (!quit) {
        // Process events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                // Change snake direction 
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    if (dir != DOWN)  dir = UP; break;
                    case SDLK_DOWN:  if (dir != UP)    dir = DOWN; break;
                    case SDLK_LEFT:  if (dir != RIGHT) dir = LEFT; break;
                    case SDLK_RIGHT: if (dir != LEFT)  dir = RIGHT; break;
                }
            }
        }

        // Calculate new head position based on current direction
        SDL_Rect newHead = snake.front();
        switch (dir) {
            case UP:    newHead.y -= GRID_SIZE; break;
            case DOWN:  newHead.y += GRID_SIZE; break;
            case LEFT:  newHead.x -= GRID_SIZE; break;
            case RIGHT: newHead.x += GRID_SIZE; break;
        }

        // Check collision with the walls
        if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH ||
            newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
            quit = true;
            break;
        }

        // Check collision with itself
        for (const auto& segment : snake) {
            if (checkCollision(newHead, segment)) {
                quit = true;
                break;
            }
        }
        if (quit) break;

        // Insert new head at the front of the snake
        snake.insert(snake.begin(), newHead);


        if (checkCollision(newHead, food)) {
            food = generateFood(snake);
        } else {
            snake.pop_back();
        }

        // Draw background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw food 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &food);

        // Draw snake 
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& segment : snake) {
            SDL_RenderFillRect(renderer, &segment);
        }

        SDL_RenderPresent(renderer);

        // --- Delay Calculation ---
        int computedDelay = initialDelay - static_cast<int>(snake.size() / 5) * 10;
        delay = std::max(50, computedDelay);
        SDL_Delay(delay);
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
