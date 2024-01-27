#include "rotations.h"
#include <glm/glm.hpp> 

#include <algorithm>
#include <iostream>


// START ANIMATION
void make_is_transtioning(int isTransitioning[], int cube_positions_index[], const std::vector<int>& make_transitioning) {
    for (int i = 0; i < make_transitioning.size(); i++) {
        isTransitioning[cube_positions_index[make_transitioning[i]]] = 1;
    }
}

// POSITIONS CHANGE 
void change_cube_postions_index( int cube_positions_index[], std::vector<int>& indexes_to_change) {
    int number_of_elements = 4;
    int cube_positions_index_previous[27];

    for (int i = 0; i < number_of_elements; i++) cube_positions_index_previous[indexes_to_change[i]] = cube_positions_index[indexes_to_change[i]];
    for (int i = 0; i < number_of_elements; i++) cube_positions_index[indexes_to_change[(i + 1) % 4]] = cube_positions_index_previous[indexes_to_change[i]];
}

// ROTATIONS
void rotateCube(float vertices[], int numVertices, float angle, int choose_axis) {
    for (int i = 0; i < numVertices; i += 8) {
        float x = vertices[i];
        float y = vertices[i + 1];
        float z = vertices[i + 2];
        if (choose_axis == 1) rotateX(x, y, z, angle);
        if (choose_axis == 2) rotateY(x, y, z, angle);
        if (choose_axis == 3) rotateZ(x, y, z, angle);
        vertices[i] = x;
        vertices[i + 1] = y;
        vertices[i + 2] = z;
    }
}
void rotateX(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);
    float newY = cosTheta * y - sinTheta * z;
    float newZ = sinTheta * y + cosTheta * z;
    y = newY;
    z = newZ;
}
void rotateY(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);
    float newX = cosTheta * x + sinTheta * z;
    float newZ = -sinTheta * x + cosTheta * z;
    x = newX;
    z = newZ;
}
void rotateZ(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);
    float newX = cosTheta * x - sinTheta * y;
    float newY = sinTheta * x + cosTheta * y;
    x = newX;
    y = newY;
}

