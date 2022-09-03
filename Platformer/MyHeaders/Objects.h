#ifndef OBJECTS
#define OBJECTS

#include "ExtendedRaylib.h"
extern Texture2D ASSET_BLOCKS;
extern Texture2D ASSET_CHARACTER;
class Player
{
    #define smart_paint(xStart,size,xOrientation,yOrientation) {xStart ,0, size*xOrientation, size*yOrientation};
public:
    float xCoord=100,xVelocity=0;
    const float MAX_X_VELOCITY_PER_SECOND=180;
    float  CURRENT_X_VELOCITY_INCREASE;
    float  CURRENT_MAX_X_VELOCITY;

    float yCoord=0,yVelocity=0;
    const float MAX_Y_VELOCITY_PER_SECOND=300;
    float  CURRENT_Y_VELOCITY_INCREASE;
    float  CURRENT_MAX_Y_VELOCITY;

    float xFacing=1;

    Rectangle const hitbox={25,25,20,39};
    bool hideHitbox=false;
    Rectangle getHitbox()
    {
        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
    }
    void move()
    {
        int fps=GetFPS();
        if(!fps)
            return;
        moveOx(fps);
        moveOy(fps);
        if(xFacing*xVelocity<0)
            xFacing=-xFacing;
    }
    void moveOy(int fps)
    {
        CURRENT_MAX_Y_VELOCITY=MAX_Y_VELOCITY_PER_SECOND/fps;
        CURRENT_Y_VELOCITY_INCREASE=CURRENT_MAX_Y_VELOCITY/2/fps;

        yVelocity+=CURRENT_Y_VELOCITY_INCREASE;

        if(IsKeyPressed(KEY_SPACE))
            yVelocity-=CURRENT_Y_VELOCITY_INCREASE*100;

        if(yVelocity>CURRENT_MAX_Y_VELOCITY)
            yVelocity=CURRENT_MAX_Y_VELOCITY;

        yCoord+=yVelocity;
    }
    void moveOx(int fps)
    {

        CURRENT_MAX_X_VELOCITY=MAX_X_VELOCITY_PER_SECOND/fps;
        CURRENT_X_VELOCITY_INCREASE=CURRENT_MAX_X_VELOCITY/2/fps;

        bool isNotMoving=true;
        if(IsKeyDown(KEY_RIGHT) && xVelocity>=0)
        {
            xVelocity+=CURRENT_X_VELOCITY_INCREASE;
            isNotMoving=false;
        }
        if(IsKeyDown(KEY_LEFT)&& xVelocity<=0)
        {
            xVelocity-=CURRENT_X_VELOCITY_INCREASE;
            isNotMoving=false;
        }
        if(isNotMoving)
        {
            if(xVelocity>CURRENT_X_VELOCITY_INCREASE*5)
                xVelocity-=CURRENT_X_VELOCITY_INCREASE*5;
            else
                if(xVelocity<-CURRENT_X_VELOCITY_INCREASE*5)
                    xVelocity+=CURRENT_X_VELOCITY_INCREASE*5;
                else
                    xVelocity=0;
        }
        if(xVelocity>CURRENT_MAX_X_VELOCITY)
            xVelocity=CURRENT_MAX_X_VELOCITY;
        if(xVelocity< -CURRENT_MAX_X_VELOCITY)
            xVelocity=-CURRENT_MAX_X_VELOCITY;
        xCoord+=xVelocity;


    }
    void reset()
    {
        xCoord=100,yCoord=0;
        xVelocity=yVelocity=0;
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
        int lastX=myPlayer.xCoord-myPlayer.xVelocity;
        int lastY=myPlayer.yCoord-myPlayer.yVelocity;
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
