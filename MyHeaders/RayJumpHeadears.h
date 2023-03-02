#ifndef RAY_JUMP_HEADERS
#define RAY_JUMP_HEADERS


#include "Loader.h"
#include "Settings.h"
#include "LevelEditor.h"
#include "Objects.h"
#include "Game.h"
#include "Player.h"

/****************************************************************************
*
*        GLOBALS - from classes declared
*
****************************************************************************/
namespace RayJump
{
    extern Object **AllObjects;
    extern Block Block1,Block2,Block3,Block4,Block5;
    extern Start myStart;
    extern Finish myFinish;
    extern MapObj myMap;
    extern Player myPlayer;
}
#endif // RAY_JUMP_HEADERS
