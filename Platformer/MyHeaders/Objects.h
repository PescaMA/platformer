#ifndef OBJECTS
#define OBJECTS

#include "ExtendedRaylib.h"
extern Texture2D ASSET_BLOCKS;
extern Texture2D ASSET_CHARACTER;
bool hideHitbox=false;
class Player
{
    #define smart_paint(xStart,size,xOrientation,yOrientation) {xStart ,0, size*xOrientation, size*yOrientation};
public:
    float initialXCoord,initialYCoord;

    float xCoord,xVelocity=0,xMovement;
    const float MAX_X_VELOCITY_PER_FRAME=0.8;
    const float X_SECONDS_UNTIL_MAX=1;
    float const XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/200;
    bool isNotMoving;
    int XDirection=0;

    float yCoord,yVelocity=0,yMovement;
    const float MAX_Y_VELOCITY_PER_FRAME=2;
    const float Y_SECONDS_UNTIL_MAX=1.5;
    float const YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/200;/// 200 = gametick
    bool isGrounded=false;

    float xFacing=1;

    Rectangle const hitbox={25,25,20,39};

    Rectangle getHitbox()
    {

        return {xCoord+hitbox.x,yCoord+hitbox.y,hitbox.width,hitbox.height};
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
        if(isGrounded && IsKeyPressed(KEY_SPACE))
        {
            yVelocity=-YVelocityGain*175;
            isGrounded=false;
        }


        isNotMoving=true;
        XDirection=0;
        if(IsKeyDown(KEY_RIGHT) && xVelocity>=0)
        {
            XDirection=1;
            isNotMoving=false;
        }
        if(IsKeyDown(KEY_LEFT)&& xVelocity<=0)
        {
            XDirection=-1;
            isNotMoving=false;
        }
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
        if(isNotMoving)
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
    void reset()
    {
        xCoord=initialXCoord;
        yCoord=initialYCoord;
        xVelocity=yVelocity=0;
        xFacing=1;
        isGrounded=false;
        XDirection=0;
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
    /// D,U,L,R = directions (down, ...), p = player, b = block
    #define UbDp (1.0f*(hitbox.y+y)  -  (lastY+myPlayer.hitbox.y+myPlayer.hitbox.height))
    #define LbRp (1.0f*(x+hitbox.x)  -  (lastX+myPlayer.hitbox.x+myPlayer.hitbox.width))
    #define UpDb (1.0f*(lastY+myPlayer.hitbox.y)  -  (y+hitbox.y+hitbox.height))
    #define LpRb (1.0f*(lastX+myPlayer.hitbox.x)  -  (x+hitbox.x+hitbox.width))

    #define up    UbDp >= 0 /// if player is above the block
    #define down  UpDb >= 0 /// if player is below the block
    #define left  LbRp >= 0 /// if player is to the left of the block
    #define right LpRb >= 0 /// if player is to the left of the block
    public:
    Texture2D image;
    int imageX;
    int UID;
    int page;
    Rectangle hitbox;
    Object(){}
    Rectangle getHitbox(int x,int y)
    {
        return {x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
    }
    Object(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):page(page)
    {
        this->UID=UID;
        this->image=image;
        this->imageX=imageX;
        this->hitbox=hitbox;
    }
    void virtual draw(int x,int y,int transparency=255)
    {
        Color white=WHITE;
        white.a=transparency;
        Rectangle imageBoundaries={(float)imageX,0,getImageSize().x,getImageSize().y};
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
        {
            myPlayer.yCoord=y+hitbox.y-myPlayer.hitbox.y-myPlayer.hitbox.height;
            myPlayer.isGrounded=true;
        }

        if(!strcmp(c,"left"))
            myPlayer.xCoord=x+hitbox.x-myPlayer.hitbox.x-myPlayer.hitbox.width;
        if(!strcmp(c,"down"))
            myPlayer.yCoord=y+hitbox.y+hitbox.height-myPlayer.hitbox.y;
        if(!strcmp(c,"right"))
            myPlayer.xCoord=x+hitbox.x+hitbox.width-myPlayer.hitbox.x;
    }
    void virtual collisionEffect(int x,int y){}
    void virtual specialEffect(){}
    Vector2 const virtual getImageSize()
    {
        return {32,32};
    }
    bool collision(int x,int y,Rectangle entity)
    {
        if(CheckCollisionRecs(entity,this->getHitbox(x,y)))
            return true;
        return false;
    }

};
class Block : public Object
{
    public:
    Block(){}
    Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){}
    void collisionEffect(int x,int y)
    {
        int lastX=myPlayer.xCoord-myPlayer.xMovement;
        int lastY=myPlayer.yCoord-myPlayer.yMovement;
        bool UD = up || down;
        bool LR = left || right;
        if(UD && LR)
        {
            float vertical,horizontal;
            if(myPlayer.yMovement>0)
                vertical=1.0f*UbDp/myPlayer.yMovement;
            else
                vertical=-1.0f*UpDb/myPlayer.yMovement;
            if(myPlayer.xMovement>0)
                horizontal=1.0f*LbRp/myPlayer.xMovement;
            else
                horizontal=-1.0f*LpRb/myPlayer.xMovement;
            if(vertical!=1/0.0f && horizontal!=1/0.0f && vertical >= 0 && horizontal >= 0)
            {
                if(vertical<horizontal)
                    LR=false;
                else
                    UD=false;
            }
        }
        if(UD)
        {
            myPlayer.yVelocity=0;
            if(up)
                movePlayer("up",x,y);
            else
                movePlayer("down",x,y);
        }
        if(LR)
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
class Start : public Object
{
    public:
    int x=0,y=0;
    bool exists=false;
    Start(){}
    Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){}
    void specialEffect()
    {
        myPlayer.xCoord=myPlayer.initialXCoord=x+16-myPlayer.hitbox.x-myPlayer.hitbox.width/2;
        myPlayer.yCoord=myPlayer.initialYCoord=y+16-myPlayer.hitbox.y-myPlayer.hitbox.height/2;
    }
    Vector2 const getImageSize()
    {
        return {32,64};
    }
    void draw(int transparency=255)
    {
        return Object::draw(x,y,transparency);
    }

}myStart;
class Finish : public Object
{
    public:
    int x=100,y=0;
    bool exists=false;
    bool won=false;
    Finish(){}
    Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){}
    void draw(int transparency)
    {
        Object::draw(x,y,transparency);
    }
    void collisionEffect(int x,int y)
    {
        won=true;
    }
}myFinish;

#endif // OBJECTS
