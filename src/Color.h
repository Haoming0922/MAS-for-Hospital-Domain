#ifndef COLOR_H
#define COLOR_H

#include <map>
#include <string>

enum Color {
    Blue,
    Red,
    Cyan,
    Purple,
    Green,
    Orange,
    Pink,
    Grey,
    Lightblue,
    Brown
};

extern std::map<std::string,Color> ColorMap;

#endif