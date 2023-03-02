#include "../MyHeaders/RayJumpHeadears.h"
/**********************************
*
*
* Input handling:
*
*
***********************************/

/// To do : when holding wall, dashing up fails.
void RayJump::Player::checkInput()
{
    if(isGrounded)
        dashes=1;

    checkJump();
    checkCling();
    checkDash();

    XDirection=0;
    if(IsKeyDown(KEY_RIGHT) && xVelocity>=0)
        XDirection=1;
    if(IsKeyDown(KEY_LEFT)&& xVelocity<=0)
        XDirection=-1;
}
void RayJump::Player::checkJump()
{
    if(!IsKeyDown(KEY_SPACE))
        return;
    if(isGrounded)
    {
        yVelocity=-JUMP_VELOCITY;
        isGrounded=false;
        isdashing = false;
        return;
    }
    if(WJ_direction && IsKeyPressed(KEY_SPACE))
    {
        /// wall jump
        if(isClinging)
            jumpTime = getTimeMS();

        /// horizontal distance is bigger if a directional key is pressed
        /// because that's how it works in Celeste
        if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
        {
            xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME*1.2;
            yVelocity=-JUMP_VELOCITY*0.6;
        }

        else
        {
            xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME*0.9;
            yVelocity=-JUMP_VELOCITY*0.75;
        }
        isdashing = false;

    }
}
void RayJump::Player::checkCling()
{
    if(!IsKeyDown(KEY_X)
        || std::min(closeLeft,closeRight)>WC_MAX_DISTANCE
        || getTimeMS() - jumpTime < MAX_JUMP_TIME)
    {
        isClinging=false;
        return;
    }
    isClinging=true;
    xVelocity=0;
    yVelocity=0;

    horizontalCling();   /// (forcefully) moves player closer to wall
    checkVerticalCling();///(voluntarily) moves player up / down wall
}
void RayJump::Player::horizontalCling()
{
    /// moves player closer to wall
    if(closeLeft<closeRight)
    {
        xMovement-=closeLeft;
        xCoord-=closeLeft;
    }
    else
    {
        xMovement+=closeRight;
        xCoord+=closeRight;
    }
}
void RayJump::Player::checkVerticalCling()
{
    if(IsKeyDown(KEY_UP))
        yVelocity=-MAX_Y_VELOCITY_PER_FRAME*0.5;
    if(IsKeyDown(KEY_DOWN))
        yVelocity=MAX_Y_VELOCITY_PER_FRAME*0.7;
    /// moving down is faster than up but slower then falling
}
void RayJump::Player::checkDash()
{
    if(!IsKeyPressed(KEY_Z) || !dashes)
        return;
    dashes--;
    isdashing=true;
    dashTime=getTimeMS();

    float vert=0,horiz=0;

    if(IsKeyDown(KEY_DOWN))
        vert=1;
    if(IsKeyDown(KEY_UP))
        vert=-1;

    if(IsKeyDown(KEY_LEFT))
        horiz=-1;
    if(IsKeyDown(KEY_RIGHT))
        horiz=1;

    if(!vert && !horiz) /// no directional input => dash in facing direction
        horiz=xFacing;
    if(vert && horiz) /// diagonal dash
    {
        xVelocity=dashXDiagVal * horiz;
        yVelocity=dashYDiagVal * vert;
    }
    else
    {
        xVelocity=dashXVal * horiz;
        yVelocity=dashYVal * vert;
    }
}

/**********************************
*
*
* Movement calculations:
*
*
***********************************/
void RayJump::Player::move()
{
    exitDash();

    if(!isdashing)
    {
        calcMoveOx();
        calcMoveOy();
        if(xFacing*xVelocity<0)
            xFacing=-xFacing;
    }

    addMovement();
}

void RayJump::Player::exitDash()
{
    if(!isdashing)
        return;
    if(getTimeMS()-dashTime>MAX_DASH_TIME)
    {
        isdashing=false;
        xVelocity*=0.8;
        yVelocity*=0.8;
    }

}

