#ifndef OBJECTS_H
#define OBJECTS_H
#include "ExtendedRaylib.h"
#include "globals.h"
/****************************************************************************
*
*       O B J E C T S - Object, Block, Start, Finish; MapObj
*
****************************************************************************/
namespace RayJump
{
    class Object
    {
        public:
        enum extra{X,Y,nrExtra};
        int extraCount = 2;

        bool canWJ=false;
        bool isSolid = true;
        Texture2D image;
        int imageX;
        int UID;
        int page;
        Rectangle hitbox;

        Object(){}
        Directions getDir(int* v);
        Rectangle getHitbox(int* v);
        Vector2 makeCentered(Vector2 coord);
        float makeXCentered(float X);
        float makeYCentered(float Y);
        Object(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
        void virtual draw(int* v,int transparency=255);
        void movePlayer(char const c[10],int* v);
        void virtual collisionEffect(int* v){}
        void virtual specialEffect(){}
        virtual int* makeExtra(int x,int y);
        Vector2 const virtual getImageSize();
        bool collision(int* v,Rectangle entity);
    };

    class Block : public Object
    {
        public:
        Block(){}
        Block(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
        void collisionEffect(int* v);
    };

    class Start : public Object
    {
        public:
        enum extra{xa,ya,nrExtra};
        int *v;
        bool exists=false;
        Start(){}
        Start(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
        void specialEffect();
        Vector2 const getImageSize();
        bool collision(Rectangle entity);
        void draw(int transparency=255);
    };

    class Finish : public RayJump::Object
    {
        public:
        enum extra{xa,ya,nrExtra};
        int *v;
        bool exists=false;
        bool won=false;
        Finish(){}
        Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
        void draw(int transparency);
        bool collision(Rectangle entity);
        void collisionEffect();
    };
}
#endif // OBJECTS_H
