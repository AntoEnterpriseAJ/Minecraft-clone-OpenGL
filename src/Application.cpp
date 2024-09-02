#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "include/Game.h"

int initOpenGL();
GLFWwindow* createWindow();

int main()
{
	GLFWwindow* window = createWindow();
	initOpenGL();

	Game game(window);
	game.render();

    return 0;
}
