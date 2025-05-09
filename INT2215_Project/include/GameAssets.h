#pragma once
#include "BaseObject.h"
#include "MainObject.h"
#include "ButtonObject.h"
#include "GameBase.h"

//Base Object
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

// Button
extern ButtonObject bPause;
extern ButtonObject bStart;
extern ButtonObject bContinue;
extern ButtonObject bVolumeOff;
extern ButtonObject bVolumeOn;
extern ButtonObject bContinueOverButton;
extern ButtonObject bQuitOverButton;
extern ButtonObject bQuit;
extern ButtonObject bSelect;
extern ButtonObject bSelectCorner;

// Texture
extern SDL_Texture* tsSunken;
extern SDL_Texture* tsVertical;
extern SDL_Texture* tsHealth;
extern SDL_Texture* tsLightning;
extern SDL_Texture* tsHorizontal;
extern SDL_Texture* tWaiting;
extern SDL_Texture* tDrawing;
extern SDL_Texture* tHurt;
extern SDL_Texture* tSunken;
extern SDL_Texture* tVertical;
extern SDL_Texture* tLightning;
extern SDL_Texture* tHorizontal;
extern SDL_Texture* tEnemyLeft;
extern SDL_Texture* tEnemyRight;
extern SDL_Texture* tEnemyDỉeRight;
extern SDL_Texture* tEnemyDỉeLeft;
extern SDL_Texture* tEnemyHurtRight;
extern SDL_Texture* tEnemyHurtLeft;
extern SDL_Texture* digitTextures[10];
