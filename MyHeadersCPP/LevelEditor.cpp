
#include "../MyHeaders/RayJumpHeadears.h"

/// TO DO: add undo/redo, extra options

/*********************************************
*
*               OBJ SELECTOR
*
**********************************************/

/// bug start and finish resetting positions.

template <class a>
RayJump::ObjectSelector::ObjectSelector(a obj)
{
    currentPage=&obj->currentPage;
    currentObject=&obj->currentObject;
    isObjSpecial=&obj->isObjSpecial;
    extraInfo = &obj->extraInfo;
    *currentObject = -1;

    int pagesNr=AllObjects[nrOfObjects-1]->page;
    lastOnPage=new int[pagesNr+1];
    lastOnPage[0]=0;
    int poz=1;

    for(int i=1; i<nrOfObjects; i++)
        if(AllObjects[i]->page!=AllObjects[i-1]->page)
        {
            lastOnPage[poz]=i;
            poz++;
        }
    lastOnPage[poz]=nrOfObjects;
}
void RayJump::ObjectSelector::run()
{
    for(int i=lastOnPage[*currentPage]; i<lastOnPage[*currentPage+1]; i++)
    {
        Rectangle rec= {getX(i),getY(),32,AllObjects[i]->hitbox.height};
        if(IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(),rec))
        {

            *currentObject=i;

            Object *obj = AllObjects[*currentObject];

            *extraInfo= new int[obj->nrExtra];

            if(obj->UID == myStart->UID || obj->UID == myFinish->UID)
                *isObjSpecial=true;
            else
                *isObjSpecial=false;
        }

    }
}
int RayJump::ObjectSelector::getN()
{
    return lastOnPage[*currentPage+1]-lastOnPage[*currentPage];
}
float RayJump::ObjectSelector::getX (int i)
{ /// centers N objects with width 32 and space 32 between and calculates current pos.
    return screenInfo.width/2-16-(getN()-1)*32+(i-lastOnPage[*currentPage])*64;
}
float RayJump::ObjectSelector::getY ()
{
    return screenInfo.height-70;
}

void RayJump::ObjectSelector::draw_content(int transparency)
{
    for(int i=lastOnPage[*currentPage]; i<lastOnPage[*currentPage+1]; i++)
    {
        Object *obj = AllObjects[i];
        obj->draw(obj->makeExtra(getX(i),getY()),transparency);
    }
}

/*********************************************
*
*               Button Selector
*
**********************************************/
template<class a>
RayJump::LE_Buttons::LE_Buttons(a obj)
{
    currentPage = &obj->currentPage;
    *currentPage = 0;
    pagesNr = AllObjects[nrOfObjects-1]->page;
    buttons=new FixedButton[pagesNr];
    Button *pointerArray[pagesNr];

    for(int i=0; i<pagesNr; i++)
    {
        pointerArray[i]=&(buttons[i]);
        int pos=screenInfo.width/2-(pagesNr-1)*24-15+i*48;
        buttons[i]=FixedButton(TextFormat("%i",i+1),pos,screenInfo.height-110,30,30,28,BLACK,YELLOW);
    }
    kbdMove=KBD_Btn_Move(pointerArray,pagesNr,false);

    buttons[*currentPage].normalColor.text=GREEN;
    buttons[*currentPage].hoverColor.text=GREEN;
}

void RayJump::LE_Buttons::run()
{
    kbdMove.run();
    for(int i=0; i<pagesNr; i++)
    {
        int pos=screenInfo.width/2-(pagesNr-1)*24-15+i*48;
        buttons[i].rect.x = pos;
        buttons[i].rect.y = screenInfo.height - 110;
    }
    for(int i=0; i<pagesNr; i++)
        if(buttons[i].Lclicked())
            changeButton(i);
}
void RayJump::LE_Buttons::changeButton(int newPage)
{
    buttons[*currentPage].normalColor.text=BLACK;
    buttons[*currentPage].hoverColor.text=YELLOW;
    *currentPage=newPage;
    buttons[newPage].normalColor.text=GREEN;
    buttons[newPage].hoverColor.text=GREEN;
}
void RayJump::LE_Buttons::draw_content(int transparency)
{
    for(int i=0; i<pagesNr; i++)
        buttons[i].draw(transparency);
}

/*********************************************
*
*               LVL EDITOR
*
**********************************************/

RayJump::LevelEditor::LevelEditor():objSel(this),buttons(this)
{

}

void RayJump::LevelEditor::run()
{

    exit.run(this);
    if(exit.state == Exit::States::starting)
        isObjectShown=false;
    if(exit.state != Exit::States::off)
        return;

    objSel.run();
    buttons.run();

    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
        hideHitbox=!hideHitbox;
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S))
        myMap->saveMap("Levels/Lvl_Editor.txt");

    if(GetMouseY()+16>screenInfo.height-100)
        isObjectShown=false,state=selecting;
    else
    {
        isObjectShown=true;
        if(state == selecting)
            state=holding;
    }

    if(state == holding)
    {
        if(IsMouseButtonDown(0))
            state=building;
    }

    PlaceBlocks();

    draw();
}
/// fix down

