#include "../MyHeaders/globals.h"

/// file needed because it's only used once, unlike its header. Which used to make an error.
namespace RayJump
{
    char doing[21]="MainMenu";
    const int iscreenWidth=800;
    const int iscreenHeight=600;
    int fps;
    int hideHitbox=false;
    int nrOfObjects;
    std::map<int,int> UID_pairing;
    int const gameTicks = 200;
    Texture2D ASSET_CHARACTER;
    Texture2D ASSET_BLOCKS;
    Texture2D ASSET_SPECIAL;
    Texture2D ASSET_BACKGROUND;
}
