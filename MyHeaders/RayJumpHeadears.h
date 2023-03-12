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
    /*class MapObj;
    class Player;
    class Object;
    class Block;
    class Start;
    class Finish;*/
    typedef Block* BlockPtr;
    Object **AllObjects;
    BlockPtr Block1,Block2,Block3,Block4,Block5;
    Start *myStart;
    Finish *myFinish;
    MapObj *myMap;
    Player *myPlayer;
}
#endif // RAY_JUMP_HEADERS