void RayJump::LevelEditor::PlaceBlocks()
{
    if(!isObjectShown || currentObject==-1)
        return;

    getPlaceStart();/// calculates placing coordinates
    setFillColor();
    placeAll();

    deleteObj();
}
void RayJump::LevelEditor::getPlaceStart()
{
    if(!IsMouseButtonPressed(0))
        return;
    state=building;

    Object *obj= (AllObjects[currentObject]);
    std::pair<int,int> start=myMap->getCollisionMouse();

    if(start.first <= -9999)
    {
        startX=obj->makeXCentered(GetMouseX());
        startY=obj->makeYCentered(GetMouseY());
        return;
    }
    startX=start.first;
    startY=start.second;

    if(!isObjSpecial)
        myMap->deletePair(start);
}
void RayJump::LevelEditor::setFillColor()
{
    if(state!=building)
        return;
    Object *obj= (AllObjects[currentObject]);
    bool collisions = false;
    if(isObjSpecial)
        collisions=myMap->onlyUID(getBigRectangle(),obj->UID);
    else
        collisions=!myMap->checkAllCollisionsE(getBigRectangle());
    if(collisions)
        areaColor=GREEN;
    else
        areaColor=RED;
}
void RayJump::LevelEditor::deleteObj()
{
    bool isCommandPressed = IsKeyDown(KEY_DELETE) || (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(1));
    if(isCommandPressed)
        myMap->deleteClick(GetMousePosition());
}
Rectangle RayJump::LevelEditor::getBigRectangle()
{
    Object *obj= (AllObjects[currentObject]);
    int mouseX=GetMouseX();
    int mouseY=GetMouseY();
    if(isObjSpecial)
        return obj->getHitbox(obj->makeExtra(obj->makeXCentered(GetMouseX() )
                                                ,obj->makeYCentered(GetMouseY() ) ));

    int nrHorObjs= ERay::abs(mouseX-startX)/obj->hitbox.width + 1;
    int nrVerObjs= ERay::abs(mouseY-startY)/obj->hitbox.height + 1;

    int X = (startX<mouseX) ? startX : (startX - obj->hitbox.width*nrHorObjs);
    int Y = (startY<mouseY) ? startY : (startY - obj->hitbox.height*nrVerObjs);

    if(mouseX<startX)
        nrHorObjs++;
    if(mouseY<startY)
        nrVerObjs++;
    return {(float)X,(float)Y,nrHorObjs*obj->hitbox.width,nrVerObjs*obj->hitbox.height};
}

void RayJump::LevelEditor::placeAll()
{
    if(!IsMouseButtonReleased(0) || state != building)
        return;

    state=holding; /// what happens if we stop clicking in the select place??

    Object *obj= (AllObjects[currentObject]);

    if(isObjSpecial)
    {
        if(!myMap->onlyUID(getBigRectangle(),obj->UID))
            return;

        int mouseX=obj->makeXCentered(GetMouseX());
        int mouseY=obj->makeYCentered(GetMouseY());
        if(obj->UID == myStart->UID)
        {
            myStart->v[0]=mouseX;
            myStart->v[1]=mouseY;
        }
        if(obj->UID == myFinish->UID)
        {
            myFinish->v[0]=mouseX;
            myFinish->v[1]=mouseY;
        }
        return;
    }
    if(myMap->checkAllCollisionsE(getBigRectangle()))
        return;

    Rectangle rect=getBigRectangle();
    int width=obj->hitbox.width;
    int height=obj->hitbox.height;
    int UID = AllObjects[currentObject]->UID;

    for(int i=0; width*i<rect.width; i++){
        for(int j=0; height*j<rect.height; j++)
        {
            std::pair<int,int> iPos= {rect.x+i*width,rect.y+j*height};

            std::pair<int,int*>rez;
            rez.first=UID;
            rez.second = new int[obj->nrExtra];
            std::copy(extraInfo, extraInfo+obj->nrExtra, rez.second);
            rez.second[0]=iPos.first;
            rez.second[1]=iPos.second;


            myMap->lvlObj[iPos]=rez;
        }
    }
}

void RayJump::LevelEditor::draw()
{
    BeginDrawing();
    draw_content(255);
    EndDrawing();
}

void RayJump::LevelEditor::draw_content(int transparency)
{
    ClearBackground(BLUE);

    Rectangle BKGdrawnPart = { 0.0f, 0.0f, 256.0f, 256.0f };
    ERay::drawTextureDest(ASSET_BACKGROUND, BKGdrawnPart, screenInfo);

    drawLevel(transparency);
    drawSelector(transparency);
}

void RayJump::LevelEditor::drawLevel(int transparency)
{
    drawHeldObj(transparency);
    drawBuildRect(transparency);
    myMap->drawMap(transparency);
}
void RayJump::LevelEditor::drawBuildRect(int transparency)
{
    if(state != building)
        return;
    if(IsMouseButtonDown(0) && !ERay::same_color(areaColor,WHITE))
    {
        Rectangle rect=getBigRectangle();
        DrawRectangleLinesEx(rect,2,areaColor);
    }
}
void RayJump::LevelEditor::drawHeldObj(int transparency)
{
    if(currentObject==-1 || (state != holding && !isObjSpecial))
        return;

    Object *obj= (AllObjects[currentObject]);
    obj->draw(obj->makeExtra(obj->makeXCentered(GetMouseX()),
                             obj->makeYCentered(GetMouseY())),
              transparency);
}

void RayJump::LevelEditor::drawSelector(int transparency)
{
    DrawRectangle(0,screenInfo.height-114,screenInfo.width,114, {237, 237, 157,(unsigned char)transparency});
    Color T_BLACK=BLACK;
    T_BLACK.a=transparency;
    DrawRectangle(0,screenInfo.height-120,screenInfo.width,5,T_BLACK);
    objSel.draw_content(transparency);
    buttons.draw_content(transparency);
}
