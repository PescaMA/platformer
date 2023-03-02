/*********************************************
*
*                   G A M E
*
**********************************************/
#include <vector>
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
    {///skipping some collision checks for very high frame rates. End with 0 so modulo 5 works at least once.
        myPlayer.move();
        if(i%5==0)
        {
            myPlayer.presume();
            myMap.checkPlayerCollisions();
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
    ClearBackground(BLUE);

    Rectangle BKGdrawnPart = { 0.0f, 0.0f, 256.0f, 256.0f };
    ERay::drawTextureDest(ASSET_BACKGROUND, BKGdrawnPart, screenInfo);

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
    fout<<"start "<<myStart.v[Start::xa]<<' '<<myStart.v[Start::ya]<<'\n';
    fout<<"finish "<<myFinish.v[Finish::xa]<<' '<<myFinish.v[Finish::ya]<<'\n';
    for(objInfo it=lvlObj.begin();it!=lvlObj.end();it++)
    {
        int UID = (it->second).first;
        fout<<UID;
        int sz = AllObjects[UID_pairing[UID]]->extraCount;
        for(int i=0;i<sz;i++)
            fout<<' '<<(it->second).second[i];
        fout<<'\n';
    }
    fout.close();
}
void RayJump::MapObj::loadMap(std::string fileName,std::string next_level_name)
{/// TO DO: add stuf 4 order
    /// loads map with a couple verifications for being only 1 start and finish
    lvlObj.clear();
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
    for(int i=0;i<Start::extra::nrExtra;i++)
        fin>>myStart.v[i];
    fin>>c;
    if(strcmp(c,"finish"))
    {
        std::cout<<"\n\nError, no finish provided\n\n";
        strcpy(doing,"Exiting");
        return;
    }
    for(int i=0;i<Finish::extra::nrExtra;i++)
        fin>>myStart.v[i];

    myPlayer.reset();

    int UID;
    while(fin>>UID)
    {
        if(UID==myStart.UID || UID==myFinish.UID)
        {
            std::cout<<"\n\nError, multiple starts or finishes provided.\n\n";
            strcpy(doing,"Exiting");
            return;
        }
        int sz = AllObjects[UID_pairing[UID]]->extraCount;
        int *v=new int[sz];
        for(int i=0;i<sz;i++)
            fin>>v[i];
        lvlObj[{v[0],v[1]}]={UID,v};
    }
    fin.close();
}
void RayJump::MapObj::restartMap()
{
    /// great function so far
}
int RayJump::MapObj::getUID(std::pair<int,int> coord)
{
    if(lvlObj.find(coord)!=lvlObj.end())
        return lvlObj[coord].first;
    if(myStart.v[Start::xa] == coord.first && myStart.v[Start::ya] == coord.second)
        return myStart.UID;
    if(myFinish.v[Finish::xa] == coord.first && myFinish.v[Finish::ya] == coord.second)
        return myFinish.UID;
    return -9999;
}
bool RayJump::MapObj::onlyUID(Rectangle rect, int UID)
{
    std::vector <std::pair<int,int>> collisions;
    collisions = getAllCollisionsE(rect);
    for(int unsigned i=0;i<collisions.size();i++)
        if(getUID(collisions[i]) != UID)
            return false;

    return true;
}
void RayJump::MapObj::deletePair(std::pair<int,int> coord)
{
    if(lvlObj.find(coord)!=lvlObj.end())
        lvlObj.erase(lvlObj.find(coord));
}
void RayJump::MapObj::deleteClick(Vector2 pos)
{
    Rectangle entity= {pos.x,pos.y,1,1};

    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
    {
        if(AllObjects[UID_pairing[(it->second).first]]->collision((it->second).second, entity))
        {
            lvlObj.erase(it);
        }
    }

}

void RayJump::MapObj::drawMap(int transparency)
{
    myStart.draw(transparency);
    myFinish.draw(transparency);///TO DO: fix world hunger or this code
    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
    {
        AllObjects[UID_pairing[(it->second).first]]->draw((it->second).second,transparency);
    }


}

bool RayJump::MapObj::checkAllCollisionsE(Rectangle entity)
{
    if(myFinish.collision(entity))
        return true;
    if(myStart.collision(entity))
        return true;
    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
        if(AllObjects[UID_pairing[(it->second).first]]->collision((it->second).second, entity))
            return true;
    return false;
}
bool RayJump::MapObj::checkSolidCollisionsE(Rectangle entity)
{
    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
        if(AllObjects[UID_pairing[(it->second).first]]->collision((it->second).second, entity))
            if(AllObjects[UID_pairing[(it->second).first]]->isSolid)
                return true;
    return false;
}
std::vector<std::pair<int,int>> RayJump::MapObj::getAllCollisionsE(Rectangle entity)
{
    std::vector <std::pair<int,int>> result;
    if(myFinish.collision(entity))
        result.push_back({myFinish.v[Finish::xa],myFinish.v[Finish::ya]});
    if(myStart.collision(entity))
        result.push_back({myStart.v[Start::xa],myStart.v[Start::ya]});
    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
        if(AllObjects[UID_pairing[(it->second).first]]->collision((it->second).second, entity))
            result.push_back(it->first);
    return result;
}
void RayJump::MapObj::checkPlayerCollisions()
{
    if(myFinish.collision(myPlayer.getHitbox()))
        myFinish.collisionEffect();
    for(objInfo it=lvlObj.begin(); it!=lvlObj.end(); it++)
    {
        Object *obj=AllObjects[UID_pairing[(it->second).first]];
        if(obj->collision((it->second).second, myPlayer.getHitbox()))
            obj->collisionEffect((it->second).second);
        if(obj->canWJ)
            myPlayer.tryWallJump(obj->getDir((it->second).second));
    }

    myPlayer.newMovement();
}
bool RayJump::MapObj::checkAllCollisionsMouse()
{
    return checkAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),0,0});
}
std::pair<int,int> RayJump::MapObj::getCollisionMouse()
{
    std::vector<std::pair<int,int>> collisions;
    collisions = getAllCollisionsE({(float)GetMouseX(),(float)GetMouseY(),0,0});
    if(collisions.size()>0)
        return collisions[0];
    else
        return {-99999,-99999};
}
