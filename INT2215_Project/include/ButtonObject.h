// ButtonObject.h: Khai báo class xử lý nút bấm trong game
#pragma once

#include "BaseObject.h"

// === Class ButtonObject ===
class ButtonObject : public BaseObject {
public:
    // Khởi tạo và hủy
    ButtonObject(int x_, int y_);
    ~ButtonObject();
    // Xử lý sự kiện chuột để kiểm tra click
    void handleEvent(SDL_Event* gEvent);
    // Kiểm tra nếu nút được nhấn
    bool wasClicked();
    // Trạng thái đã được nhấn hay chưa
    bool isClicked;
private:
    // Vị trí nút trên màn hình
    int xpos;
    int ypos;
};
