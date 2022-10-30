#include <iostream>
#include <string.h>
#include "MyHeaders/Loader.h"
#include "MyHeaders/Settings.h"
#include "MyHeaders/Game.h"
#include "MyHeaders/LevelEditor.h"

char doing[21]="MainMenu";
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
    LevelSelect lvlSelect;
    Game game;
    while(!WindowShouldClose())
    {
        mouseAction=MOUSE_CURSOR_ARROW;

        if( !strcmp(doing,"Game")) game.run();
        if( !strcmp(doing,"MainMenu")) mainMenu.run();
        if( !strcmp(doing,"LevelEditor")) lvlEditor.run();
        if( !strcmp(doing,"LevelSelect")) lvlSelect.run();
        if( !strcmp(doing,"Exiting")) break;

        if(mouseAction==MOUSE_CURSOR_ARROW)
            SetMouseCursor(mouseAction); /// exiting holdClicking from a button
    }
}
void Loader::loadMap(const char levelName[])
{
    myMap.loadMap(levelName);
}

int main()
{
    Loader::load();
    Loader::mainloop();
    Loader::unload();
}
