#ifndef EXTENDED_RAYLIB ///contains the whole header
#define EXTENDED_RAYLIB

#ifndef RAYLIB_H
#include "raylib.h"
#endif // RAYLIB.H
#include <chrono>

int mouseAction;

long long getTimeMS();
long long getTimeMCS();
class Misc;
struct Txt;
struct TxtAligned;
struct Button;
struct ButtonAligned;
struct ButtonTxt;
struct ButtonOnOff;
struct ButtonInput;
struct FixedButton;
class KBD_Move;
class GameTickRate;
struct Directions
{
    float left,up,right,down;
};
class Misc
{
public:
    static void fullscreen(int iSw,int iSh,int &Sw,int &Sh)
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
    static type abs(type a)
    {
        return (a<0) ? -a : a;
    }
    static bool same_color(Color a,Color b)
    {
        return a.a==b.a && a.b==b.b && a.g==b.g && a.r==b.r;
    }
    static bool collision2D(Camera2D c,Rectangle r)
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
{
    TxtAligned(char const text[105],char const Xalign[],int width,char const Yalign[],int height,int fontSize,Color color)
    :Txt(text,0,0,fontSize,color)
    {
        align(Xalign,width,x,MeasureText(text,Fsz));
        align(Yalign,height,y,Fsz);
    }
    TxtAligned(char const text[105],int startX,int width,char const Xalign[],int startY,int height,char const Yalign[],int fontSize,Color color)
    :Txt(text,startX,startY,fontSize,color)
    {
        align(Xalign,width,x,MeasureText(text,Fsz));
        align(Yalign,height,y,Fsz);
    }
    void align(char const way[],int length,float &coord,int textSize)
    {
        if(!strcmp(way,"center"))
            coord=coord+(length/2-textSize/2);
        if(!strcmp(way,"end"))
            coord=coord+(length-textSize);
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
        rect= {(float)startX,(float)startY,(float)MeasureText(text,fontSize)+10+thickness*2,fontSize+10+thickness*2};
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
    ButtonAligned(char const text[105],int startX,int width,char const Xalign[],int startY,int height,char const Yalign[],int fontSize,Color color,Color hoverColor)
    :Button(text,startX,startY,fontSize,color,hoverColor)
    {
        align(Xalign,width,rect.x,MeasureText(text,fontSize));
        align(Yalign,height,rect.y,fontSize);
    }
    void align(char const way[],int length,float &coord,int textSize)
    {
        if(!strcmp(way,"center"))
            coord=coord+(length/2-textSize/2);
        if(!strcmp(way,"end"))
            coord=coord+(length-textSize);
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

class KBD_Move
{
    static const int INITIAL_DELAY=500;
    static const int MOVEMENT_DELAY=70;
    long long initialTime=-3;
    public:

    int backKey,frontKey;
    int n,currentPosition;
    int defaulposition;
    Button **myArray;
    KBD_Move(){}
    KBD_Move(Button *myArray[],int n,bool vertical=true,int defaulposition=0)
    {
        this->defaulposition=currentPosition=defaulposition;
        this->myArray=new Button* [n];
        for(int i=0;i<n;i++)
            this->myArray[i]=myArray[i];
        this->n=n;
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
        currentPosition=defaulposition;
        initialTime=-3;

    }
    void run()
    {
        long long time = getTimeMS();

        if(initialTime<0)
            initialTime=getTimeMS();
        bool timePassed=false;
        if(time-initialTime>INITIAL_DELAY+MOVEMENT_DELAY)
        {
            initialTime+=MOVEMENT_DELAY;
            timePassed=true;
        }
        if(IsKeyDown(backKey))
        {
            if(IsKeyPressed(backKey) || timePassed)
            {
                myArray[currentPosition]->forceHover=false;
                if(currentPosition<=0)
                    currentPosition=n-1;
                else
                    currentPosition--;
                myArray[currentPosition]->forceHover=true;
            }
            return;
        }
        if(IsKeyDown(frontKey))
        {
            if(IsKeyPressed(frontKey) || timePassed)
            {
                myArray[currentPosition]->forceHover=false;
                if(currentPosition>=n-1)
                    currentPosition=0;
                else
                    currentPosition++;
                myArray[currentPosition]->forceHover=true;
            }

            return;
        }
        initialTime=getTimeMS();
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
