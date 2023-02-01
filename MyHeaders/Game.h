#include "Objects.h"
#include "ExtendedRaylib.h"
#include "RayJumpHeadears.h"

    void RayJump::Game::commands()
    {
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            RayJump::hideHitbox=!RayJump::hideHitbox;
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
            myPlayer.reset();
    }
    void RayJump::Game::run()
    {
        if(RayJump::myFinish.won==true)
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
                RayJump::myMap.checkAllCollisions();
            }
        }
        draw();
    }
    void RayJump::Game::draw()
    {
        BeginDrawing();
            draw_content(255);
        EndDrawing();
    }
    void RayJump::Game::draw_content(int transparency)
    {
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;
        ClearBackground(T_BLUE);
        RayJump::myMap.drawMap(transparency);
        myPlayer.draw(transparency);
    }
    void RayJump::Game::restart()
    {
        RayJump::myFinish.won = false;
        myPlayer.reset();
        RayJump::myMap.restartMap();
    }
