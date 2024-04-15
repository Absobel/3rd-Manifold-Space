#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "globals.h"
#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    UNUSED(window);
    glViewport(0, 0, width, height);
}

int main() {
    printf("\n///////// STARTING /////////\n");

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE     // Removes deprecated features
    );

    // Open a window and create its OpenGL context
    GLFWwindow *window;
    window = glfwCreateWindow(
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        WINDOW_TITLE, 
        NULL, 
        NULL
    );
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        return -1;
    }    

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Set callback function to resize viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    InitData init_data = init_game(window);

    while(!glfwWindowShouldClose(window))
    {
        game_loop(window, init_data);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    delete_game(init_data);
    glfwTerminate();

    printf("///////// ENDING   /////////\n");
    return 0;
}