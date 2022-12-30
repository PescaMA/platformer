#ifndef EXTENDED_RAYLIB ///contains the whole header
#define EXTENDED_RAYLIB

#ifndef RAYLIB_H
#include "raylib.h"
#endif // RAYLIB.H
#include <chrono>

int mouseAction;

long long getTimeMS();
long long getTimeMCS();
struct Txt;
struct TxtAligned;
struct Button;
struct ButtonAligned;
struct ButtonTxt;
struct ButtonOnOff;
struct ButtonInput;
struct FixedButton;
class KBD_Btn_Move;
class GameTickRate;
struct Directions
{
    float left,up,right,down;
};
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
    template <class type>
    type abs(type a)
    {
        return (a<0) ? -a : a;
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
        int oldCoord=coord;
        coord=coord + length*percent/100 -textSize/2;
        if(coord + textSize > oldCoord + length)
            coord = oldCoord + length - textSize;
        if(coord<oldCoord)
            coord=oldCoord;
    }

};
struct Txt
{
    float x,y,Fsz;
    char text[105];
    Color color;
    Txt(char const text[105],int x,int y,int fontSize,Color color)
    {
        this->x=x;
        this->y=y;
        this->Fsz=fontSize;
        strcpy(this->text,text);
        this->color=color;
    }
    void draw(bool shouldUnderline=false)
    {
        DrawText(text,x,y,Fsz,color);
        if(shouldUnderline)
            underline();
    }
    void underline()
    {
        int len=MeasureText(text,Fsz);
        Vector2 init= {x,y+Fsz};
        Vector2 endp= {x+len,y+Fsz};
        DrawLineEx(init,endp,Fsz/20+1,color);
    }
};
struct TxtAligned : public Txt
{ /// remember
    TxtAligned(char const text[105],Rectangle container,int xPercent,int yPercent,int fontSize,Color color)
    :Txt(text,container.x,container.y,fontSize,color)
    {
        align(x,container.x,container.width,xPercent,MeasureText(text,Fsz));
        align(y,container.y,container.height,yPercent,Fsz);
    }
    void align(float &coord,int startCoord,int length,int percent,int textSize)
    {
        ERay::align(coord,startCoord,length,percent,textSize);
    }
};
struct Button
{
    char text[105];
    Rectangle rect;
    struct ColorPair
    {
        Color text,background;
    }normalColor,hoverColor;
    int fontSize;
    float thickness;
    bool isHovering;
    bool forceHover;
    Button(){}
    Button(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor)
    {
        isHovering=false;
        strcpy(this->text,text);
        thickness=std::max(1,fontSize/10);
        this->fontSize=fontSize;
        rect= {(float)startX,(float)startY,
        (float)MeasureText(text,fontSize)+10+thickness*2 , fontSize+10+thickness*2};
        forceHover=false;
        normalColor.text=color;
        (this->hoverColor).text=hoverColor;
        normalColor.background=(this->hoverColor).background=BLANK;
    }
    ColorPair getCurrentColor(int transparency)
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
    void draw(int transparency=255)
    {
        ColorPair color=getCurrentColor(transparency);
        DrawRectangleRec(rect,color.background);
        DrawRectangleLinesEx(rect,thickness,color.text);
        DrawText(text,rect.x+5+thickness,rect.y+5+thickness,fontSize,color.text);
    }
    bool Lclicked()
    {
        return clicked(0);
    }
    bool Rclicked()
    {
        return clicked(1);
    }
    protected:
    bool clicked(int nr)
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
    void re_measure()
    {
        rect.width=MeasureText(text,rect.height-5)+10;
    }
};
struct ButtonAligned : public Button
{
    ButtonAligned(){}
    ButtonAligned(char const text[105],Rectangle container,int xPercent,int yPercent,int fontSize,Color color,Color hoverColor)
    :Button(text,container.x,container.y,fontSize,color,hoverColor)
    {
        align(rect.x,container.x,container.width,xPercent,rect.width);
        align(rect.y,container.y,container.height,yPercent,rect.height);
    }
    void align(float &coord,int startCoord,int length,int percent,int textSize)
    {
        ERay::align(coord,startCoord,length,percent,textSize);
    }
};
struct ButtonTxt : public Button
{
    ButtonTxt(){}
    ButtonTxt(char const text[],Txt a,Color color,Color hoverColor)
    :Button(text,(a.x)+MeasureText(a.text,a.Fsz)+10,(a.y)-3,a.Fsz,color,hoverColor)
    {}/// Button to the right of a Txt
};
struct ButtonOnOff : public Button
{
    bool isOn;
    ButtonOnOff(){}
    ButtonOnOff(int startX,int startY,int fontSize,bool isOn,Color colorOn=GREEN,Color colorOff=RED)
    :Button("",startX,startY,fontSize,colorOn,colorOff)
    {
        this->isOn=isOn;
    }
    void switchValue()
    {
        if(isOn)
            strcpy(text,"On");
        else
            strcpy(text,"Off");
        re_measure();
    }
    ColorPair getCurrentColor(int transparency)
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
    bool clicked(int nr)
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
};
struct ButtonInput : public Button
{
    ButtonInput(){}
    ButtonInput(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor)
    :Button(text,startX,startY,fontSize,color,hoverColor){}
    bool getinputI(int &rez,int max_value,char const ending[])
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
    bool not_clicked()
    {
        Vector2 mouse=GetMousePosition();
        if (!CheckCollisionPointRec(mouse, rect) && (IsMouseButtonPressed(0) || IsMouseButtonPressed(1)))
                return true;
        return false;
    }
};
struct FixedButton : public Button
{

