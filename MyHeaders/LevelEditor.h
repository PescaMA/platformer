
/// TO DO: make these classes readable by humanoid beings

/*********************************************
*
*               OBJ SELECTOR
*
**********************************************/
void RayJump::ObjectSelector::draw(int transparency)
{
    for(int i=lastOnPage[*currentPage]; i<lastOnPage[*currentPage+1]; i++)
    {
        AllObjects[i]->draw(getX(i),getY(),transparency);
    }
}
void RayJump::ObjectSelector::run()
{
    for(int i=lastOnPage[*currentPage]; i<lastOnPage[*currentPage+1]; i++)
    {
        Rectangle rec= {getX(i),getY(),AllObjects[i]->hitbox.width,AllObjects[i]->hitbox.height};
        if(IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(),rec))
            *currentObject=i;
    }
}
void RayJump::ObjectSelector::buildLastOnPage()
{
    pagesNr=AllObjects[nrOfObjects-2]->page;
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

int RayJump::ObjectSelector::getN()
{
    return lastOnPage[*currentPage+1]-lastOnPage[*currentPage];
}
float RayJump::ObjectSelector::getX (int i)
{
    return screenWidth/2-(getN()-1)*32-16+(i-lastOnPage[*currentPage])*64;
}
float RayJump::ObjectSelector::getY ()
{
    return screenHeight-70;
}

/*********************************************
*
*               LVL EDITOR
*
**********************************************/
RayJump::LevelEditor::LevelEditor()
{

    objSel.currentPage=&currentPage;
    objSel.lastOnPage=lastOnPage;
    objSel.currentObject=&currentObject;

    objSel.buildLastOnPage();
    MAX_PAGES=objSel.pagesNr;

    buttons=new FixedButton[MAX_PAGES];
    Button *pointerArray[MAX_PAGES];
    for(int i=0; i<MAX_PAGES; i++)
    {
        pointerArray[i]=&(buttons[i]);
        int pos=screenWidth/2-(MAX_PAGES-1)*24-15+i*48;
        buttons[i]=FixedButton(TextFormat("%i",i+1),pos,screenHeight-110,30,30,28,BLACK,YELLOW);
    }
    kbdMove=KBD_Btn_Move(pointerArray,MAX_PAGES,false);

    buttons[currentPage].normalColor.text=GREEN;
    buttons[currentPage].hoverColor.text=GREEN;
}

void RayJump::LevelEditor::run()
{
    exit.run(this);
    if(exit.state == Exit::States::starting)
        isObjectShown=false;
    if(exit.state != Exit::States::off)
        return;
    objSel.run();
    kbdMove.run();

    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
        hideHitbox=!hideHitbox;
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S))
        myMap.saveMap("Levels/Lvl_Editor.txt");

    for(int i=0; i<MAX_PAGES; i++)
        if(buttons[i].Lclicked())
            changeButton(i);

    if(GetMouseY()+16>screenHeight-100)
        isObjectShown=false;
    else
        isObjectShown=true;

    PlaceBlocks();

    draw();
}

