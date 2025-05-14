// SoundManager.h: Khai báo class quản lý âm thanh trong game
#pragma once

#include "GameBase.h"

// === Class SoundManager ===
class SoundManager {
public:
    // === Phương thức quản lý âm thanh ===
    static bool loadSound();
    static void cleanUp();
    
    // === Các âm thanh và nhạc nền ===
    // Âm thanh tiêu đề
    static Mix_Chunk* title;
    // Các hiệu ứng âm thanh
    static Mix_Chunk* select;
    // Nhạc nền
    static Mix_Music* bgm;
    // Các hiệu ứng âm thanh
    static Mix_Chunk* pause_bgm;
    // Các hiệu ứng âm thanh
    static Mix_Chunk* hit;
    // Các hiệu ứng âm thanh
    static Mix_Chunk* selected;
    // Các hiệu ứng âm thanh
    static Mix_Chunk* dead;
};
