#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "common.h"

const char *gl_severity_to_string(GLenum severity)
{
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return "High Severity";
        case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium Severity";
        case GL_DEBUG_SEVERITY_LOW:          return "Low Severity";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        default:                             return "Unknown Severity";
    }
}

const char *gl_type_to_string(GLenum type)
{
    switch (type) {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behavior";
        default:                                return "Unknown";
    }
}

static void APIENTRY gl_debug_output(GLenum source,
                                     GLenum type,
                                     unsigned int id,
                                     GLenum severity,
                                     GLsizei length,
                                     const char *message,
                                     const void *userParam)
{
    UNUSED_PARAMETER(source);
    UNUSED_PARAMETER(id);
    UNUSED_PARAMETER(length);
    UNUSED_PARAMETER(userParam);

    fprintf(stderr, "SuperAlienBro: OpenGL(%s) %s: %s\n", gl_severity_to_string(severity),
                                                          gl_type_to_string(type),
                                                          message);

    if (type == GL_DEBUG_TYPE_ERROR)
        exit(2);
}

int main(int argc, char** argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (!glfwInit()) {
        fprintf(stderr, "SuperAlienBro: an error occurred while initializing GLFW\n");

        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow *mainWindow = glfwCreateWindow(1000, 1000, "Super Alien Bro", NULL, NULL);
    if (mainWindow == NULL) {
        fprintf(stderr, "Could not create an OpenGL window\n");

        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(mainWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    game_begin();

    clock_t old_ticks = clock();
    while (!glfwWindowShouldClose(mainWindow)) {
        clock_t new_ticks = clock();
        clock_t elapsed_ticks = new_ticks - old_ticks;
        float elapsed_secs = (float)elapsed_ticks / CLOCKS_PER_SEC;
        old_ticks = new_ticks;

        game_update(elapsed_secs);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    game_end();
    glfwTerminate();
    return 0;
}