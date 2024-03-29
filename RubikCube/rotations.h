#pragma once

#include <vector>
#include <algorithm>

extern float speed_1;
extern float speed_2;

void make_is_transtioning(int isTransitioning[], int cube_positions_index[], const std::vector<int>& make_transitioning);
void change_cube_postions_index(int cube_positions_index[], std::vector<int>& indexes_to_change);

void rotateX(float& x, float& y, float& z, float angle);
void rotateY(float& x, float& y, float& z, float angle);
void rotateZ(float& x, float& y, float& z, float angle);
void rotateCube(float vertices[], int numVertices, float angle, int choose_axis);