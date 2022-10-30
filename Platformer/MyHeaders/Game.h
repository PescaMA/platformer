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
extern bool hideHitbox;
class MapObj
{
public:
    std::map <std::pair<int,int>,int> currentMap;
    void saveMap(std::string fileName)
    {
        remove(fileName.c_str());
        std::ofstream fout(fileName);
        fout<<"start "<<myStart.x<<' '<<myStart.y<<'\n';
        fout<<"finish "<<myFinish.x<<' '<<myFinish.y<<'\n';
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            fout<<(it->first).first<<' '<<(it->first).second<<' '<<it->second<<'\n';
        fout.close();
    }
    void loadMap(std::string fileName)
    {
        currentMap.clear();
        std::ifstream fin(fileName);
        if(!fin)
        {std::cout<<"Error";return;}

        char c[10];
        fin>>c;
        if(strcmp(c,"start"))
        {std::cout<<"Error";return;}
        fin>>myStart.x>>myStart.y;
        fin>>c;
        if(strcmp(c,"finish"))
        {std::cout<<"Error";return;}
        fin>>myFinish.x>>myFinish.y;

        myStart.specialEffect(); /// in this case sets player position

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
        myStart.draw(transparency);
        myFinish.draw(transparency);
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            AllObjects[(it->second)]->draw((it->first).first,(it->first).second,transparency);
    }
    void checkAllCollisions()
    {
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(AllObjects[it->second]->collision((it->first).first , (it->first).second , myPlayer.getHitbox()))
                AllObjects[it->second]->collisionEffect((it->first).first , (it->first).second);
    }
    bool checkAllCollisionsE(Rectangle entity)
    {
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(AllObjects[it->second]->collision((it->first).first , (it->first).second , entity))
                return true;
        return false;
    }
    void deleteClick(Vector2 pos)
    {
        Rectangle entity={pos.x,pos.y,1,1};
        for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
            if(AllObjects[it->second]->collision((it->first).first , (it->first).second , entity))
            {
                currentMap.erase(it);

                return;
            }

    }
} myMap;


class Game
{
    Exit exit;
    GameTickRate gameTick=GameTickRate(200);
public:
    bool exiting=false;
    void commands()
    {
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            hideHitbox=!hideHitbox;
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
            myPlayer.reset();
    }
    void run()
    {
        if(exiting)
            return exitscreen();
        commands();
        myPlayer.checkInput();
        for(int i=gameTick.getFrames()-1;i>=0;i--)
        {
            myPlayer.move();
            if(i%5==0)
            {
                myPlayer.isGrounded=false;
                myMap.checkAllCollisions();
                myPlayer.newMovement();
            }

        }

        draw();

        if(IsKeyPressed(KEY_ESCAPE))
        {
            exiting=true;
            gameTick.pause();
        }
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
        myMap.drawMap(transparency);
        myPlayer.draw(transparency);
        if(transparency!=255)
            exit.run();
        EndDrawing();
    }
};
