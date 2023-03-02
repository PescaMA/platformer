
/*********************************************
*
*               E X I T
*
**********************************************/
RayJump::Exit::Exit()
{
    Button *all[]= {&yes,&no};
    int n=sizeof(all)/sizeof(all[0]);
    kbdMove=KBD_Btn_Move(all,n,false,1);
}
void RayJump::Exit::align()
{
    Srect.update();
    sure.align();
}
template <class drawable>
void RayJump::Exit::run(drawable background)
{
    /// if we want to return to parent or main menu we close exit menu
    if(state == returning || state == exiting)
        state = off;

    /// After we signaled to parent we enter exit mode
    if(state == starting)
        state = going;

    /// Starting exit screen
    if(state == off && IsKeyPressed(KEY_ESCAPE))
        state=starting,kbdMove.reset();

    /// If we are in parent
    if(state == off)
        return;

    /// Returning to parent
    if(state == going && (IsKeyPressed(KEY_ESCAPE) || no.Lclicked()))
        state=returning;

    /// Going to Main
    if(state == going && yes.Lclicked())
    {
        state =exiting;
        strcpy(doing,"MainMenu");
    }
    align();

    kbdMove.run();
    BeginDrawing();

    background->draw_content(100); /// draw the parent's draw content as blurred background
    DrawRectangleRec(Srect.rect,GRAY);
    sure.draw();
    yes.draw();
    no.draw();

    EndDrawing();
}

/*********************************************
*
*               WIN SCREEN
*
**********************************************/
RayJump::Win_Screen::Win_Screen()
{
    Button *all[]= {&restart};
    int n=sizeof(all)/sizeof(all[0]);
    kbdMove=KBD_Btn_Move(all,n,false,0);
}
template <class Game>
void RayJump::Win_Screen::run(Game game)
{
    align();
    kbdMove.run();
    if(restart.Lclicked())
        game->restart();
    draw();
}
void RayJump::Win_Screen::align()
{
    message.align();
}
void RayJump::Win_Screen::draw()
{
    BeginDrawing();
    ClearBackground(BLUE);
    message.draw();
    message.underline();
    restart.draw();
    EndDrawing();
}

/*********************************************
*
*                 MAIN MENU
*
**********************************************/
RayJump::MainMenu::MainMenu()
{
    Button *bList[]= {&playOn,&lvlSelect,&lvlEditor,&exit};
    int n=sizeof(bList)/sizeof(bList[0]);
    kbdMove=KBD_Btn_Move(bList,n);
}
void RayJump::MainMenu::run()
{
    if(playOn.Lclicked())
    {
        strcpy(doing,"Game");
        Loader::loadMap("Levels/Lvl_Editor.txt");
        ///Loader::loadMap("Levels/Lvl_1.txt");
    }
    if(lvlSelect.Lclicked())
        strcpy(doing,"LevelSelect");
    if(lvlEditor.Lclicked())
    {
        strcpy(doing,"LevelEditor");
        Loader::loadMap("Levels/Lvl_Editor.txt");
    }

    if(exit.Lclicked())
        strcpy(doing,"Exiting");
    recalcuate();

    /// if we want to leave
    if(strcmp(doing,"MainMenu"))
        kbdMove.reset();

    draw();
    kbdMove.run();
}
void RayJump::MainMenu::recalcuate()
{
    name.align();
}
void RayJump::MainMenu::draw()
{
    BeginDrawing();
    ClearBackground(YELLOW);
    name.draw(true);
    playOn.draw();
    lvlSelect.draw();
    lvlEditor.draw();
    exit.draw();
    EndDrawing();
}

/*********************************************
*
*                SETTINGS
*
**********************************************/
/// phenomenal
void RayJump::Settings::run() {}
