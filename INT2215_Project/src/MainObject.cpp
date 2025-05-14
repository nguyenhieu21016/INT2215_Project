// MainObject.cpp: Quản lý trạng thái và hoạt ảnh nhân vật chính
#include "MainObject.h"

// Cắt texture thành các frame animation theo trạng thái hiện tại
void MainObject::set_clips(int s)
{
    mWidth = mWidth / s;
    // Chia đều các frame theo chiều ngang
    if (mWidth > 0 && mHeight > 0)
    {
        for (int i = 0; i < s; i++)
        {
            gSpriteClips[i].x = i * mWidth;
            gSpriteClips[i].y = 0;
            gSpriteClips[i].w = mWidth;
            gSpriteClips[i].h = mHeight;
        }
    }
    total_frames_ = s;
}

// Đặt trạng thái nhân vật sang chờ hành động
void MainObject::loop()
{
    status_ = WAITING;
}

// Reset trạng thái (dùng cho skill đặc biệt)
void MainObject::skill()
{
    status_ = -1;
}

// Đặt trạng thái bị thương
void MainObject::hurt()
{
    status_ = HURT;
}

void MainObject::show()
{
    // Hiển thị frame hiện tại tại vị trí trung tâm
    SDL_Rect renderQuad = {(SCREEN_WIDTH - mWidth)/2, (SCREEN_HEIGHT - mHeight)/2, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, &gSpriteClips[frame_], &renderQuad);
    Uint32 current_time = SDL_GetTicks();
    // Cập nhật animation sau mỗi 150ms
    if (current_time > last_frame_time_ + 150) // 100ms mới đổi frame 1 lần (10 fps)
    {
        if (status_ == WAITING || status_ == DRAWING)
        {
            // Lặp lại animation liên tục khi đang chờ hoặc vẽ
            frame_ = (frame_ + 1) % total_frames_;
        }
        else
        {
            // Animation chạy 1 lần cho trạng thái đặc biệt (vd: bị thương)
            frame_ += 1;
            if (frame_ >= total_frames_)
            {
                frame_ = 0;
                status_ = WAITING;
                setTexture(waitingTexture);
                set_clips(WAITING_ANIMATION_FRAMES);
            }
        }
        last_frame_time_ = current_time;
    }
}

// Gán texture cho trạng thái chờ
void MainObject::setWaitingTexture(SDL_Texture* textureToLoad)
{
    waitingTexture = textureToLoad;
}

MainObject::MainObject()
{
    mTexture = nullptr;
    frame_ = 0;
    last_frame_time_ = 0;
    status_ = -1;

}
MainObject::~MainObject()
{
    free();
}
