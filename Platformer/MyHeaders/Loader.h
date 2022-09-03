#include "ExtendedRaylib.h"
#include "Objects.h"
const int screenWidth=1000;
const int screenHeight=640;
int fps;
Texture2D ASSET_CHARACTER;
Texture2D ASSET_BLOCKS;

void loadAllObjects();
class Loader
{
public:
    static void load()
    {
        InitWindow(screenWidth,screenHeight,"Platformer");
        fps=60;
        SetTargetFPS(fps);
        SetExitKey(0);
        ASSET_CHARACTER=LoadTexture("Images/character.png");
        ASSET_BLOCKS=LoadTexture("Images/blocks.png");
        loadAllObjects();
        loadFirstMap();
    }
    static void unload()
    {
        UnloadTexture(ASSET_CHARACTER);
        UnloadTexture(ASSET_BLOCKS);
        CloseWindow();
    }
    static void mainloop();
    static void loadFirstMap();
};
Object **AllObjects;
void loadAllObjects()
{
    Object static *AllObjectss[]=
    {
        &(Block1=Block(ASSET_BLOCKS,0,{0,0,32,32})),
        &(Block2=Block(ASSET_BLOCKS,32,{0,0,32,32})),
        &(Block3=Block(ASSET_BLOCKS,64,{0,0,32,32})),
        &(Block4=Block(ASSET_BLOCKS,96,{0,0,32,32})),
        &(Block5=Block(ASSET_BLOCKS,128,{0,0,32,32}))
    };
    AllObjects=AllObjectss;
}
