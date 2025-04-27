#pragma once

#include "GameBase.h"
#include "BaseObject.h"
struct Point {
    int x, y;
};

bool isVLine(const std::vector<Point>& points);
bool isHorizontalLine(const std::vector<Point>& points);
bool isVerticalLine(const std::vector<Point>& points);
