
#include <map>

    void RayJump::Loader::load()
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
    void RayJump::Loader::unload()
    {
        UnloadTexture(ASSET_CHARACTER);
        UnloadTexture(ASSET_BLOCKS);
        CloseWindow();
    }

void RayJump::Loader::loadAllObjects()
{
    RayJump::myStart=RayJump::Start(0,2,ASSET_SPECIAL,0,{5,0,22,64});
    RayJump::myFinish=RayJump::Finish(1,2,ASSET_SPECIAL,32,{0,0,32,32});
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
        &RayJump::myStart,
        &RayJump::myFinish
    };
    nrOfObjects=sizeof(AllObjectss)/sizeof(AllObjectss[0]);
    RayJump::AllObjects=AllObjectss;

    extraCheck();
}
void RayJump::Loader::extraCheck()
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
void RayJump::Loader::loadMap(const char levelName[])
{
    RayJump::myMap.loadMap(levelName,"");
}


    void RayJump::LevelSelect::run()
    {
        draw();
    }
    void RayJump::LevelSelect::draw()
    {
        BeginDrawing();
        ClearBackground(BLUE);
        EndDrawing();
    }
