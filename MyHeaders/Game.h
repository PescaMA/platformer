#include "Objects.h"
#include "ExtendedRaylib.h"


class Game
{
    Exit exit;
    Win_Screen winScreen;
    GameTickRate gameTick=GameTickRate(200);
public:
    void commands()
    {
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            hideHitbox=!hideHitbox;
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
            myPlayer.reset();
    }
    void run()
    {
        if(myFinish.won==true)
        {
            winScreen.run(this);
            return;
        }

        if(exit.state == Exit::States::starting)
            gameTick.pause();
        exit.run(this);
        if(exit.state != Exit::States::off)
            return;

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
    }
    void  draw()
    {
        BeginDrawing();
            draw_content(255);
        EndDrawing();
    }
    void draw_content(int transparency)
    {
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;
        ClearBackground(T_BLUE);
        myMap.drawMap(transparency);
        myPlayer.draw(transparency);
    }
    void restart()
    {
        myFinish.won = false;
        myPlayer.reset();
        myMap.restartMap();
    }
};