    FixedButton(){}
    FixedButton(char const text[],int startX,int startY,int width,int height,int fontSizeee,Color color,Color hoverColor)
    :Button(text,1,1,fontSizeee,color,hoverColor)
    {
        rect= {(float)startX,(float)startY,(float)width,(float)height};
    }
    void draw(int transparency)
    {
        ColorPair color=getCurrentColor(transparency);
        DrawRectangleRec(rect,color.background);
        DrawRectangleLinesEx(rect,thickness,color.text);
        float centerX=rect.x+(rect.width/2-MeasureText(text,fontSize)/2);
        float centerY=rect.y+(rect.height/2-fontSize/2.0);
        DrawText(text,centerX,centerY,fontSize,color.text);
    }
};
class KBD_Btn_Hold
{ /// always part of the KBD_Btn_Move class
    static const int INITIAL_DELAY=800;
    static const int MOVEMENT_DELAY=80;
    long long initialTime=-3;

    public:
    void reset()
    {
        initialTime=-3;
    }
    int run(int frontKey,int backKey)
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
};
class KBD_Btn_Move
{ /// allows to move through buttons with keyboard

    KBD_Btn_Hold hold;
    public:
    int backKey,frontKey;
    int currentPosition,defaultPosition;

    Button **myArray;
    int n=-1;

    KBD_Btn_Move(){}
    KBD_Btn_Move(Button *myArray[],int n,bool vertical=true,int defaultPosition=0)
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
    void reset()
    {
        myArray[currentPosition]->forceHover=false;
        currentPosition=defaultPosition;
        hold.reset();
    }
    void run()
    {
        if(n<1)
            return;
        if(IsKeyPressed(frontKey))
            move(1);
        if(IsKeyPressed(backKey))
            move(-1);

        move(hold.run(frontKey,backKey));
    }
    void move(int delta)
    {
        myArray[currentPosition]->forceHover=false;
        currentPosition+=delta;
        currentPosition= (currentPosition+100*n)%n;
        myArray[currentPosition]->forceHover=true;
    }
};
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
class GameTickRate
{
    int tickrate;
    long long prevTime;
    public:
    GameTickRate(int tickrate)
    {
        this->tickrate=1000000/tickrate;
        reset();
    }
    void reset()
    {
        prevTime=0;
    }
    void pause()
    {
        if(prevTime>=0)
            prevTime-=1LL*getTimeMCS();
    }
    int getFrames()
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
};
#endif // EXTENDED_RAYLIB
