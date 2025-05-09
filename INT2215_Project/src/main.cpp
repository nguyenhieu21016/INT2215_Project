#include "GameBase.h"
#include "GameAssets.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "SkillObject.h"
#include "EnemyObject.h"
#include "ButtonObject.h"

using namespace std;
std::vector <Point> points;
std::vector <SDL_Texture*> hp;
std::vector <EnemyObject> enemies;
std::vector <SDL_Texture*> skillTexture;
int score = 0;
int lastScoreCheckpoint = 0;
int ultimate = 0;
struct LoadAsset {
    BaseObject* object;
    const char* path;
    SDL_Texture** texturePtr;
};

// Vector quản lí assets
std::vector <LoadAsset> assets = {
    { &gBackground, "assets/background/background.png", NULL},
    { &gMenu, "assets/background/menu_title.png", NULL},
    { &gMenuBackground, "assets/background/backgroundmenu.png", NULL},
    { &gGameOverBackground, "assets/background/gameover_background.png", NULL},
    { &bPause, "assets/button/pause_button.png", NULL},
    { &bStart, "assets/button/start_button.png", NULL},
    { &bContinue, "assets/button/continue_button.png", NULL},
    { &bContinueOverButton, "assets/button/continue_over_button.png", NULL},
    { &bQuitOverButton, "assets/button/quit_over_button.png", NULL},
    { &bSelect, "assets/button/select_button.png", NULL},
    { &bSelectCorner, "assets/button/select_corner.png", NULL},
    { &bQuit, "assets/button/quit_button.png", NULL},
    { &gWaiting, "assets/cat/waiting.png", &tWaiting},
    { &gDrawing, "assets/cat/drawing.png", &tDrawing},
    { &gSunken, "assets/cat/sunken.png", &tSunken},
    { &gVertical, "assets/cat/vertical.png", &tVertical},
    { &gLightning, "assets/cat/lightning.png", &tLightning},
    { &gHurt, "assets/cat/hurt.png", &tHurt},
    { &gHorizontal, "assets/cat/horizontal.png", &tHorizontal},
    { &gEnemyLeft, "assets/ghost/ghost_normal_left.png", &tEnemyLeft},
    { &gEnemyRight, "assets/ghost/ghost_normal_right.png", &tEnemyRight},
    { &sSunken, "assets/symbols/sSunken.png", &tsSunken},
    { &sVertical, "assets/symbols/sVertical.png", &tsVertical},
    { &sHealth, "assets/symbols/sHealth.png", &tsHealth},
    { &sLightning, "assets/symbols/sLightning.png", &tsLightning},
    { &sHorizontal, "assets/symbols/sHorizontal.png", &tsHorizontal},
    { &sEnemyDieRight, "assets/ghost/ghost_die_right.png", &tEnemyDỉeRight},
    { &sEnemyDieLeft, "assets/ghost/ghost_die_left.png", &tEnemyDỉeLeft},
    { &sEnemyHurtRight, "assets/ghost/ghost_hurt_right.png", &tEnemyHurtRight},
    { &sEnemyHurtLeft, "assets/ghost/ghost_hurt_left.png", &tEnemyHurtLeft}
};

