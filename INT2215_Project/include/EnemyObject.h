#pragma once

#include "GameBase.h"
#include "BaseObject.h"


class EnemyObject : public BaseObject
{
public:
    EnemyObject(int x0, int y0, std ::vector<char> skills);
    ~EnemyObject();
    int xpos;
    int ypos;
    std::vector <char> skillQueue;
    bool isDying;
    bool isHurt;
    void show(int& x, int& y, SDL_Texture* enemyTexture, std::vector <SDL_Texture*> skillTexture, SDL_Texture* tex);
private :
};
void enemyLive(std::vector <EnemyObject>& enemies, Mix_Chunk* dead, SDL_Texture* tex);
void attack(char skill, std::vector <EnemyObject>& enemies);
void spawnEnemy(std::vector <EnemyObject>& enemies);
std::vector <char> generateRandomSkill();

