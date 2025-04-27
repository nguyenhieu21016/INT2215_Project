#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
static SDL_Event gEvent;

//Screen
const int FRAME_PER_SECOND = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
const int FRAME_CHARACTER_WIDTH = 190;
const int FRAME_CHARACTER_HEIGHT = 170;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

struct Input{
    int left_;
    int right_;
};

//Base
void close();
bool InitData();

//Xử lí FPS
class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
    
    void start();
    void stop();
    void pause();
    void unpause();
    
    int get_ticks();
    
    bool is_started();
    bool is_paused();


private:
    int start_tick_;
    int paused_tick_;
    
    bool is_paused_;
    bool is_started_;
};
