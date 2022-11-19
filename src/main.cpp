#include <iostream>
#include <string.h>
#include "Loader.h"
#include "Settings.h"
#include "Game.h"
#include "LevelEditor.h"

char doing[21]="MainMenu";
extern MapObj myMap;

/**********************************
*
*
* Cross header function definitions:
*
*
***********************************/
void Loader::mainloop() {
    LevelEditor lvlEditor;
    MainMenu mainMenu;
    LevelSelect lvlSelect;
    Game game;
    while(!WindowShouldClose())
    {
        mouseAction=MOUSE_CURSOR_ARROW;

        // Might i suggest doing a switch?
        if( !strcmp(doing,"Game")) game.GameLoop();
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
