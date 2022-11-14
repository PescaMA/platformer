#include "ExtendedRaylib.h"
#include "Objects.h"
#include <map>
extern char doing[21];

const int screenWidth=1000;
const int screenHeight=640;
int fps;
Texture2D ASSET_CHARACTER;
Texture2D ASSET_BLOCKS;
Texture2D ASSET_SPECIAL;
Object **AllObjects;
int nrOfObjects;
std::map<int,int> UID_pairing;


void loadAllObjects();
class Loader
{
public:
    static void load()
    {
        InitWindow(screenWidth,screenHeight,"Platformer");
        fps=30;
        SetTargetFPS(fps);
        SetExitKey(0);
        ASSET_CHARACTER=LoadTexture("Images/character.png");
        ASSET_BLOCKS=LoadTexture("Images/blocks.png");
        ASSET_SPECIAL=LoadTexture("Images/special.png");
        loadAllObjects();
    }
    static void unload()
    {
        UnloadTexture(ASSET_CHARACTER);
        UnloadTexture(ASSET_BLOCKS);
        CloseWindow();
    }
    static void mainloop();
    static void loadMap(const char levelName[]);
};

void loadAllObjects()
{
    myStart=Start(0,2,ASSET_SPECIAL,0,{5,0,22,64});
    myFinish=Finish(1,2,ASSET_SPECIAL,32,{0,0,32,32});
    Object static *AllObjectss[]=/// taking advantage of easier definition
    {
        &(Block1=Block(10,1,ASSET_BLOCKS,0,{0,0,32,32})),
        &(Block2=Block(11,1,ASSET_BLOCKS,32,{0,0,32,32})),
        &(Block3=Block(12,1,ASSET_BLOCKS,64,{0,0,32,32})),
        &(Block4=Block(13,1,ASSET_BLOCKS,96,{0,0,32,32})),
        &(Block5=Block(14,1,ASSET_BLOCKS,128,{0,0,32,32})),
        &myStart,
        &myFinish
    };
    nrOfObjects=sizeof(AllObjectss)/sizeof(AllObjectss[0]);
    for(int i=0;i<nrOfObjects;i++) /// extra check
    {
        if(UID_pairing.find(AllObjectss[i]->UID) == UID_pairing.end())
            UID_pairing[AllObjectss[i]->UID]=i; /// building map for loading
        else
        {
            std::cout<<"\n\nERROR. REPETITIVE OBJECT UNIQUE ID.\n\n";
            strcpy(doing,"Exiting");
        }
        if(i && AllObjectss[i]->page < AllObjectss[i-1]->page)
        {
            std::cout<<"\n\nERROR. PAGES NOT IN ASCENDING ORDER.\n\n";
            strcpy(doing,"Exiting");
        }
    }

    AllObjects=AllObjectss;
}

class LevelSelect
{
    public:
    void run()
    {
        draw();
    }
    void draw()
    {
        BeginDrawing();
        ClearBackground(BLUE);
        EndDrawing();
    }
};
