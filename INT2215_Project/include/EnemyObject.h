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
    void show(int x, int y, SDL_Texture* enemyTexture);
private :
    
    std::vector <char> skillQueue;
};


void spawnEnemy(std::vector <EnemyObject>& enemies);
std::vector <char> generateRandomSkill();
