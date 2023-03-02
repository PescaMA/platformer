#include <map>
#include <utility> /// for pair
#include <fstream> /// for accessing stored levels
#include <stdio.h> /// for replacing (meaning deleting) current level

/*********************************************
*
*               O B J E C T
*
**********************************************/

/// considering something like http://cplusplus.bordoon.com/namedArrayElements.html
RayJump::Object::Object(int UID,int page,Texture2D image,int imageX,Rectangle hitbox)
{
    this->page=page;
    this->UID=UID;
    this->image=image;
    this->imageX=imageX;
    this->hitbox=hitbox;
}
Directions RayJump::Object::getDir(int* v)
{
    Directions rez;
    rez.up=v[Y]+hitbox.y;
    rez.down=v[Y]+hitbox.y+hitbox.height;
    rez.left=v[X]+hitbox.x;
    rez.right=v[X]+hitbox.x+hitbox.width;
    return rez;
}
Vector2 RayJump::Object::makeCentered(Vector2 coord)
{
    return {makeXCentered(coord.x),makeYCentered(coord.y)};
}
float RayJump::Object::makeXCentered(float x)
{
    return x-hitbox.x-hitbox.width/2;
}
float RayJump::Object::makeYCentered(float y)
{
    return y-hitbox.y-hitbox.height/2;
}
Rectangle RayJump::Object::getHitbox(int *v)
{
    return {v[X]+hitbox.x,v[Y]+hitbox.y,hitbox.width,hitbox.height};
}
Vector2 const RayJump::Object::getImageSize()
{ /// a virtual function. most common size is 32x32. Only height should change.
    return {32,32};
}
void RayJump::Object::draw(int *v,int transparency)
{
    Color white=WHITE;
    white.a=transparency;
    Rectangle imageBoundaries= {(float)imageX,0,getImageSize().x,getImageSize().y};
    Vector2 pos= {(float)v[X],(float)v[Y]};
    DrawTextureRec(image,imageBoundaries,pos,white);
    if(!hideHitbox)
    {
        Rectangle objectRect= {v[X]+hitbox.x,v[Y]+hitbox.y,hitbox.width,hitbox.height};
        DrawRectangleLinesEx(objectRect,1,WHITE);
    }

}
int* RayJump::Object::makeExtra(int x,int y)
{
    int *v = new int[nrExtra];
    v[0]=x;
    v[1]=y;
    return v;
}

bool RayJump::Object::collision(int *v,Rectangle entity)
{
    return CheckCollisionRecs(entity,this->getHitbox(v));
}

/// Considering changing below function (too complex)
void RayJump::Object::movePlayer(char const c[10],int* v)
{
    Directions playerDir=myPlayer.getPrevDir();
    Directions objDir=getDir(v);
    float moveUp    = objDir.up - myPlayer.hitbox.y - myPlayer.hitbox.height;
    float moveDown  = objDir.down - myPlayer.hitbox.y;
    float moveLeft  = objDir.left - myPlayer.hitbox.x - myPlayer.hitbox.width;
    float moveRight = objDir.right - myPlayer.hitbox.x +1e-4; /// error correction
    if(!strcmp(c,"up"))
    {

        myPlayer.yCoord=moveUp;
        myPlayer.yVelocity=0;
        myPlayer.isGrounded=true;
    }

    if(!strcmp(c,"left"))
    {
        Rectangle playerHitbox = myPlayer.getHitbox();
        playerHitbox.y=myPlayer.hitbox.y + moveUp;
        if(objDir.up-playerDir.down>-10
                && !myMap.checkSolidCollisionsE(playerHitbox))
        {
            return movePlayer("up",v);
        }

        myPlayer.xCoord=moveLeft;
        myPlayer.xVelocity=0;

    }
    if(!strcmp(c,"right"))
    {
        Rectangle playerHitbox = myPlayer.getHitbox();
        playerHitbox.y= myPlayer.hitbox.y + moveUp;
        if(objDir.up-playerDir.down>-10
                && !myMap.checkSolidCollisionsE(playerHitbox))
        {
            return movePlayer("up",v);
        }
        myPlayer.xCoord=moveRight;
        myPlayer.xVelocity=0;
    }
    if(!strcmp(c,"down"))
    {
        if((objDir.left-playerDir.right>=-1 || playerDir.left-objDir.right>=-1))
        {
            if(objDir.left-playerDir.right>=-1)
                myPlayer.xCoord=moveLeft;
            else
                myPlayer.xCoord=moveRight;
        }
        else
        {
            myPlayer.yCoord=moveDown;
            myPlayer.yVelocity=0;
        }
    }
}

/*********************************************
*
*               B L O C K
*
**********************************************/
RayJump::Block::Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox)
{
    canWJ=true;
}
void RayJump::Block::collisionEffect(int *v)
{
    Directions playerDir=myPlayer.getPrevDir();
    Directions objDir=getDir(v);
    int up   =  objDir.up - playerDir.down;
    int down =  playerDir.up - objDir.down;
    int left =  objDir.left - playerDir.right;
    int right = playerDir.left - objDir.right;
    if(up>=0)
        movePlayer("up",v);
    else if(down>=0)
        movePlayer("down",v);
    if(left>=0)
        movePlayer("left",v);
    else if(right>=0)
        movePlayer("right",v);
}

/*********************************************
*
*               S T A R T
*
**********************************************/
RayJump::Start::Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox)
{
    isSolid = false;
    v=new int[nrExtra];
    v[0]=0,v[1]=0;
}
void RayJump::Start::specialEffect()
{
    myPlayer.xCoord=v[xa]+16-myPlayer.hitbox.x-myPlayer.hitbox.width/2;
    myPlayer.yCoord=v[ya]+16-myPlayer.hitbox.y-myPlayer.hitbox.height/2;
}
Vector2 const RayJump::Start::getImageSize()
{
    return {32,64};
}
bool RayJump::Start::collision(Rectangle entity)
{

    return Object::collision(v,entity);
}
void RayJump::Start::draw(int transparency)
{
    return Object::draw(v,transparency);
}

/*********************************************
*
*               F I N I S H
*
**********************************************/
RayJump::Finish::Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox)
{
    isSolid = false;
    v=new int[nrExtra];
    v[0]=100,v[1]=100;
}
void RayJump::Finish::draw(int transparency)
{
    Object::draw(v,transparency);
}
bool RayJump::Finish::collision(Rectangle entity)
{
    return Object::collision(v,entity);
}
void RayJump::Finish::collisionEffect()
{
    won=true;
}
