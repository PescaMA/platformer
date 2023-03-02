#ifndef Globalss
#define Globalss
#include <map>
#include <raylib.h>

namespace RayJump
{
    extern char doing[21];
    extern const int iscreenWidth;
    extern const int iscreenHeight;
    extern int fps;
    extern int hideHitbox;
    extern int nrOfObjects;
    extern std::map<int,int> UID_pairing;
    extern int const gameTicks;
    extern Texture2D ASSET_CHARACTER;
    extern Texture2D ASSET_BLOCKS;
    extern Texture2D ASSET_SPECIAL;
    extern Texture2D ASSET_BACKGROUND;
}
#endif // Globals
