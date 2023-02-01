#ifndef OBJECTS
#define OBJECTS

#include "ExtendedRaylib.h"
#include "Player.h"
#include "RayJumpHeadears.h"
#include <map>
#include <utility> /// for pair
#include <fstream> /// for accessing stored levels
#include <stdio.h> /// for replacing (meaning deleting) current level

class MapObj;

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
        Rectangle imageBoundaries={(float)imageX,0,getImageSize().x,getImageSize().y};
        Vector2 pos={(float)x,(float)y};
        DrawTextureRec(image,imageBoundaries,pos,white);
        if(!hideHitbox)
        {
            Rectangle objectRect={x+hitbox.x,y+hitbox.y,hitbox.width,hitbox.height};
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
{ /// needs to access the map to check if moving the player to a certain position creates a collision
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


    RayJump::Block::Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){canWJ=true;}
    void RayJump::Block::collisionEffect(int x,int y)
    {
        Directions playerDir=myPlayer.getPrevDir();
        Directions objDir=getDir(x,y);
        int up   =  objDir.up - playerDir.down;
        int down =  playerDir.up - objDir.down;
        int left =  objDir.left - playerDir.right;
        int right = playerDir.left - objDir.right;
        if(up>=0)
            movePlayer("up",x,y);else
        if(down>=0)
            movePlayer("down",x,y);
        if(left>=0)
            movePlayer("left",x,y);else
        if(right>=0)
            movePlayer("right",x,y);
    }


    RayJump::Start::Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){}
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

/***********************************************
*
* cross header
*
***********************************************/
void Player::reset()
{
    RayJump::myStart.specialEffect();
    xVelocity=yVelocity=0;
    xFacing=1;
    XDirection=0;
    isdashing=false;
    presume();
}

    RayJump::Finish::Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox):Object(UID,page,image,imageX,hitbox){}
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


    void RayJump::MapObj::saveMap(std::string fileName)
    {
        /// TO DO: make file maybe if aint existin'
        remove(fileName.c_str());

        std::ofstream fout(fileName);
        fout<<"start "<<RayJump::myStart.x<<' '<<RayJump::myStart.y<<'\n';
        fout<<"finish "<<RayJump::myFinish.x<<' '<<RayJump::myFinish.y<<'\n';
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            fout<<(it->first).first<<' '<<(it->first).second<<' '<<RayJump::AllObjects[it->second]->UID<<'\n';
        fout.close();
    }
    void RayJump::MapObj::loadMap(std::string fileName,std::string next_level_name)
    {
        currentMap.clear();
        next_level_name.clear();
        this->next_level_name=next_level_name;
        std::ifstream fin(fileName);
        if(!fin)
        {std::cout<<"Error, file does not exist!";return;}

        char c[10];
        fin>>c;
        if(strcmp(c,"start"))
        {
            std::cout<<"\n\nError, no start provided\n\n";
            strcpy(doing,"Exiting");return;
        }
        fin>>RayJump::myStart.x>>RayJump::myStart.y;
        fin>>c;
        if(strcmp(c,"finish"))
        {
            std::cout<<"\n\nError, no finish provided\n\n";
            strcpy(doing,"Exiting");return;
        }
        fin>>RayJump::myFinish.x>>RayJump::myFinish.y;

        myPlayer.reset();

        std::pair<int,int> coord;
        int UID;
        while(fin>>coord.first)
        {
            fin>>coord.second>>UID;
            if(UID==RayJump::myStart.UID || UID==RayJump::myFinish.UID)
            {
                std::cout<<"\n\nError, multiple starts or finishes provided.\n\n";
                strcpy(doing,"Exiting");return;
            }
            currentMap[coord]=UID_pairing[UID];
        }
        fin.close();
    }
    void RayJump::MapObj::restartMap()
    {
        /// great function so far
    }
    void RayJump::MapObj::drawMap(int transparency)
    {
        RayJump::myStart.draw(transparency);
        RayJump::myFinish.draw(transparency);
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            RayJump::AllObjects[(it->second)]->draw((it->first).first,(it->first).second,transparency);
    }
    void RayJump::MapObj::checkAllCollisions()
    {
        if(RayJump::myFinish.collision(myPlayer.getHitbox()))RayJump::myFinish.collisionEffect();
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        {
            RayJump::Object *obj=RayJump::AllObjects[it->second];
            if(obj->collision((it->first).first , (it->first).second , myPlayer.getHitbox()))
                obj->collisionEffect((it->first).first , (it->first).second);
            if(obj->canWJ)
                myPlayer.tryWallJump(obj->getDir(it->first.first,it->first.second));

        }

        myPlayer.newMovement();
    }
    bool RayJump::MapObj::checkAllCollisionsE(Rectangle entity)
    {
        if(RayJump::myFinish.collision(entity)) return true;
        if(RayJump::myStart.collision(entity))  return true;
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(RayJump::AllObjects[it->second]->collision((it->first).first , (it->first).second , entity))
                return true;
        return false;
    }
    bool RayJump::MapObj::checkAllCollisionsMouse()
    {
        return checkAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),0,0});
    }
    std::pair<int,int> RayJump::MapObj::getCollisionE(Rectangle entity)
    {
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(RayJump::AllObjects[it->second]->collision((it->first).first , (it->first).second , entity))
                return {(it->first).first , (it->first).second};
        return {-999999,-999999};
    }
    std::pair<int,int> RayJump::MapObj::getCollisionMouse()
    {
        return getCollisionE({(float)GetMouseX(),(float)GetMouseY(),0,0});
    }
    void RayJump::MapObj::deletePair(std::pair<int,int> coord)
    {
        currentMap.erase(currentMap.find(coord));
    }
    void RayJump::MapObj::deleteClick(Vector2 pos)
    {
        Rectangle entity={pos.x,pos.y,1,1};
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(RayJump::AllObjects[it->second]->collision((it->first).first , (it->first).second , entity))
            {
                currentMap.erase(it);
                return;
            }
    }

#endif // OBJECTS
