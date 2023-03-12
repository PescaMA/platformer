#ifndef GAME_H
#define GAME_H
#include "ExtendedRaylib.h"
#include "Settings.h"
#include "globals.h"
#include <vector>
/****************************************************************************
*
*        G A M E - Game, MapObj
*
****************************************************************************/
namespace RayJump
{
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
}
#endif // GAME_H
