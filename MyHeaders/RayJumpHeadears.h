#ifndef RAYJUMP ///contains the whole header
#define RAYJUMP

#include <map>
namespace RayJump
{
    class Object
{
    public:
    bool canWJ=false;
    Texture2D image;
    int imageX;
    int UID;
    int page;
    Rectangle hitbox;
    Object(){}
    Directions getDir(int x,int y);
    Rectangle getHitbox(int x,int y);
    Object(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
    void virtual draw(int x,int y,int transparency=255);
    void movePlayer(char const c[10],int x,int y); /// Had to declare elsewhere
    void virtual collisionEffect(int x,int y){}
    void virtual specialEffect(){}
    Vector2 const virtual getImageSize();
    bool collision(int x,int y,Rectangle entity);
};
/*class Block : public Object
{
    public:
    Block(){}
    Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
    void collisionEffect(int x,int y);
};
class Start : public Object
{
    public:
    int x=0,y=0;
    bool exists=false;
    Start(){}
    Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
    void specialEffect();
    Vector2 const getImageSize();
    bool collision(Rectangle entity);
    void draw(int transparency=255);
};*/


/********************************************
*
*                 GLOBALS
*
********************************************/

    Object **AllObjects;
    int nrOfObjects;
    ///Block Block1,Block2,Block3,Block4,Block5;
    ///Start myStart;
    char doing[21];
    const int screenWidth=800;
    const int screenHeight=600;
    int fps;
    Texture2D ASSET_CHARACTER;
    Texture2D ASSET_BLOCKS;
    Texture2D ASSET_SPECIAL;

    std::map<int,int> UID_pairing;
}

#endif
