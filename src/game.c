#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

#include "game.h"
#include "globals.h"
#include "utils.h"

// TODO : Add error handling
uint32_t shader_program() {
    int success;
    char info_log[512];
    
    // Vertex shader
    const char *vertex_shader_source = read_file("shaders/test.vert");
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Fragment shader
    const char *fragment_shader_source = read_file("shaders/test.frag");
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Shader program aka linking the shaders
    uint32_t shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader);
    glAttachShader(shader_program_id, fragment_shader);
    glLinkProgram(shader_program_id);

    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }

    // Free resources

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);  
    free((void *) vertex_shader_source);
    free((void *) fragment_shader_source);

    return shader_program_id;
}

InitData init_game(GLFWwindow *window) {
    UNUSED(window);

    uint32_t shader_program_id = shader_program();

    // Init Objects
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    uint32_t STATIC_VBO;
    glGenBuffers(1, &STATIC_VBO);
    uint32_t EBO;
    glGenBuffers(1, &EBO);


    // Actual data
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    uint32_t indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    // Bind objects
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, STATIC_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // Donne les infos sur les données
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Donne l'info d'à quoi ressemble une donnée via l'id 0 défini par le vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbinding objects
    glBindVertexArray(0); // VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO (could be unbound whenever)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO (must be unbound after VAO)

    return (InitData) {
        .shader_program_id = shader_program_id,
        .EBO = EBO,
        .STATIC_VBO = STATIC_VBO,
        .VAO = VAO
    };
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void game_loop(GLFWwindow *window, InitData init_data) {
    processInput(window);
    // Init game loop
    glUseProgram(init_data.shader_program_id);
    glBindVertexArray(init_data.VAO);
    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Actual drawing
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Error handling
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: %d\n", error);
    }

    // End game loop
    glBindVertexArray(0);
}

void delete_game(InitData init_data) {
    glDeleteVertexArrays(1, &init_data.VAO);
    glDeleteBuffers(1, &init_data.STATIC_VBO);
    glDeleteBuffers(1, &init_data.EBO);
    glDeleteProgram(init_data.shader_program_id);
}