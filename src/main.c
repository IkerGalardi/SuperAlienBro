#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

int main(int argc, char** argv)
{
    if (!glfwInit()) {
        fprintf(stderr, "SuperAlienBro: an error occurred while initializing GLFW\n");

        glfwTerminate();
        return 1;
    }

    GLFWwindow *mainWindow = glfwCreateWindow(1000, 1000, "Super Alien Bro", NULL, NULL);
    if (mainWindow == NULL) {
        fprintf(stderr, "Could not create an OpenGL window\n");

        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(mainWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    game_begin();

    clock_t old_ticks = clock();
    while (!glfwWindowShouldClose(mainWindow)) {
        clock_t new_ticks = clock();
        clock_t elapsed_ticks = new_ticks - old_ticks;
        float elapsed_secs = (float)elapsed_ticks / CLOCKS_PER_SEC;
        old_ticks = new_ticks;

        printf("delta_time = %f\n", elapsed_secs);

        game_update(elapsed_secs);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    game_end();
    glfwTerminate();
    return 0;
}