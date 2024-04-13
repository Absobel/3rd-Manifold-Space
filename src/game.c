#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

#include "game.h"
#include "globals.h"
#include "utils.h"

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

    // Actual data

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // VAO
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBOs
    uint32_t STATIC_VBO;
    glGenBuffers(1, &STATIC_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, STATIC_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // No more VAO
    glBindVertexArray(0);

    return (InitData) {
        .shader_program_id = shader_program_id,
        .STATIC_VBO = STATIC_VBO,
        .VAO = VAO
    };
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void game_loop(GLFWwindow *window, InitData init_data)
{
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(init_data.shader_program_id);
    glBindVertexArray(init_data.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void delete_game(InitData init_data)
{
    glDeleteVertexArrays(1, &init_data.VAO);
    glDeleteBuffers(1, &init_data.STATIC_VBO);
    glDeleteProgram(init_data.shader_program_id);

}