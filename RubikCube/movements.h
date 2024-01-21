#pragma once

#include <vector>

extern float speed_1;
extern float speed_2;

void movemment(int direction, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[], const std::vector<int>& make_transitioning, std::vector<int>& indexes_to_change_A, std::vector<int>& indexes_to_change_B);

void movement_U(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_R(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_L(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_F(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_D(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_B(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_X(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
void movement_Y(int info, int animation, int direction, int stickers[36], float& rotationSpeed, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);
