#pragma once

#include "GameBase.h"
#include "BaseObject.h"

//Hằng số frame
const int SUNKEN_ANIMATION_FRAMES = 8;
const int WAITING_ANIMATION_FRAMES = 11;
const int DRAWING_ANIMATION_FRAMES = 6;

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();
    
    void show();
    void set_clips(int s);
    void waiting();
    void skill();
private:
    SDL_Rect gSpriteClips[11];
    enum Animation
    {
        RUNNING, SCARED, WAITING, DRAWING, HEART,
        SUNKEN, HURT, DIE, VICTORY,
    };
    int frame_;
    Uint32 last_frame_time_;
    int status_;
    int total_frames_;
};
