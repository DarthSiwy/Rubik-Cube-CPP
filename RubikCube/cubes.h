#pragma once

#include <vector>
#include <algorithm>

void make_axis(float axisVertices[]);

void make_vertices(float vertices_main[]);
void make_walls_black(float vertices[][288]);
void move_cubes(float vertices[][288], float vertices_main[288]);
void cube_indexes(int cube_positions_index[], float vertices[27][288], float vertices_main[288]);
