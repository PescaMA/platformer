#include "ExtendedRaylib.h"
extern char doing[21];
extern const int screenWidth;
extern const int screenHeight;
extern int nrOfObjects;
extern Object **AllObjects;
extern bool hideHitbox;
extern Start myStart;
extern Finish myFinish;

class ObjectSelector
{
public:
    int *lastOnPage;
    int pagesNr;
    int *currentPage;
    int *currentObject;
    void draw(int transparency)
    {
        for(int i=lastOnPage[*currentPage];i<lastOnPage[*currentPage+1];i++)
        {
            AllObjects[i]->draw(getX(i),getY(),transparency);
        }
    }
    void run()
    {
        for(int i=lastOnPage[*currentPage];i<lastOnPage[*currentPage+1];i++)
        {
            Rectangle rec={getX(i),getY(),AllObjects[i]->hitbox.width,AllObjects[i]->hitbox.height};
            if(IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(),rec))
               *currentObject=i;
        }
    }
    void buildLastOnPage()
    {
        pagesNr=AllObjects[nrOfObjects-2]->page;
        lastOnPage=new int[pagesNr+1];
        lastOnPage[0]=0;
        int poz=1;

        for(int i=1;i<nrOfObjects;i++)
            if(AllObjects[i]->page!=AllObjects[i-1]->page)
            {
                lastOnPage[poz]=i;
                poz++;
            }
        lastOnPage[poz]=nrOfObjects;
    }
protected:
    int getN()
    {
        return lastOnPage[*currentPage+1]-lastOnPage[*currentPage];
    }
    float getX (int i)
    {
        return screenWidth/2-(getN()-1)*32-16+(i-lastOnPage[*currentPage])*64;
    }
    float getY ()
    {
        return screenHeight-70;
    }
} objSel;
class LevelEditor
{
    bool exiting=false;
    int *lastOnPage;
    int MAX_PAGES;
    FixedButton  *buttons;
    int currentPage=0;
    int currentObject=1; bool isObjectShown=false;
    Exit exit;
    KBD_Move kbdMove;

public:
    LevelEditor()
    {

        objSel.currentPage=&currentPage;
        objSel.lastOnPage=lastOnPage;
        objSel.currentObject=&currentObject;
        ///we now have 2 pointers for the same 3 addresses

        objSel.buildLastOnPage();
        MAX_PAGES=objSel.pagesNr;

        buttons=new FixedButton[MAX_PAGES];
        Button *pointerArray[MAX_PAGES];
        for(int i=0;i<MAX_PAGES;i++)
        {
            pointerArray[i]=&(buttons[i]);
            int pos=screenWidth/2-(MAX_PAGES-1)*24-15+i*48;
            buttons[i]=FixedButton(TextFormat("%i",i+1),pos,screenHeight-110,30,30,28,BLACK,YELLOW);
        }
        kbdMove=KBD_Move(pointerArray,MAX_PAGES,false);

        buttons[currentPage].normalColor.text=GREEN;
        buttons[currentPage].hoverColor.text=GREEN;
    }
    void  run()
    {
        if(exiting)
            return exitscreen();
        objSel.run();
        kbdMove.run();

        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
            hideHitbox=!hideHitbox;
        if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S))
            myMap.saveMap("Levels/Lvl_Editor.txt");

        for(int i=0;i<MAX_PAGES;i++)
            if(buttons[i].Lclicked())
                changeButton(i);

        if(GetMouseY()+16>screenHeight-100)
            isObjectShown=false;
        else
            isObjectShown=true;
        if(isObjectShown)
        {
            if(IsMouseButtonDown(0) && canPlace())
            {
                Object *obj= (AllObjects[currentObject]);
                int x=GetMouseX()-obj->hitbox.x-obj->hitbox.width/2;
                int y=GetMouseY()-obj->hitbox.y-obj->hitbox.height/2;

                if(obj->UID == myStart.UID || obj->UID == myFinish.UID)
                {
                    if(obj->UID == myStart.UID)
                        myStart.x=x,myStart.y=y;
                    else
                        myFinish.x=x,myFinish.y=y;
                }
                else
                {
                    std::pair<int,int> mousepos={x,y};
                    myMap.currentMap[mousepos]=currentObject;
                }

            }

            if( (IsKeyDown(KEY_DELETE) || (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(1)))
            && myMap.checkAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),1,1}) )
                myMap.deleteClick(GetMousePosition());
        }

        draw();

        if(IsKeyPressed(KEY_ESCAPE))
        {
            exiting=true;
            isObjectShown=false;
        }

    }
    bool canPlace()
    {
        Object *obj= (AllObjects[currentObject]);
        int x=GetMouseX()-obj->hitbox.x-obj->hitbox.width/2;
        int y=GetMouseY()-obj->hitbox.y-obj->hitbox.height/2;
        return !myMap.checkAllCollisionsE(obj->getHitbox(x,y));
    }
    void exitscreen()
    {
        draw(100);
        exit.kbdMove.run();
        if(IsKeyPressed(KEY_ESCAPE) || exit.stay())
        {
            kbdMove.reset();
            exit.kbdMove.reset();
            exiting=false;
        }
        if(exit.leave())
        {
            exiting=false;
            kbdMove.reset();
            strcpy(doing,"MainMenu");
        }

    }
    void  draw(int transparency=255)
    {
        BeginDrawing();
        Color T_BLUE=BLUE;   T_BLUE.a=transparency;
        ClearBackground(T_BLUE);
        if(currentObject!=-1 && isObjectShown)
        {
            Object *obj= (AllObjects[currentObject]);
            obj->draw(GetMouseX()-obj->hitbox.x-obj->hitbox.width/2,
                     GetMouseY()-obj->hitbox.y-obj->hitbox.height/2,
                     transparency);
        }
        myMap.drawMap(transparency);
        GUI(transparency);

        if(transparency!=255)
            exit.run();
        EndDrawing();
    }
    void GUI(int transparency)
    {
        DrawRectangle(0,screenHeight-114,screenWidth,114,{237, 237, 157,(unsigned char)transparency});
        Color T_BLACK=BLACK; T_BLACK.a=transparency;
        DrawRectangle(0,screenHeight-120,screenWidth,5,T_BLACK);
        objSel.draw(transparency);
        for(int i=0;i<MAX_PAGES;i++)
            buttons[i].draw(transparency);

    }
    void selectable()
    {
        if(currentPage)
            return;
    }
    void changeButton(int newPage)
    {
        buttons[currentPage].normalColor.text=BLACK;
        buttons[currentPage].hoverColor.text=YELLOW;
        currentPage=newPage;
        buttons[newPage].normalColor.text=GREEN;
        buttons[newPage].hoverColor.text=GREEN;
    }
};