void RayJump::Player::newMovement()
{
    xMovement=yMovement=0;
}
void RayJump::Player::calcMoveOy()
{
    if(isClinging)
        return;
    yVelocity+=YVelocityGain;
    if(yVelocity>MAX_Y_VELOCITY_PER_FRAME)
        yVelocity=MAX_Y_VELOCITY_PER_FRAME;
}
void RayJump::Player::calcMoveOx()
{/// weird func?
    if(isClinging)
        return;
    float oldXVelocity=xVelocity;
    xVelocity+=XVelocityGain*XDirection;
    if(ERay::abs(xVelocity)>MAX_X_VELOCITY_PER_FRAME)
        xVelocity=oldXVelocity*0.999;
    if(!XDirection)
    {
        if(xVelocity>XVelocityGain*3)
            xVelocity-=XVelocityGain*3;
        else if(xVelocity<-XVelocityGain*3)
            xVelocity+=XVelocityGain*3;
        else
            xVelocity=0;
    }
    if(isGrounded)
    {
        if(xVelocity>MAX_X_VELOCITY_PER_FRAME)
            xVelocity=std::max(MAX_X_VELOCITY_PER_FRAME,
                               xVelocity*0.98f);
        if(xVelocity< -MAX_X_VELOCITY_PER_FRAME)
            xVelocity=std::min(-MAX_X_VELOCITY_PER_FRAME,
                               xVelocity*0.98f);
    }
}
void RayJump::Player::addMovement()
{
    xMovement+=xVelocity;
    xCoord+=xVelocity;
    yMovement+=yVelocity;
    yCoord+=yVelocity;
}
/**********************************
*
*
* Information setters and getters
*
*
***********************************/
void RayJump::Player::reset()
{
    myStart.specialEffect();
    xVelocity=yVelocity=0;
    xFacing=1;
    XDirection=0;
    isdashing=false;
    dashes = 1;
    presume();
}


void RayJump::Player::presume()
{
    isGrounded=false;
    WJ_direction=0;
    closeLeft=closeRight=999;
}

Rectangle RayJump::Player::getPrevHitbox()
{
    return {xCoord+hitbox.x-xMovement,yCoord+hitbox.y-yMovement,hitbox.width,hitbox.height};
}
Rectangle RayJump::Player::getHitbox()
{
    return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
}
Directions RayJump::Player::getPrevDir()
{
    Directions rez;
    rez.left=xCoord+hitbox.x-xMovement;
    rez.right=xCoord+hitbox.x-xMovement+hitbox.width;
    rez.up=yCoord+hitbox.y-yMovement;
    rez.down=yCoord+hitbox.y-yMovement+hitbox.height;
    return rez;
}
Directions RayJump::Player::getDir()
{
    Directions rez;
    rez.left=xCoord+hitbox.x;
    rez.right=xCoord+hitbox.x+hitbox.width;
    rez.up=yCoord+hitbox.y;
    rez.down=yCoord+hitbox.y+hitbox.height;
    return rez;
}

/**********************************
*
*
* Miscellaneous:
*
*
***********************************/
void RayJump::Player::tryWallJump(Directions box)
{
    Directions PDir=getDir();
    if(PDir.down-10<=box.up || box.down<=PDir.up+1)
        return; /// if we arent close to the box on the y axis
    if(box.left-PDir.right<=WJ_MAX_DISTANCE && box.left-PDir.right>=-1)
    {
        WJ_direction=-1;
        closeRight=std::min(closeRight,std::max(box.left-PDir.right,0.0f));
    }

    if(PDir.left-box.right<=WJ_MAX_DISTANCE && PDir.left-box.right>=-1)
    {
        WJ_direction=1;
        closeLeft=std::min(closeLeft,std::max(PDir.left-box.right,0.0f));
    }
}
void RayJump::Player::draw(int transparency)
{
    Color white=WHITE;
    white.a=transparency;
    Rectangle rect=ERay::smart_paint(0,64,xFacing,1);
    Vector2 position= {xCoord,yCoord};
    DrawTextureRec(ASSET_CHARACTER,rect,position,white);
    if(!hideHitbox)
        DrawRectangleLinesEx(getHitbox(),1,GREEN);
}
