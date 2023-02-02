#ifndef RAYJUMP ///contains the whole header
#define RAYJUMP
#include "ExtendedRaylib.h"
#include <cmath>
#include <map>
namespace RayJump
{
/****************************************************************************
*
*      G L O B A L S - normal data types
*
/***************************************************************************/
    char doing[21]="MainMenu";
    const int screenWidth=800;
    const int screenHeight=600;
    int fps;
    int hideHitbox=false;
    int nrOfObjects;
    std::map<int,int> UID_pairing;
/****************************************************************************
*
*        Settings - Event Screens, Settings, Main Menu
*
****************************************************************************/
    class Exit /// Generalized exit screen to main menu. Needs a parent in which to be used.
    {
        const float  X=screenWidth/8, Y=screenHeight/3.0f;
        const float WIDTH=screenWidth*6/8.0f,HEIGHT=screenHeight/3.0f;
        Button yes=Button("Yes",X+40,Y+HEIGHT-50,30,BLACK,RED);;
        Button no=Button("No",X+WIDTH-50-MeasureText("No",30),Y+HEIGHT-50,30,BLACK,GREEN);
        KBD_Btn_Move kbdMove;
    public:
        enum States {off, starting, going, returning, exiting};
        States state = off;
        /// we can communicate with parent directly through state

        Exit();
        template <class drawable> void run(drawable background);
    };

    class Win_Screen
    {
        TxtAligned message=TxtAligned("You won!",ERay::getWindowSize(),50,30,80,GREEN);
        ButtonAligned restart=ButtonAligned("Restart",ERay::getWindowSize(),75,85,35,BLACK,RED);
        KBD_Btn_Move kbdMove;
        public:
        Win_Screen();
        template <class Game>
        void run(Game game);
        void draw();
    };

    class Settings
    {
    public:
        void static run();
    };
    class MainMenu
    {
         TxtAligned name=TxtAligned("RayJump",ERay::getWindowSize(),50,20,50,BLACK);
         ButtonAligned playOn=ButtonAligned("Continue",ERay::getWindowSize(),50,40,30,BLACK,GREEN);
         ButtonAligned lvlSelect=ButtonAligned("Level Select",ERay::getWindowSize(),50,50,30,BLACK,GREEN);
         ButtonAligned lvlEditor=ButtonAligned("Level Editor",ERay::getWindowSize(),50,60,30,BLACK,GREEN);
         ButtonAligned exit=ButtonAligned("Exit",ERay::getWindowSize(),50,70,30,BLACK,GREEN);
         int keyboardSelected = 0;
         KBD_Btn_Move kbdMove;
    public:
        MainMenu();
        void run();
    private:
        void draw();
    };
/****************************************************************************
*
*       L O A D E R S - Loader, Level_Select
*
/***************************************************************************/
    class Loader
    {
    public:
        static void load();
        static void unload();
        static void mainloop();
        static void extraCheck();
        static void loadAllObjects();
        static void loadMap(const char levelName[]);
    };

    class LevelSelect
    {
        Exit exit;
    public:
        void run();
        void draw();
        void draw_content(int transparency);
    };
/****************************************************************************
*
*       O B J E C T S - Object, Block, Start, Finish; MapObj
*
/***************************************************************************/
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
        void movePlayer(char const c[10],int x,int y);
        void virtual collisionEffect(int x,int y){}
        void virtual specialEffect(){}
        Vector2 const virtual getImageSize();
        bool collision(int x,int y,Rectangle entity);
    };

    class Block : public Object
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
    };

    class Finish : public RayJump::Object
    {
        public:
        int x=100,y=0;
        bool exists=false;
        bool won=false;
        Finish(){}
        Finish(int UID,int page,Texture2D image,int imageX,Rectangle hitbox);
        void draw(int transparency);
        bool collision(Rectangle entity);
        void collisionEffect();
    };
