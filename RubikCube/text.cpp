#include "text.h"

#include <string>

void make_text_vector_help(std::vector<std::string>& text_vector) {
    text_vector.push_back("W - UP");
    text_vector.push_back("A - LEFT");
    text_vector.push_back("S - FRONT");
    text_vector.push_back("D - RIGHT");
    text_vector.push_back("R - BACK");
    text_vector.push_back("F - DOWN");
    text_vector.push_back("+LSHIFT DIRECTION");
    text_vector.push_back("1-5 - SPEED");
    text_vector.push_back("O - DEMO MODE");
    text_vector.push_back("P - RESET");
    text_vector.push_back("M - MIX");
    text_vector.push_back("N - RESET COUNTER");
    text_vector.push_back("SPACE - CHANGE VIEW");
    text_vector.push_back("ARROWS - CHANGE POSITION");
    text_vector.push_back("ESC - EXIT");

}