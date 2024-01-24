#pragma once

#include <vector>

//extern float speed_1;
//extern float speed_2;

void speed(int gear, int &current_speed, float& speed_1, float& speed_2, int &animation);
void movemment(int direction, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[], const std::vector<int>& make_transitioning, std::vector<int>& indexes_to_change_A, std::vector<int>& indexes_to_change_B);
void movements(int info, int animation, int direction, int stickers[36], int movement_index,float& rotationSpeed, int v, float& transitionProgress, float& angle_f, int& axis_of_rotation, int isTransitioning[], int cube_positions_index[], int cube_positions_index_previous[], int cube_positions_index_next[]);


