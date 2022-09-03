#include "Objects.h"
#include "ExtendedRaylib.h"
#include <map>
#include <utility> /// for pair
#include <fstream> /// for accessing stored levels
#include <stdio.h> /// for deleting files
extern int fps;
extern Texture2D ASSET_CHARACTER;
extern Player myPlayer;
extern char doing[21];
extern Object **AllObjects;
class MapObj
{
public:
    std::map <std::pair<int,int>,int> currentMap;
void saveMap(std::string fileName)
{
    remove(fileName.c_str());
    std::ofstream fout(fileName);
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin();it!=currentMap.end();it++)
        fout<<(it->first).first<<' '<<(it->first).second<<' '<<it->second<<'\n';
    fout.close();
}
void loadMap(std::string fileName)
{
    currentMap.clear();
    std::ifstream fin(fileName);
    if(!fin)
        return;
    std::pair<int,int> coord;
    int objectId;
    while(fin>>coord.first)
    {
        fin>>coord.second>>objectId;
        currentMap[coord]=objectId;
    }
    fin.close();
}
void drawMap(int transparency)
{
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin();it!=currentMap.end();it++)
        AllObjects[(it->second)]->draw((it->first).first,(it->first).second,transparency);
}
void checkMapCollision(std::pair<int,int> coord)
{
    coord.first=coord.first/32*32;
    coord.second=coord.second/32*32;
    if (currentMap.find(coord) != currentMap.end())
        AllObjects[currentMap[coord]]->collision(coord.first,coord.second);
}
void proximityCollisions()
{
    for(float i=0;i<=1;i+=0.5)
       for(float j=0;j<=1;j+=0.5)
            checkMapCollision({myPlayer.getHitbox().x+myPlayer.hitbox.width*i,(myPlayer.getHitbox().y+myPlayer.hitbox.height*j)});
}
}myMap;


class Game
{
    Exit exit;
public:
    bool exiting=false;
    void run()
    {
        if(exiting)
            return exitscreen();
        myPlayer.move();
        myMap.proximityCollisions();
        draw();
        if(IsKeyPressed(KEY_ESCAPE))
            exiting=true;
    }
    void exitscreen()
    {
        draw(100);
        exit.kbdMove.run();
        if(IsKeyPressed(KEY_ESCAPE) || exit.stay())
        {
            exiting=false;
            exit.kbdMove.reset();
        }

        if(exit.leave())
        {
            myPlayer.reset();
            exiting=false;
            strcpy(doing,"MainMenu");
            exit.kbdMove.reset();
        }

    }
    void  draw(int transparency=255)
    {
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;

        BeginDrawing();
        ClearBackground(T_BLUE);
        myPlayer.draw(transparency);
        myMap.drawMap(transparency);
        if(transparency!=255)
            exit.run();
        EndDrawing();
    }
};
