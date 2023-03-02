#ifndef RAYJUMP ///contains the whole header
#define RAYJUMP
#include "ExtendedRaylib.h"
#include <cmath>
#include <map>
#include <vector>
/// TO DO: make so not horrible.


namespace RayJump
{
/****************************************************************************
*
*      G L O B A L S - normal data types
*
****************************************************************************/
    char doing[21]="MainMenu";
    const int iscreenWidth=800;
    const int iscreenHeight=600;
    int fps;
    int hideHitbox=false;
    int nrOfObjects;
    std::map<int,int> UID_pairing;
    int const gameTicks = 200;
    Texture2D ASSET_CHARACTER;
    Texture2D ASSET_BLOCKS;
    Texture2D ASSET_SPECIAL;
    Texture2D ASSET_BACKGROUND;
/****************************************************************************
*
*        Settings - Event Screens, Settings, Main Menu
*
****************************************************************************/
    class Exit /// Generalized exit screen to main menu. Needs a parent in which to be used.
    { /// TO DO FIX THIS UNCOMPLETED MES
        ScaledRectangle Srect = ScaledRectangle(&screenInfo,15,85,20,80);
        TxtAligned sure=TxtAligned("Are you sure you want to quit?",&(Srect.rect),50,10,30,BLACK);
        ///DrawText(sure,X+WIDTH/2-MeasureText(sure,29)/2,Y+20,29,BLACK);
        ButtonAligned yes=ButtonAligned("Yes",&(Srect.rect),10,90,30,BLACK,RED);
        ButtonAligned no=ButtonAligned("No",&(Srect.rect),90,90,30,BLACK,GREEN);
        KBD_Btn_Move kbdMove;
    public:
        enum States {off, starting, going, returning, exiting};
        States state = off;
        /// we can communicate with parent directly through state

        Exit();
        void align();
        template <class drawable> void run(drawable background);
    };

    class Win_Screen
    {
        TxtAligned message=TxtAligned("You won!",&screenInfo,50,30,80,GREEN);
        ButtonAligned restart=ButtonAligned("Restart",&screenInfo,50,85,35,BLACK,RED);
        KBD_Btn_Move kbdMove;
        public:
        Win_Screen();
        template <class Game>
        void run(Game game);
        void align();
        void draw();
    };

    class Settings
    {
    public:
        void static run();
    };
    class MainMenu
    {
         TxtAligned name=TxtAligned("RayJump",&screenInfo,50,20,50,BLACK);
         ButtonAligned playOn=ButtonAligned("Continue",&screenInfo,50,40,30,BLACK,GREEN);
         ButtonAligned lvlSelect=ButtonAligned("Level Select",&screenInfo,50,50,30,BLACK,GREEN);
         ButtonAligned lvlEditor=ButtonAligned("Level Editor",&screenInfo,50,60,30,BLACK,GREEN);
         ButtonAligned exit=ButtonAligned("Exit",&screenInfo,50,70,30,BLACK,GREEN);
         int keyboardSelected = 0;
         KBD_Btn_Move kbdMove;
    public:
        MainMenu();
        void run();
        void recalcuate();
    private:
        void draw();
    };
/****************************************************************************
*
*       L O A D E R S - Loader, Level_Select
*
****************************************************************************/
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
****************************************************************************/
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
        static int* makeExtra(int x,int y);
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
/****************************************************************************
*
*        G A M E - Game, MapObj
*
****************************************************************************/

    class Game
    {
        Exit exit;
        Win_Screen winScreen;
        GameTickRate gameTick=GameTickRate(gameTicks);
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
        std::map<std::pair<int,int>,std::pair<int,int *>> lvlObj;
        typedef std::map<std::pair<int,int>,std::pair<int,int *>>::iterator objInfo;
        void saveMap(std::string fileName);
        void loadMap(std::string fileName,std::string next_level_name);
        int getUID(std::pair<int,int> coord);
        void restartMap();
        void drawMap(int transparency);
        bool onlyUID(Rectangle rect, int UID);
        void checkPlayerCollisions();
        bool checkAllCollisionsE(Rectangle entity);
        bool checkAllCollisionsMouse();
        bool checkSolidCollisionsE(Rectangle entity);
        std::vector<std::pair<int,int>> getAllCollisionsE(Rectangle entity);
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
        KE = mv�/2          // Kinetic energy at initial jump velocity v
        mgh = mv�/2         // set them equal and solve for v
        v� = 2gh => v=sqrt(2gh)*/
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
/****************************************************************************
*
*        Level Editor - Obj Selector, Editor
*
****************************************************************************/
    class LE_Buttons
    {
    public:
        FixedButton  *buttons;
        KBD_Btn_Move kbdMove;
        int pagesNr;
        int *currentPage;

        template<class a>
        LE_Buttons(a obj);
        void run();
        void changeButton(int newPage);
        void draw_content(int transparency);
    };
    class ObjectSelector
    {
    public:
        int *lastOnPage;/// array
        int *currentPage; /// pointer
        int *currentObject;
        int **extraInfo;
        bool *isObjSpecial;
        template <class a>
        ObjectSelector(a obj);
        void draw_content(int transparency);
        void run();
    protected:
        int getN();
        float getX (int i);
        float getY ();
        int *getInfo(int i);
    };

    class LevelEditor
    {
        int startX=0,startY=0;
        Color areaColor=WHITE;
        RayJump::Exit exit;
        ObjectSelector objSel;
        LE_Buttons buttons;
        enum States{selecting,holding,building};
        States state;
    public:
        bool isObjectShown=false;
        int currentObject;
        int currentPage;
        int* extraInfo;
        bool isObjSpecial=false;

        LevelEditor();
        void  run();
        void PlaceBlocks();
        void getPlaceStart();
        Rectangle getBigRectangle();
        void placeAll();
        void setFillColor();
        void deleteObj();
        void  draw();
        void draw_content(int transparency);
        void drawLevel(int transparency);
        void drawHeldObj(int transparency);
        void drawBuildRect(int transparency);
        void drawSelector(int transparency);

    };

/****************************************************************************
*
*        GLOBALS - from classes declared
*
****************************************************************************/

    Object **AllObjects;
    Block Block1,Block2,Block3,Block4,Block5;
    Start myStart;
    Finish myFinish;
    MapObj myMap;
    Player myPlayer;
}

/// Definitions are found here:
#include "Settings.h"
#include "Objects.h"
#include "Loader.h"
#include "Game.h"
#include "Player.h"
#include "LevelEditor.h"

#endif
