#include "cubes.h"
#include <glm/glm.hpp> 

#include <algorithm>
#include <iostream>

// CREATE AXIS
void make_axis(float axisVertices[]) {
    float axisVertices_[] = {
        //   POSITION             COLOR 
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   // X-axis
        5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // Y-axis
        0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // Z-axis
        0.0f, 0.0f, 5.0f,   0.0f, 0.0f, 1.0f
    };
    for (int i = 0; i < 36; i++) axisVertices[i] = axisVertices_[i];
}

// CREATE CUBE
void make_vertices(float vertices_main[]) {
    float vertices[] = {
        //     POSITION          TEXTURE             COLOR
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 1.0f,   // UP
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 0.0f,   // BOTTOM
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // FRONT
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 0.5f, 0.0f,   // BACK 
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 0.5f, 0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // RIGHT
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // LEFT
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f
    };
    for (int i = 0; i < 288; i++) vertices_main[i] = vertices[i];
}

// PAINT WALLS BLACK 
void make_walls_black(float vertices[][288]) {
    const int numCopies = 27; const int vertices_main_size = 288;
    std::vector<int> walls_1 = { 1, 7,  8,  9,  10, 19, 20, 21, 22 };  // UP [WHITE]
    std::vector<int> walls_2 = { 2, 11, 12, 13, 14, 23, 24, 25, 26 };  // BOTTOM [YELLOW]
    std::vector<int> walls_3 = { 3, 7,  11, 15, 16, 19, 20, 23, 24 };  // FRONT [RED]
    std::vector<int> walls_4 = { 4, 8,  12, 17, 18, 21, 22, 25, 26 };  // BACK [ORANGE]
    std::vector<int> walls_5 = { 5, 9,  13, 16, 18, 20, 22, 24, 26 };  // RIGHT [BLUE]
    std::vector<int> walls_6 = { 6, 10, 14, 15, 17, 19, 21, 23, 25 };  // LEFT [GREEN]
    for (int j = 1; j < 27; j++) {
        if (std::find(walls_1.begin(), walls_1.end(), j) == walls_1.end()) {
            for (int i = 5; i < 48; i += 8) {       // UP       
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_2.begin(), walls_2.end(), j) == walls_2.end()) {
            for (int i = 53; i < 96; i += 8) {      // BOTTOM
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_3.begin(), walls_3.end(), j) == walls_3.end()) {
            for (int i = 101; i < 144; i += 8) {      // FRONT
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_4.begin(), walls_4.end(), j) == walls_4.end()) {
            for (int i = 149; i < 192; i += 8) {      // BACK 
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_5.begin(), walls_5.end(), j) == walls_5.end()) {
            for (int i = 197; i < 240; i += 8) {      // RIGHT 
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_6.begin(), walls_6.end(), j) == walls_6.end()) {
            for (int i = 245; i < 288; i += 8) {      // LEFT
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
    }
}

// TRANSLATE CUBES
void move_cubes(float vertices[][288], float vertices_main[288]) {
    glm::vec3  vertices_cubeposition[27];
    glm::vec3 cube_positions_translation[] = {
        glm::vec3(0.0f,   0.0f,   0.0f),  // [0] CENTER OF THE CUBE
        glm::vec3(0.0f,   1.0f,   0.0f),  // [1] WHITE CENTER [UP]
        glm::vec3(0.0f,  -1.0f,   0.0f),  // [2] YELLOW CENTER [BOTTOM]
        glm::vec3(0.0f,   0.0f,   1.0f),  // [3] RED CENTER [FRONT]
        glm::vec3(0.0f,   0.0f,  -1.0f),  // [4] ORANGE CENTER [BACK]
        glm::vec3(1.0f,   0.0f,   0.0f),  // [5] BLUE CENTER [RIGHT]
        glm::vec3(-1.0f,   0.0f,   0.0f),  // [6] GREEN CENTER [LEFT]

        glm::vec3(0.0f,   1.0f,   1.0f),  // [7] EDGES ON WHITE SIDE [UP] 
        glm::vec3(0.0f,   1.0f,  -1.0f),  // [8]
        glm::vec3(1.0f,   1.0f,   0.0f),  // [9]
        glm::vec3(-1.0f,   1.0f,   0.0f),  // [10]

        glm::vec3(0.0f,  -1.0f,   1.0f),  // [11] EDGES ON YELLOW SIDE [BOTTOM] 
        glm::vec3(0.0f,  -1.0f,  -1.0f),  // [12]
        glm::vec3(1.0f,  -1.0f,   0.0f),  // [13]
        glm::vec3(-1.0f,  -1.0f,   0.0f),  // [14]

        glm::vec3(-1.0f,   0.0f,   1.0f),  // [15] EDGES VERTICAL 
        glm::vec3(1.0f,   0.0f,   1.0f),  // [16]
        glm::vec3(-1.0f,   0.0f,  -1.0f),  // [17]
        glm::vec3(1.0f,   0.0f,  -1.0f),  // [18]

        glm::vec3(-1.0f,   1.0f,   1.0f),  // [19] CORNERS ON WHITE SIDE [UP]
        glm::vec3(1.0f,   1.0f,   1.0f),  // [20]
        glm::vec3(-1.0f,   1.0f,  -1.0f),  // [21]
        glm::vec3(1.0f,   1.0f,  -1.0f),  // [22]

        glm::vec3(-1.0f,  -1.0f,   1.0f),  // [23] CORNERS ON YELLOW SIDE [BOTTOM] 
        glm::vec3(1.0f,  -1.0f,   1.0f),  // [24]
        glm::vec3(-1.0f,  -1.0f,  -1.0f),  // [25]
        glm::vec3(1.0f,  -1.0f,  -1.0f),  // [26]
    };
    for (int i = 0; i < 27; i++) {
        for (int j = 0; j < 288; j += 8) {
            glm::vec3 originalVertex(vertices_main[j], vertices_main[j + 1], vertices_main[j + 2]);
            vertices_cubeposition[i] = originalVertex + cube_positions_translation[i];
            vertices[i][j + 0] = vertices_cubeposition[i].x;
            vertices[i][j + 1] = vertices_cubeposition[i].y;
            vertices[i][j + 2] = vertices_cubeposition[i].z;
        }
    }
}

void cube_indexes(int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[], float vertices[27][288], float vertices_main[288]) {
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 288; ++j) {
            vertices[i][j] = vertices_main[j];
            //vertices_previous[i][j] = vertices_main[j];
            //vertices_next[i][j] = vertices_main[j];
        }
        cube_positions_index[i] = i;
        cube_positions_index_previous[i] = i;
        cube_positions_index_next[i] = i;
    }
}