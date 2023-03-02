#include <map>
#include "../MyHeaders/RayJumpHeadears.h"
/*********************************************
*
*               L O A D E R
*
**********************************************/
void RayJump::Loader::load()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); /// unfortunately raylib (and glfw) has
    /// a resize that pauses other actions so I can't fix black part when resizing.
    InitWindow(iscreenWidth,iscreenHeight,"Platformer");
    fps=60;
    SetTargetFPS(fps);
    SetExitKey(0);
    ASSET_CHARACTER=LoadTexture("Images/character.png");
    ASSET_BLOCKS=LoadTexture("Images/blocks.png");
    ASSET_SPECIAL=LoadTexture("Images/special.png");
    ASSET_BACKGROUND= LoadTexture("Images/background.png");
    loadAllObjects();
}
void RayJump::Loader::unload()
{
    UnloadTexture(ASSET_CHARACTER);
    UnloadTexture(ASSET_BLOCKS);
    UnloadTexture(ASSET_SPECIAL);
    UnloadTexture(ASSET_BACKGROUND);
    CloseWindow();
}

void RayJump::Loader::loadAllObjects()
{
    myStart=Start(0,2,ASSET_SPECIAL,0, {-4,0,40,64}); /// hitbox big for player to fit inside
    myFinish=Finish(1,2,ASSET_SPECIAL,32, {0,0,32,32});
    Object static *AllObjectss[]=/// taking advantage of easier definition
    {
        ///Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox) is the function
        ///page 1:
        &( Block1=Block(10,1,ASSET_BLOCKS,0,{0,0,32,32})),
        &( Block2=Block(11,1,ASSET_BLOCKS,32,{0,0,32,32})),
        &( Block3=Block(12,1,ASSET_BLOCKS,64,{0,0,32,32})),
        &( Block4=Block(13,1,ASSET_BLOCKS,96,{0,0,32,32})),
        &( Block5=Block(14,1,ASSET_BLOCKS,128,{0,0,32,32})),
        /// page 2:
        &myStart,
        &myFinish
    };
    nrOfObjects=sizeof(AllObjectss)/sizeof(AllObjectss[0]);
    AllObjects=AllObjectss;

    extraCheck();
}
void RayJump::Loader::extraCheck()
{
    for(int i=0; i<nrOfObjects; i++)
    {
        /// makes sure UID is unique and also creats a map for them
        if(UID_pairing.find(AllObjects[i]->UID) == UID_pairing.end())
            UID_pairing[AllObjects[i]->UID]=i; /// building map for loading
        else
        {
            std::cout<<"\n\nERROR. REPETITIVE OBJECT UNIQUE ID.\n\n";
            strcpy(doing,"Exiting");
        }

        /// makes sure pages are in ascending order. Useful for level editor
        if(i && AllObjects[i]->page < AllObjects[i-1]->page)
        {
            std::cout<<"\n\nERROR. PAGES NOT IN ASCENDING ORDER.\n\n";
            strcpy(doing,"Exiting");
        }
    }
}
void RayJump::Loader::loadMap(const char levelName[])
{
    myMap.loadMap(levelName,"");
}

/*********************************************
*
*               LVL SELECT
*
**********************************************/

void RayJump::LevelSelect::run()
{
    exit.run(this);
    if(exit.state != Exit::States::off)
        return;
    draw();
}
void RayJump::LevelSelect::draw()
{
    BeginDrawing();
    draw_content(255);
    EndDrawing();
}
void RayJump::LevelSelect::draw_content(int transparency)
{
    ClearBackground(BLUE);
}

