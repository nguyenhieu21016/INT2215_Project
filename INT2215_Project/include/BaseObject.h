#pragma once

// BaseObject.h: Khai báo class cơ bản quản lý texture, render, và hiển thị text/HP/score

#include "GameBase.h"

// === Class BaseObject ===
class BaseObject
{
public:
    // Khởi tạo và hủy
    BaseObject();
    ~BaseObject();

    // Load texture từ file hoặc từ text
    bool loadFromFile(std::string path);
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    // Vẽ texture ra màn hình
    void render(int x, int y, const SDL_Rect* clip);

    // Quản lý bộ nhớ và cập nhật texture
    void free();
    void setTexture(SDL_Texture* textureToLoad);

    // Getter
    int getWidth();
    int getHeight();
    SDL_Texture* getTexture();
private:

protected:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
    SDL_Rect rect_;
};
// === Hàm tiện ích ngoài class ===
// Vẽ điểm số và thanh máu
void renderScore(SDL_Renderer* renderer, int score, int x, int y, SDL_Texture* digitTextures[10]);
void renderHP(std::vector <SDL_Texture*> hp);