/****************************************************************************
*
*        G A M E - Game, MapObj
*
****************************************************************************/

    class Game
    {
        Exit exit;
        Win_Screen winScreen;
        GameTickRate gameTick=GameTickRate(200);
    public:
        void commands();
        void run();
        void draw();
        void draw_content(int transparency);
        void restart();
    };

    class MapObj
    {
    public:
        std::string next_level_name; /// TO DO: finish implementing next_lvl
        std::map <std::pair<int,int>,int> currentMap;
        void saveMap(std::string fileName);
        void loadMap(std::string fileName,std::string next_level_name);
        void restartMap();
        void drawMap(int transparency);
        void checkAllCollisions();
        bool checkAllCollisionsE(Rectangle entity);
        bool checkAllCollisionsMouse();
        std::pair<int,int> getCollisionE(Rectangle entity);
        std::pair<int,int> getCollisionMouse();
        void deletePair(std::pair<int,int> coord);
        void deleteClick(Vector2 pos);
    };

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
        const float MAX_X_VELOCITY_PER_FRAME=MAX_X_VELOCITY_PER_SECOND/200;
        const float X_SECONDS_UNTIL_MAX=0.5;
        float const XVelocityGain=MAX_X_VELOCITY_PER_FRAME/X_SECONDS_UNTIL_MAX/200;
        int XDirection=0;

        /**  VERTICAL INFORMATION  **/
        float yCoord,yVelocity=0,yMovement;
        const float MAX_Y_VELOCITY_PER_SECOND=300;
        const float MAX_Y_VELOCITY_PER_FRAME=MAX_Y_VELOCITY_PER_SECOND/200;
        const float Y_SECONDS_UNTIL_MAX=0.2;
        const float YVelocityGain=MAX_Y_VELOCITY_PER_FRAME/Y_SECONDS_UNTIL_MAX/200;/// 200 = gametick
        bool isGrounded=false;

        /**  JUMPING  **/
        const float JUMP_HEIGHT=90;
        /*Fizica: (https://fenomas.com/2016/07/game-jumping-physics/)
        PE = mgh            // Potential energy at peak of jump
        KE = mv²/2          // Kinetic energy at initial jump velocity v
        mgh = mv²/2         // set them equal and solve for v
        v² = 2gh => v=sqrt(2gh)*/
        const float JUMP_VELOCITY=sqrt(JUMP_HEIGHT*YVelocityGain*2);

        /**  DASHING  **/
        int dashes=1;
        bool isdashing=false;
        long long dashTime=0;
        const float dashXVal=2;
        const float dashYVal=1.9;
        const float dashXDiagVal=1.50;
        const float dashYDiagVal=1.50;
        const long long MAX_DASH_TIME=200; /// 0.1 sec (in milisec)

        /**  WALL JUMP/CLING-ING  **/
        const float WJ_MAX_DISTANCE=20; /// wall jump
        int WJ_direction=0;
        const float WC_MAX_DISTANCE=10; /// wall cling
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
/****************************************************************************
*
*        Level Editor - Obj Selector, Editor
*
****************************************************************************/
    class ObjectSelector
    {
    public:
        int *lastOnPage;/// array
        int *currentPage; /// pointer
        int *currentObject; /// pointer
        int pagesNr;
        void draw(int transparency);
        void run();
        void buildLastOnPage();
    protected:
        int getN();
        float getX (int i);
        float getY ();
    };

    class LevelEditor
    {
        int *lastOnPage;
        int MAX_PAGES;
        FixedButton  *buttons;
        int currentPage=0;
        int currentObject=1;
        int startX=0,startY=0;
        bool isObjectShown=false;
        Color areaColor=WHITE;
        RayJump::Exit exit;
        KBD_Btn_Move kbdMove;

    public:
        LevelEditor();
        void  run();
        void PlaceBlocks();
        Rectangle getBigRectangle();
        void placeAll();
        void  draw();
        void draw_content(int transparency);
        void drawLevel(int transparency);
        void drawHeldObj(int transparency);
        void drawSelector(int transparency);
        void changeButton(int newPage);
    };

/****************************************************************************
*
*        GLOBALS - from classes declared
*
/***************************************************************************/

    Object **AllObjects;
    Block Block1,Block2,Block3,Block4,Block5;
    Start myStart;
    Finish myFinish;
    MapObj myMap;
    Texture2D ASSET_CHARACTER;
    Texture2D ASSET_BLOCKS;
    Texture2D ASSET_SPECIAL;
    Player myPlayer;
    ObjectSelector objSel;
}

/// Definitions are found here:
#include "Settings.h"
#include "Objects.h"
#include "Loader.h"
#include "Game.h"
#include "Player.h"
#include "LevelEditor.h"

#endif
