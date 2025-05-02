#include "MainObject.h"

MainObject::MainObject()
{
    mTexture = NULL;
    frame_ = 0;
    last_frame_time_ = 0;
    status_ = -1;

}
MainObject::~MainObject()
{
    free();
}
void MainObject::set_clips(int s)
{
    mWidth = mWidth / s;
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
void MainObject::loop()
{
    status_ = WAITING;
}
void MainObject::skill()
{
    status_ = -1;
}
void MainObject::hurt()
{
    status_ = HURT;
}

void MainObject::show()
{
    SDL_Rect renderQuad = {(SCREEN_WIDTH - mWidth)/2, (SCREEN_HEIGHT - mHeight)/2, mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, &gSpriteClips[frame_], &renderQuad);
    Uint32 current_time = SDL_GetTicks();
    if (current_time > last_frame_time_ + 150) // 100ms mới đổi frame 1 lần (10 fps)
    {
        if (status_ == WAITING || status_ == DRAWING)
        {
            frame_ = (frame_ + 1) % total_frames_;
        }
        else
        {
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
void MainObject::setWaitingTexture(SDL_Texture* textureToLoad)
{
    waitingTexture = textureToLoad;
}
