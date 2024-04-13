#include <GLFW/glfw3.h>

#ifndef GAME_H
#define GAME_H

typedef struct {
    uint32_t shader_program_id;
    uint32_t STATIC_VBO;
    uint32_t VAO;
} InitData;

InitData init_game(GLFWwindow *window);
void game_loop(GLFWwindow *window, InitData init_data);
void delete_game(InitData init_data);

#endif