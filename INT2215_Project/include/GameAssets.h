// GameAssets.h: Khai báo các đối tượng, nút bấm, và texture dùng trong game
#pragma once
#include "BaseObject.h"
#include "MainObject.h"
#include "ButtonObject.h"
#include "GameBase.h"

// === Các đối tượng trong game ===
// Quái
extern MainObject gPlayer;
extern BaseObject gBackground;
extern BaseObject gGameOverBackground;
extern BaseObject gMenu;
extern BaseObject gWaiting;
extern BaseObject gSunken;
extern BaseObject gVertical;
extern BaseObject gDrawing;
extern BaseObject gHurt;
extern BaseObject gLightning;
extern BaseObject gHorizontal;
extern BaseObject gEnemyLeft;
extern BaseObject gEnemyRight;
extern BaseObject sLightning;
extern BaseObject sHorizontal;
extern BaseObject sSunken;
extern BaseObject sHealth;
extern BaseObject sVertical;
extern BaseObject sEnemyDieRight;
extern BaseObject gMenuBackground;
extern BaseObject sEnemyDieLeft;
extern BaseObject sEnemyHurtRight;
extern BaseObject sEnemyHurtLeft;

// === Các nút bấm trong game ===
// Nút bấm
extern ButtonObject pauseButton;
extern ButtonObject startButton;
extern ButtonObject continueButton;
extern ButtonObject volumeOffButton;
extern ButtonObject volumeOnButton;
extern ButtonObject continueOverButton;
extern ButtonObject quitOverButton;
extern ButtonObject quitButton;
extern ButtonObject selectButton;
extern ButtonObject selectCornerButton;

// === Các texture dùng trong game ===
// Texture các biểu tượng
extern SDL_Texture* sunkenSymbolTexture;
extern SDL_Texture* verticalSymbolTexture;
extern SDL_Texture* healthSymbolTexture;
extern SDL_Texture* lightningSymbolTexture;
extern SDL_Texture* horizontalSymbolTexture;
extern SDL_Texture* waitingTexture;
extern SDL_Texture* drawingTexture;
extern SDL_Texture* hurtTexture;
extern SDL_Texture* sunkenTexture;
extern SDL_Texture* verticalTexture;
extern SDL_Texture* lightningTexture;
extern SDL_Texture* horizontalTexture;
extern SDL_Texture* enemyLeftTexture;
extern SDL_Texture* enemyRightTexture;
extern SDL_Texture* enemyDieRightTexture;
extern SDL_Texture* enemyDieLeftTexture;
extern SDL_Texture* enemyHurtRightTexture;
extern SDL_Texture* enemyHurtLeftTexture;
extern SDL_Texture* digitTextures[10];
