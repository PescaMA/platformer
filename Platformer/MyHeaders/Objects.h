#ifndef OBJECTS
#define OBJECTS

#include "ExtendedRaylib.h"
extern Texture2D ASSET_BLOCKS;
extern Texture2D ASSET_CHARACTER;
class Player
{
    #define smart_paint(xStart,size,xOrientation,yOrientation) {xStart ,0, size*xOrientation, size*yOrientation};
public:
    float xCoord=40,xVelocity=0,xMovement;
    const float MAX_X_VELOCITY_PER_SECOND=180;
    const float X_SECONDS_UNTIL_MAX=2;

    float yCoord=250,yVelocity=0,yMovement;
    const float MAX_Y_VELOCITY_PER_SECOND=10;
    const float Y_SECONDS_UNTIL_MAX=0.2;

    float xFacing=1;
    long long prevTime=0;

    Rectangle const hitbox={25,25,20,39};
    bool hideHitbox=false;
    void pause()
    {

    }
    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
    void move()
    {

        if(!prevTime)
        {
            prevTime=getTimeMCS();
            return;
        }
        long long currentTime=getTimeMCS();
        ///moveOx(currentTime);
        moveOy(currentTime);
        if(xFacing*xVelocity<0)
            xFacing=-xFacing;
        prevTime=currentTime;

    }
    void moveOy(long long currentTime)
    { /// TO DO: FIX THIS MESS
        float timeDiff=(float)(1LL*currentTime-prevTime);
        const float MCS_MAX_VELOCITY=MAX_Y_VELOCITY_PER_SECOND/1000000;
        const float MCS_GAINED_VELOCITY=MCS_MAX_VELOCITY/1000000/Y_SECONDS_UNTIL_MAX;
        if(IsKeyPressed(KEY_SPACE))
            yVelocity=-0.00009;
         if(xVelocity+timeDiff*MCS_GAINED_VELOCITY>MCS_MAX_VELOCITY)
         {
             float MCS_increasing=(MCS_MAX_VELOCITY-yVelocity)/MCS_GAINED_VELOCITY;
             yMovement=MCS_increasing*yVelocity
             + MCS_increasing * (MCS_GAINED_VELOCITY + 1) * MCS_increasing / 2;
             yMovement+=(timeDiff-MCS_increasing) * MCS_MAX_VELOCITY;
             yVelocity=MCS_MAX_VELOCITY;
             std::cout<<"MAX";
         }
         else
         {
             yMovement=timeDiff*yVelocity+((timeDiff+1)*MCS_GAINED_VELOCITY*timeDiff/2);
             yVelocity+=timeDiff*MCS_GAINED_VELOCITY;
         }
        yCoord+=yMovement;

        std::cout<<yVelocity<<'\n';
    }
    void OldmoveOy(long long currentTime)
    {
        float timeDiff=(float)(1LL*currentTime-prevTime)/1000000;
        float max_velocity=MAX_Y_VELOCITY_PER_SECOND*timeDiff;
        float increase=max_velocity/Y_SECONDS_UNTIL_MAX*timeDiff;

        yVelocity+=increase;

        if(IsKeyPressed(KEY_SPACE))
            yVelocity=-2;

        if(yVelocity>max_velocity)
            yVelocity=max_velocity;

        yCoord+=yVelocity;
        std::cout<<yVelocity<<'\n';
    }
    void moveOx(long long currentTime)
    {
        float timeDiff=(float)(1LL*currentTime-prevTime)/1000000;
        float max_velocity=MAX_X_VELOCITY_PER_SECOND*timeDiff;
        float increase=max_velocity/X_SECONDS_UNTIL_MAX*timeDiff;

        bool isNotMoving=true;

        if(IsKeyDown(KEY_RIGHT) && xVelocity>=0)
        {
            xVelocity+=increase;
            isNotMoving=false;
        }
        if(IsKeyDown(KEY_LEFT)&& xVelocity<=0)
        {
            xVelocity-=increase;
            isNotMoving=false;
        }
        if(isNotMoving)
        {
            if(xVelocity>increase*5)
                xVelocity-=increase*5;
            else
                if(xVelocity<-increase*5)
                    xVelocity+=increase*5;
                else
                    xVelocity=0;
        }
        if(xVelocity>max_velocity)
            xVelocity=max_velocity;
        if(xVelocity< -max_velocity)
            xVelocity=-max_velocity;
        xCoord+=xVelocity;


    }
    void reset()
    {
        xCoord=40;yCoord=250;
        xVelocity=yVelocity=0;
        prevTime=0;
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
class Object
{
    #define up lastY+myPlayer.hitbox.y+myPlayer.hitbox.height <= hitbox.y+y
    #define down lastY+myPlayer.hitbox.y >= y+hitbox.y+hitbox.height
    #define left lastX+myPlayer.hitbox.x+myPlayer.hitbox.width <= x+hitbox.x
    #define right lastX+myPlayer.hitbox.x >= x+hitbox.x+hitbox.width
    public:
    Texture2D image;
    int imageX;
    Rectangle hitbox;
    bool hideHitbox=false;
    Object(){}
    Rectangle getHitbox(int x,int y)
    {
        return {x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
    }
    Object(Texture2D image,int imageX,Rectangle hitbox)
    {
        this->image=image;
        this->imageX=imageX;
        this->hitbox=hitbox;
    }
    void draw(int x,int y,int transparency=255)
    {
        Color white=WHITE;
        white.a=transparency;
        Rectangle imageBoundaries={(float)imageX,0,32,32};
        Vector2 pos={(float)x,(float)y};
        DrawTextureRec(image,imageBoundaries,pos,white);
        if(!hideHitbox)
        {
            Rectangle objectRect={x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
            DrawRectangleLinesEx(objectRect,1,WHITE);
        }

    }
    void movePlayer(char const c[10],int x,int y)
    {
        if(!strcmp(c,"up"))
            myPlayer.yCoord=y+hitbox.y-myPlayer.hitbox.y-myPlayer.hitbox.height;
        if(!strcmp(c,"left"))
            myPlayer.xCoord=x+hitbox.x-myPlayer.hitbox.x-myPlayer.hitbox.width;
        if(!strcmp(c,"down"))
            myPlayer.yCoord=y+hitbox.y+hitbox.height-myPlayer.hitbox.y;
        if(!strcmp(c,"right"))
            myPlayer.xCoord=x+hitbox.x+hitbox.width-myPlayer.hitbox.x;


    }
    void virtual effect(int x,int y){}
    void virtual permanentMovement(){}
    void collision(int x,int y)
    {
        if(CheckCollisionRecs(myPlayer.getHitbox(),this->getHitbox(x,y)))
            effect(x,y);
    }

};
class Block : public Object
{
    public:
    Block(){}
    Block(Texture2D image,int imageX,Rectangle hitbox):Object(image,imageX,hitbox){}
    void effect(int x,int y)
    {
        int lastX=myPlayer.xCoord-myPlayer.xMovement;
        int lastY=myPlayer.yCoord-myPlayer.yMovement;
        if(up || down)
        {
            myPlayer.yVelocity=0;
            if(up)
                movePlayer("up",x,y);
            else
                movePlayer("down",x,y);
        }
        if(left || right)
        {
            myPlayer.xVelocity=0;
            if(left)
                movePlayer("left",x,y);
            else
                movePlayer("right",x,y);
        }
    }
};
Block Block1,Block2,Block3,Block4,Block5;

#endif // OBJECTS
