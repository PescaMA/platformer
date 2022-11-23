#include "Objects.h"
#include "ExtendedRaylib.h"


class Game
{
    Exit exit;
    GameTickRate gameTick=GameTickRate(200);
public:
    bool exiting=false;
    void commands()
    {
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            hideHitbox=!hideHitbox;
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
            myPlayer.reset();
    }
    void run()
    {
        if(exiting)
            return exitscreen();
        commands();
        myPlayer.checkInput();

        for(int i=gameTick.getFrames()-1;i>=0;i--)
        {
            myPlayer.move();
            if(i%5==0)
            {
                myPlayer.presume();
                myMap.checkAllCollisions();
            }

        }

        draw();

        if(IsKeyPressed(KEY_ESCAPE))
        {
            exiting=true;
            gameTick.pause();
        }
    }
    void exitscreen()
    {
        draw(100);
        exit.kbdMove.run();
        if(IsKeyPressed(KEY_ESCAPE) || exit.stay())
        {
            exiting=false;
            exit.kbdMove.reset();
        }

        if(exit.leave())
        {
            myPlayer.reset();
            exiting=false;
            strcpy(doing,"MainMenu");
            exit.kbdMove.reset();
        }

    }
    void  draw(int transparency=255)
    {
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;

        BeginDrawing();
        ClearBackground(T_BLUE);
        myMap.drawMap(transparency);
        myPlayer.draw(transparency);
        if(transparency!=255)
            exit.run();
        EndDrawing();
    }
};
