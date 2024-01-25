#include "stickers.h"

#include <algorithm>
#include <iostream>
#include <cstdlib>

void create_stickers(int stickers[54]) {
    for (int i = 0; i < 9; i++) stickers[i] = 1;
    for (int i = 9; i < 18; i++) stickers[i] = 2;
    for (int i = 18; i < 27; i++) stickers[i] = 3;
    for (int i = 27; i < 36; i++) stickers[i] = 4;
    for (int i = 36; i < 45; i++) stickers[i] = 5;
    for (int i = 45; i < 54; i++) stickers[i] = 6;

    stickers[2] = 7;
    stickers[3] = 8;
    stickers[20] = 20;
    //stickers[3] = 8;
}

void print_stickers(int stickers[54]) {
    std::cout << "UP" << std::endl;
    for (int i = 0;  i < 9;  i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
    std::cout << "DOWN" << std::endl;
    for (int i = 9; i < 18;  i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
    std::cout << "FRONT" << std::endl;
    for (int i = 18; i < 27; i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
    std::cout << "BACK" << std::endl;
    for (int i = 27; i < 36; i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
    std::cout << "RIGHT" << std::endl;
    for (int i = 36; i < 45; i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
    std::cout << "LEFT" << std::endl;
    for (int i = 45; i < 54; i += 3) std::cout << stickers[i] << " " << stickers[i + 1] << " " << stickers[i + 2] << " " << std::endl;
    std::cout << std::endl;
}

void movement_stickers(int stickers[54], std::vector<int>& stickers_1, int direction) {
    std::vector<int> stickers_1_P(stickers_1.size());
    int x = 3;
    if (stickers_1.size() < 10) {
        std::reverse(stickers_1.begin(), stickers_1.end());
        x = 2;
    }
    if (direction == -1)    std::reverse(stickers_1.begin(), stickers_1.end());
    for (size_t i = 0; i < stickers_1.size(); ++i) stickers_1_P[i] = stickers[stickers_1[i]];
    for (size_t i = 0; i < stickers_1.size(); ++i) stickers[stickers_1[i]] = stickers_1_P[(i + x) % stickers_1.size()];
}

void movement_stickers_select(int stickers[54], int movement_index, int direction) {
    std::vector<int> stickers_1 = { 0 };
    std::vector<int> stickers_2 = { 0 };

    if (movement_index == 1) {
        stickers_1 = { 0, 1, 2, 5, 8, 7, 6, 3 };
    }   stickers_2 = { 18, 19, 20, 36, 37, 38, 27, 28, 29, 45, 46, 47 };

    movement_stickers(stickers, stickers_1, direction);
    movement_stickers(stickers, stickers_2, direction);
    
    //system("cls");
    //print_stickers(stickers);
}