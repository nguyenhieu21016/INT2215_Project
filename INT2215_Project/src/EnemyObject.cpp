// EnemyObject.cpp: Quản lý hành vi, hiển thị và tương tác của quái vật trong game
#include "EnemyObject.h"
#include "BaseObject.h"

int bossDirection = 1;


EnemyObject::EnemyObject(int x0, int y0, std::vector <char> skills)
{
    xpos = x0;
    ypos = y0;
    skillQueue = skills;
    isDying = false;
    isHurt = false;
    hasCollided = false;
}

EnemyObject::EnemyObject()
{
    xpos = ypos = 0;
    isHurt = false;
    isDying = false;
    hasCollided = false;
    isBoss = false;
    skillQueue.clear();
}

EnemyObject::~EnemyObject()
{
    free();
}

int spawntime = 2000;
Uint32 lastSpawnTime = 0;
Uint32 lastDifficultyUpdate = 0;
void spawnEnemy(std::vector <EnemyObject>& enemies)
{
    // Sinh quái nếu đã đủ thời gian
    if (SDL_GetTicks() > lastSpawnTime + spawntime) 
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "SDL_mixer lỗi: " << Mix_GetError() << std::endl;
        }
        Mix_Chunk* spawn = Mix_LoadWAV("assets/audio/spawn.mp3");

        // Vị trí ngẫu nhiên và kỹ năng ngẫu nhiên
        int y = rand() % 571 + 1;
        int x = (rand() % 2 == 0) ? 0 : 1130;
        std::vector<char> skills = generateRandomSkill();
        EnemyObject newEnemy(x, y, skills);
        enemies.push_back(newEnemy);
        // In thông tin quái vừa sinh
        std::cout << "Spawn : " << x << " " << y << " Skill : ";
        for (char c : skills)
        {
            std::cout << c << " ";
        }
        
        // Phát âm thanh spawn
        Mix_PlayChannel(-1, spawn, 0);
        lastSpawnTime = SDL_GetTicks();
    }
    // Tăng độ khó theo thời gian
    if (SDL_GetTicks() - lastDifficultyUpdate >= 60000)
    {
        // Sau một phút giảm 100ms cho thời gian spawn quái, min là 1000ms
        std::cout << "Giảm thời gian spawn quái" << std::endl;
        spawntime = std::max(1000, spawntime - 100);
        lastDifficultyUpdate = SDL_GetTicks();
    }
}

int frame0_ = 0;
Uint32 last_frame_time0_ = 0;
SDL_Rect clip0[20];
void EnemyObject::show(int& x, int& y, SDL_Texture* enemyTexture, std::vector <SDL_Texture*> skillTexture, SDL_Texture* hurtRTex, SDL_Texture* hurtLTex)
{
    SDL_Texture* texture = NULL;
    SDL_Rect renderQuad = {x, y, 150, 150};
    int x0 = x + 40;
    // Hiển thị quái nếu không bị thương hay đang chết
    if (!isDying && !isHurt)
    {
        SDL_RenderCopy(gRenderer, enemyTexture, NULL, &renderQuad);
    }
    // Vẽ các biểu tượng kỹ năng
    for (char skill : skillQueue)
    {
        if (skill == '-')
        {
            texture = skillTexture[SYMBOL_HORIZONTAL];
        } else if (skill == 'v')
        {
            texture = skillTexture[SYMBOl_SUNKEN];
        } else if (skill == '|')
        {
            texture = skillTexture[SYMBOL_VERTICAL];
        }
        if (texture != NULL)
        {
            SDL_Rect renderQuad = {x0, y, 24, 24};
            SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
        }
        x0+=25;
    }
    // Hiển thị hiệu ứng bị thương
    if (isHurt)
    {
        for (int j = 0; j < 5; j++)
        {
            clip0[j].x = j * 150;
            clip0[j].y = 0;
            clip0[j].w = 150;
            clip0[j].h = 150;
        }
        SDL_Rect renderQuad = {x, y, 150, 150};
        // Hiệu ứng bị thương trái/phải màn hình
        if (x < SCREEN_WIDTH / 2)
        {
            SDL_RenderCopy(gRenderer, hurtLTex, &clip0[frame0_], &renderQuad);
        } else
        {
            SDL_RenderCopy(gRenderer, hurtRTex, &clip0[frame0_], &renderQuad);
        }
        Uint32 current_time = SDL_GetTicks();
        if (current_time > last_frame_time0_ + 100)
        {
            if (frame0_ < 5)
                frame0_++;
            else
            {
                isHurt = false;
                frame0_ = 0;
                last_frame_time0_ = 0;
            }
            last_frame_time0_ = current_time;
        }
        return;
    }
    // Di chuyển enemy thường, boss thì đứng yên
    if (!isBoss) {
        if (x < (SCREEN_WIDTH - FRAME_CHARACTER_WIDTH)/2)
        {
            x+=1;
        } else
        {
            x-=1;
        }
        if (y < ((SCREEN_HEIGHT - FRAME_CHARACTER_HEIGHT)/2 + 40))
        {
            y+=1;
        } else
        {
            y-=1;
        }
    } else {
        // Boss di chuyển lên xuống theo trục y (tọa độ x giữ nguyên)
        if (y <= 80 || y >= 500)
        {
            bossDirection *= -1; // Đảo chiều
        }
        y += bossDirection ; // Di chuyển theo trục y từ 50 đến 521
    }
    
}
std::vector <char> generateRandomSkill() {
    std::vector <char> skillSymbols = {'-', '|', 'v'};
    std::vector <char> result;
    // Chọn kỹ năng ngẫu nhiên
    int numSkill = rand() % 5 + 1;
    for(int i = 0; i < numSkill; ++i)
    {
        int randomIndex = rand() % skillSymbols.size();
        result.push_back(skillSymbols[randomIndex]);
    }
    return result;
}

