
/*********************************************
*
*               E X I T
*
**********************************************/
#include "../MyHeaders/RayJumpHeadears.h"
RayJump::Exit::Exit()
{
    Button *all[]= {&yes,&no};
    int n=sizeof(all)/sizeof(all[0]);
    kbdMove=KBD_Btn_Move(all,n,false,1);
}
void RayJump::Exit::align()
{
    Srect.update();
    sure.align();
}


/*********************************************
*
*               WIN SCREEN
*
**********************************************/
RayJump::Win_Screen::Win_Screen()
{
    Button *all[]= {&restart};
    int n=sizeof(all)/sizeof(all[0]);
    kbdMove=KBD_Btn_Move(all,n,false,0);
}

void RayJump::Win_Screen::align()
{
    message.align();
}
void RayJump::Win_Screen::draw()
{
    BeginDrawing();
    ClearBackground(BLUE);
    message.draw();
    message.underline();
    restart.draw();
    EndDrawing();
}

/*********************************************
*
*                 MAIN MENU
*
**********************************************/
RayJump::MainMenu::MainMenu()
{
    Button *bList[]= {&playOn,&lvlSelect,&lvlEditor,&exit};
    int n=sizeof(bList)/sizeof(bList[0]);
    kbdMove=KBD_Btn_Move(bList,n);
}
void RayJump::MainMenu::run()
{
    if(playOn.Lclicked())
    {
        strcpy(doing,"Game");
        Loader::loadMap("Levels/Lvl_Editor.txt");
        ///Loader::loadMap("Levels/Lvl_1.txt");
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
    recalcuate();

    /// if we want to leave
    if(strcmp(doing,"MainMenu"))
        kbdMove.reset();

    draw();
    kbdMove.run();
}
void RayJump::MainMenu::recalcuate()
{
    name.align();
}
void RayJump::MainMenu::draw()
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

/*********************************************
*
*                SETTINGS
*
**********************************************/
/// phenomenal
void RayJump::Settings::run() {}
