#include "../MyHeaders/ExtendedRaylib.h"
int mouseAction;
Rectangle screenInfo;
void updateScreenSize()
{
    screenInfo = {0,0, (float)GetScreenWidth(),(float)GetScreenHeight()};
}
namespace ERay
{
    void fullscreen(int iSw,int iSh,int &Sw,int &Sh)
    {
        int display=GetCurrentMonitor();
        if (IsWindowFullscreen())
        {
            ToggleFullscreen();
            SetWindowSize(iSw,iSh);
        }
        else
        {
            SetWindowSize(GetMonitorWidth  (display),
                GetMonitorHeight (display));
            WaitTime(10);
            ToggleFullscreen();
        }
        Sw=GetScreenWidth();
        Sh=GetScreenHeight();
    }
    bool same_color(Color a,Color b)
    {
        return a.a==b.a && a.b==b.b && a.g==b.g && a.r==b.r;
    }
    bool collision2D(Camera2D c,Rectangle r)
    {
        ///position of camera = c.offset.x-c.target.x
        ///distance between camera and rectangle = (c.target.x-r.x) = L
        ///distance modified by zooming = L*c.zoom = N
        ///new rectangle x =c.target-N
        ///simplify -c.target +c.target
        r.x=c.offset.x-(c.target.x-r.x)*c.zoom;
        r.y=c.offset.y-(c.target.y-r.y)*c.zoom;
        r.width*=c.zoom;
        r.height*=c.zoom;
        return CheckCollisionPointRec(GetMousePosition(),r);
    }
    Rectangle smart_paint(float xStart,float size,short xOrientation,float yOrientation)
    {/// can flip images
        return {xStart ,0, size*xOrientation, size*yOrientation};
    }
    Rectangle getWindowSize()
    {
        return {0,0,(float)GetScreenWidth(),(float)GetScreenHeight()};
    }
    void align(float &coord,int startCoord,int length,int percent,int textSize)
    {
        coord=startCoord + length*percent/100 -textSize/2;
        if(coord + textSize > startCoord + length)
            coord = startCoord + length - textSize;
        if(coord<startCoord)
            coord=startCoord;
    }
    void drawTextureDest(Texture2D asset, Rectangle drawnPart, Rectangle destination)
    {
        /// not entirely sure what all the parameters mean but seems to work.
        NPatchInfo ninePatchInfo = { drawnPart, 0, 0, 0, 0, NPATCH_NINE_PATCH };
        DrawTextureNPatch(asset, ninePatchInfo, destination, {0,0}, 0.0f, WHITE);
    }

};

    Txt::Txt(char const text[105],int x,int y,int fontSize,Color color)
    {
        this->x=x;
        this->y=y;
        this->Fsz=fontSize;
        strcpy(this->text,text);
        this->color=color;
    }
    void Txt::draw(bool shouldUnderline)
    {
        DrawText(text,x,y,Fsz,color);
        if(shouldUnderline)
            underline();
    }
    void Txt::underline()
    {
        int len=MeasureText(text,Fsz);
        Vector2 init= {x,y+Fsz};
        Vector2 endp= {x+len,y+Fsz};
        DrawLineEx(init,endp,Fsz/20+1,color);
    }
    TxtAligned::TxtAligned(char const text[105],Rectangle *container,float xPercent,float yPercent,int fontSize,Color color)
    :Txt(text,container->x,container->y,fontSize,color)
    {
        this->container = container;
        this->xPercent = xPercent;
        this->yPercent = yPercent;
        align();
    }
    void TxtAligned::align()
    {
        ERay::align(x,container->x,container->width,xPercent,MeasureText(text,Fsz));
        ERay::align(y,container->y,container->height,yPercent,Fsz);
    }


    Button::Button(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor)
    {
        strcpy(this->text,text);
        thickness=std::max(1,fontSize/10);
        this->fontSize=fontSize;
        rect= {(float)startX,(float)startY,
        (float)MeasureText(text,fontSize)+10+thickness*2 , fontSize+10+thickness*2};
        normalColor.text=color;
        (this->hoverColor).text=hoverColor;
        normalColor.background=(this->hoverColor).background=BLANK;
    }
    ColorPair Button::getCurrentColor(int transparency)
    {
        ColorPair color;
        if(isHovering || forceHover)
            color=hoverColor;
        else
            color=normalColor;
        color.text.a=std::min(transparency,(int)color.text.a);
        color.background.a=std::min(transparency,(int)color.background.a);
        return color;
    }
    void Button::draw(int transparency)
    {
        ColorPair color=getCurrentColor(transparency);
        DrawRectangleRec(rect,color.background);
        DrawRectangleLinesEx(rect,thickness,color.text);
        DrawText(text,rect.x+5+thickness,rect.y+5+thickness,fontSize,color.text);
    }
    bool Button::Lclicked()
    {
        return clicked(0);
    }
    bool Button::Rclicked()
    {
        return clicked(1);
    }
    bool Button::clicked(int nr)
    {
        Vector2 mouse=GetMousePosition();
        if(forceHover && IsKeyPressed(KEY_ENTER))
        {
            forceHover=false;
            return true;
        }

        if (CheckCollisionPointRec(mouse, rect))
        {
            isHovering=true;
            mouseAction=MOUSE_CURSOR_POINTING_HAND;
            SetMouseCursor(mouseAction);
            if(IsMouseButtonPressed(nr))
                return true;
        }
        else
            isHovering=false;
        return false;
    }
    void Button::re_measure()
    {
        rect.width=MeasureText(text,rect.height-5)+10;
    }

    ButtonAligned::ButtonAligned(char const text[105],Rectangle *container,int xPercent,int yPercent,int fontSize,Color color,Color hoverColor)
    :Button(text,container->x,container->y,fontSize,color,hoverColor)
    {
        this->xPercent=xPercent;
        this->yPercent=yPercent;
        this->container=container;
        align();
    }
    bool ButtonAligned::Lclicked()
    { /// so it auto aligns when checking for clicked
        align();
        return Button::Lclicked();
    }
    void ButtonAligned::align()
    {
        ERay::align(rect.x,container->x,container->width,xPercent,rect.width);
        ERay::align(rect.y,container->y,container->height,yPercent,rect.height);
    }

    ButtonOnOff::ButtonOnOff(int startX,int startY,int fontSize,bool isOn,Color colorOn,Color colorOff)
    :Button("",startX,startY,fontSize,colorOn,colorOff)
    {
        this->isOn=isOn;
    }
    void ButtonOnOff::switchValue()
    {
        if(isOn)
            strcpy(text,"On");
        else
            strcpy(text,"Off");
        re_measure();
    }
    ColorPair ButtonOnOff::getCurrentColor(int transparency)
    {
        ColorPair color;
        if(isHovering)
            color.background=hoverColor.background;
        else
            color.background=normalColor.background;
        if(isOn)
            color.text=normalColor.text;
        else
            color.text=hoverColor.text;
        color.text.a=std::min(transparency,(int)color.text.a);
        color.background.a=std::min(transparency,(int)color.background.a);
        return color;
    }
    bool ButtonOnOff::clicked(int nr)
    {
        Vector2 mouse=GetMousePosition();
        if (CheckCollisionPointRec(mouse, rect))
        {
            isHovering=true;
            if(IsMouseButtonPressed(nr))
            {
                isOn=!isOn;
                switchValue();
                return true;
            }
        }
        else
            isHovering=false;
        return false;
    }

    ButtonInput::ButtonInput(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor)
    :Button(text,startX,startY,fontSize,color,hoverColor){}
    bool ButtonInput::getinputI(int &rez,int max_value,char const ending[])
    {
        int c=0;
        c=GetCharPressed();
        if (c>='0' && c<='9')
        {
            if(rez*10+c-'0'<=max_value)
                rez=rez*10+c-'0';
        }
        if(IsKeyPressed(KEY_BACKSPACE))
            rez/=10;
        strcpy(text,TextFormat("%i",rez));
        strcat(text,ending);
        re_measure();

        if(IsKeyPressed(KEY_ENTER) ||
        not_clicked() ||
        IsKeyPressed(KEY_ESCAPE))
            return true;
        return false;
    }
    bool ButtonInput::not_clicked()
    {
        Vector2 mouse=GetMousePosition();
        if (!CheckCollisionPointRec(mouse, rect) && (IsMouseButtonPressed(0) || IsMouseButtonPressed(1)))
                return true;
        return false;
    }

    FixedButton::FixedButton(char const text[],int startX,int startY,int width,int height,int fontSizeee,Color color,Color hoverColor)
    :Button(text,1,1,fontSizeee,color,hoverColor)
    {
        rect= {(float)startX,(float)startY,(float)width,(float)height};
    }
    void FixedButton::draw(int transparency)
    {
        ColorPair color=getCurrentColor(transparency);
        DrawRectangleRec(rect,color.background);
        DrawRectangleLinesEx(rect,thickness,color.text);
        float centerX=rect.x+(rect.width/2-MeasureText(text,fontSize)/2);
        float centerY=rect.y+(rect.height/2-fontSize/2.0);
        DrawText(text,centerX,centerY,fontSize,color.text);
    }

    ScaledRectangle::ScaledRectangle(Rectangle *parent,float startingXpercent,float endingXpercent,float startingYpercent,float endingYpercent)
    {
        this->parent = parent;
        xPrc1 = startingXpercent;
        xPrc2 = endingXpercent;
        yPrc1 = startingYpercent;
        yPrc2 = endingYpercent;
        update();
    }
    void ScaledRectangle::update()
    {
        float x1 = parent->x + parent->width * xPrc1 / 100;
        float y1 = parent->y + parent->height * yPrc1 / 100;
        float x2 = parent->x + parent->width * xPrc2 / 100;
        float y2 = parent->y + parent->height * yPrc2 / 100;
        rect={x1,y1,x2-x1,y2-y1};
    }

    void KBD_Btn_Hold::reset()
    {
        initialTime=-3;
    }
    int KBD_Btn_Hold::run(int frontKey,int backKey)
    {
        if(IsKeyReleased(frontKey) || IsKeyReleased(backKey))
        { /// if we just stopped hitting a key
            reset();
            return 0;
        }
        if( !(IsKeyDown(frontKey) || IsKeyDown(backKey)) )
            return 0; /// if no key is pressed

        if(initialTime<0)
            initialTime=getTimeMS();

        long long currentTime = getTimeMS();
        long long timePassed = currentTime - initialTime;
        long long movement = (timePassed - INITIAL_DELAY)/MOVEMENT_DELAY;

        if(movement<=0)
            return 0;

        initialTime += movement*MOVEMENT_DELAY;

        if(IsKeyDown(frontKey))
            return movement;
        return -movement;
    }

    KBD_Btn_Move::KBD_Btn_Move(Button *myArray[],int n,bool vertical,int defaultPosition)
    {
        if(n<1)
            return;
        this->n=n;
        this->defaultPosition=this->currentPosition=defaultPosition;

        this->myArray=new Button* [n];
        for(int i=0;i<n;i++)
            this->myArray[i]=myArray[i];

        if(vertical)
        {
            backKey=KEY_UP;
            frontKey=KEY_DOWN;
        }
        else
        {
            backKey=KEY_LEFT;
            frontKey=KEY_RIGHT;
        }
    }
    void KBD_Btn_Move::reset()
    {
        myArray[currentPosition]->forceHover=false;
        currentPosition=defaultPosition;
        hold.reset();
    }
    void KBD_Btn_Move::run()
    {
        if(n<1)
            return;
        if(IsKeyPressed(frontKey))
            move(1);
        if(IsKeyPressed(backKey))
            move(-1);

        move(hold.run(frontKey,backKey));
    }
    void KBD_Btn_Move::move(int delta)
    {
        myArray[currentPosition]->forceHover=false;
        currentPosition+=delta;
        currentPosition= (currentPosition+100*n)%n;
        myArray[currentPosition]->forceHover=true;
    }

long long getTimeMS()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
long long getTimeMCS()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

    GameTickRate::GameTickRate(int tickrate)
    {
        this->tickrate=1000000/tickrate;
        reset();
    }
    void GameTickRate::reset()
    {
        prevTime=0;
    }
    void GameTickRate::pause()
    {
        if(prevTime>=0)
            prevTime-=1LL*getTimeMCS();
    }
    int GameTickRate::getFrames()
    {
        long long currentTime=getTimeMCS();
        if(!prevTime)
            prevTime=currentTime;
        if(prevTime<0)
            prevTime+=currentTime;
        int result = (currentTime-prevTime) / tickrate;
        prevTime+= tickrate * result;
        return result;
    }
