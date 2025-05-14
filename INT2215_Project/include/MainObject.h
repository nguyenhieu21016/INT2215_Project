// MainObject.h: Khai báo class MainObject xử lý nhân vật chính và các animation
#pragma once

#include "BaseObject.h"

//Hằng số frame
const int SUNKEN_ANIMATION_FRAMES = 8;
const int WAITING_ANIMATION_FRAMES = 11;
const int DRAWING_ANIMATION_FRAMES = 6;
const int HORIZONTAL_ANIMATION_FRAMES = 5;
const int VERTICAL_ANIMATION_FRAMES = 5;
const int LIGHTNING_ANIMATION_FRAMES = 5;
const int HEART_ANIMATION_FRAMES = 5;
const int HURT_ANIMATION_FRAMES = 4;

// === Class MainObject ===
class MainObject : public BaseObject
{
public:
    // Khởi tạo và hủy
    MainObject();
    ~MainObject();

    // Hiển thị nhân vật lên màn hình
    void show();

    // Cập nhật các clip animation cho nhân vật
    void set_clips(int s);

    // Đặt trạng thái nhân vật
    void loop();

    // Xử lý khi nhân vật sử dụng skill
    void skill();

    // Đặt trạng thái bị thương cho nhân vật
    void hurt();

    int status_;

    // Cập nhật texture cho trạng thái chờ
    void setWaitingTexture(SDL_Texture* textureToLoad);
private:
    SDL_Rect gSpriteClips[20];
    SDL_Texture* waitingTexture;
    int frame_;
    Uint32 last_frame_time_;
    int total_frames_;
};
