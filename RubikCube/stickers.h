#pragma once

#include <vector>

void print_stickers(int stickers[54]);
void movement_stickers(int stickers[54], const std::vector<int>& stickers_1, int direction);
void movement_stickers_select(int stickers[54], int movement_index, int direction);
void create_stickers(int stickers[54]);