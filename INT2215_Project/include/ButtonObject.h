#pragma once

#include "BaseObject.h"

class ButtonObject : public BaseObject {
public:
    ButtonObject(int x_, int y_);
    ~ButtonObject();
    void handleEvent(SDL_Event* gEvent);
    bool wasClicked();
    bool isClicked;
private:
    int xpos;
    int ypos;
};
