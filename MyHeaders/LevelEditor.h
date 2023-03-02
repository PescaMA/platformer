#ifndef LVLEDITOR_H
#define LVLEDITOR_H
#include "ExtendedRaylib.h"
#include "Settings.h"
#include "globals.h"
/****************************************************************************
*
*Level Editor - Buttons selector, Object Selector, which are all parts of Editor
*
****************************************************************************/
namespace RayJump{
    class LE_Buttons
    {
    public:
        FixedButton  *buttons;
        KBD_Btn_Move kbdMove;
        int pagesNr;
        int *currentPage;

        template<class a>
        LE_Buttons(a obj);
        void run();
        void changeButton(int newPage);
        void draw_content(int transparency);
    };
    class ObjectSelector
    {
    public:
        int *lastOnPage;/// array
        int *currentPage; /// pointer
        int *currentObject;
        int **extraInfo;
        bool *isObjSpecial;
        template <class a>
        ObjectSelector(a obj);
        void draw_content(int transparency);
        void run();
    protected:
        int getN();
        float getX (int i);
        float getY ();
        int *getInfo(int i);
    };

    class LevelEditor
    {
        int startX=0,startY=0;
        Color areaColor=WHITE;
        RayJump::Exit exit;
        ObjectSelector objSel;
        LE_Buttons buttons;
        enum States{selecting,holding,building};
        States state;
    public:
        bool isObjectShown=false;
        int currentObject;
        int currentPage;
        int* extraInfo;
        bool isObjSpecial=false;

        LevelEditor();
        void run();
        void PlaceBlocks();
        void getPlaceStart();
        Rectangle getBigRectangle();
        void placeAll();
        void setFillColor();
        void deleteObj();
        void draw();
        void draw_content(int transparency);
        void drawLevel(int transparency);
        void drawHeldObj(int transparency);
        void drawBuildRect(int transparency);
        void drawSelector(int transparency);

    };
}

#endif // LVLEDITOR_H
