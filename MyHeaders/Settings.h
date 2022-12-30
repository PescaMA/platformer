#include "ExtendedRaylib.h"
extern char doing[21];

class Exit
{
    const float  X=screenWidth/8, Y=screenHeight/3.0f;
    const float WIDTH=screenWidth*6/8.0f,HEIGHT=screenHeight/3.0f;
    Button yes=Button("Yes",X+40,Y+HEIGHT-50,30,BLACK,RED);;
    Button no=Button("No",X+WIDTH-50-MeasureText("No",30),Y+HEIGHT-50,30,BLACK,GREEN);


public:
    KBD_Btn_Move kbdMove;
    enum States {off, starting, going, returning, exiting};
    States state = off;
    Exit()
    {
        Button *all[]={&yes,&no};
        int n=sizeof(all)/sizeof(all[0]);
        kbdMove=KBD_Btn_Move(all,2,false,1);
    }
    void run()
    {const char sure[]="Are you sure you want to quit?";

        DrawRectangle(X,Y,WIDTH,HEIGHT,GRAY);
        DrawText(sure,X+WIDTH/2-MeasureText(sure,29)/2,Y+20,29,BLACK);
        yes.draw();
        no.draw();
    }
    template <class drawable>
    void run(drawable background)
    {
        if(state == returning || state == exiting)
            state = off;
        if(state == starting)
            state = going;
        if(state == off && IsKeyPressed(KEY_ESCAPE))
            state=starting,kbdMove.reset();
        if(state == off)
            return;
        if(state == going && (IsKeyPressed(KEY_ESCAPE) || no.Lclicked()))
            state=returning;

        if(state == going && yes.Lclicked())
        {
            state =exiting;
            strcpy(doing,"MainMenu");
        }


        kbdMove.run();
        const char sure[]="Are you sure you want to quit?";
        BeginDrawing();
        background->draw_content(100);


        DrawRectangle(X,Y,WIDTH,HEIGHT,GRAY);
        DrawText(sure,X+WIDTH/2-MeasureText(sure,29)/2,Y+20,29,BLACK);
        yes.draw();
        no.draw();
        EndDrawing();
    }
    bool leave()
    {
        return yes.Lclicked();
    }
    bool stay()  {return  no.Lclicked() ;}
};
class Win_Screen
{
    TxtAligned message=TxtAligned("You won!",ERay::getWindowSize(),50,30,80,GREEN);
    ButtonAligned restart=ButtonAligned("Restart",ERay::getWindowSize(),75,85,35,BLACK,RED);
    KBD_Btn_Move kbdMove;
    public:
    Win_Screen()
    {
        Button *all[]={&restart};
        int n=sizeof(all)/sizeof(all[0]);
        kbdMove=KBD_Btn_Move(all,n,false,0);
    }
    template <class Game>
    void run(Game game)
    {
        kbdMove.run();
        if(restart.Lclicked())
            game->restart();
        draw();
    }
    void draw()
    {
        BeginDrawing();
        ClearBackground(BLUE);
        message.draw();
        message.underline();
        restart.draw();
        EndDrawing();
    }
};
class MainMenu
{
     TxtAligned name=TxtAligned("RayJump",ERay::getWindowSize(),50,20,50,BLACK);
     ButtonAligned playOn=ButtonAligned("Continue",ERay::getWindowSize(),50,40,30,BLACK,GREEN);
     ButtonAligned lvlSelect=ButtonAligned("Level Select",ERay::getWindowSize(),50,50,30,BLACK,GREEN);
     ButtonAligned lvlEditor=ButtonAligned("Level Editor",ERay::getWindowSize(),50,60,30,BLACK,GREEN);
     ButtonAligned exit=ButtonAligned("Exit",ERay::getWindowSize(),50,70,30,BLACK,GREEN);
     int keyboardSelected = 0;
     KBD_Btn_Move kbdMove;
public:
    MainMenu()
    {
        Button *bList[]={&playOn,&lvlSelect,&lvlEditor,&exit};
        int n=sizeof(bList)/sizeof(bList[0]);
        kbdMove=KBD_Btn_Move(bList,n);
    }
    void run()
    {
        if(playOn.Lclicked())
        {
            strcpy(doing,"Game");
            Loader::loadMap("Levels/Lvl_1.txt");
        }
        if(lvlSelect.Lclicked())
            strcpy(doing,"LevelSelect");
        if(lvlEditor.Lclicked())
        {
            strcpy(doing,"LevelEditor");
            Loader::loadMap("Levels/Lvl_Editor.txt");
        }

        if(exit.Lclicked())
            strcpy(doing,"Exiting");
        if(strcmp(doing,"MainMenu"))
            kbdMove.reset();
        draw();
        kbdMove.run();
    }
private:
    void draw()
    {
        BeginDrawing();
        ClearBackground(YELLOW);
        name.draw(true);
        playOn.draw();
        lvlSelect.draw();
        lvlEditor.draw();
        exit.draw();
        EndDrawing();
    }
};
class Settings
{
public:
    void static run()
    {

    }
};
