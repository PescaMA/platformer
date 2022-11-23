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

    Rectangle const hitbox={15,16,35,46};


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
    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
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
    void presume()
    {
        isGrounded=false;
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
        {
            dashes=1;
            if(IsKeyDown(KEY_SPACE))
            {
                yVelocity=-JUMP_VELOCITY;
                isGrounded=false;
            }
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
        yVelocity+=YVelocityGain;
        ///std::cout<<yVelocity<<'\n';
        if(yVelocity>MAX_Y_VELOCITY_PER_FRAME)
            yVelocity=MAX_Y_VELOCITY_PER_FRAME;
    }
    void calcMoveOx()
    {  /// TO DO : stuff so it aint terrible
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
