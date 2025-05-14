// main.cpp: Vòng đời chính của game, từ menu đến gameplay và game over
#include "GameBase.h"
#include "GameAssets.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "SkillObject.h"
#include "EnemyObject.h"
#include "ButtonObject.h"
#include "SoundManager.h"

using namespace std;
// === Khai báo biến toàn cục ===
std::vector <Point> points;
std::vector <SDL_Texture*> hp;
std::vector <EnemyObject> enemies;
std::vector <SDL_Texture*> skillTexture;
int score = 0;
int lastScoreCheckpoint = 0;
int ultimate = 0;
bool isMuted = false;
// Cấu trúc để load và liên kết asset với texture
struct LoadAsset {
    BaseObject* object;
    const char* path;
    SDL_Texture** texturePtr;
};

// === Danh sách asset cần load khi bắt đầu ===
// Vector quản lí assets
std::vector <LoadAsset> assets = {
    { &gBackground, "assets/background/background.png", nullptr},
    { &gMenu, "assets/background/menu_title.png", nullptr},
    { &gMenuBackground, "assets/background/backgroundmenu.png", nullptr},
    { &gGameOverBackground, "assets/background/gameover_background.png", nullptr},
    { &pauseButton, "assets/button/pause_button.png", nullptr},
    { &startButton, "assets/button/start_button.png", nullptr},
    { &continueButton, "assets/button/continue_button.png", nullptr},
    { &continueOverButton, "assets/button/continue_over_button.png", nullptr},
    { &quitOverButton, "assets/button/quit_over_button.png", nullptr},
    { &selectButton, "assets/button/select_button.png", nullptr},
    { &selectCornerButton, "assets/button/select_corner.png", nullptr},
    { &volumeOnButton, "assets/button/volume_on_button.png", nullptr},
    { &volumeOffButton, "assets/button/volume_off_button.png", nullptr},
    { &quitButton, "assets/button/quit_button.png", nullptr},
    { &gWaiting, "assets/cat/waiting.png", &waitingTexture},
    { &gDrawing, "assets/cat/drawing.png", &drawingTexture},
    { &gSunken, "assets/cat/sunken.png", &sunkenTexture},
    { &gVertical, "assets/cat/vertical.png", &verticalTexture},
    { &gLightning, "assets/cat/lightning.png", &lightningTexture},
    { &gHurt, "assets/cat/hurt.png", &hurtTexture},
    { &gHorizontal, "assets/cat/horizontal.png", &horizontalTexture},
    { &gEnemyLeft, "assets/ghost/ghost_normal_left.png", &enemyLeftTexture},
    { &gEnemyRight, "assets/ghost/ghost_normal_right.png", &enemyRightTexture},
    { &sSunken, "assets/symbols/sSunken.png", &sunkenSymbolTexture},
    { &sVertical, "assets/symbols/sVertical.png", &verticalSymbolTexture},
    { &sHealth, "assets/symbols/sHealth.png", &healthSymbolTexture},
    { &sLightning, "assets/symbols/sLightning.png", &lightningSymbolTexture},
    { &sHorizontal, "assets/symbols/sHorizontal.png", &horizontalSymbolTexture},
    { &sEnemyDieRight, "assets/ghost/ghost_die_right.png", &enemyDieRightTexture},
    { &sEnemyDieLeft, "assets/ghost/ghost_die_left.png", &enemyDieLeftTexture},
    { &sEnemyHurtRight, "assets/ghost/ghost_hurt_right.png", &enemyHurtRightTexture},
    { &sEnemyHurtLeft, "assets/ghost/ghost_hurt_left.png", &enemyHurtLeftTexture}
};

