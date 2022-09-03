#include "ExtendedRaylib.h"
extern Texture2D ASSET_BLOCKS;
extern char doing[21];
extern const int screenWidth;
extern const int screenHeight;
extern Object **AllObjects;
class LevelEditor
{
    bool exiting=false;
    static int constexpr lastOnPage[2]={0,5};
    const int static MAX_PAGES=sizeof(lastOnPage)/sizeof(lastOnPage[0])-1;
    FixedButton  buttons[MAX_PAGES];
    int currentPage=0;
    Exit exit;
    KBD_Move kbdMove;
public:
    LevelEditor()
    {
        Button *pointerArray[MAX_PAGES];
        for(int i=0;i<MAX_PAGES;i++)
        {
            pointerArray[i]=&(buttons[i]);
            int pos=screenWidth/2-MAX_PAGES*16+i*32;
            buttons[i]=FixedButton(TextFormat("%i",i+1),pos,screenHeight-94,30,30,28,BLACK,YELLOW);
        }
        kbdMove=KBD_Move(pointerArray,MAX_PAGES,false);

        buttons[currentPage].normalColor.text=GREEN;
        buttons[currentPage].hoverColor.text=GREEN;
    }
    void  run()
    {

        if(exiting)
            return exitscreen();

        for(int i=0;i<MAX_PAGES;i++)
            if(buttons[i].Lclicked())
                changeButton(i);
        kbdMove.run();
        draw();

        if(IsKeyPressed(KEY_ESCAPE))
            exiting=true;
    }
    void exitscreen()
    {
        draw(100);
        exit.kbdMove.run();
        if(IsKeyPressed(KEY_ESCAPE) || exit.stay())
        {
            kbdMove.reset();
            exiting=false;
        }
        if(exit.leave())
        {
            exiting=false;
            kbdMove.reset();
            strcpy(doing,"MainMenu");
        }

    }
    void  draw(int transparency=255)
    {
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;
        Color T_BLACK=BLACK; T_BLACK.a=transparency;

        BeginDrawing();
        ClearBackground(T_BLUE);
        DrawRectangle(0,screenHeight-100,screenWidth,5,T_BLACK);
        for(int i=0;i<MAX_PAGES;i++)
            buttons[i].draw(transparency);

        if(transparency!=255)
            exit.run();
        EndDrawing();
    }
    void selectable()
    {
        if(currentPage)
            return;
    }
    void changeButton(int newPage)
    {
        buttons[currentPage].normalColor.text=BLACK;
        buttons[currentPage].hoverColor.text=YELLOW;
        currentPage=newPage;
        buttons[newPage].normalColor.text=GREEN;
        buttons[newPage].hoverColor.text=GREEN;
    }
};
