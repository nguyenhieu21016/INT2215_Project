// SoundManager.cpp: Quản lý nhạc nền và hiệu ứng âm thanh cho game
#include "SoundManager.h"

// === Biến static dùng để quản lý âm thanh ===
Mix_Music* SoundManager::bgm = nullptr;
Mix_Chunk* SoundManager::pause_bgm = nullptr;
Mix_Chunk* SoundManager::title = nullptr;
Mix_Chunk* SoundManager::hit = nullptr;
Mix_Chunk* SoundManager::select = nullptr;
Mix_Chunk* SoundManager::selected = nullptr;
Mix_Chunk* SoundManager::dead = nullptr;

bool SoundManager::loadSound() {
    // Tải nhạc nền
    bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    // Tải âm thanh tiêu đề
    title = Mix_LoadWAV("assets/audio/title.mp3");
    // Tải âm thanh chọn
    select = Mix_LoadWAV("assets/audio/select.mp3");
    // Tải âm thanh khi tạm dừng
    pause_bgm = Mix_LoadWAV("assets/audio/pause_bgm.mp3");
    // Tải âm thanh đánh
    hit = Mix_LoadWAV("assets/audio/hit.mp3");
    // Tải âm thanh khi xác nhận
    selected = Mix_LoadWAV("assets/audio/selected.mp3");
    // Tải âm thanh khi chết
    dead = Mix_LoadWAV("assets/audio/dead.mp3");
    // Trả về true nếu tải âm thanh tiêu đề thành công
    return title;
}

void SoundManager::cleanUp()
{
    // Giải phóng âm thanh tiêu đề
    Mix_FreeChunk(title);
    // Giải phóng âm thanh chọn
    Mix_FreeChunk(select);
    // Giải phóng âm thanh khi tạm dừng
    Mix_FreeChunk(pause_bgm);
    // Giải phóng âm thanh đánh
    Mix_FreeChunk(hit);
    // Giải phóng âm thanh khi xác nhận
    Mix_FreeChunk(selected);
    // Giải phóng âm thanh khi chết
    Mix_FreeChunk(dead);
    // Giải phóng nhạc nền
    Mix_FreeMusic(bgm);
}
