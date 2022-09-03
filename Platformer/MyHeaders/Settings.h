#include "ExtendedRaylib.h"
extern char doing[21];

class Exit
{
    const float  X=screenWidth/8, Y=screenHeight/3.0f;
    const float WIDTH=screenWidth*6/8.0f,HEIGHT=screenHeight/3.0f;
    Button yes=Button("Yes",X+40,Y+HEIGHT-50,30,BLACK,RED);;
    Button no=Button("No",X+WIDTH-50-MeasureText("No",30),Y+HEIGHT-50,30,BLACK,GREEN);
public:
    KBD_Move kbdMove;
    Exit()
    {
        Button *all[2]={&yes,&no};
        kbdMove=KBD_Move(all,2,false,1);
    }
    void run()
    {const char sure[]="Are you sure you want to quit?";

        DrawRectangle(X,Y,WIDTH,HEIGHT,GRAY);
        DrawText(sure,X+WIDTH/2-MeasureText(sure,29)/2,Y+20,29,BLACK);
        yes.draw();
        no.draw();
    }
    bool leave()
    {
        if(yes.Lclicked())
        {
            kbdMove.reset();
            return true;
        }
        return yes.Lclicked() ;
    }
    bool stay()  {return  no.Lclicked() ;}
};
class MainMenu
{
     TxtAligned name=TxtAligned("Platformer",0,screenWidth,"center",screenHeight*2/10,screenHeight/6,"start",50,BLACK);
     ButtonAligned playOn=ButtonAligned("Continue",0,screenWidth,"center",screenHeight*4/10,screenHeight/6,"start",30,BLACK,GREEN);
     ButtonAligned lvlSelect=ButtonAligned("Level Select",0,screenWidth,"center",screenHeight*5/10,screenHeight/6,"start",30,BLACK,GREEN);
     ButtonAligned lvlEditor=ButtonAligned("Level Editor",0,screenWidth,"center",screenHeight*6/10,screenHeight/6,"start",30,BLACK,GREEN);
     ButtonAligned exit=ButtonAligned("Exit",0,screenWidth,"center",screenHeight*7/10,screenHeight/6,"start",30,BLACK,GREEN);
     int keyboardSelected = 0;
     KBD_Move kbdMove;
public:
    MainMenu()
    {
        Button *bList[4]={&playOn,&lvlSelect,&lvlEditor,&exit};
        kbdMove=KBD_Move(bList,4);
    }
    void run()
    {

        if(playOn.Lclicked())
            strcpy(doing,"Game");
        if(lvlSelect.Lclicked())
            strcpy(doing,"LevelSelect");
        if(lvlEditor.Lclicked())
            strcpy(doing,"LevelEditor");
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
