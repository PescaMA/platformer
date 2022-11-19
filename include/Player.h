#pragma once

#include "ExtendedRaylib.h"

bool hideHitbox=false;
extern Texture2D ASSET_CHARACTER;
class Player
{
    #define smart_paint(xStart,size,xOrientation,yOrientation) {xStart ,0, size*xOrientation, size*yOrientation};
    /// can flip images
public:
    float xCoord,xVelocity=0,xMovement;
    const float MAX_X_VELOCITY_PER_FRAME=0.8;
    const float X_SECONDS_UNTIL_MAX=1;
    float const XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/200; /// 0.0004 now
    float xFacing=1;
    int XDirection=0;

    float yCoord,yVelocity=0,yMovement;
    const float MAX_Y_VELOCITY_PER_FRAME=2;
    const float Y_SECONDS_UNTIL_MAX=1.5;
    float const YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/200;/// 200 = gametick
    bool isGrounded=false;

    Rectangle const hitbox={15,16,35,46};

    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
    Directions getOldDir()
    {
        Directions rez;
        rez.left=xCoord+hitbox.x-xMovement;
        rez.right=xCoord+hitbox.x-xMovement+hitbox.width;
        rez.up=yCoord+hitbox.y-yMovement;
        rez.down=yCoord+hitbox.y-yMovement+hitbox.height;
        return rez;
    }
    Rectangle getPrevHitbox()
    {
        return {xCoord+hitbox.x-xMovement,yCoord+hitbox.y-yMovement,hitbox.width,hitbox.height};
    }
    void move()
    {
        moveOx();
        moveOy();
        if(xFacing*xVelocity<0)
            xFacing=-xFacing;
    }
    void checkInput()
    {
        if(isGrounded && IsKeyDown(KEY_SPACE))
        {
            yVelocity=-YVelocityGain*175; /// 0.7
            isGrounded=false;
        }

        XDirection=0;
        if(IsKeyDown(KEY_RIGHT) && xVelocity>=0)
            XDirection=1;
        if(IsKeyDown(KEY_LEFT)&& xVelocity<=0)
            XDirection=-1;
    }
    void newMovement()
    {
        xMovement=yMovement=0;
    }
    void moveOy()
    {

        yVelocity+=YVelocityGain;

        if(yVelocity>MAX_Y_VELOCITY_PER_FRAME)
            yVelocity=MAX_Y_VELOCITY_PER_FRAME;
        yMovement+=yVelocity;
        yCoord+=yVelocity;
    }
    void moveOx()
    {
        xVelocity+=XVelocityGain*XDirection;
        if(!XDirection)
        {
            if(xVelocity>XVelocityGain*5)
                xVelocity-=XVelocityGain*5;
            else
                if(xVelocity<-XVelocityGain*5)
                    xVelocity+=XVelocityGain*5;
                else
                    xVelocity=0;
        }
        if(xVelocity>MAX_X_VELOCITY_PER_FRAME)
            xVelocity=MAX_X_VELOCITY_PER_FRAME;
        if(xVelocity< -MAX_X_VELOCITY_PER_FRAME)
            xVelocity=-MAX_X_VELOCITY_PER_FRAME;

        xMovement+=xVelocity;
        xCoord+=xVelocity;

    }
    void reset();
    void draw(int transparency)
    {
        Color white=WHITE;
        white.a=transparency;
        Rectangle rect=smart_paint(0,64,xFacing,1);
        Vector2 position={xCoord,yCoord};
        DrawTextureRec(ASSET_CHARACTER,rect,position,white);
        if(!hideHitbox)
            DrawRectangleLinesEx(getHitbox(),1,GREEN);
    }

} myPlayer;
