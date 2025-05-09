#include "GameBase.h"

// Biến toàn cục
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
TTF_Font* gFont = NULL;

// Giải phóng
void close()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

// Khởi tạo SDL, TTF, Mixer, window và renderer
bool InitData()
{
    bool success = true;

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
                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
                {
                    success = false;
                }
            }
        }
    }
    return success;
}

// Check va chạm
bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    if (a.x + a.w > b.x && a.x < b.x + b.w && a.y + a.h > b.y && a.y < b.y + b.h)
    {
        return true;
    }
    return false;
}

// Lưu điểm cao nhất vào file
void saveBestScore(int score) {
    std::ofstream out("best_score.txt");
    out << score;
}

// Đọc điểm cao nhất từ file
int loadBestScore() {
    std::ifstream in("best_score.txt");
    int score = 0;
    if (in >> score) return score;
    return 0;
}

// Bộ đo thời gian
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

void ImpTimer::start()
{
    is_started_ = true;
    is_paused_ = false;
    start_tick_ = SDL_GetTicks();
}

void ImpTimer::stop()
{
    is_paused_ = false;
    is_started_ = false;
}

void ImpTimer::pause()
{
    if (is_started_ == true && is_paused_ == false)
    {
        is_paused_ = true;
        paused_tick_ = SDL_GetTicks() - start_tick_;
    }
}

void ImpTimer::unpause()
{
    if (is_paused_ == true)
    {
        is_paused_ = false;
        start_tick_ = SDL_GetTicks() - paused_tick_;
        paused_tick_ = 0;
    }
}

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

bool ImpTimer::is_started()
{
    return is_started_;
}

bool ImpTimer::is_paused()
{
    return is_paused_;
}