// Load media
bool loadMedia()
{
    SDL_Color textColor = {255, 255, 255};
    bool success = true;

    // Load các asset và gán texture cho đối tượng
    for (const auto& asset : assets)
    {
        if (!asset.object->loadFromFile(asset.path))
        {
            success = false;
        } else if (asset.texturePtr)
        {
            *asset.texturePtr = asset.object->getTexture();
        }
    }

    // Load font chữ
    gFont = TTF_OpenFont( "assets/fonts/m6x11.ttf", 34 );
    if( gFont == NULL )
    {
        cout << TTF_GetError();
        success = false;
    }

    // Tạo texture cho từng chữ số (0-9) để hiển thị score cho mượt
    for (int i = 0; i < 10; i++)
    {
        std::string digitStr = std::to_string(i);
        SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, digitStr.c_str(), textColor );
        if( textSurface == NULL )
        {
            cout << TTF_GetError();
        }
        else
        {
            digitTextures[i] = SDL_CreateTextureFromSurface( gRenderer, textSurface );
            SDL_FreeSurface(textSurface);
        }
    }

    // Đẩy texture skill để render skillQueue
    skillTexture.push_back(tsSunken);
    skillTexture.push_back(tsLightning);
    skillTexture.push_back(tsVertical);
    skillTexture.push_back(tsHorizontal);

    // Khởi tạo HP
    for (int i = 0; i < 5;  i++)
    {
        hp.push_back(tsHealth);
    }
    return success;
}
int main(int argc, char* argv[])
{
    // Khởi tạo dữ liệu, âm thanh, trạng thái
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;

    Mix_Music* bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_Chunk* pause_bgm = Mix_LoadWAV("assets/audio/pause_bgm.mp3");
    Mix_Chunk* title = Mix_LoadWAV("assets/audio/title.mp3");
    Mix_Chunk* hit = Mix_LoadWAV("assets/audio/hit.mp3");
    Mix_Chunk* select = Mix_LoadWAV("assets/audio/select.mp3");
    Mix_Chunk* selected = Mix_LoadWAV("assets/audio/selected.mp3");
    Mix_Chunk* dead = Mix_LoadWAV("assets/audio/dead.mp3");
    Mix_VolumeChunk(hit, MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(dead, MIX_MAX_VOLUME);
    int bestScore = loadBestScore();
    if (!bgm) {
        std::cout << "Không load được nhạc nền: " << Mix_GetError() << std::endl;
        return -1;
    }

    bool drawingMode = false;
    bool isRunning = false;
    bool isInMenu = true;
    bool isPaused = false;
    bool isGameOver = false;

    gPlayer.setWaitingTexture(tWaiting);
    gPlayer.setTexture(tWaiting);
    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);

    // Hiển thị menu chính
    if (isInMenu)
    {
        isRunning = false;
        Mix_PauseMusic();
        Mix_PlayChannel(-1, title, -1);
    }
    bool selectup = false;
    bool selectdown = false;
    while (isInMenu)
    {
        while (SDL_PollEvent(&gEvent))
        {
            bStart.handleEvent(&gEvent);
            bQuit.handleEvent(&gEvent);
            if (gEvent.type == SDL_QUIT)
            {
                isRunning = false;
                isInMenu = false;
            }
            else if (gEvent.key.keysym.sym == SDLK_RETURN)
            {
                isRunning = true;
                isInMenu = false;
            }
            else if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_UP)
            {
                Mix_PlayChannel(-1, select, 0);
                selectup = true;
                selectdown = false;
            }
            else if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_DOWN)
            {
                Mix_PlayChannel(-1, select, 0);
                selectup = false;
                selectdown = true;
            }
        }

        // Render menu
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        if (bStart.wasClicked())
        {
            isRunning = true;
            isInMenu = false;
        }
        if (bQuit.wasClicked())
        {
            isRunning = false;
            isInMenu = false;
        }

        // Render menu background và các nút
        gMenu.render(0, 0, NULL); // Render menu chính
        bStart.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2, NULL); // Render nút Start
        if (selectup)
        {
            bSelect.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2, NULL);
            bSelectCorner.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2, NULL);
        }
        if (selectdown)
        {
            bSelect.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2 + 90, NULL);
            bSelectCorner.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2 + 90, NULL);
        }
        bQuit.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90, NULL); // Render nút Quit
        SDL_RenderPresent(gRenderer);
    }

    // Vào game
    if (isRunning)
    {
        Mix_HaltChannel(-1);
        Mix_PlayChannel(-1, selected, 0);
        Mix_PlayMusic(bgm, -1);
    }

    // Vòng lặp chính
    while (isRunning)
    {
        // Check score để tăng ultimate
        if (score >=1000 && score / 1000 > lastScoreCheckpoint)
        {
            ultimate += 1;
            lastScoreCheckpoint = score / 1000;
        }
        fps_timer.start();

        // Xử lý khi nhấn nút pause
        if (bPause.wasClicked())
        {
            if (!isPaused)
            {
                isPaused = true;
                Mix_PauseMusic();
                Mix_PlayChannel(-1, pause_bgm, -1);
            }
        }

        // Xử lý event
        while (SDL_PollEvent(&gEvent))
        {
            // Thoát game
            if (gEvent.type == SDL_QUIT)
            {
                isRunning = false;
            }

            // Xử lý vẽ skill
            else if (gEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                // Khi nhấn chuột trái, bắt đầu vẽ skill
                // Xóa các điểm cũ, chuyển sang chế độ vẽ
                // Đổi animation sang vẽ
                if (!isPaused)
                {
                    points.clear();
                    drawingMode = true;
                    gPlayer.setTexture(tDrawing);
                    gPlayer.set_clips(DRAWING_ANIMATION_FRAMES);
                    gPlayer.loop();
                }
            }
            else if (gEvent.type == SDL_MOUSEBUTTONUP)
            {
                // Khi nhả chuột trái, dừng vẽ skill
                // Kiểm tra hình dạng đường vẽ -> skill
                if (!isPaused)
                {
                    drawingMode = false;
                    gPlayer.setTexture(tWaiting);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    // Check đường vẽ
                    if (isHorizontalLine(points))
                    {
                        gPlayer.setTexture(tHorizontal);
                        gPlayer.set_clips(HORIZONTAL_ANIMATION_FRAMES);
                        gPlayer.skill();
                        attack('-', enemies);
                        Mix_PlayChannel(-1, hit, 0);
                    }
                    else if (isVerticalLine(points))
                    {
                        gPlayer.setTexture(tVertical);
                        gPlayer.set_clips(VERTICAL_ANIMATION_FRAMES);
                        gPlayer.skill();
                        Mix_PlayChannel(-1, hit, 0);
                        attack('|', enemies);
                        cout << "ĐƯỜNG DỌC" << endl;
                    }
                    else if (isVLine(points))
                    {
                        gPlayer.setTexture(tSunken);
                        gPlayer.set_clips(SUNKEN_ANIMATION_FRAMES);
                        gPlayer.skill();
                        attack('v', enemies);
                        Mix_PlayChannel(-1, hit, 0);
                        cout << "CHỮ V" << endl;
                    }
                    else if (isLightningLine(points))
                    {
                        gPlayer.setTexture(tLightning);
                        gPlayer.set_clips(LIGHTNING_ANIMATION_FRAMES);
                        gPlayer.skill();
                        Mix_PlayChannel(-1, hit, 0);
                        if (ultimate>0)
                        {
                            ultimate-=1;
                            attack('L', enemies);
                        }
                        cout << "ĐƯỜNG SÉT" << endl;
                    }
                    else
                    {
                        // Không nhận diện được skill -> waiting
                        gPlayer.setTexture(tWaiting);
                        gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                        cout << "KHÔNG PHẢI SKILL" << endl;
                    }
                }
            }
            else if (gEvent.type == SDL_MOUSEMOTION && drawingMode)
            {
                // Khi di chuột, thêm điểm vào mảng point để check đường vẽ
                if (!isPaused)
                {
                    points.push_back({gEvent.motion.x, gEvent.motion.y});
                    cout << gEvent.motion.x << " " << gEvent.motion.y << endl;
                }
            }

            // Ấn P để dừng, ấn R để tiếp tục
            else if (gEvent.type == SDL_KEYDOWN)
            {
                if (gEvent.key.keysym.sym == SDLK_p)
                {
                    if (!isPaused)
                    {
                        isPaused = true;
                        Mix_PauseMusic();
                    }
                }
                if (gEvent.key.keysym.sym == SDLK_r)
                {
                    isPaused = false;
                    drawingMode = false;
                    points.clear();
                    gPlayer.setTexture(tWaiting);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    Mix_HaltChannel(-1);
                    Mix_ResumeMusic();
                }
            }

            // Xử lí event cho pause
            bPause.handleEvent(&gEvent);
            bContinue.handleEvent(&gEvent);
            bContinueOverButton.handleEvent(&gEvent);
            bQuitOverButton.handleEvent(&gEvent);

        }

        // Render màn hình
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        if (!isPaused)
        {
            // Render background
            gBackground.render(0, 0, NULL);

            // Render đường vẽ skill
            if (points.size()>1)
            {
                for (size_t i = 1; i < points.size(); ++i)
                {
                    SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y, points[i].x+1, points[i].y);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x+2, points[i-1].y, points[i].x+2, points[i].y);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y+1, points[i].x, points[i].y+1);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y+2, points[i].x, points[i].y+2);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y, points[i].x-1, points[i].y);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x-2, points[i-1].y, points[i].x-2, points[i].y);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y-1, points[i].x, points[i].y-1);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y-2, points[i].x, points[i].y-2);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y+1, points[i].x+1, points[i].y+1);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y-1, points[i].x-1, points[i].y-1);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y-1, points[i].x+1, points[i].y-1);
                    SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y+1, points[i].x-1, points[i].y+1);
                }
            }

            // Spawn enemy mới
            spawnEnemy(enemies);

            //Kiểm tra va chạm giữa player và enemy
            for (EnemyObject& enemy : enemies)
            {
                SDL_Rect playerRect = { (SCREEN_WIDTH - gPlayer.getWidth())/2, (SCREEN_HEIGHT - gPlayer.getHeight())/2, gPlayer.getWidth()-90, gPlayer.getHeight()};
                SDL_Rect enemyRect = { enemy.xpos, enemy.ypos, 60, 150 };

                // Nếu enemy chưa va chạm và có va chạm với player -> xử lý mất máu, chuyển trạng thái player, enemy
                if (!enemy.hasCollided && checkCollision(playerRect, enemyRect))
                {
                    enemy.hasCollided = true;
                    enemy.skillQueue.clear();
                    gPlayer.setTexture(tHurt);
                    gPlayer.set_clips(HURT_ANIMATION_FRAMES);
                    gPlayer.hurt();
                    hp.erase(hp.begin());
                    if (hp.empty())
                    {
                        isPaused = true;
                        isGameOver = true;
                    }
                }
            }

            // Check enemy sống chết
            enemyLive(enemies, dead, tEnemyDỉeRight, tEnemyDỉeLeft, score);

            // Render điểm số và ultimate
            renderScore(gRenderer, score, 122, 35, digitTextures);
            renderScore(gRenderer, ultimate, 165, 73, digitTextures);

            // Render enemy từng con (trái/phải)
            for (EnemyObject& enemy : enemies)
            {
                if (enemy.xpos < (SCREEN_WIDTH - FRAME_CHARACTER_WIDTH)/2)
                {
                    enemy.show(enemy.xpos, enemy.ypos, tEnemyLeft, skillTexture, tEnemyHurtRight, tEnemyHurtLeft);
                }
                else
                {
                    enemy.show(enemy.xpos, enemy.ypos, tEnemyRight, skillTexture, tEnemyHurtRight, tEnemyHurtLeft);
                }
            }

            // Render player
            gPlayer.show();

            // Render thanh máu
            renderHP(hp);

            // Render nút pause
            bPause.render (10, 660, NULL);
        }
        else if (isPaused && !isGameOver)
        {
            gMenuBackground.render(0, 0, NULL);
            renderHP(hp);
            renderScore(gRenderer, score, 50, 30, digitTextures);
            bContinue.render (150, 660, NULL);
            if (bContinue.wasClicked())
            {
                isPaused = false;
                drawingMode = false;
                points.clear();
                gPlayer.setTexture(tWaiting);
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                Mix_HaltChannel(-1);
                Mix_ResumeMusic();
            }
        }

        // Render màn hình Game Over
        if (isGameOver)
        {

            Mix_HaltChannel(-1);
            Mix_HaltMusic();
            if (score > bestScore) {
                bestScore = score;
                saveBestScore(bestScore);
            }
            // Render background game over
            gGameOverBackground.render(0, 0, NULL);
            renderScore(gRenderer, score, 900, 200, digitTextures);
            renderScore(gRenderer, bestScore, 900, 270, digitTextures);
            bContinueOverButton.render (350, 600, NULL);
            bQuitOverButton.render (700, 600, NULL);
            if (bContinueOverButton.wasClicked())
            {
                // Reset game state
                score = 0;
                ultimate = 0;
                lastScoreCheckpoint = 0;
                hp.clear();
                for (int i = 0; i < 5; i++) hp.push_back(tsHealth);
                enemies.clear();
                isGameOver = false;
                isPaused = false;
                points.clear();
                gPlayer.setTexture(tWaiting);
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                Mix_PlayMusic(bgm, -1);
                //lastDifficultyUpdate = SDL_GetTicks();
            }
            if (bQuitOverButton.wasClicked())
            {
                isRunning = false;
                isInMenu = false;
            }
            
        }
        SDL_RenderPresent(gRenderer);

        //Giới hạn FPS
        int real_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        if (real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            if (delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }
    }
    // Giải phóng tài nguyên
    gBackground.free();
    gPlayer.free();
    Mix_FreeChunk(hit);
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    return 0;
}
