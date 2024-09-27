#include <stdio.h>

#include <GLFW/glfw3.h>

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

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
}