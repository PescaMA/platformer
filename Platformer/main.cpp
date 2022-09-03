#include <iostream>
#include <string.h>
#include "MyHeaders/Loader.h"
#include "MyHeaders/Settings.h"
#include "MyHeaders/Game.h"
#include "MyHeaders/LevelEditor.h"

char doing[21]="Game";
extern MapObj myMap;

/**********************************
*
*
* Cross header function definitions:
*
*
***********************************/
void Loader::mainloop()
{
    LevelEditor lvlEditor;
    MainMenu mainMenu;
    Game game;
    while(!WindowShouldClose())
    {
        if( !strcmp(doing,"Game")) game.run();
        if( !strcmp(doing,"MainMenu")) mainMenu.run();
        if( !strcmp(doing,"LevelEditor")) lvlEditor.run();
        if( !strcmp(doing,"Exiting")) break;
    }
}
void Loader::loadFirstMap()
{
    myMap.loadMap("Levels/Lvl1.txt");
}

int main()
{
    Loader::load();
    Loader::mainloop();
    Loader::unload();
}
