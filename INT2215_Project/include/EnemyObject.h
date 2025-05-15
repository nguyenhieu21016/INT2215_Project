// EnemyObject.h: Định nghĩa class EnemyObject và các hàm xử lý quái
#pragma once

#include "GameBase.h"
#include "MainObject.h"

// === Class EnemyObject ===
class EnemyObject : public BaseObject
{
public:
    // Khởi tạo và hủy
    EnemyObject();
    EnemyObject(int x0, int y0, std ::vector<char> skills);
    ~EnemyObject();
    // Thuộc tính trạng thái quái
    int xpos;
    int ypos;
    std::vector <char> skillQueue;
    bool isDying;
    bool hasCollided;
    bool isHurt;
    bool isBoss;
    // Hiển thị quái và kỹ năng tương ứng
    void show(int& x, int& y, SDL_Texture* enemyTexture, std::vector <SDL_Texture*> skillTexture, SDL_Texture* hurtRTex, SDL_Texture* hurtLTex);
private :
};

// Xử lý trạng thái sống/chết của quái
void enemyLive(std::vector<EnemyObject>& enemies, Mix_Chunk* dead, SDL_Texture* dieRTex, SDL_Texture* dieLTex, int& score);

// Gây sát thương lên quái dựa theo kỹ năng
void attack(char skill, std::vector <EnemyObject>& enemies);

// Sinh quái mới sau mỗi khoảng thời gian
void spawnEnemy(std::vector <EnemyObject>& enemies);

// Tạo danh sách kỹ năng ngẫu nhiên cho quái
std::vector <char> generateRandomSkill();