void attack(char skill, std::vector <EnemyObject>& enemies)
{
    // Nếu là skill đặc biệt 'L', xóa toàn bộ skill của 5 quái đầu
    if (skill == 'L')
    {
        for (int i = 0; i < enemies.size();i++)
        {
            if (i < 5)
            {
                enemies[i].skillQueue.clear();
            }
        }
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        // Tấn công quái nếu skill đầu khớp
        if (!enemies[i].skillQueue.empty())
        {
            if (enemies[i].skillQueue[0] == skill)
            {
                enemies[i].isHurt = true;
                enemies[i].skillQueue.erase(enemies[i].skillQueue.begin());
                std::cout << "đã tấn công quái " << i << std::endl;
            }
        }
    }
}
int frame_ = 0;
Uint32 last_frame_time_ = 0;
SDL_Rect clip[20];
void enemyLive(std::vector <EnemyObject>& enemies, Mix_Chunk* dead, SDL_Texture* dieRTex, SDL_Texture* dieLTex, int& score)
{
    
    for (int i = 0; i < enemies.size(); i++)
    {
        // Nếu quái không còn skill và không bị thương → bắt đầu chết
        if (enemies[i].skillQueue.empty() && !enemies[i].isHurt)
        {
            enemies[i].isDying = true;
            // Thiết lập animation chết
            for (int j = 0; j < 8; j++)
            {
                clip[j].x = j * 150;
                clip[j].y = 0;
                clip[j].w = 150;
                clip[j].h = 150;
            }
            SDL_Rect renderQuad = {enemies[i].xpos, enemies[i].ypos, 150, 150};
            if (enemies[i].xpos < SCREEN_WIDTH / 2)
            {
                SDL_RenderCopy(gRenderer, dieLTex, &clip[frame_], &renderQuad);
            } else
            {
                SDL_RenderCopy(gRenderer, dieRTex, &clip[frame_], &renderQuad);
            }
            // Kiểm soát tốc độ animation
            Uint32 current_time = SDL_GetTicks();
            if (current_time > last_frame_time_ + 100)
            {
                if (frame_ < 8)
                {
                    frame_ += 1;
                } else
                {
                    // Xóa quái, tăng điểm, phát âm thanh
                    Mix_PlayChannel(-1, dead, 0);
                    frame_ = 0;
                    last_frame_time_ = 0;
                    enemies.erase(enemies.begin() + i);
                    score += 100;
                    i--;
                    continue;
                }
                last_frame_time_ = current_time;
            }
        }
    }
}
