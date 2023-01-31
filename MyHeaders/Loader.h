#include "ExtendedRaylib.h"
#include "Objects.h"
#include <map>
extern char doing[21];

const int screenWidth=800;
const int screenHeight=600;
int fps;
Texture2D ASSET_CHARACTER;
Texture2D ASSET_BLOCKS;
Texture2D ASSET_SPECIAL;

int nrOfObjects;
std::map<int,int> UID_pairing;

void extraCheck();
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
     RayJump::Object static *AllObjectss[]=/// taking advantage of easier definition
    {
        ///Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox) is the function
        ///page 1:
        &( Block1= Block(10,1,ASSET_BLOCKS,0,{0,0,32,32})),
        &( Block2= Block(11,1,ASSET_BLOCKS,32,{0,0,32,32})),
        &( Block3= Block(12,1,ASSET_BLOCKS,64,{0,0,32,32})),
        &( Block4= Block(13,1,ASSET_BLOCKS,96,{0,0,32,32})),
        &( Block5= Block(14,1,ASSET_BLOCKS,128,{0,0,32,32})),
        /// page 2:
        &myStart,
        &myFinish
    };
    nrOfObjects=sizeof(AllObjectss)/sizeof(AllObjectss[0]);
    RayJump::AllObjects=AllObjectss;

    extraCheck();
}
void extraCheck()
{
    for(int i=0;i<nrOfObjects;i++)
    {
        /// makes sure UID is unique and also creats a map for them
        if(UID_pairing.find(RayJump::AllObjects[i]->UID) == UID_pairing.end())
            UID_pairing[RayJump::AllObjects[i]->UID]=i; /// building map for loading
        else
        {
            std::cout<<"\n\nERROR. REPETITIVE OBJECT UNIQUE ID.\n\n";
            strcpy(doing,"Exiting");
        }

        /// makes sure pages are in ascending order
        if(i && RayJump::AllObjects[i]->page < RayJump::AllObjects[i-1]->page)
        {
            std::cout<<"\n\nERROR. PAGES NOT IN ASCENDING ORDER.\n\n";
            strcpy(doing,"Exiting");
        }
    }
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
