#include <iostream>
#include <string.h>
#include "MyHeaders/RayJumpHeadears.h"

void RayJump::Loader::mainloop()
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

int main()
{
    RayJump::Loader::load();
    RayJump::Loader::mainloop();
    RayJump::Loader::unload();
}
