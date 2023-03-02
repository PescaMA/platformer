/****************************************************************************
*
*       L O A D E R S - Loader, LevelSelect
*
****************************************************************************/
#ifndef Loader_H
#define Loader_H

#include "globals.h"
#include "Settings.h"
namespace RayJump
{
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
}
#endif // Loader
