#include "ExtendedRaylib.h"

bool hideHitbox=false;
extern Texture2D ASSET_CHARACTER;
class Player
{
    #define smart_paint(xStart,size,xOrientation,yOrientation) {xStart ,0, size*xOrientation, size*yOrientation};
    /// can flip images
public:
    float xCoord,xVelocity=0,xMovement;
    const float MAX_X_VELOCITY_PER_SECOND=220;
    const float MAX_X_VELOCITY_PER_FRAME=MAX_X_VELOCITY_PER_SECOND/200;
    const float X_SECONDS_UNTIL_MAX=0.5;
    float const XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/200;
    float xFacing=1;
    int XDirection=0;

    float yCoord,yVelocity=0,yMovement;

    const float MAX_Y_VELOCITY_PER_SECOND=300;
    const float MAX_Y_VELOCITY_PER_FRAME=MAX_Y_VELOCITY_PER_SECOND/200;
    const float Y_SECONDS_UNTIL_MAX=0.2;
    const float YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/200;/// 200 = gametick
    const float JUMP_VELOCITY=2.2;
    bool isGrounded=false;

    int dashes=1;
    bool isdashing=false;
    long long dashTime=0;
    const float dashXVal=2;
    const float dashYVal=1.9;
    const float dashXDiagVal=1.50;
    const float dashYDiagVal=1.50;
    const long long MAX_DASH_TIME=200; /// 0.1 sec (in milisec)

    const float WJ_MAX_DISTANCE=20;
    const float WALL_CLING_MAX_DISTANCE=10;
    float closeLeft=999,closeRight=999;
    int WJ_direction=0;
    bool isClinging=false;

    Rectangle const hitbox={15,16,35,46};

    void move()
    {
        checkDash();
        if(!isdashing)
        {
            calcMoveOx();
            calcMoveOy();
            if(xFacing*xVelocity<0)
                xFacing=-xFacing;
        }

        addMovement();
    }

    void checkDash()
    {
        if(!isdashing)return;
        if(getTimeMS()-dashTime>MAX_DASH_TIME)
        {
            isdashing=false;
            xVelocity/=2;
            yVelocity/=2;
        }

    }
    void checkInput()
    {
        if(isdashing)return;
        if(isGrounded)
            dashes=1;
        if(IsKeyDown(KEY_SPACE))
        {

            if(WJ_direction && !isGrounded && IsKeyPressed(KEY_SPACE))
            {
                yVelocity=-JUMP_VELOCITY*0.8;
                if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
                    xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME;
                else
                    xVelocity=WJ_direction*MAX_X_VELOCITY_PER_FRAME*0.9;
            }
            if(isGrounded)
            {
                yVelocity=-JUMP_VELOCITY;
                isGrounded=false;
            }

        }
        if(IsKeyDown(KEY_X) && WJ_direction)
        {
            if(closeLeft<closeRight)
            {
                if(closeLeft<10)
                {
                    isClinging=true;
                    xVelocity=0;
                    yVelocity=0;
                    xMovement-=closeLeft;
                    xCoord-=closeLeft;
                }
                else
                    isClinging=false;
            }
            else
            {
                if(closeRight<10)
                {
                    isClinging=true;
                    xVelocity=0;
                    yVelocity=0;
                    xMovement+=closeRight;
                    xCoord+=closeRight;
                }
                else
                    isClinging=false;
            }
        }
        else
            isClinging=false;
        if(isClinging)
        {
            if(IsKeyDown(KEY_DOWN))
                yVelocity=MAX_Y_VELOCITY_PER_FRAME*0.6;
            if(IsKeyDown(KEY_UP))
                yVelocity=-MAX_Y_VELOCITY_PER_FRAME*0.4;
        }
        if(IsKeyDown(KEY_Z) && dashes>0)
        {
            dashes--;
            isdashing=true;
            dashTime=getTimeMS();

            if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
            {
                if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
                {
                    xVelocity=dashXDiagVal;
                    yVelocity=dashYDiagVal;
                }
                else
                    {yVelocity=dashYVal;xVelocity=0;}
            }
            else
                {xVelocity=dashXVal;yVelocity=0;}
            if(IsKeyDown(KEY_UP))
                yVelocity=-yVelocity;
            if(IsKeyDown(KEY_LEFT))
                xVelocity=-xVelocity;
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
    void calcMoveOy()
    {
        if(isClinging)return;
        yVelocity+=YVelocityGain;
        ///std::cout<<yVelocity<<'\n';
        if(yVelocity>MAX_Y_VELOCITY_PER_FRAME)
            yVelocity=MAX_Y_VELOCITY_PER_FRAME;
    }
    void calcMoveOx()
    {  /// TO DO : stuff so it aint terrible
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
            if(Misc::abs(xVelocity)>MAX_X_VELOCITY_PER_FRAME
               && Misc::abs(xVelocity)>Misc::abs(oldXVelocity))
                    xVelocity=oldXVelocity;
    }
    void addMovement()
    {
        xMovement+=xVelocity;
        xCoord+=xVelocity;
        yMovement+=yVelocity;
        yCoord+=yVelocity;
    }
    void reset();
    void presume()
    {
        isGrounded=false;
        WJ_direction=0;
        closeLeft=closeRight=999;
    }
    void tryWallJump(Directions box)
    {
        Directions PDir=getNewDir();
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
    Directions getOldDir()
    {
        Directions rez;
        rez.left=xCoord+hitbox.x-xMovement;
        rez.right=xCoord+hitbox.x-xMovement+hitbox.width;
        rez.up=yCoord+hitbox.y-yMovement;
        rez.down=yCoord+hitbox.y-yMovement+hitbox.height;
        return rez;
    }
    Directions getNewDir()
    {
        Directions rez;
        rez.left=xCoord+hitbox.x;
        rez.right=xCoord+hitbox.x+hitbox.width;
        rez.up=yCoord+hitbox.y;
        rez.down=yCoord+hitbox.y+hitbox.height;
        return rez;
    }
    Rectangle getPrevHitbox()
    {
        return {xCoord+hitbox.x-xMovement,yCoord+hitbox.y-yMovement,hitbox.width,hitbox.height};
    }
    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
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

}myPlayer;
