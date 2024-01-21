#include "stickers.h"

#include <algorithm>
#include <iostream>
#include <cstdlib>

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

void movement_stickers(int stickers[54], const std::vector<int>& stickers_1) {
    std::vector<int> stickers_1_P(stickers_1.size());
    for (size_t i = 0; i < stickers_1.size(); ++i) stickers_1_P[i] = stickers[stickers_1[i]];
    for (size_t i = 0; i < stickers_1.size(); ++i) stickers[stickers_1[i]] = stickers_1_P[(i + 1) % stickers_1.size()];
}

void movement_U_stickers(int stickers[54]) {
    std::vector<int> stickers_1 = { 0, 1, 2, 5, 8, 7, 6, 3 };
    std::vector<int> stickers_2 = { 22 };
    movement_stickers(stickers, stickers_1);
    movement_stickers(stickers, stickers_2);
    system("cls");
    print_stickers(stickers);
}