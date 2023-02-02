/*********************************************
*
*                   G A M E
*
**********************************************/
void RayJump::Game::commands()
{
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_H))
        hideHitbox=!hideHitbox;
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R))
        myPlayer.reset();
}
void RayJump::Game::run()
{
    if(myFinish.won==true)
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
    {///skipping some collision checks for very high frame rates. End in 0 so modulo works at least once.
        myPlayer.move();
        if(i%5==0)
        {
            myPlayer.presume();
            myMap.checkAllCollisions();
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
    myMap.drawMap(transparency);
    myPlayer.draw(transparency);
}
void RayJump::Game::restart()
{
    myFinish.won = false;
    myPlayer.reset();
    myMap.restartMap();
}

/*********************************************
*
*               MAP OBJECT
*
**********************************************/

void RayJump::MapObj::saveMap(std::string fileName)
{
    std::ofstream fout(fileName);
    fout<<"start "<<myStart.x<<' '<<myStart.y<<'\n';
    fout<<"finish "<<myFinish.x<<' '<<myFinish.y<<'\n';
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        fout<<(it->first).first<<' '<<(it->first).second<<' '<<AllObjects[it->second]->UID<<'\n';
    fout.close();
}
void RayJump::MapObj::loadMap(std::string fileName,std::string next_level_name)
{/// TO DO: add stuf 4 order
    /// loads map with a couple verifications for being only 1 start and finish
    currentMap.clear();
    this->next_level_name=next_level_name;
    std::ifstream fin(fileName);
    if(!fin)
    {
        if(fileName != "Levels/Lvl_Editor.txt")
        {
            std::cout<<"\n\n Error, file does not exist!\n\n";
            strcpy(doing,"Exiting");
        }
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
    fin>>myStart.x>>myStart.y;
    fin>>c;
    if(strcmp(c,"finish"))
    {
        std::cout<<"\n\nError, no finish provided\n\n";
        strcpy(doing,"Exiting");
        return;
    }
    fin>>myFinish.x>>myFinish.y;

    myPlayer.reset();

    std::pair<int,int> coord;
    int UID;
    while(fin>>coord.first)
    {
        fin>>coord.second>>UID;
        if(UID==myStart.UID || UID==myFinish.UID)
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
    myStart.draw(transparency);
    myFinish.draw(transparency);
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        AllObjects[(it->second)]->draw((it->first).first,(it->first).second,transparency);
}
void RayJump::MapObj::checkAllCollisions()
{
    if(myFinish.collision(myPlayer.getHitbox()))
        myFinish.collisionEffect();
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
    {
        Object *obj=AllObjects[it->second];
        if(obj->collision((it->first).first, (it->first).second, myPlayer.getHitbox()))
            obj->collisionEffect((it->first).first, (it->first).second);
        if(obj->canWJ)
            myPlayer.tryWallJump(obj->getDir(it->first.first,it->first.second));

    }

    myPlayer.newMovement();
}
bool RayJump::MapObj::checkAllCollisionsE(Rectangle entity)
{
    if(myFinish.collision(entity))
        return true;
    if(myStart.collision(entity))
        return true;
    for(std::map <std::pair<int,int>,int>::iterator it=currentMap.begin(); it!=currentMap.end(); it++)
        if(AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
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
        if(AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
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
        if(AllObjects[it->second]->collision((it->first).first, (it->first).second, entity))
        {
            currentMap.erase(it);
            return;
        }
}
