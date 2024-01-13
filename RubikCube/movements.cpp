#include "movements.h"
#include "functions.h"

#include <algorithm>
#include <iostream>

// MOVEMENTS     U D F B R L 
void movemment(int direction, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[], const std::vector<int>& make_transitioning, std::vector<int>& indexes_to_change_A, std::vector<int>& indexes_to_change_B) {
    if (direction == -1) {
        std::reverse(indexes_to_change_A.begin(), indexes_to_change_A.end());
        std::reverse(indexes_to_change_B.begin(), indexes_to_change_B.end());
    }
    make_is_transtioning(isTransitioning, cube_positions_index, make_transitioning);
    change_cube_postions_index(cube_positions_index_previous, cube_positions_index_next, cube_positions_index, indexes_to_change_A);
    change_cube_postions_index(cube_positions_index_previous, cube_positions_index_next, cube_positions_index, indexes_to_change_B);
}

void movement_U(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tU";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = -90.0f * direction;
    axis_of_rotation = 2;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 1, 7, 8, 9, 10, 19, 20, 21, 22 };
    std::vector<int> indexes_to_change_A = { 7,  10, 8,  9 };
    std::vector<int> indexes_to_change_B = { 19, 21, 22, 20 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_R(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tR";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = -90.0f * direction;
    axis_of_rotation = 1;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 5, 9, 18, 13, 16, 20, 22, 24, 26 };
    std::vector<int> indexes_to_change_A = { 18, 13, 16, 9 };
    std::vector<int> indexes_to_change_B = { 20, 22, 26, 24 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_F(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tF";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = -90.0f * direction;
    axis_of_rotation = 3;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 3, 7, 11, 15, 16, 19, 20, 23, 24 };
    std::vector<int> indexes_to_change_A = { 7, 16, 11, 15 };
    std::vector<int> indexes_to_change_B = { 19, 20, 24, 23 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_D(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tD";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = 90.0f * direction;
    axis_of_rotation = 2;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 2, 11, 12, 13, 14, 23, 24, 25, 26 };
    std::vector<int> indexes_to_change_A = { 11,  13, 12,  14 };
    std::vector<int> indexes_to_change_B = { 23, 24, 26, 25 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_L(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tL";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = 90.0f * direction;
    axis_of_rotation = 1;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 6, 10, 14, 15, 17, 19, 21, 23, 25 };
    std::vector<int> indexes_to_change_A = { 10, 15, 14, 17 };
    std::vector<int> indexes_to_change_B = { 21, 19, 23, 25 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_B(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tB";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = 90.0f * direction;
    axis_of_rotation = 3;
    rotationSpeed = speed_1;
    std::vector<int> make_transitioning = { 4, 8, 12, 17, 18, 21, 22, 25, 26 };
    std::vector<int> indexes_to_change_A = { 8, 17, 12, 18 };
    std::vector<int> indexes_to_change_B = { 22, 21, 25, 26 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

// MOVEMENTS    X Y 
void movement_x(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tx";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = -90.0f * direction;
    axis_of_rotation = 1;
    std::vector<int> make_transitioning = { 3, 1, 4, 2, 7, 8, 11, 12 };
    std::vector<int> indexes_to_change_A = { 7, 8, 12, 11 };
    std::vector<int> indexes_to_change_B = { 3, 1, 4,  2 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_y(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\ty";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    if (animation == 1) transitionProgress = 0.0f;
    angle_f = -90.0f * direction;
    axis_of_rotation = 2;
    std::vector<int> make_transitioning = { 3, 4, 5, 6, 15, 16, 17, 18 };
    std::vector<int> indexes_to_change_A = { 3,  6,  4,  5 };
    std::vector<int> indexes_to_change_B = { 15, 17, 18, 16 };
    movemment(direction, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next, make_transitioning, indexes_to_change_A, indexes_to_change_B);
}

void movement_X(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tX";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    movement_R(0, animation, 1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    movement_L(0, animation, -1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    movement_x(0, animation, 1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    rotationSpeed = speed_2;
}

void movement_Y(int info, int animation, int direction, float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]) {
    if (info == 1) {
        std::cout << "Movement\tY";
        if (direction == -1) std::cout << "p";
        std::cout << std::endl;
    }
    movement_U(0, animation, 1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    movement_D(0, animation, -1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    movement_y(0, animation, 1 * direction, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
    rotationSpeed = speed_2;
}