// GameBase.cpp: Xử lý khởi tạo hệ thống, logic điểm số và bộ đếm thời gian
#include "GameBase.h"

// Biến toàn cục
SDL_Renderer* gRenderer = nullptr;
SDL_Window* gWindow = nullptr;
TTF_Font* gFont = nullptr;
TTF_Font* gFontBig = nullptr;

// Giải phóng
void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;
    IMG_Quit();
    // Giải phóng SDL subsystem
    SDL_Quit();
}

bool InitData()
{
    bool success = true;

    // Khởi tạo SDL, TTF, Mixer và tạo cửa sổ
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        success = false;
    }
    else if (TTF_Init() == -1) {
        std::cout << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
        success = false;
    }
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer lỗi: " << Mix_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Tăng chất lượng render
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        gWindow = SDL_CreateWindow("Feline Frights", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                // Khởi tạo SDL_image
                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
                {
                    success = false;
                }
            }
        }
    }
    return success;
}

// Kiểm tra va chạm giữa hai hình chữ nhật
bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    if (a.x + a.w > b.x && a.x < b.x + b.w && a.y + a.h > b.y && a.y < b.y + b.h)
    {
        return true;
    }
    return false;
}

// Ghi điểm cao nhất vào file
void saveBestScore(int score) {
    std::ofstream out("best_score.txt");
    out << score;
}

// Đọc điểm cao nhất từ file (nếu có)
int loadBestScore() {
    std::ifstream in("best_score.txt");
    int score = 0;
    if (in >> score) return score;
    return 0;
}

// === Lớp bộ đếm thời gian (ImpTimer) ===
ImpTimer::ImpTimer()
{
    start_tick_ = 0;
    paused_tick_ = 0;
    is_paused_ = false;
    is_started_ = false;
}

ImpTimer::~ImpTimer()
{
    
}

// Bắt đầu đếm thời gian
void ImpTimer::start()
{
    is_started_ = true;
    is_paused_ = false;
    start_tick_ = SDL_GetTicks();
}

// Dừng timer
void ImpTimer::stop()
{
    is_paused_ = false;
    is_started_ = false;
}

// Tạm dừng
void ImpTimer::pause()
{
    if (is_started_ == true && is_paused_ == false)
    {
        is_paused_ = true;
        paused_tick_ = SDL_GetTicks() - start_tick_;
    }
}

// Tiếp tục sau khi tạm dừng
void ImpTimer::unpause()
{
    if (is_paused_ == true)
    {
        is_paused_ = false;
        start_tick_ = SDL_GetTicks() - paused_tick_;
        paused_tick_ = 0;
    }
}

// Lấy số mili-giây đã trôi qua
int ImpTimer::get_ticks()
{
    if (is_started_ == true)
    {
        if (is_paused_ == true)
        {
            return paused_tick_;
        } else
        {
            return SDL_GetTicks() - start_tick_;
        }
    }
    return 0;
}

// Trả về trạng thái đã bắt đầu
bool ImpTimer::is_started()
{
    return is_started_;
}

// Trả về trạng thái tạm dừng
bool ImpTimer::is_paused()
{
    return is_paused_;
}