void RayJump::LevelEditor::PlaceBlocks()
{
    if(!isObjectShown)
        return;
    Object *obj= (AllObjects[currentObject]);
    int mouseX=GetMouseX()-obj->hitbox.x-obj->hitbox.width/2;
    int mouseY=GetMouseY()-obj->hitbox.y-obj->hitbox.height/2;

    if(IsMouseButtonPressed(0))
    {
        areaColor=BLACK;
        if(!myMap.checkAllCollisionsMouse())
        {
            startX=GetMouseX()-obj->hitbox.x-obj->hitbox.width/2;
            startY=GetMouseY()-obj->hitbox.y-obj->hitbox.height/2;
        }
        else
        {
            std::pair<int,int> start=myMap.getCollisionMouse();
            if(start.first>-9999)
            {
                startX=start.first;
                startY=start.second;
                myMap.deletePair(start);
            }
            else
                areaColor=WHITE;
        }
        if(obj->UID == myStart.UID || obj->UID == myFinish.UID)
            areaColor=WHITE;
    }
    if(IsMouseButtonDown(0) && !ERay::same_color(areaColor,WHITE))
    {
        if(myMap.checkAllCollisionsE(getBigRectangle()))
            areaColor=RED;
        else
            areaColor=GREEN;
    }
    if(IsMouseButtonReleased(0))
    {
        if(obj->UID == myStart.UID || obj->UID == myFinish.UID || ERay::same_color(areaColor,WHITE))
        {
            if(!myMap.checkAllCollisionsE(obj->getHitbox(mouseX,mouseY)))
            {
                if(obj->UID == myStart.UID)
                    myStart.x=mouseX,myStart.y=mouseY;
                if(obj->UID == myFinish.UID)
                    myFinish.x=mouseX,myFinish.y=mouseY;
            }
        }
        else
            placeAll();
        areaColor=WHITE;
    }

    if( (IsKeyDown(KEY_DELETE) || (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(1)))
            && myMap.checkAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),0,0}) )
        myMap.deleteClick(GetMousePosition());
}

Rectangle RayJump::LevelEditor::getBigRectangle()
{
    Object *obj= (AllObjects[currentObject]);
    int mouseX=GetMouseX();
    int mouseY=GetMouseY();

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
    Rectangle rect=getBigRectangle();
    ///std::cout<<rect.x<<' '<<rect.y<<' '<<rect.width<<' '<<rect.height<<'\n';
    if(myMap.checkAllCollisionsE(rect))
        return;
    int width=AllObjects[currentObject]->hitbox.width;
    int height=AllObjects[currentObject]->hitbox.height;
    for(int i=0; width*i<rect.width; i++)
        for(int j=0; height*j<rect.height; j++)
        {
            std::pair<int,int> iPos= {rect.x+i*width,rect.y+j*height};
            myMap.currentMap[iPos]=currentObject;
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
    Color T_BLUE=BLUE;
    T_BLUE.a=transparency;
    ClearBackground(T_BLUE);

    drawLevel(transparency);
    drawSelector(transparency);
}

void RayJump::LevelEditor::drawLevel(int transparency)
{
    drawHeldObj(transparency);
    myMap.drawMap(transparency);
}

void RayJump::LevelEditor::drawHeldObj(int transparency)
{
    if(currentObject!=-1 && isObjectShown)
    {
        if(IsMouseButtonDown(0) && !ERay::same_color(areaColor,WHITE))
        {
            Rectangle rect=getBigRectangle();
            DrawLine(rect.x,rect.y,rect.x+rect.width,rect.y,areaColor);
            DrawLine(rect.x,rect.y,rect.x,rect.y+rect.height,areaColor);
            DrawLine(rect.x+rect.width,rect.y,rect.x+rect.width,rect.y+rect.height,areaColor);
            DrawLine(rect.x,rect.y+rect.height,rect.x+rect.width,rect.y+rect.height,areaColor);
        }
        else
        {
            Object *obj= (AllObjects[currentObject]);
            obj->draw(GetMouseX()-obj->hitbox.x-obj->hitbox.width/2,
                      GetMouseY()-obj->hitbox.y-obj->hitbox.height/2,
                      transparency);
        }

    }
}

void RayJump::LevelEditor::drawSelector(int transparency)
{
    DrawRectangle(0,screenHeight-114,screenWidth,114, {237, 237, 157,(unsigned char)transparency});
    Color T_BLACK=BLACK;
    T_BLACK.a=transparency;
    DrawRectangle(0,screenHeight-120,screenWidth,5,T_BLACK);
    objSel.draw(transparency);
    for(int i=0; i<MAX_PAGES; i++)
        buttons[i].draw(transparency);

}

void RayJump::LevelEditor::changeButton(int newPage)
{
    buttons[currentPage].normalColor.text=BLACK;
    buttons[currentPage].hoverColor.text=YELLOW;
    currentPage=newPage;
    buttons[newPage].normalColor.text=GREEN;
    buttons[newPage].hoverColor.text=GREEN;
}

