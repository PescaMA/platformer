/****************************************************************************
*
*        Settings - Exit, Win_Screen, Settings, MainMenu
*
****************************************************************************/
///To do: fix button spawning in oorner + maybe find solution for template

#ifndef Settings_H
#define Settings_H
#include "ExtendedRaylib.h"
namespace RayJump
{
class Exit /// Generalized exit screen to main menu. Needs a parent in which to be used.
{
    /// TO DO FIX THIS UNCOMPLETED MES
    ScaledRectangle Srect = ScaledRectangle(&screenInfo,15,85,20,80);
    TxtAligned sure=TxtAligned("Are you sure you want to quit?",&(Srect.rect),50,10,30,BLACK);
    ///DrawText(sure,X+WIDTH/2-MeasureText(sure,29)/2,Y+20,29,BLACK);
    ButtonAligned yes=ButtonAligned("Yes",&(Srect.rect),10,90,30,BLACK,RED);
    ButtonAligned no=ButtonAligned("No",&(Srect.rect),90,90,30,BLACK,GREEN);
    KBD_Btn_Move kbdMove;
public:
    enum States {off, starting, going, returning, exiting};
    States state = off;
    /// we can communicate with parent directly through state

    Exit();
    void align();
    template <class drawable>
    void run(drawable background)
    {
        /// defined here because otherwise template doesn't work.
        /// if we want to return to parent or main menu we close exit menu
        if(state == returning || state == exiting)
            state = off;

        /// After we signaled to parent we enter exit mode
        if(state == starting)
            state = going;

        /// Starting exit screen
        if(state == off && IsKeyPressed(KEY_ESCAPE))
            state=starting,kbdMove.reset();

        /// If we are in parent
        if(state == off)
            return;

        /// Returning to parent
        if(state == going && (IsKeyPressed(KEY_ESCAPE) || no.Lclicked()))
            state=returning;

        /// Going to Main
        if(state == going && yes.Lclicked())
        {
            state =exiting;
            strcpy(doing,"MainMenu");
        }
        align();

        kbdMove.run();
        BeginDrawing();

        background->draw_content(100); /// draw the parent's draw content as blurred background
        DrawRectangleRec(Srect.rect,GRAY);
        sure.draw();
        yes.draw();
        no.draw();

        EndDrawing();
    }
};

class Win_Screen
{
    TxtAligned message=TxtAligned("You won!",&screenInfo,50,30,80,GREEN);
    ButtonAligned restart=ButtonAligned("Restart",&screenInfo,50,85,35,BLACK,RED);
    KBD_Btn_Move kbdMove;
public:
    Win_Screen();
    template <class Game>
    void run(Game game)
    {
        align();
        kbdMove.run();
        if(restart.Lclicked())
            game->restart();
        draw();
    }
    void align();
    void draw();
};

class Settings
{
public:
    void static run();
};
class MainMenu
{
    TxtAligned name=TxtAligned("RayJump",&screenInfo,50,20,50,BLACK);
    ButtonAligned playOn=ButtonAligned("Continue",&screenInfo,50,40,30,BLACK,GREEN);
    ButtonAligned lvlSelect=ButtonAligned("Level Select",&screenInfo,50,50,30,BLACK,GREEN);
    ButtonAligned lvlEditor=ButtonAligned("Level Editor",&screenInfo,50,60,30,BLACK,GREEN);
    ButtonAligned exit=ButtonAligned("Exit",&screenInfo,50,70,30,BLACK,GREEN);
    int keyboardSelected = 0;
    KBD_Btn_Move kbdMove;
public:
    MainMenu();
    void run();
    void recalcuate();
private:
    void draw();
};
}
#endif // Settings_H
