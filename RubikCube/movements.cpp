#include "movements.h"
#include "rotations.h"
#include "stickers.h"

#include <algorithm>
#include <iostream>
#include <cstdlib>

// MOVEMENTS     U D F B R L 
void movemment(int direction, int isTransitioning[], int cube_positions_index[], std::vector<int>& make_transitioning, std::vector<int>& indexes_to_change_A, std::vector<int>& indexes_to_change_B) {
    if (direction == -1) {
        std::reverse(indexes_to_change_A.begin(), indexes_to_change_A.end());
        std::reverse(indexes_to_change_B.begin(), indexes_to_change_B.end());
    }
    make_is_transtioning(isTransitioning, cube_positions_index, make_transitioning);
    change_cube_postions_index(cube_positions_index, indexes_to_change_A);
    change_cube_postions_index(cube_positions_index, indexes_to_change_B);
}

void speed(int gear, int &current_speed, float& speed_1, float& speed_2, int &animation) {
    current_speed = gear;
    if (current_speed == 1) { speed_1 = 0.5f;     speed_2 = 0.2f; }
    if (current_speed == 2) { speed_1 = 1.0f;     speed_2 = 0.4f; }
    if (current_speed == 3) { speed_1 = 1.5f;     speed_2 = 0.6f; }
    if (current_speed == 4) { speed_1 = 2.0f;     speed_2 = 0.8f; }
    if (current_speed == 5) animation = 0;
    if (current_speed != 5) animation = 1;
}

void movements(int info, int animation, int direction, int stickers[36], int movement_index, float& rotationSpeed, int v, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[]){
    std::vector<int> make_transitioning = { 1, 7, 8, 9, 10, 19, 20, 21, 22 };
    std::vector<int> indexes_to_change_A = { 7,  10, 8,  9 };
    std::vector<int> indexes_to_change_B = { 19, 21, 22, 20 };
    if (animation == 0) transitionProgress = 1.0f;
    if (v == 1) rotationSpeed = speed_1;
    if (v == 2) rotationSpeed = speed_2;
    

    if (movement_index == 1) {
        if (info == 1) {
            std::cout << "Movement\tU";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = -90.0f * direction;
        axis_of_rotation = 2;
        make_transitioning = { 1, 7, 8, 9, 10, 19, 20, 21, 22 };
        indexes_to_change_A = { 7,  10, 8,  9 };
        indexes_to_change_B = { 19, 21, 22, 20 };
    }

    if (movement_index == 2){ 
        if (info == 1) {
            std::cout << "Movement\tD";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = 90.0f * direction;
        axis_of_rotation = 2;
        make_transitioning = { 2, 11, 12, 13, 14, 23, 24, 25, 26 };
        indexes_to_change_A = { 11,  13, 12,  14 };
        indexes_to_change_B = { 23, 24, 26, 25 };
    }
    
    if (movement_index == 3) {
        if (info == 1) {
            std::cout << "Movement\tF";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = -90.0f * direction;
        axis_of_rotation = 3;
        make_transitioning = { 3, 7, 11, 15, 16, 19, 20, 23, 24 };
        indexes_to_change_A = { 7, 16, 11, 15 };
        indexes_to_change_B = { 19, 20, 24, 23 };
    }

    if (movement_index == 4) {
        if (info == 1) {
            std::cout << "Movement\tB";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = 90.0f * direction;
        axis_of_rotation = 3;
        make_transitioning = { 4, 8, 12, 17, 18, 21, 22, 25, 26 };
        indexes_to_change_A = { 8, 17, 12, 18 };
        indexes_to_change_B = { 22, 21, 25, 26 };
    }

    if (movement_index == 5) {
        if (info == 1) {
            std::cout << "Movement\tR";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = -90.0f * direction;
        axis_of_rotation = 1;
        make_transitioning = { 5, 9, 18, 13, 16, 20, 22, 24, 26 };
        indexes_to_change_A = { 18, 13, 16, 9 };
        indexes_to_change_B = { 20, 22, 26, 24 };
    }

    if (movement_index == 6) {
        if (info == 1) {
            std::cout << "Movement\tL";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = 90.0f * direction;
        axis_of_rotation = 1;
        make_transitioning = { 6, 10, 14, 15, 17, 19, 21, 23, 25 };
        indexes_to_change_A = { 10, 15, 14, 17 };
        indexes_to_change_B = { 21, 19, 23, 25 };
    }

    if (movement_index == 7) {
        if (info == 1) {
            std::cout << "Movement\tX";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        movements(0, animation, 1 * direction, stickers, 5, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index);
        movements(0, animation,-1 * direction, stickers, 6, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index);
        movements(0, animation, 1 * direction, stickers, 9, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index);
    }

    if (movement_index == 8) {
        if (info == 1) {
            std::cout << "Movement\tY";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        movements(0, animation,-1 * direction, stickers, 1, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index);
        movements(0, animation, 1 * direction, stickers, 2, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index);
        movements(0, animation,-1 * direction, stickers,10, rotationSpeed, 2, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index); 
    }

    if (movement_index == 9) {
        if (info == 1) {
            std::cout << "Movement\tx";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = -90.0f * direction;
        axis_of_rotation = 1;
        make_transitioning = { 3, 1, 4, 2, 7, 8, 11, 12 };
        indexes_to_change_A = { 7, 8, 12, 11 };
        indexes_to_change_B = { 3, 1, 4,  2 };
    }

    if (movement_index == 10) {
        if (info == 1) {
            std::cout << "Movement\ty";
            if (direction == -1) std::cout << "p";
            std::cout << std::endl;
        }
        angle_f = -90.0f * direction;
        axis_of_rotation = 2;
        make_transitioning = { 3, 4, 5, 6, 15, 16, 17, 18 };
        indexes_to_change_A = { 3,  6,  4,  5 };
        indexes_to_change_B = { 15, 17, 18, 16 };
    }
        
    if (movement_index != 7 and movement_index != 8) {
        movemment(direction, isTransitioning, cube_positions_index, make_transitioning, indexes_to_change_A, indexes_to_change_B);
    }
           
    movement_stickers_select(stickers, movement_index, direction);
}

