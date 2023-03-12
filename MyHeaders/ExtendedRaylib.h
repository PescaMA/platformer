#ifndef EXTENDED_RAYLIB_H ///contains the whole header
#define EXTENDED_RAYLIB_H

#ifndef RAYLIB_H
#include "raylib.h"
#endif // RAYLIB.H
#include <chrono>
#include <iostream>
#include <string.h>

int mouseAction;
Rectangle screenInfo;
void updateScreenSize();
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
struct ScaledRectangle;
class KBD_Btn_Move;
class GameTickRate;
struct Directions
{
    float left,up,right,down;
};
struct ColorPair
{
    Color text,background;
};
namespace ERay
{
    void fullscreen(int iSw,int iSh,int &Sw,int &Sh);
    template <class type>
    type abs(type a)
    {
        return (a<0) ? -a : a;
    };
    bool same_color(Color a,Color b);
    bool collision2D(Camera2D c,Rectangle r);
    Rectangle smart_paint(float xStart,float size,short xOrientation,float yOrientation);
    Rectangle getWindowSize();
    void align(float &coord,int startCoord,int length,int percent,int textSize);
    void drawTextureDest(Texture2D asset, Rectangle drawnPart, Rectangle destination);
};
struct Txt
{
    float x,y,Fsz;
    char text[105];
    Color color;
    Txt(char const text[105],int x,int y,int fontSize,Color color);
    void draw(bool shouldUnderline=false);
    void underline();
};
struct TxtAligned : public Txt
{ /// remember
    Rectangle *container;
    float xPercent,yPercent;
    TxtAligned(char const text[105],Rectangle *container,float xPercent,float yPercent,int fontSize,Color color);
    void align();
};
struct Button
{
    char text[105];
    Rectangle rect;
    ColorPair normalColor,hoverColor;
    int fontSize;
    float thickness;
    bool isHovering = false;
    bool forceHover = false;
    Button(){}
    Button(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor);
    ColorPair getCurrentColor(int transparency);
    void draw(int transparency=255);
    bool Lclicked();
    bool Rclicked();
    protected:
    bool clicked(int nr);
    void re_measure();
};
struct ButtonAligned : public Button
{
    int xPercent,yPercent;
    Rectangle *container;
    ButtonAligned(){}
    ButtonAligned(char const text[105],Rectangle *container,int xPercent,int yPercent,int fontSize,Color color,Color hoverColor);
    bool Lclicked();
    void align();
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
    ButtonOnOff(int startX,int startY,int fontSize,bool isOn,Color colorOn=GREEN,Color colorOff=RED);
    void switchValue();
    ColorPair getCurrentColor(int transparency);
    bool clicked(int nr);
};
struct ButtonInput : public Button
{
    ButtonInput(){}
    ButtonInput(char const text[],int startX,int startY,int fontSize,Color color,Color hoverColor);
    bool getinputI(int &rez,int max_value,char const ending[]);
    bool not_clicked();
};
struct FixedButton : public Button
{

    FixedButton(){}
    FixedButton(char const text[],int startX,int startY,int width,int height,int fontSizeee,Color color,Color hoverColor);
    void draw(int transparency);
};
struct ScaledRectangle
{
    Rectangle rect;
    Rectangle *parent;
    float xPrc1,xPrc2,yPrc1,yPrc2;
    ScaledRectangle(Rectangle *parent,float startingXpercent,float endingXpercent,float startingYpercent,float endingYpercent);
    void update();
};
class KBD_Btn_Hold
{ /// always part of the KBD_Btn_Move class
    static const int INITIAL_DELAY=800;
    static const int MOVEMENT_DELAY=80;
    long long initialTime=-3;

    public:
    void reset();
    int run(int frontKey,int backKey);
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
    KBD_Btn_Move(Button *myArray[],int n,bool vertical=true,int defaultPosition=0);
    void reset();
    void run();
    void move(int delta);
};
long long getTimeMS();
long long getTimeMCS();
class GameTickRate
{
    int tickrate;
    long long prevTime;
    public:
    GameTickRate(int tickrate);
    void reset();
    void pause();
    int getFrames();
};
#endif // EXTENDED_RAYLIB
