#ifndef RAYJUMP ///contains the whole header
#define RAYJUMP

#include <map>
namespace RayJump
{
    char doing[21]="MainMenu";
    const int screenWidth=800;
    const int screenHeight=600;
    int fps;
    int hideHitbox=false;


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

class Exit
{
    const float  X=RayJump::screenWidth/8, Y=RayJump::screenHeight/3.0f;
    const float WIDTH=RayJump::screenWidth*6/8.0f,HEIGHT=RayJump::screenHeight/3.0f;
    Button yes=Button("Yes",X+40,Y+HEIGHT-50,30,BLACK,RED);;
    Button no=Button("No",X+WIDTH-50-MeasureText("No",30),Y+HEIGHT-50,30,BLACK,GREEN);

public:
    KBD_Btn_Move kbdMove;
    enum States {off, starting, going, returning, exiting};
    States state = off;
    Exit();
    void run();
    template <class drawable>
    void run(drawable background);
    bool leave();
    bool stay();
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
    public:
    void run();
    void draw();
};

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

/********************************************
*
*                 GLOBALS
*
********************************************/

    Object **AllObjects;
    int nrOfObjects;
    Block Block1,Block2,Block3,Block4,Block5;
    Start myStart;
    Finish myFinish;
    MapObj myMap;
    Texture2D ASSET_CHARACTER;
    Texture2D ASSET_BLOCKS;
    Texture2D ASSET_SPECIAL;

    std::map<int,int> UID_pairing;
}

#endif
