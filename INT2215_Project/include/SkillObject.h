// SkillObject.h: Khai báo các hàm nhận diện các hình dạng vẽ skill như đường thẳng, chữ V, tia sét, v.v.
#pragma once

#include "BaseObject.h"

// === Cấu trúc điểm ===
struct Point {
    int x, y;
};

// Kiểm tra đường vẽ dạng chữ V
bool isVLine(const std::vector<Point>& points);
// Kiểm tra đường vẽ ngang
bool isHorizontalLine(const std::vector<Point>& points);
// Kiểm tra đường vẽ dọc
bool isVerticalLine(const std::vector<Point>& points);
// Kiểm tra đường vẽ tia sét
bool isLightningLine(const std::vector<Point>& points);
// Kiểm tra đường vẽ hình trái tim
bool isHeartLine(const std::vector<Point>& points);
