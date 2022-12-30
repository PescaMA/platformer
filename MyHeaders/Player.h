#include "ExtendedRaylib.h"

#include <cmath>
bool hideHitbox=false;
extern Texture2D ASSET_CHARACTER;
class Player
{
/**********************************
*
*
* V a r i a b l e s:
*
*
***********************************/

public:
    /**  HORIZONTAL INFORMATION  **/
    float xCoord,xVelocity=0,xMovement;
    const float MAX_X_VELOCITY_PER_SECOND=220;
    const float MAX_X_VELOCITY_PER_FRAME=MAX_X_VELOCITY_PER_SECOND/200;
    const float X_SECONDS_UNTIL_MAX=0.5;
    float const XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/200;
    int XDirection=0;

    /**  VERTICAL INFORMATION  **/
    float yCoord,yVelocity=0,yMovement;
    const float MAX_Y_VELOCITY_PER_SECOND=300;
    const float MAX_Y_VELOCITY_PER_FRAME=MAX_Y_VELOCITY_PER_SECOND/200;
    const float Y_SECONDS_UNTIL_MAX=0.2;
    const float YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/200;/// 200 = gametick
    bool isGrounded=false;

    /**  JUMPING  **/
    const float JUMP_HEIGHT=90;
    /*Fizica: (https://fenomas.com/2016/07/game-jumping-physics/)
    PE = mgh            // Potential energy at peak of jump
    KE = mv²/2          // Kinetic energy at initial jump velocity v
    mgh = mv²/2         // set them equal and solve for v
    v² = 2gh => v=sqrt(2gh)*/
    const float JUMP_VELOCITY=sqrt(JUMP_HEIGHT*YVelocityGain*2);

    /**  DASHING  **/
    int dashes=1;
    bool isdashing=false;
    long long dashTime=0;
    const float dashXVal=2;
    const float dashYVal=1.9;
    const float dashXDiagVal=1.50;
    const float dashYDiagVal=1.50;
    const long long MAX_DASH_TIME=200; /// 0.1 sec (in milisec)

    /**  WALL JUMP/CLING-ING  **/
    const float WJ_MAX_DISTANCE=20; /// wall jump
    int WJ_direction=0;
    const float WC_MAX_DISTANCE=10; /// wall cling
    bool isClinging=false;
    float closeLeft=999,closeRight=999; /// distance to closest block

    /**  OTHER USEFUL INFO  **/
    float xFacing=1; /// for sprite rotation
    Rectangle const hitbox={15,16,35,46};

/**********************************
*
*
* Input handling:
*
*
***********************************/

    void checkInput()
    {
        if(isdashing)return;
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
    void checkJump()
    {
        if(!IsKeyDown(KEY_SPACE))
            return;
        if(isGrounded)
        {
            yVelocity=-JUMP_VELOCITY;
            isGrounded=false;
            return;
        }
        if(WJ_direction && IsKeyPressed(KEY_SPACE))
        { /// wall jump
            yVelocity=-JUMP_VELOCITY*0.8;

            /// horizontal distance is bigger if a directional key is pressed
            /// because that's how it works in Celeste

            if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
                xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME;
            else
                xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME*0.9;
        }
    }
    void checkCling()
    {
        if(!IsKeyDown(KEY_X) || std::min(closeLeft,closeRight)>WC_MAX_DISTANCE)
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
    void horizontalCling()
    { /// moves player closer to wall
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
    void checkVerticalCling()
    {
        if(IsKeyDown(KEY_UP))
            yVelocity=-MAX_Y_VELOCITY_PER_FRAME*0.4;
        if(IsKeyDown(KEY_DOWN))
            yVelocity=MAX_Y_VELOCITY_PER_FRAME*0.6;
        /// moving down is faster than up but slower then falling
    }
    void checkDash()
    {
        if(!IsKeyDown(KEY_Z) || !dashes)
            return;
        dashes--;
        isdashing=true;
        dashTime=getTimeMS();

        float vert=0,horiz=0;

        if(IsKeyDown(KEY_DOWN))vert=1;
        if(IsKeyDown(KEY_UP))vert=-1;

        if(IsKeyDown(KEY_LEFT))horiz=-1;
        if(IsKeyDown(KEY_RIGHT))horiz=1;

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
    void move()
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

    void exitDash()
    {
        if(!isdashing)return;
        if(getTimeMS()-dashTime>MAX_DASH_TIME)
        {
            isdashing=false;
            xVelocity/=2;
            yVelocity/=2;
        }

    }

    void newMovement()
    {
        xMovement=yMovement=0;
    }
    void calcMoveOy()
    {
        if(isClinging)return;
        yVelocity+=YVelocityGain;
        ///std::cout<<yVelocity<<'\n';
        if(yVelocity>MAX_Y_VELOCITY_PER_FRAME)
            yVelocity=MAX_Y_VELOCITY_PER_FRAME;
    }
    void calcMoveOx()
    {
        if(isClinging)return;
        float oldXVelocity=xVelocity;
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
        if(isGrounded)
        {
            if(xVelocity>MAX_X_VELOCITY_PER_FRAME)
                xVelocity=std::max(MAX_X_VELOCITY_PER_FRAME,
                                   xVelocity*0.99f);
            if(xVelocity< -MAX_X_VELOCITY_PER_FRAME)
                xVelocity=std::min(-MAX_X_VELOCITY_PER_FRAME,
                                   xVelocity*0.99f);
        }
        else
            if(ERay::abs(xVelocity)>MAX_X_VELOCITY_PER_FRAME
               && ERay::abs(xVelocity)>ERay::abs(oldXVelocity))
                    xVelocity=oldXVelocity;
    }
    void addMovement()
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
    void reset();
    void presume()
    {
        isGrounded=false;
        WJ_direction=0;
        closeLeft=closeRight=999;
    }

    Rectangle getPrevHitbox()
    {
        return {xCoord+hitbox.x-xMovement,yCoord+hitbox.y-yMovement,hitbox.width,hitbox.height};
    }
    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
    Directions getPrevDir()
    {
        Directions rez;
        rez.left=xCoord+hitbox.x-xMovement;
        rez.right=xCoord+hitbox.x-xMovement+hitbox.width;
        rez.up=yCoord+hitbox.y-yMovement;
        rez.down=yCoord+hitbox.y-yMovement+hitbox.height;
        return rez;
    }
    Directions getDir()
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
    void tryWallJump(Directions box)
    {
        Directions PDir=getDir();
        if(PDir.down-3<=box.up || box.down<=PDir.up-5)
            return;
        if(box.left-PDir.right<=WJ_MAX_DISTANCE && box.left-PDir.right>=-3)
        {
            WJ_direction=-1;
            closeRight=std::min(closeRight,std::max(box.left-PDir.right,0.0f));
        }

        if(PDir.left-box.right<=WJ_MAX_DISTANCE && PDir.left-box.right>=-3)
        {
            WJ_direction=1;
            closeLeft=std::min(closeLeft,std::max(PDir.left-box.right,0.0f));
        }
    }
    void draw(int transparency)
    {
        Color white=WHITE;
        white.a=transparency;
        Rectangle rect=ERay::smart_paint(0,64,xFacing,1);
        Vector2 position={xCoord,yCoord};
        DrawTextureRec(ASSET_CHARACTER,rect,position,white);
        if(!hideHitbox)
            DrawRectangleLinesEx(getHitbox(),1,GREEN);
    }

}myPlayer;
