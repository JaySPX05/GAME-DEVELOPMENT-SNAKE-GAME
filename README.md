# GAME-DEVELOPMENT-SNAKE-GAME

*COMPANY*: CODTECH IT SOLUTIONS

*NAME*: JAY PANDIAN

*INTERN ID*: CT04WX43

*DOMAIN*: C++ PROGRAMMING

*DURATION*: 4 WEEKS

*MENTOR*: NEELA SANTOSH


This code is a simple Snake game built using SDL2. It starts by setting some basic constants for the screen size and grid cell size, which define how the game’s play area is laid out. The snake moves on a grid where each cell has a fixed size, and its position is tracked using SDL_Rect structures.

The program begins by initializing the random number generator and then initializing the SDL video subsystem. If SDL can’t initialize, it prints an error and quits. It creates an SDL window with a defined width and height and a renderer that handles all the drawing on the window.

The game starts with the snake in the center of the screen. A helper function, createRect(), is used to generate an SDL_Rect for a given x and y coordinate. Another function, generateFood(), randomly picks a grid location for the food, making sure it doesn’t appear on the snake by checking for collisions with each segment of the snake using the checkCollision() function.

Inside the main game loop, the program first processes all incoming SDL events. It listens for the quit event (like clicking the close window button) and keyboard events. When an arrow key is pressed, it updates the snake’s direction while ensuring the snake cannot immediately reverse direction.

For each frame, the code calculates a new head position for the snake based on its current direction. It checks if the new head collides with the wall or with the snake itself; if so, the game loop is terminated. If the new head is valid, it is added to the front of the snake’s vector. If the new head’s position matches the food’s position, the snake grows (by not removing the tail) and new food is generated; otherwise, the snake’s tail is removed, which simulates movement.

After updating the snake’s position, the program clears the renderer, sets the background color to black, and then draws the food in red and the snake in green. Finally, the rendered frame is presented on the screen.

The game’s speed increases as the snake grows. After drawing, the code computes a new delay by subtracting a value based on the snake’s length from an initial delay, ensuring that it never goes below 50 milliseconds using std::max. This dynamic delay makes the game progressively faster and more challenging as the snake length increases.

Once the game loop ends—either by the player quitting or the snake colliding with the wall or itself—the code cleans up by destroying the renderer and window and calling SDL_Quit() to free up resources.

Overall, this code demonstrates fundamental game development concepts: initializing and managing a window and renderer with SDL2, handling user input, updating game state based on logic, and rendering the state to the screen, all while gradually increasing difficulty as the game progresses.
