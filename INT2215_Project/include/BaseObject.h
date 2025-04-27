#pragma once

#include "GameBase.h"

class BaseObject
{
public:
    //Constructor
    BaseObject();
    //Destructor
    ~BaseObject();
    //Hàm load ảnh
    bool loadFromFile(std::string path);
    void render(int x, int y, const SDL_Rect* clip);
    //Hàm giải phóng bộ nhớ
    void free();
    
    //Hàm trả
    int getWidth();
    int getHeight();
private:

protected:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
    SDL_Rect rect_;
};



