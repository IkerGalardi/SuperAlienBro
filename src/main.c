#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "common.h"
#include "globals.h"

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
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    game_window = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "Super Alien Bro", NULL, NULL);
    if (game_window == NULL) {
        fprintf(stderr, "Could not create an OpenGL window\n");

        glfwTerminate();
        return 1;
    }

    srand(time(NULL));

    glfwMakeContextCurrent(game_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    game_begin();

    /// NOTE: before using uint64_t and doubles this system used simple floats and ints. But frames
    ///       in the animation did not advance accordingly due to precision errors.
    struct timespec timespec;
    assert((timespec_get(&timespec, TIME_UTC)));
    double old_time = timespec.tv_sec + ((double)timespec.tv_nsec / (double)1000000000);
    while (!glfwWindowShouldClose(game_window)) {
        assert((timespec_get(&timespec, TIME_UTC)));
        double new_time = timespec.tv_sec + ((double)timespec.tv_nsec / (double)1000000000);
        double elapsed_time = new_time - old_time;
        old_time = new_time;

        game_update((float)elapsed_time);

        glfwSwapBuffers(game_window);
        glfwPollEvents();
    }

    game_end();
    glfwTerminate();
    return 0;
}