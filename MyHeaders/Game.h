/*********************************************
*
*                   G A M E
*
**********************************************/
void RayJump::Game::commands()
{
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
        RayJump::hideHitbox=!RayJump::hideHitbox;
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
        myPlayer.reset();
}
void RayJump::Game::run()
{
    if(RayJump::myFinish.won==true)
    {
        winScreen.run(this);
        return;
    }

    if(exit.state == Exit::States::starting)
        gameTick.pause();
    exit.run(this);
    if(exit.state != Exit::States::off)
        return;

    commands();
    myPlayer.checkInput();

    for(int i=gameTick.getFrames()-1; i>=0; i--)
    {
        myPlayer.move();
        if(i%5==0)
        {
            myPlayer.presume();
            RayJump::myMap.checkAllCollisions();
        }
    }
    draw();
}
void RayJump::Game::draw()
{
    BeginDrawing();
    draw_content(255);
    EndDrawing();
}
void RayJump::Game::draw_content(int transparency)
{
    Color T_BLUE=BLUE;
    T_BLUE.a=transparency;
    ClearBackground(T_BLUE);
    RayJump::myMap.drawMap(transparency);
    myPlayer.draw(transparency);
}
void RayJump::Game::restart()
{
    RayJump::myFinish.won = false;
    myPlayer.reset();
    RayJump::myMap.restartMap();
}

/*********************************************
*
*               MAP OBJECT
*
**********************************************/

void RayJump::MapObj::saveMap(std::string fileName)
{
    /// TO DO: make file maybe if aint existin'
    remove(fileName.c_str());

    std::ofstream fout(fileName);
    fout<<"start "<<RayJump::myStart.x<<' '<<RayJump::myStart.y<<'\n';
    fout<<"finish "<<RayJump::myFinish.x<<' '<<RayJump::myFinish.y<<'\n';
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        fout<<(it->first).first<<' '<<(it->first).second<<' '<<RayJump::AllObjects[it->second]->UID<<'\n';
    fout.close();
}
void RayJump::MapObj::loadMap(std::string fileName,std::string next_level_name)
{
    currentMap.clear();
    next_level_name.clear();
    this->next_level_name=next_level_name;
    std::ifstream fin(fileName);
    if(!fin)
    {
        std::cout<<"Error, file does not exist!";
        return;
    }

    char c[10];
    fin>>c;
    if(strcmp(c,"start"))
    {
        std::cout<<"\n\nError, no start provided\n\n";
        strcpy(doing,"Exiting");
        return;
    }
    fin>>RayJump::myStart.x>>RayJump::myStart.y;
    fin>>c;
    if(strcmp(c,"finish"))
    {
        std::cout<<"\n\nError, no finish provided\n\n";
        strcpy(doing,"Exiting");
        return;
    }
    fin>>RayJump::myFinish.x>>RayJump::myFinish.y;

    myPlayer.reset();

    std::pair<int,int> coord;
    int UID;
    while(fin>>coord.first)
    {
        fin>>coord.second>>UID;
        if(UID==RayJump::myStart.UID || UID==RayJump::myFinish.UID)
        {
            std::cout<<"\n\nError, multiple starts or finishes provided.\n\n";
            strcpy(doing,"Exiting");
            return;
        }
        currentMap[coord]=UID_pairing[UID];
    }
    fin.close();
}
void RayJump::MapObj::restartMap()
{
    /// great function so far
}
void RayJump::MapObj::drawMap(int transparency)
{
    RayJump::myStart.draw(transparency);
    RayJump::myFinish.draw(transparency);
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        RayJump::AllObjects[(it->second)]->draw((it->first).first,(it->first).second,transparency);
}
void RayJump::MapObj::checkAllCollisions()
{
    if(RayJump::myFinish.collision(myPlayer.getHitbox()))
        RayJump::myFinish.collisionEffect();
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
    {
        RayJump::Object *obj=RayJump::AllObjects[it->second];
        if(obj->collision((it->first).first, (it->first).second, myPlayer.getHitbox()))
            obj->collisionEffect((it->first).first, (it->first).second);
        if(obj->canWJ)
            myPlayer.tryWallJump(obj->getDir(it->first.first,it->first.second));

    }

    myPlayer.newMovement();
}
bool RayJump::MapObj::checkAllCollisionsE(Rectangle entity)
{
    if(RayJump::myFinish.collision(entity))
        return true;
    if(RayJump::myStart.collision(entity))
        return true;
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        if(RayJump::AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
            return true;
    return false;
}
bool RayJump::MapObj::checkAllCollisionsMouse()
{
    return checkAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),0,0});
}
std::pair<int,int> RayJump::MapObj::getCollisionE(Rectangle entity)
{
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        if(RayJump::AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
            return {(it->first).first, (it->first).second};
    return {-999999,-999999};
}
std::pair<int,int> RayJump::MapObj::getCollisionMouse()
{
    return getCollisionE({(float)GetMouseX(),(float)GetMouseY(),0,0});
}
void RayJump::MapObj::deletePair(std::pair<int,int> coord)
{
    currentMap.erase(currentMap.find(coord));
}
void RayJump::MapObj::deleteClick(Vector2 pos)
{
    Rectangle entity= {pos.x,pos.y,1,1};
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        if(RayJump::AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
        {
            currentMap.erase(it);
            return;
        }
}
