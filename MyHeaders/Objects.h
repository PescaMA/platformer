#include <map>
#include <utility> /// for pair
#include <fstream> /// for accessing stored levels
#include <stdio.h> /// for replacing (meaning deleting) current level

/*********************************************
*
*               O B J E C T
*
**********************************************/
Directions RayJump::Object::getDir(int x,int y)
{
    Directions rez;
    rez.up=y+hitbox.y;
    rez.down=y+hitbox.y+hitbox.height;
    rez.left=x+hitbox.x;
    rez.right=x+hitbox.x+hitbox.width;
    return rez;
}
Rectangle RayJump::Object::getHitbox(int x,int y)
{
    return {x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
}
RayJump::Object::Object(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):page(page)
{
    this->UID=UID;
    this->image=image;
    this->imageX=imageX;
    this->hitbox=hitbox;
}
void RayJump::Object::draw(int x,int y,int transparency)
{
    Color white=WHITE;
    white.a=transparency;
    Rectangle imageBoundaries= {(float)imageX,0,getImageSize().x,getImageSize().y};
    Vector2 pos= {(float)x,(float)y};
    DrawTextureRec(image,imageBoundaries,pos,white);
    if(!hideHitbox)
    {
        Rectangle objectRect= {x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
        DrawRectangleLinesEx(objectRect,1,WHITE);
    }

}
Vector2 const RayJump::Object::getImageSize()
{
    return {32,32};
}
bool RayJump::Object::collision(int x,int y,Rectangle entity)
{
    if(CheckCollisionRecs(entity,this->getHitbox(x,y)))
        return true;
    return false;
}
void RayJump::Object::movePlayer(char const c[10],int x,int y)
{
    /// needs to access the map to check if moving the player to a certain position creates a collision
    Directions playerDir=myPlayer.getPrevDir();
    Directions objDir=getDir(x,y);
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
        if(objDir.up-playerDir.down>-10
                && !myMap.checkAllCollisionsE({myPlayer.xCoord,moveUp,myPlayer.getHitbox().width,myPlayer.getHitbox().height}))
        {
            return movePlayer("up",x,y);
        }

        myPlayer.xCoord=moveLeft;
        myPlayer.xVelocity=0;

    }
    if(!strcmp(c,"right"))
    {
        if(objDir.up-playerDir.down>-10
                && !myMap.checkAllCollisionsE({myPlayer.xCoord,moveUp,myPlayer.getHitbox().width,myPlayer.getHitbox().height}))
        {
            return movePlayer("up",x,y);
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
void RayJump::Block::collisionEffect(int x,int y)
{
    Directions playerDir=myPlayer.getPrevDir();
    Directions objDir=getDir(x,y);
    int up   =  objDir.up - playerDir.down;
    int down =  playerDir.up - objDir.down;
    int left =  objDir.left - playerDir.right;
    int right = playerDir.left - objDir.right;
    if(up>=0)
        movePlayer("up",x,y);
    else if(down>=0)
        movePlayer("down",x,y);
    if(left>=0)
        movePlayer("left",x,y);
    else if(right>=0)
        movePlayer("right",x,y);
}

/*********************************************
*
*               S T A R T
*
**********************************************/
RayJump::Start::Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox) {}
void RayJump::Start::specialEffect()
{
    myPlayer.xCoord=x+16-myPlayer.hitbox.x-myPlayer.hitbox.width/2;
    myPlayer.yCoord=y+16-myPlayer.hitbox.y-myPlayer.hitbox.height/2;
}
Vector2 const RayJump::Start::getImageSize()
{
    return {32,64};
}
bool RayJump::Start::collision(Rectangle entity)
{
    return Object::collision(x,y,entity);
}
void RayJump::Start::draw(int transparency)
{
    return Object::draw(x,y,transparency);
}

/*********************************************
*
*               F I N I S H
*
**********************************************/
RayJump::Finish::Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox) {}
void RayJump::Finish::draw(int transparency)
{
    RayJump::Object::draw(x,y,transparency);
}
bool RayJump::Finish::collision(Rectangle entity)
{
    return RayJump::Object::collision(x,y,entity);
}
void RayJump::Finish::collisionEffect()
{
    won=true;
}
