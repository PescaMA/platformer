#ifndef PLAYER_H
#define PLAYER_H
#include "ExtendedRaylib.h"
#include "globals.h"
#include <cmath>
namespace RayJump
{

/****************************************************************************
*
*        P L A Y E R - Player
*
****************************************************************************/
    class Player
    {
    public:
        /**  HORIZONTAL INFORMATION  **/
        float xCoord,xVelocity=0,xMovement;
        const float MAX_X_VELOCITY_PER_SECOND=220;
        const float MAX_X_VELOCITY_PER_FRAME=MAX_X_VELOCITY_PER_SECOND/gameTicks;
        const float X_SECONDS_UNTIL_MAX=0.5;
        const float XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/gameTicks;
        int XDirection=0;

        /**  VERTICAL INFORMATION  **/
        float yCoord,yVelocity=0,yMovement;
        const float MAX_Y_VELOCITY_PER_SECOND=300;
        const float MAX_Y_VELOCITY_PER_FRAME=MAX_Y_VELOCITY_PER_SECOND/gameTicks;
        const float Y_SECONDS_UNTIL_MAX=0.2;
        const float YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/gameTicks;/// 200 = gametick
        bool isGrounded=false;

        /**  JUMPING  **/
        const float JUMP_HEIGHT=90;
        /*Fizica: (https://fenomas.com/2016/07/game-jumping-physics/)
        PE = mgh            // Potential energy at peak of jump
        KE = mv²/2          // Kinetic energy at initial jump velocity v
        mgh = mv²/2         // set them equal and solve for v
        v² = 2gh => v=sqrt(2gh)*/
        const float JUMP_VELOCITY=sqrt(JUMP_HEIGHT*YVelocityGain*2);
        long long jumpTime;
        const long long MAX_JUMP_TIME=300; ///(in milisec)

        /**  DASHING  **/
        int dashes=1;
        bool isdashing=false;
        long long dashTime=0;
        const float dashXVal=2;
        const float dashYVal=1.9;
        const float dashXDiagVal=1.50;
        const float dashYDiagVal=1.50;
        const int MAX_DASH_TIME=200; /// (in milisec)

        /**  WALL JUMP/CLING-ING  **/
        const float WJ_MAX_DISTANCE=12; /// wall jump
        int WJ_direction=0;
        const float WC_MAX_DISTANCE=4; /// wall cling
        bool isClinging=false;
        float closeLeft=999,closeRight=999; /// distance to closest block

        /**  OTHER USEFUL INFO  **/
        float xFacing=1; /// for sprite rotation
        Rectangle const hitbox={15,16,35,46};

        /**  FUNCTIONS  **/
        void checkInput();
        void checkJump();
        void checkCling();
        void horizontalCling();
        void checkVerticalCling();
        void checkDash();
        void move();
        void exitDash();
        void newMovement();
        void calcMoveOy();
        void calcMoveOx();
        void addMovement();
        void reset();
        void presume();
        Rectangle getPrevHitbox();
        Rectangle getHitbox();
        Directions getPrevDir();
        Directions getDir();
        void tryWallJump(Directions box);
        void draw(int transparency);
    };


}
#endif // PLAYER