// Load media
bool loadMedia()
{
    SDL_Color textColor = {255, 255, 255};
    bool success = true;

    // Load các asset và gán texture cho đối tượng
    // Load ảnh cho từng asset
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
    gFontBig = TTF_OpenFont( "assets/fonts/m6x11.ttf", 40 );
    if( gFont == nullptr )
    {
        cout << TTF_GetError();
        success = false;
    }

    // Tạo texture cho từng chữ số (0-9) để hiển thị score cho mượt
    // Tạo texture chữ số 0-9
    for (int i = 0; i < 10; i++)
    {
        std::string digitStr = std::to_string(i);
        SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, digitStr.c_str(), textColor );
        if( textSurface == nullptr )
        {
            cout << TTF_GetError();
        }
        else
        {
            digitTextures[i] = SDL_CreateTextureFromSurface( gRenderer, textSurface );
            SDL_FreeSurface(textSurface);
        }
    }

    // Nạp texture kỹ năng để hiển thị trên enemy
    skillTexture.push_back(sunkenSymbolTexture);
    skillTexture.push_back(lightningSymbolTexture);
    skillTexture.push_back(verticalSymbolTexture);
    skillTexture.push_back(horizontalSymbolTexture);

    // Tạo thanh máu ban đầu (5 HP)
    for (int i = 0; i < 1;  i++)
    {
        hp.push_back(healthSymbolTexture);
    }
    return success;
}
int main(int argc, char* argv[])
{
    // === Khởi tạo game và trạng thái ban đầu ===
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;
    if (!SoundManager::loadSound()) return -1;

    saveBestScore(0); // Reset best score mỗi lần chạy game
    int bestScore = loadBestScore();

    bool drawingMode = false;
    bool isRunning = false;
    bool isInMenu = true;
    bool isPaused = false;
    bool isGameOver = false;

    gPlayer.setWaitingTexture(waitingTexture);
    gPlayer.setTexture(waitingTexture);
    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);

    // Hiển thị menu chính
    if (isInMenu)
    {
        isRunning = false;
        Mix_PauseMusic();
        Mix_PlayChannel(-1, SoundManager::title, -1);
    }
    bool selectup = false;
    bool selectdown = false;

    // === Vòng lặp menu chính ===
    while (isInMenu)
    {
        while (SDL_PollEvent(&gEvent))
        {
            startButton.handleEvent(&gEvent);
            quitButton.handleEvent(&gEvent);
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
                Mix_PlayChannel(-1, SoundManager::select, 0);
                selectup = true;
                selectdown = false;
            }
            else if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_DOWN)
            {
                Mix_PlayChannel(-1, SoundManager::select, 0);
                selectup = false;
                selectdown = true;
            }
        }

        // Render menu
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        if (startButton.wasClicked())
        {
            isRunning = true;
            isInMenu = false;
        }
        if (quitButton.wasClicked())
        {
            isRunning = false;
            isInMenu = false;
        }

        // Render menu background và các nút
        gMenu.render(0, 0, NULL); // Render menu chính
        startButton.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2, NULL); // Render nút Start
        if (selectup)
        {
            selectButton.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2, NULL);
            selectCornerButton.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2, NULL);
        }
        if (selectdown)
        {
            selectButton.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2 + 90, NULL);
            selectCornerButton.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2 + 90, NULL);
        }
        quitButton.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90, NULL); // Render nút Quit
        SDL_RenderPresent(gRenderer);
    }

    // === Vào gameplay: phát nhạc nền và âm thanh chọn ===
    if (isRunning)
    {
        Mix_HaltChannel(-1);
        Mix_PlayChannel(-1, SoundManager::selected, 0);
        Mix_PlayMusic(SoundManager::bgm, -1);
    }

    // === Vòng lặp chính gameplay ===
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
        if (pauseButton.wasClicked())
        {
            if (!isPaused)
            {
                isPaused = true;
                Mix_PauseMusic();
                Mix_PlayChannel(-1, SoundManager::pause_bgm, -1);
            }
        }

        // Xử lý sự kiện chuột/phím
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
                    gPlayer.setTexture(drawingTexture);
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
                    gPlayer.setTexture(waitingTexture);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    // Check đường vẽ
                    if (isHorizontalLine(points))
                    {
                        gPlayer.setTexture(horizontalTexture);
                        gPlayer.set_clips(HORIZONTAL_ANIMATION_FRAMES);
                        gPlayer.skill();
                        attack('-', enemies);
                        Mix_PlayChannel(-1, SoundManager::hit, 0);
                    }
                    else if (isVerticalLine(points))
                    {
                        gPlayer.setTexture(verticalTexture);
                        gPlayer.set_clips(VERTICAL_ANIMATION_FRAMES);
                        gPlayer.skill();
                        Mix_PlayChannel(-1, SoundManager::hit, 0);
                        attack('|', enemies);
                        cout << "ĐƯỜNG DỌC" << endl;
                    }
                    else if (isVLine(points))
                    {
                        gPlayer.setTexture(sunkenTexture);
                        gPlayer.set_clips(SUNKEN_ANIMATION_FRAMES);
                        gPlayer.skill();
                        attack('v', enemies);
                        Mix_PlayChannel(-1, SoundManager::hit, 0);
                        cout << "CHỮ V" << endl;
                    }
                    else if (isLightningLine(points))
                    {
                        gPlayer.setTexture(lightningTexture);
                        gPlayer.set_clips(LIGHTNING_ANIMATION_FRAMES);
                        gPlayer.skill();
                        Mix_PlayChannel(-1, SoundManager::hit, 0);
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
                        gPlayer.setTexture(waitingTexture);
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
                    gPlayer.setTexture(waitingTexture);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    Mix_HaltChannel(-1);
                    Mix_ResumeMusic();
                }
            }

        // Xử lí event cho pause
        pauseButton.handleEvent(&gEvent);
        continueButton.handleEvent(&gEvent);
        continueOverButton.handleEvent(&gEvent);
        quitOverButton.handleEvent(&gEvent);

        // Xử lý event cho volume khi pause
        if (isPaused && !isGameOver)
        {
            volumeOnButton.handleEvent(&gEvent);
            volumeOffButton.handleEvent(&gEvent);

            if (volumeOnButton.wasClicked())
            {
                isMuted = true;
                Mix_VolumeMusic(0);
                Mix_Volume(-1, 0); // Tắt luôn hiệu ứng âm thanh
            }
            if (volumeOffButton.wasClicked())
            {
                isMuted = false;
                Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
                Mix_Volume(-1, MIX_MAX_VOLUME / 2); // Bật lại hiệu ứng âm thanh
            }
        }

        }

        // === Render màn hình chính khi đang chơi ===
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        if (!isPaused)
        {
            // Render background
            gBackground.render(0, 0, nullptr);

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
                    gPlayer.setTexture(hurtTexture);
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
            enemyLive(enemies, SoundManager::dead, enemyDieRightTexture, enemyDieLeftTexture, score);

            // Render điểm số và ultimate
            renderScore(gRenderer, score, 122, 35, digitTextures);
            renderScore(gRenderer, ultimate, 165, 73, digitTextures);

            // Render enemy từng con (trái/phải)
            for (EnemyObject& enemy : enemies)
            {
                if (enemy.xpos < (SCREEN_WIDTH - FRAME_CHARACTER_WIDTH)/2)
                {
                    enemy.show(enemy.xpos, enemy.ypos, enemyLeftTexture, skillTexture, enemyHurtRightTexture, enemyHurtLeftTexture);
                }
                else
                {
                    enemy.show(enemy.xpos, enemy.ypos, enemyRightTexture, skillTexture, enemyHurtRightTexture, enemyHurtLeftTexture);
                }
            }

            // Render player
            gPlayer.show();

            // Render thanh máu
            renderHP(hp);

            // Render nút pause
            pauseButton.render (10, 660, nullptr);
        }
        // === Render màn hình pause ===
        else if (isPaused && !isGameOver)
        {
            gMenuBackground.render(0, 0, nullptr);
            renderHP(hp);
            renderScore(gRenderer, score, 50, 30, digitTextures);
            continueButton.render (150, 660, nullptr);
            if (continueButton.wasClicked())
            {
                isPaused = false;
                drawingMode = false;
                points.clear();
                gPlayer.setTexture(waitingTexture);
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                Mix_HaltChannel(-1);
                Mix_ResumeMusic();
            }
            // Render volume buttons
            if (isMuted)
                volumeOffButton.render(200, 600, nullptr);
            else
                volumeOnButton.render(260, 600, nullptr);
        }

        // === Render màn hình Game Over ===
        if (isGameOver)
        {

            Mix_HaltChannel(-1);
            Mix_HaltMusic();
            if (score > bestScore) {
                bestScore = score;
                saveBestScore(bestScore);
            }
            // Render background game over
            gGameOverBackground.render(0, 0, nullptr);
            renderScore(gRenderer, score, 790, 212, digitTextures);
            renderScore(gRenderer, bestScore, 850, 261, digitTextures);
            continueOverButton.render (350, 600, nullptr);
            quitOverButton.render (700, 600, nullptr);
            if (continueOverButton.wasClicked())
            {
                // Reset game state
                score = 0;
                ultimate = 0;
                lastScoreCheckpoint = 0;
                hp.clear();
                for (int i = 0; i < 5; i++) hp.push_back(healthSymbolTexture);
                enemies.clear();
                isGameOver = false;
                isPaused = false;
                points.clear();
                gPlayer.setTexture(waitingTexture);
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                Mix_PlayMusic(SoundManager::bgm, -1);
                //lastDifficultyUpdate = SDL_GetTicks();
            }
            if (quitOverButton.wasClicked())
            {
                isRunning = false;
                isInMenu = false;
            }
            
        }
        SDL_RenderPresent(gRenderer);

        // === Giới hạn FPS ===
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
    // === Giải phóng tài nguyên trước khi thoát ===
    gBackground.free();
    gPlayer.free();
    // SoundManager handles freeing sound resources
    Mix_CloseAudio();
    return 0;
}
