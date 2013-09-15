#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <sstream> 
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <algorithm>


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//======================================================================
//  Compile: g++ a1.cpp -o a1 –L/usr/X11R6/lib –lX11
//======================================================================

using namespace std;

//======================================================================
//
//  Constants definition
//
//======================================================================

//======================================================================
//  Window Constants
//======================================================================

/*----- Window Border  -----*/
const int Border = 5;

/*----- Window Position -----*/
const int windowX = 100;
const int windowY = 100;

/*----- Window Size -----*/
int windowWidth = 800;
int windowHeight= 500;

/*----- For Key event -----*/
const int BufferSize = 10;

//======================================================================
//  Layout Constants
//======================================================================

const double layoutHeightU = 0.6;
const int layoutHeight = layoutHeightU * windowHeight;
const int layoutInitialX = 0;
const int layoutInitialY = 0.5 * (1 - layoutHeightU) * windowHeight;

//======================================================================
//  Bird Constants
//======================================================================

/*----- Bird Initial Positions -----*/
const int birdX = 0.2 * windowWidth;
const int birdY = 0.6 * windowHeight; 

/*----- Bird other constans -----*/
const int birdR = 20;
const int birdD = 2*birdR; 


/*----- Velocity Coeficient -----*/
double uVelocity = 0.1;

/*----- Gravity Accelation -----*/
const double g = 0.1;

/*----- usleep time -----*/
int usleepTime = 20000;

/*----- Second/Microsecond -----*/
const int secondMicrosecond = 1000000;

//======================================================================
//  Brick Constants
//======================================================================

//----------------------------------------------------------------------
// Common Used Variables

// Bottom of Layout
const double brickY = windowHeight * ( (0.5 * (1 + layoutHeightU) ));

/*----- For 3D Effect -----*/
const int brickShift = 3;

/*----- Brick Length  -----*/
const int brickLength = 30;


/*----- Bottom Brick Initial Positions -----*/
// Need reset
//
double brickX_1;
double brickX_2; 
double brickX_3;

/*----- Number of Bricks -----*/
// Need reset
//
int brickNum_1;
int brickNum_2;
int brickNum_3;

//----------------------------------------------------------------------
// Level One

/*----- Bottom Brick Initial Positions -----*/
const double brickX_1_UlevelOne = 0.7; 

const double brickX_2_UlevelOne = 0.8; 

/*----- Number of Bricks -----*/
const int brickNum_1_levelOne = 4;
const int brickNum_2_levelOne = 6;

//----------------------------------------------------------------------
// Level Two

/*----- Bottom Brick Initial Positions -----*/
const double brickX_1_UlevelTwo = 0.7; 

// brick_2 depends on the position of brick_1, and connected
// to it

const double brickX_3_UlevelTwo = 0.9;


/*----- Number of Bricks -----*/
const int brickNum_1_levelTwo = 7;
const int brickNum_2_levelTwo = 2;
const int brickNum_3_levelTwo = 10;

//======================================================================
//  Ball Constants 
//======================================================================
//
// Common Used Variables

const int ballDiameter = brickLength;

/*----- Ball Initial Positions -----*/
// Need reset

int ballX_1; 
int ballY_1;
int ballX_2;
int ballY_2;

/*----- For Level Two -----*/
int ballX_3;
int ballY_3;

/*----- Number of Balls  -----*/
// Need reset
int ballNum;


/*----- Level 1  -----*/
const int ballNumLevelOne = 2;

/*----- Level 2  -----*/
const int ballNumLevelTwo = 3;
double velocityBall_3 = 1;

//======================================================================
//  Score Initial Positions
//======================================================================
int score = 0;

const int scoreX = 0.8 * windowWidth;
const int scoreY = layoutInitialY - 10;

const int pointsBall = 10;
const int pointsShoot = 10;
//======================================================================
//  Other Constants
//======================================================================

int level;
int flying = 0;
int hit = 0;
int dead = 0;
int over = 0;
int birdsLeft;
int birdsLeft_1 = 2;
int birdsLeft_2 = 3;

const int scoreVelocity = 1; 
int scoreRaiseCounter = 0;
int hitBall_1 = 0;
int hitBall_2 = 0;
int hitBall_3 = 0;

const double lineWidth = 2.5;
const int pathDiameter = 5;

const int slingshotWidth = 0.25 * birdR;
const int slingshotLength = 2 * birdR;

/*----- Stick -----*/
const int slingshotInitX = birdX + 0.5*birdR;
const int slingshotInitY = birdY + 2*birdR;

/*----- Leftmost -----*/
const int slingshotLeftmostInitX = slingshotInitX - 0.5*slingshotLength;
const int slingshotLeftmostInitY = slingshotInitY - slingshotLength; 

/*----- Rightmost -----*/
const int slingshotRightmostInitX = slingshotInitX + 0.5*slingshotLength;
const int slingshotRightmostInitY = slingshotInitY - slingshotLength; 

/*----- Cheating Function  -----*/
const int cheatingBoxInitX = 0.1 * windowWidth;  
const int cheatingBoxInitY = layoutInitialY - 30;
const int cheatingBoxWidth = 70;
const int cheatingBoxLength = 20;

/*----- Restart Button -----*/
const int restartBoxInitX = 0.03 * windowWidth;  
const int restartBoxInitY = cheatingBoxInitY; 
const int restartBoxWidth = 50;
const int restartBoxLength = cheatingBoxLength;
//======================================================================
//
//  Global Variables
//
//======================================================================


//----------------------------------------------------------------------
//  One GC can be used to paint many birds, add bricks etc
//  later
GC birdGC;
GC ordinaryGC;
GC eyeGC;
GC ballGC;
GC noseGC;
GC brickGC;
GC backgroundGC;

//======================================================================
//
//  Classes and structures
//  
//======================================================================

//======================================================================
//  Information to draw on the window.
//======================================================================
struct XInfo
{  
    Display  *display;
    Window   window;
    GC       gc;
    int screen;
};

//======================================================================
//  An abstract class representing displayable things. 
//======================================================================

class Displayable
{ 
    public:
        virtual void paint(XInfo &xinfo) = 0;

        int getX(){
            return x;
        }

        int getY(){
            return y;
        } 

        int getWidth(){
            return width;
        }

        int getLength(){
            return length;
        } 

        int getDiameter(){
            return diameter;
        } 

        void setX(int X){
            x = X;
        }

        void setY(int Y){
            y = Y;
        }

        void setWidth(int w){
            width = w;
        }

        void setLength(int l){
            length = l;
        }

        void setDiameter(int d){
            diameter = d;
        }

    private:
        /*----- For rectangle  -----*/
        int x;
        int y;
        int width;
        int length;
        int diameter;

};       

vector<Displayable *> dVector;
vector<Displayable *> dVectorPath;
vector<Displayable *> dVectorBird;

//======================================================================
//  Bird Class (now rectangle, modified later)
//======================================================================

class Bird : public Displayable {
    public:
        virtual void paint(XInfo &xinfo){
            int eyeDiameter = 0.3 * getWidth();

            // Bird
            XDrawRectangle(
                    xinfo.display, xinfo.window, 
                    ordinaryGC,
                    getX(), getY(), 
                    getWidth(), getLength()
                    ); 

            XDrawArc(
                    xinfo.display, xinfo.window,
                    ordinaryGC,
                    (getX() + getWidth()*0.5) - getWidth() * 0.65, 
                    (getY() + getWidth()*0.5) - getWidth() * 0.65,
                    1.3*getWidth(), 1.4*getWidth(),
                    0, 360*64
                    );
            
            // Bird Circle
            XFillArc(
                    xinfo.display, xinfo.window,
                    birdGC,
                    (getX() + getWidth()*0.5) - getWidth() * 0.65,
                    (getY() + getWidth()*0.5) - getWidth() * 0.65, 
                    1.3*getWidth(), 1.4*getWidth(),
                    0, 360*64
                    );

            // Eye 1
            XDrawArc(
                    xinfo.display, xinfo.window,
                    ordinaryGC,
                    (getX() + getWidth()*0.65) - (eyeDiameter/2),
                    (getY() + getWidth()*0.4) - (eyeDiameter/2),
                    eyeDiameter, eyeDiameter,
                    0, 360*64
                    );

            XFillArc(
                    xinfo.display, xinfo.window,
                    eyeGC,
                    (getX() + getWidth()*0.65) - (eyeDiameter/2),
                    (getY() + getWidth()*0.4) - (eyeDiameter/2),
                    eyeDiameter, eyeDiameter,
                    0, 360*64
                    );

            XDrawPoint(
                    xinfo.display,
                    xinfo.window,
                    ordinaryGC,
                    (getX() + getWidth()*0.65),
                    (getY() + getWidth()*0.4)
                    );

            // Eye 2
            XDrawArc(
                    xinfo.display, xinfo.window,
                    ordinaryGC,
                    (getX() + getWidth()*0.95) - (eyeDiameter/2),
                    (getY() + getWidth()*0.4) - (eyeDiameter/2),
                    eyeDiameter, eyeDiameter,
                    0, 360*64
                    );

            XFillArc(
                    xinfo.display, xinfo.window,
                    eyeGC,
                    (getX() + getWidth()*0.95) - (eyeDiameter/2),
                    (getY() + getWidth()*0.4) - (eyeDiameter/2),
                    eyeDiameter, eyeDiameter,
                    0, 360*64
                    );

            XDrawPoint(
                    xinfo.display,
                    xinfo.window,
                    ordinaryGC,
                    (getX() + getWidth()*0.95),
                    (getY() + getWidth()*0.4)
                    );

            // Tail 1
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX(), getY() + 0.5 * getWidth(),
                    getX() -0.4*getWidth(),
                    getY() + 0.5 * getWidth() 
                    );

            // Tail 2
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX(), getY() + 0.5 * getWidth(),
                    getX() -0.4*getWidth(),
                    getY() + 0.3 * getWidth() 
                    );

            // Tail 3
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX(), getY() + 0.5 * getWidth(),
                    getX() -0.4*getWidth(),
                    getY() + 0.7 * getWidth() 
                    );

            
            // Mouth 1
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX() + 0.7 * getWidth(), getY() + 0.8 * getWidth(),
                    getX() + 0.9 * getWidth(),
                    getY() + 0.7 * getWidth() 
                    );

            // Mouth 2
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX() + 0.7 * getWidth(), getY() + 0.8 * getWidth(),
                    getX() + 0.9 * getWidth(),
                    getY() + 0.9 * getWidth() 
                    );

            // Eyebrow 1  
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX() + getWidth() * 0.85, getY() + 0.3 * getWidth(),
                    getX() + 0.5*getWidth(),
                    getY() - 0.3 * getWidth() 
                    );

            // Eyebrow 2
            XDrawLine(xinfo.display, xinfo.window,
                    ordinaryGC,
                    getX() + getWidth() * 0.85, getY() + 0.3 * getWidth(),
                    getX() + 1.2*getWidth(),
                    getY() - 0.3 * getWidth() 
                    );

            // Bad flying variable to save time :(
            if (getDiameter() == 0 && flying == 0){

                /*----- Slingshot Line Left -----*/
                XDrawLine(xinfo.display, xinfo.window,
                        ordinaryGC,
                        getX(), getY() + getWidth(),
                        slingshotLeftmostInitX, slingshotLeftmostInitY
                        );

                /*----- Slingshot Line Right -----*/
                XDrawLine(xinfo.display, xinfo.window,
                        ordinaryGC,
                        getX()+getWidth(), getY() + getWidth(),
                        slingshotRightmostInitX, slingshotRightmostInitY
                        );
            }

              

            XFlush(xinfo.display);
        }
        

        
        /*-----  Constructor -----*/
        Bird(int x, int y, int w, int l, int d){
            setX(x);
            setY(y);
            setWidth(w);
            setLength(l);
            setDiameter(d);
        }
};

//======================================================================
//  Brick Class 
//======================================================================

/*----- Ordinary Brick -----*/

class Brick: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){


            XDrawRectangle(
                    xinfo.display, xinfo.window,
                    ordinaryGC, getX(), getY(), getWidth(), getLength()
                    );
            
            XSetForeground(xinfo.display, brickGC, 0xab916e); 
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    brickGC, 
                    getX(), getY(), 
                    getWidth(), getLength()
                    );

            XSetForeground(xinfo.display, brickGC, 0x714226); 
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    brickGC, 
                    getX()+brickShift, getY()+brickShift, 
                    getWidth()-brickShift, getLength()-brickShift
                    );

            /*
             
            XDrawRectangle(
                    xinfo.display, xinfo.window, 
                    brick, (getX()+brickShift), (getY()-brickShift), getWidth(), getLength()
                    ); 
            
            //----- Line 1 -----
            XDrawLine(
                    xinfo.display, xinfo.window,
                    brick, getX(), getY(), (getX()+brickShift), (getY()-brickShift)
                    );

            //----- Line 2 -----
            XDrawLine(
                    xinfo.display, xinfo.window,
                    brick, (getX()+brickLength), getY(), (getX()+brickShift+brickLength), (getY()-brickShift)
                    );

            //----- Line 3 -----
            XDrawLine(
                    xinfo.display, xinfo.window,
                    brick, (getX()+brickLength), (getY()+brickLength), (getX()+brickShift+brickLength), (getY()-brickShift+brickLength)
                    );
            */

            XFlush(xinfo.display);
        }

        /*-----  Constructor -----*/
        Brick(int x, int y, int w, int l){
            setX(x);
            setY(y);
            setWidth(w);
            setLength(l);
        }
};

//======================================================================
//  Ball Class
//======================================================================

class Ball: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){
            int ballEyeD = 0.15 * getDiameter();

            XDrawArc(
                    xinfo.display, xinfo.window, 
                    ordinaryGC, getX()-(getDiameter()/2), getY()-(getDiameter()/2), 
                    getDiameter(), getDiameter(),
                    0, 360*64
                    ); 

            XFillArc(
                    xinfo.display, xinfo.window, 
                    ballGC, getX()-(getDiameter()/2), getY()-(getDiameter()/2), 
                    getDiameter(), getDiameter(),
                    0, 360*64
                    ); 

            XDrawArc(
                    xinfo.display, xinfo.window, 
                    ordinaryGC, (getX() + 0.25*getDiameter())-(getDiameter()/2), (getY()+0.3*getDiameter()-(getDiameter()/2)),
                    ballEyeD, ballEyeD,
                    0, 360*64
                    ); 

            XFillArc(
                    xinfo.display, xinfo.window, 
                    eyeGC, (getX() + 0.3*getDiameter())-(getDiameter()/2), (getY()+0.3*getDiameter()-(getDiameter()/2)),
                    ballEyeD, ballEyeD,
                    0, 360*64
                    ); 

            XDrawArc(
                    xinfo.display, xinfo.window, 
                    ordinaryGC, (getX() + 0.7*getDiameter())-(getDiameter()/2), (getY()+0.3*getDiameter()-(getDiameter()/2)),
                    ballEyeD, ballEyeD, 
                    0, 360*64
                    ); 

            XFillArc(
                    xinfo.display, xinfo.window, 
                    eyeGC, (getX() + 0.7*getDiameter())-(getDiameter()/2), (getY()+0.3*getDiameter()-(getDiameter()/2)),
                    ballEyeD, ballEyeD, 
                    0, 360*64
                    ); 

            // nose
            XDrawArc(
                    xinfo.display, xinfo.window, 
                    ordinaryGC, (getX()+0.4*getDiameter()) - getDiameter()/2, getY()+0.4*getDiameter()-(getDiameter()/2),
                    2*ballEyeD, 2*ballEyeD, 
                    0, 360*64
                    ); 

            XFillArc(
                    xinfo.display, xinfo.window, 
                    noseGC, 
                    (getX()+0.4*getDiameter()) - getDiameter()/2, getY()+0.4*getDiameter()-(getDiameter()/2),
                    2*ballEyeD, 2*ballEyeD, 
                    0, 360*64
                    ); 


            XDrawPoint(
                    xinfo.display,
                    xinfo.window,
                    ordinaryGC,
                    (getX()+0.50*getDiameter()) - getDiameter()/2, getY()+0.55*getDiameter()-(getDiameter()/2)
                    );

            XDrawPoint(
                    xinfo.display,
                    xinfo.window,
                    ordinaryGC,
                    (getX()+0.60*getDiameter()) - getDiameter()/2, getY()+0.55*getDiameter()-(getDiameter()/2)
                    );


            XFlush(xinfo.display);
        }
        

        /*-----  Constructor -----*/
        Ball(int x, int y, int d){
            setX(x);
            setY(y);
            setDiameter(d);
        }

};


//======================================================================
//  Slingshot Class  
//======================================================================

class Slingshot: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){

            // Rectangle stick
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    ordinaryGC, 
                    getX(), getY(),
                    getWidth(), brickY - getY()
                    );

            // Rectangle leftmost 
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    ordinaryGC, 
                    getX() - 0.5*getLength(), getY() - getLength(), 
                    getWidth(), getLength()
                    );
            // Rectangle rightmost
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    ordinaryGC, 
                    getX() + 0.5*getLength(), getY() - getLength(),
                    getWidth(), getLength()
                    );
            
            // Rectangle bottom 
            XFillRectangle(
                    xinfo.display, xinfo.window,
                    ordinaryGC, 
                    getX() - 0.5*getLength(), getY() - getWidth(),
                    getLength(), getWidth()
                    );

            XFlush(xinfo.display);
        }

        /*-----  Constructor -----*/
        Slingshot(int x, int y, int w, int l){
            setX(x);
            setY(y);
            setWidth(w);
            setLength(l);
        }
};


//======================================================================
//  Layout Line Class
//======================================================================

class Layout: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){
       
            /*----- Line 1  -----*/
            XDrawLine(
                    xinfo.display, xinfo.window,
                    ordinaryGC, getX(), getY(), getX() + windowWidth, getY()
                    );

            /*----- Line 2  -----*/
            XDrawLine(
                    xinfo.display, xinfo.window,
                    ordinaryGC, getX(), getY() + layoutHeight, getX() + windowWidth, getY() + layoutHeight
                    );


            XFlush(xinfo.display);
        }

        /*-----  Constructor -----*/
        Layout(int x, int y){
            setX(x);
            setY(y);
        }
};

//======================================================================
//  Path Class 
//======================================================================

class Path: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){
            XFillArc(
                    xinfo.display, xinfo.window, 
                    ordinaryGC, getX()-(getDiameter()/2), getY()-(getDiameter()/2),
                    getDiameter(), getDiameter(), 
                    0, 360*64
                    ); 
            XFlush(xinfo.display);
        }
        

        /*-----  Constructor -----*/
        Path(int x, int y, int d){
            setX(x);
            setY(y);
            setDiameter(d);
        }

};


//======================================================================
//  Empty Class 
//======================================================================

class Empty: public Displayable {
    public:
        virtual void paint(XInfo &xinfo){
        }
        /*-----  Constructor -----*/
        Empty(){
        }
};

//======================================================================
//  Helper Functions
//======================================================================

//----------------------------------------------------------------------
//  Functions to show error messages

void error( string str ) {
    cerr << str << endl;
    exit(0);
}

//----------------------------------------------------------------------
//  Function to repaint a display vector

void repaint( vector<Displayable *> dVector, XInfo &xinfo) 
{
    
    stringstream points;
    points << "Score: " << score;
    string s = points.str();
    const char * scoreFinal = s.c_str();


    XClearWindow( xinfo.display, xinfo.window );

    // Paint background
    XSetForeground(xinfo.display, backgroundGC, 0xbee9f2);
    XFillRectangle(xinfo.display,
            xinfo.window,
            backgroundGC,
            layoutInitialX,
            layoutInitialY,
            windowWidth,
            layoutHeight/3
            ); 
    
    XSetForeground(xinfo.display, backgroundGC, 0xd1eef4);
    XFillRectangle(xinfo.display,
            xinfo.window,
            backgroundGC,
            layoutInitialX,
            layoutInitialY + layoutHeight/3,
            windowWidth,
            layoutHeight/3
            ); 

    XSetForeground(xinfo.display, backgroundGC, 0xf3fdfe);
    XFillRectangle(xinfo.display,
            xinfo.window,
            backgroundGC,
            layoutInitialX,
            layoutInitialY + 2 * layoutHeight/3,
            windowWidth,
            layoutHeight/3
            ); 

    vector<Displayable *>::reverse_iterator begin = dVector.rbegin();
    vector<Displayable *>::reverse_iterator end = dVector.rend();

    while( begin != end ) 
    {  Displayable *d = *begin;
        d->paint(xinfo);
        begin++;
    }


    // Paint score
    XDrawString(xinfo.display,
                xinfo.window,
                ordinaryGC,
                scoreX,
                scoreY,
                scoreFinal,
                strlen(scoreFinal));
    
    string ballP = "+10 points";
    const char * ballPoints = ballP.c_str();

    if (hitBall_1 == 1){
        XDrawString(
                xinfo.display,
                xinfo.window,
                ordinaryGC,
                ballX_1 - 0.5 * ballDiameter,
                ballY_1 - 0.5 * ballDiameter - scoreRaiseCounter*scoreVelocity,
                ballPoints,
                strlen(ballPoints));
    }

    if (hitBall_2 == 1){
        XDrawString(
                xinfo.display,
                xinfo.window,
                ordinaryGC,
                ballX_2 - 0.5 * ballDiameter,
                ballY_2 - 0.5 * ballDiameter - scoreRaiseCounter*scoreVelocity,
                ballPoints,
                strlen(ballPoints));
    }

    if (hitBall_3 == 1){
        XDrawString(
                xinfo.display,
                xinfo.window,
                ordinaryGC,
                dVector[3]->getX() - 0.5 * ballDiameter,
                dVector[3]->getY() - 0.5 * ballDiameter - scoreRaiseCounter*scoreVelocity,
                ballPoints,
                strlen(ballPoints));
    }

    if (over == 1){
        string overM_1;
        string overM_2 = "Press 'r' to restart, 'q' to quit.";
        const char *overMessage_2 = overM_2.c_str();

        if (dead == 1){
            overM_1 = "Sorry, no birds left. ";
        }

        else {
            overM_1 = "Congratulations! You cleared all levels! ";
        }

        overM_1 = overM_1.append(s); // append score
        const char *overMessage_1 = overM_1.c_str();

        XDrawString(
                xinfo.display,
                xinfo.window,
                ordinaryGC,
                windowWidth * 0.29,
                layoutInitialY + layoutHeight* 0.5,
                overMessage_1,
                strlen(overMessage_1));

        XDrawString(
                xinfo.display,
                xinfo.window,
                ordinaryGC,
                windowWidth * 0.29,
                layoutInitialY + layoutHeight* 0.6,
                overMessage_2,
                strlen(overMessage_2));
    }

    if (level == 1){
        // Paint Cheating Function 
        string s = "Next Level";
        const char * cheating = s.c_str();

        XDrawRectangle(xinfo.display,
                xinfo.window,
                ordinaryGC,
                cheatingBoxInitX,
                cheatingBoxInitY,
                cheatingBoxWidth,
                cheatingBoxLength
                ); 

        XDrawString(xinfo.display,
                xinfo.window,
                ordinaryGC,
                cheatingBoxInitX + 5,
                cheatingBoxInitY + 15,
                cheating, 
                strlen(cheating));
    }

    string restartM = "Restart";
    const char * restart = restartM.c_str();

    XDrawRectangle(xinfo.display,
            xinfo.window,
            ordinaryGC,
            restartBoxInitX,
            restartBoxInitY,
            restartBoxWidth,
            restartBoxLength
            ); 

    XDrawString(xinfo.display,
            xinfo.window,
            ordinaryGC,
            restartBoxInitX + 5,
            restartBoxInitY + 15,
            restart, 
            strlen(restart));

    // Repaint birds
    vector<Displayable *>::const_iterator beginBird = dVectorBird.begin();
    vector<Displayable *>::const_iterator endBird = dVectorBird.end();
    beginBird = dVectorBird.begin();
    endBird = dVectorBird.end();

    while( beginBird != endBird ){
        Displayable *d = *beginBird;
        d->paint(xinfo);
        beginBird++;
    }

    // Repaint path
    vector<Displayable *>::const_iterator beginPath = dVector.begin();
    vector<Displayable *>::const_iterator endPath = dVector.end();
    beginPath = dVectorPath.begin();
    endPath = dVectorPath.end();

    while( beginPath != endPath ){
        Displayable *d = *beginPath;
        d->paint(xinfo);
        beginPath++;
    }

    XFlush( xinfo.display );
}

//----------------------------------------------------------------------
//  Set up all GCs used

void setupGC(XInfo &xinfo){
    /*----- birdGC -----*/
    XGCValues birdGCvalues;  

    birdGCvalues.foreground = BlackPixel(xinfo.display, xinfo.screen);
    birdGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    birdGCvalues.line_width = lineWidth;

    unsigned long birdGCvaluemask = GCForeground | GCBackground | GCLineWidth; 

    birdGC = XCreateGC(xinfo.display, xinfo.window, birdGCvaluemask, &birdGCvalues);
    if (birdGC < 0) {
        error( "XCreateGC: birdGC\n" );
    }

    XSetForeground(xinfo.display, birdGC, 0xd8002f);

    /*----- eyeGC -----*/
    XGCValues eyeGCvalues;  

    eyeGCvalues.foreground = WhitePixel(xinfo.display, xinfo.screen);
    eyeGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    eyeGCvalues.line_width = lineWidth;

    unsigned long eyeGCmask = GCForeground | GCBackground | GCLineWidth; 

    eyeGC = XCreateGC(xinfo.display, xinfo.window, eyeGCmask, &eyeGCvalues);
    if (eyeGC < 0) {
        error( "XCreateGC: eyeGC\n" );
    }

    /*----- ballGC -----*/
    XGCValues ballGCvalues;  

    ballGCvalues.foreground = WhitePixel(xinfo.display, xinfo.screen);
    ballGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    ballGCvalues.line_width = lineWidth;

    unsigned long ballGCmask = GCForeground | GCBackground | GCLineWidth; 

    ballGC = XCreateGC(xinfo.display, xinfo.window, ballGCmask, &ballGCvalues);
    if (ballGC < 0) {
        error( "XCreateGC: ballGC\n" );
    }

    XSetForeground(xinfo.display, ballGC, 0x79bf4f); 

    /*----- noseGC -----*/
    XGCValues noseGCvalues;  

    noseGCvalues.foreground = WhitePixel(xinfo.display, xinfo.screen);
    noseGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    noseGCvalues.line_width = lineWidth;

    unsigned long noseGCmask = GCForeground | GCBackground | GCLineWidth; 

    noseGC = XCreateGC(xinfo.display, xinfo.window, noseGCmask, &noseGCvalues);
    if (noseGC < 0) {
        error( "XCreateGC: noseGC\n" );
    }

    XSetForeground(xinfo.display, noseGC, 0x9dcb3e); 

    /*----- brickGC -----*/
    XGCValues brickGCvalues;  

    brickGCvalues.foreground = WhitePixel(xinfo.display, xinfo.screen);
    brickGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    brickGCvalues.line_width = lineWidth;

    unsigned long brickGCmask = GCForeground | GCBackground | GCLineWidth; 

    brickGC = XCreateGC(xinfo.display, xinfo.window, brickGCmask, &brickGCvalues);
    if (brickGC < 0) {
        error( "XCreateGC: brickGC\n" );
    }

    XSetForeground(xinfo.display, brickGC, 0xde8f26); 

    /*----- backgroundGC -----*/
    XGCValues backgroundGCvalues;  

    backgroundGCvalues.foreground = WhitePixel(xinfo.display, xinfo.screen);
    backgroundGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    backgroundGCvalues.line_width = lineWidth;

    unsigned long backgroundGCmask = GCForeground | GCBackground | GCLineWidth; 

    backgroundGC = XCreateGC(xinfo.display, xinfo.window, backgroundGCmask, &backgroundGCvalues);
    if (backgroundGC < 0) {
        error( "XCreateGC: backgroundGC\n" );
    }

    XSetForeground(xinfo.display, backgroundGC, 0xbee9f2); 

    /*----- ordinaryGC -----*/
    XGCValues ordinaryGCvalues;  

    ordinaryGCvalues.foreground = BlackPixel(xinfo.display, xinfo.screen);
    ordinaryGCvalues.background = BlackPixel(xinfo.display, xinfo.screen);
    ordinaryGCvalues.line_width = lineWidth;


    unsigned long ordinaryGCmask = GCForeground | GCBackground | GCLineWidth; 

    ordinaryGC = XCreateGC(xinfo.display, xinfo.window, ordinaryGCmask, &ordinaryGCvalues);
    if (ordinaryGC < 0) {
        error( "XCreateGC: ordinaryGC\n" );
    }
}

//----------------------------------------------------------------------
//  Setup all the objects in the game and push them into the diplay vector 

void setup(int l){

    // Clear path
    dVectorPath.clear();

    // Clear other objects
    dVector.clear();

    dVectorBird.clear();

    dead = 0;
    over = 0;

    /*----- Bird dVector[0] -----*/
    dVector.push_back (new Bird(birdX, birdY, birdR, birdR, 0));

    if (l == 1){
        /*----- Reset All Constants  -----*/

        score = 0;
        level = 1;


        brickX_1 = brickX_1_UlevelOne * windowWidth;         
        brickX_2 = brickX_2_UlevelOne * windowWidth; // Need reset for level two;

        brickNum_1 = brickNum_1_levelOne;
        brickNum_2 = brickNum_2_levelOne;

        ballNum = ballNumLevelOne;

    }

    else if (l == 2){
        /*----- Reset All Constants  -----*/

        level = 2;

        brickX_1 = brickX_1_UlevelTwo * windowWidth;         
        brickX_2 = brickX_1 + brickLength; // Need reset for level two;
        brickX_3 = brickX_3_UlevelTwo * windowWidth;

        brickNum_1 = brickNum_1_levelTwo;
        brickNum_2 = brickNum_2_levelTwo;
        brickNum_3 = brickNum_3_levelTwo;

        ballNum = ballNumLevelTwo;
    }

    ballX_1 = brickX_1 + ballDiameter/2; 
    ballY_1 = brickY - (brickNum_1)*brickLength - ballDiameter/2;
    ballX_2 = brickX_2 + ballDiameter/2; 
    ballY_2 = brickY - (brickNum_2)*brickLength - ballDiameter/2;
    ballX_3 = 0.5 * windowWidth;
    ballY_3 = layoutInitialY + 0.5 * ballDiameter;

    /*----- Balls  -----*/
    /*----- dVector[1] to dVector[ballNum] -----*/

    dVector.push_back (new Ball(ballX_1, ballY_1, ballDiameter));
    dVector.push_back (new Ball(ballX_2, ballY_2, ballDiameter));
    if (l == 2){
        dVector.push_back (new Ball(ballX_3, ballY_3, ballDiameter));
    }

    /*----- Bricks 1 -----*/
    /*----- dVector[ballNum + 1] 
     * to
     * dVector[ballNum + brickNum_1] -----*/

    for (int i=1; i<=brickNum_1; i++){
        dVector.push_back (new Brick(brickX_1, brickY - i*brickLength, brickLength, brickLength));
    } // for

    /*----- Bricks 2 -----*/
    /*----- dVector[ballNum + brickNum_1 + 1] 
     * to
     * dVector[ballNum + brickNum_1 + brickNum_2] -----*/

    for (int i=1; i<=brickNum_2; i++){
        dVector.push_back (new Brick(brickX_2, brickY - i*brickLength, brickLength, brickLength));
    } // for

    if (l == 2){
        /*----- Bricks 3 -----*/
        /*----- dVector[ballNum + brickNum_1 + brickNum_2 + 1] 
         * to
         * dVector[ballNum + brickNum_1 + brickNum_2 + brickNum_3] -----*/

        for (int i=1; i<=brickNum_3; i++){
            dVector.push_back (new Brick(brickX_3, brickY - i*brickLength, brickLength, brickLength));
        } // for

    }

    /*----- Slingshot -----*/

    dVector.push_back (new Slingshot(slingshotInitX, slingshotInitY, slingshotWidth, slingshotLength)); 

    /*----- Layout  -----*/
    /*----- always the last one in dVector -----*/

    dVector.push_back (new Layout(layoutInitialX, layoutInitialY)); 

    /*----- Birds -----*/
    if (l == 1){
        birdsLeft = birdsLeft_1;
    }
    else if (l == 2){
        birdsLeft = birdsLeft_2;
    }

    for (int i=1; i<=birdsLeft; i++){
        dVectorBird.push_back(new Bird(cheatingBoxInitX + cheatingBoxWidth + i * birdR, layoutInitialY - birdR, 0.5*birdR, 0.5*birdR, 1));
    }
}

//----------------------------------------------------------------------
// The loop responding to events from the user.

void eventLoop(XInfo &xinfo){
    XEvent event;
    KeySym key;
    int xButton;
    int yButton; 
  	char text[BufferSize];

    // In this area of bird
    int area1 = 20;
    int area2 = 3;
    int pathDensityCounter = 0;
    int pathDensity = 5;


    while ( true ){

        if (XPending(xinfo.display) > 0){
            XNextEvent(xinfo.display, &event);
            switch (event.type){
                case Expose:
                    {
                       XWindowAttributes attr;
                       XGetWindowAttributes(xinfo.display, xinfo.window,&attr); 
                        if ( event.xexpose.count == 0 ){

                            windowWidth = attr.width;
                            windowHeight = attr.height;
                            repaint( dVector, xinfo);
                        } // if
                        break;
                    }
                case MotionNotify:
                    if ( 
                            event.xbutton.x >= 
                            dVector[0]->getX() - area1
                            &&
                            event.xbutton.x <= 
                            (dVector[0]->getX() + area2*dVector[0]->getWidth())
                            &&
                            event.xbutton.y >=
                            dVector[0]->getY() - area1
                            &&
                            event.xbutton.y <= 
                            (dVector[0]->getY() + area2*dVector[0]->getLength())
                            &&
                            over == 0
                       ){

                        dVector[0]->setX( dVector[0]->getX() + 
                                event.xbutton.x - xButton);
                        dVector[0]->setY( dVector[0]->getY() +
                                event.xbutton.y - yButton);


                        xButton = event.xbutton.x;
                        yButton = event.xbutton.y;

                        repaint(dVector, xinfo);

                    } // if 
                    break;
                case ButtonPress:
                    if (
                            level == 1
                            &&
                            event.xbutton.x >= cheatingBoxInitX
                            &&
                            event.xbutton.x <= cheatingBoxInitX + cheatingBoxWidth
                            &&
                            event.xbutton.y >= cheatingBoxInitY
                            &&
                            event.xbutton.y <= cheatingBoxInitY + cheatingBoxLength
                       )
                    {
                        setup(2);
                    }

                    if (
                            event.xbutton.x >= restartBoxInitX
                            &&
                            event.xbutton.x <= restartBoxInitX + restartBoxWidth
                            &&
                            event.xbutton.y >= restartBoxInitY
                            &&
                            event.xbutton.y <= restartBoxInitY + restartBoxLength
                       )
                    {
                        setup(1);
                    }

                    xButton = event.xbutton.x;
                    yButton = event.xbutton.y;
                    break;

                case KeyPress:
                    {
                        int iNum = XLookupString( (XKeyEvent *)&event, text, BufferSize, &key, 0 );
                        if ( iNum == 1 && text[0] == 'q' ){
                            error( "Terminated normally." );
                        }
                        if ( iNum == 1 && text[0] == 'r' ){
                            setup(1);
                        }
                        break;
                    }

                case ButtonRelease:
                    int xChange = -(dVector[0]->getX() - birdX);            // birdX is initial x
                    int yChange = -(dVector[0]->getY() - birdY);            // birdY is initial y
                    double xVelocityInit = xChange * uVelocity;
                    double yVelocityInit = yChange * uVelocity;
                    double yVelocityLoss = 0.4;
                    double xVelocityLoss = 0.6; 
                    int time = 0;

                    if (over == 0 && (abs(xChange) > 1 || abs(yChange) > 0)){

                        // Kill one bird
                        if (dVectorBird.size() != 0){ 
                            dVectorBird.pop_back();
                        }

                        // Clean Slingshot Line
                        flying = 1;   


                        // Clean dVectorPath
                        dVectorPath.clear();
    
                        for (;;){

                            /*struct timeval start, end;

                              gettimeofday(&start, NULL);
                              long startTime = start.tv_sec * secondMicrosecond + start.tv_usec;

                              gettimeofday(&end, NULL);
                              long endTime = start.tv_sec * secondMicrosecond + end.tv_usec; 

                              double interval = (endTime - startTime)/secondMicrosecond;*/

                            time++;
                            usleep(usleepTime); 
                            double yVelocity = yVelocityInit + g*time;  
                            double intoEffectU = 0;

                            // If ball_1 is hit, break the
                            // loop
                            if (
                                    dVector[0]->getX() > ballX_1 - 0.5 * ballDiameter - birdR - intoEffectU*birdR && 
                                    dVector[0]->getX() < ballX_1 + 0.5 * ballDiameter                      &&
                                    dVector[0]->getY() > ballY_1 - 0.5 * ballDiameter - birdR - intoEffectU*birdR &&
                                    dVector[0]->getY() < ballY_1 + 0.2 * ballDiameter                      &&
                                    dVector[1]->getDiameter() > 0
                               ){

                                hitBall_1 = 1;
                                while (dVector[1]->getDiameter()>0){
                                    dVector[1]->setDiameter(dVector[1]->getDiameter() - scoreVelocity);
                                    usleep(3*usleepTime);
                                    repaint(dVector,xinfo);
                                    scoreRaiseCounter++;
                                }

                                scoreRaiseCounter = 0;
                                hitBall_1 = 0;
                                score = score + pointsBall;
                                break;
                            }

                            // If ball_2 is hit, break the
                            // loop
                            if (
                                    dVector[0]->getX() > ballX_2 - 0.5 * ballDiameter - birdR - intoEffectU*birdR && 
                                    dVector[0]->getX() < ballX_2 + 0.5 * ballDiameter                     &&
                                    dVector[0]->getY() > ballY_2 - 0.5 * ballDiameter - birdR - intoEffectU*birdR &&
                                    dVector[0]->getY() < ballY_2 + 0.2 * ballDiameter                     &&
                                    dVector[2]->getDiameter() > 0
                               ){
                                hitBall_2 = 1;
                                while (dVector[2]->getDiameter()>0){
                                    dVector[2]->setDiameter(dVector[2]->getDiameter() - scoreVelocity);
                                    usleep(3*usleepTime);
                                    repaint(dVector,xinfo);
                                    scoreRaiseCounter++;
                                }

                                scoreRaiseCounter = 0;
                                hitBall_2 = 0;
                                score = score + pointsBall;
                                break;
                            }

                            // If ball_3 is hit, break the
                            // loop
                            if (level == 2){
                                if (
                                        dVector[0]->getX() > dVector[3]->getX() - 0.5 * ballDiameter - birdR - intoEffectU*birdR && 
                                        dVector[0]->getX() < dVector[3]->getX() + 0.5 * ballDiameter                      &&
                                        dVector[0]->getY() > dVector[3]->getY() - 0.5 * ballDiameter - birdR - intoEffectU*birdR &&
                                        dVector[0]->getY() < dVector[3]->getY() + 0.2 * ballDiameter                      &&
                                        dVector[3]->getDiameter() > 0
                                   ){
                                    hitBall_3 = 1;
                                    while (dVector[3]->getDiameter()>0){
                                        dVector[3]->setDiameter(dVector[3]->getDiameter() - scoreVelocity);
                                        usleep(3*usleepTime);
                                        repaint(dVector,xinfo);
                                        scoreRaiseCounter++;
                                    }

                                    scoreRaiseCounter = 0;
                                    hitBall_3 = 0;
                                    score = score + pointsBall;
                                    break;
                                }
                            }


                            // Effect of hitting brick_1 and
                            // brick_2
                            //
                            // Change x velocity
                            if (
                                    dVector[0]->getX() >( brickX_1 - 1.2 * birdR ) &&
                                    dVector[0]->getX() <( brickX_1 + brickLength ) &&
                                    dVector[0]->getY() > brickY - brickNum_1 * brickLength &&
                                    dVector[0]->getY() < brickY 
                               ){
                                if ((dVector[0]->getX() - brickX_1 + 1.2 * birdR) <= (brickX_1 + brickLength) - dVector[0]->getX()){
                                    dVector[0]->setX(brickX_1 - 1.2* birdR);
                                }
                                else {
                                    dVector[0]->setX(brickX_1 + brickLength);
                                }
                                hit = 1;
                                repaint(dVector,xinfo);
                                xVelocityInit = -xVelocityLoss * xVelocityInit;
                            } 

                            if (
                                    dVector[0]->getX() >( brickX_2 - 1.2 * birdR ) &&
                                    dVector[0]->getX() <( brickX_2 + brickLength ) &&
                                    dVector[0]->getY() > brickY - brickNum_2 * brickLength &&
                                    dVector[0]->getY() < brickY 
                               ){
                                if ((dVector[0]->getX() - brickX_2 + 1.2 * birdR) <= (brickX_2 + brickLength) - dVector[0]->getX()){
                                    dVector[0]->setX(brickX_2 - 1.2 * birdR);
                                }
                                else {
                                    dVector[0]->setX(brickX_2 + brickLength);
                                }
                                hit = 1;
                                repaint(dVector,xinfo);
                                xVelocityInit = -xVelocityLoss * xVelocityInit;
                            } 

                            // Change yVelocity
                            if (
                                    dVector[0]->getX() > brickX_1 &&
                                    dVector[0]->getX() < ( brickX_1 + brickLength ) &&
                                    dVector[0]->getY() > brickY - brickNum_1 * brickLength - 1.2 * birdR &&
                                    dVector[0]->getY() < brickY - brickNum_1 * brickLength
                               ){
                                dVector[0]->setY( brickY - brickNum_1 * brickLength - 1.2 * birdR);
                                repaint(dVector, xinfo);

                                //xVelocityInit = -xVelocityInit;
                                hit = 1;
                                yVelocity = -yVelocityLoss * yVelocity;
                                time = 0;
                                yVelocityInit = yVelocity;
                            } 

                            if (

                                    dVector[0]->getX() > brickX_2 &&
                                    dVector[0]->getX() < ( brickX_2 + brickLength ) &&
                                    dVector[0]->getY() > brickY - brickNum_2 * brickLength - 1.2 * birdR &&
                                    dVector[0]->getY() < brickY - brickNum_2 * brickLength

                               ){
                                dVector[0]->setY( brickY - brickNum_2 * brickLength - 1.2 * birdR);
                                repaint(dVector, xinfo);

                                //xVelocityInit = -xVelocityInit;
                                hit = 1;
                                yVelocity = -yVelocityLoss * yVelocity;
                                time = 0;
                                yVelocityInit = yVelocity;
                            } 

                            // Effect of hitting brick_3 in
                            // level 2
                            if (level == 2){
                                if (
                                        dVector[0]->getX() >( brickX_3 - 1.2 * birdR ) &&
                                        dVector[0]->getX() <( brickX_3 + brickLength ) &&
                                        dVector[0]->getY() > brickY - brickNum_3 * brickLength &&
                                        dVector[0]->getY() < brickY
                                   ){
                                    if ((dVector[0]->getX() - brickX_3 + 1.2 * birdR) <= (brickX_3 + brickLength) - dVector[0]->getX()){
                                        dVector[0]->setX(brickX_3 - 1.2 * birdR);
                                    }
                                    else {
                                        dVector[0]->setX(brickX_3 + brickLength);
                                    }

                                    hit = 1;
                                    repaint(dVector,xinfo);
                                    xVelocityInit = -xVelocityLoss * xVelocityInit;
                                } 

                                if (
                                        dVector[0]->getX() > brickX_3 &&
                                        dVector[0]->getX() < ( brickX_3 + brickLength ) &&
                                        dVector[0]->getY() > brickY - brickNum_3 * brickLength - 1.2 * birdR &&
                                        dVector[0]->getY() < brickY - brickNum_3 * brickLength
                                   ){
                                    dVector[0]->setY( brickY - brickNum_3 * brickLength -   1.2 * birdR);
                                    repaint(dVector, xinfo);

                                    //xVelocityInit = -xVelocityInit;
                                    hit = 1;
                                    yVelocity = -yVelocityLoss * yVelocity;
                                    time = 0;
                                    yVelocityInit = yVelocity;
                                } 
                            }

                            // Effect of hitting walls
                            if(

                                    dVector[0]->getX() < 0 
                              ) {
                                dVector[0]->setX(0);
                                repaint(dVector, xinfo);

                                hit = 1;
                                xVelocityInit = -xVelocityLoss * xVelocityInit;
                            }

                            if(

                                    dVector[0]->getX() > windowWidth - 1.2 * birdR
                              ) {
                                dVector[0]->setX(windowWidth - 1.2 * birdR);
                                repaint(dVector, xinfo);

                                hit = 1;
                                xVelocityInit = -xVelocityLoss * xVelocityInit;
                            }

                            // Effect of hitting the ground 
                            if (
                                    dVector[0]->getY() >= brickY - 1.2 * birdR
                               ){

                                /* yVelocity = -yVelocity * yVelocityLoss * yVelocityLoss;
                                   xVelocityInit = xVelocityInit * xVelocityLoss; 
                                   time = 0;
                                   yVelocityInit = yVelocity;

                                   cout << "yVelocity" << yVelocity << endl;
                                   cout << "xVelocityInit" << xVelocityInit << endl; 

                                   if ( (abs(yVelocity)*abs(yVelocity)) + (abs(xVelocityInit)*abs(xVelocityInit))  < 1.5) {

                                   dVector[0]->setX(dVector[0]->getX() + xVelocityInit);
                                   dVector[0]->setY(dVector[0]->getY() + yVelocity);
                                   repaint(dVector,xinfo);
                                   sleep(1);
                                   break;
                                   }*/
                                dVector[0]->setY(brickY - 1.2 * birdR);
                                repaint(dVector, xinfo);

                                yVelocity = -yVelocityLoss * yVelocity;
                                time = 0;
                                yVelocityInit = yVelocity;

                                xVelocityInit = xVelocityLoss * xVelocityInit;

                                hit = 1;

                                if ( pow(xVelocityInit, 2) + pow(yVelocity,2) < 0.002 * windowWidth){
                                    usleep(500000);
                                    break;
                                }
                            }

                            // Effect of hitting ceiling
                            if (
                                    dVector[0]->getY() < layoutInitialY
                               ){

                                // Avoid the shaking bug on
                                // the top of layout
                                dVector[0]->setY(layoutInitialY);
                                //yVelocity = abs(xVelocityInit);
                                yVelocity = -yVelocityLoss*yVelocity;
                                time = 0;
                                yVelocityInit = yVelocity;
                                // cout << "yVelocity" << yVelocity << endl;
                            }

                            // Record Path
                            if (pathDensityCounter % pathDensity == 0 && hit == 0){
                                dVectorPath.push_back (new Path(dVector[0]->getX(), dVector[0]->getY(), pathDiameter));
                            }

                            // Change position of bird

                            dVector[0]->setX(dVector[0]->getX() + xVelocityInit);
                            dVector[0]->setY(dVector[0]->getY() + yVelocity);
                            

                            // Change position of ball3 
                            
                            if (level == 2){
                                if (dVector[3]->getY() > brickY - ballDiameter * 0.5
                                        ||
                                    dVector[3]->getY() < layoutInitialY + ballDiameter * 0.5
                                   ){
                                    velocityBall_3 = -velocityBall_3;
                                }
                                dVector[3]->setY(dVector[3]->getY() + velocityBall_3);
                            }

                            pathDensityCounter++;

                            repaint(dVector,xinfo);
                        } // for


                        // Reset position of bird
                        dVector[0]->setX(birdX);
                        dVector[0]->setY(birdY);
                        
                        // Draw slingshot line since not
                        // flying
                        flying = 0;

                        // Draw path
                        hit = 0;

                        if (level == 1){
                            if (dVector[1]->getDiameter() == 0 && dVector[2]->getDiameter() == 0){
                                setup(2);
                            }
                        }
                        else{ 
                            if (dVector[1]->getDiameter() == 0 
                                    && 
                                dVector[2]->getDiameter() == 0
                                    &&
                                dVector[3]->getDiameter() == 0){

                                dVectorPath.clear();
                                dVector[0] = (new Empty()); 
                                over = 1;
                            }
                        }

                        if (over == 0 && dVectorBird.size() == 0){

                            dVectorPath.clear();
                            dVector[0] = (new Empty()); 
                            over = 1;
                            dead = 1;
                        }

                    } // if

                    // repaint(dVector, xinfo);

                    break; // break switch from the button release event
            } // switch
        } // if

        usleep(usleepTime);
        // Change position of ball3 
        //
        //
        if (level == 2){
            if (dVector[3]->getY() > brickY - ballDiameter * 0.5
                    ||
                dVector[3]->getY() < layoutInitialY + ballDiameter * 0.5
               ){
                velocityBall_3 = -velocityBall_3;
            }


            dVector[3]->setY(dVector[3]->getY() + velocityBall_3);

        }

        repaint( dVector, xinfo);
    } // while
}


//----------------------------------------------------------------------
//  Create the window;  initialize X.

void initX(int argc, char *argv[], XInfo &xinfo){
    XSizeHints hints;
    unsigned long background, foreground;
    char *display_name = getenv("DISPLAY");

    /*
     * Display opening uses the DISPLAY  environment variable.
     * It can go wrong if DISPLAY isn't set, or you don't have permission.
     */  

    xinfo.display = XOpenDisplay( display_name );
    if ( !xinfo.display )
    {    
        error( "Can't open display." );
    }

    /*
     * Find out some things about the display you're using.
     */
    xinfo.screen = DefaultScreen( xinfo.display );
    background = WhitePixel( xinfo.display, xinfo.screen );
    foreground = BlackPixel( xinfo.display, xinfo.screen );

    /*
     * Set up hints and properties for the window manager, and open a window.
     * Arguments to XCreateSimpleWindow :
     *                 display - the display on which the window is opened
     *                 parent - the parent of the window in the window tree
     *                 x,y - the position of the upper left corner
     *                 width, height - the size of the window
     *                 Border - the width of the window border
     *                 foreground - the colour of the window border
     *                 background - the colour of the window background.
     * Arguments to XSetStandardProperties :
     *                 display - the display on which the window exists
     *                 window - the window whose properties are set
     *                 Hello1 - the title of the window
     *                 Hello2 - the title of the icon
     *                 none - a pixmap for the icon
     *                 argv, argc - a comand to run in the window
     *                 hints - sizes to use for the window.
     */
    hints.x = windowX;
    hints.y = windowY;
    hints.width = windowWidth;
    hints.height = windowHeight;
    hints.flags = PPosition | PSize;
    xinfo.window = XCreateSimpleWindow( 
            xinfo.display, DefaultRootWindow( xinfo.display ),
            hints.x, hints.y, hints.width, hints.height,
            Border, foreground, background 
            );
    XSetStandardProperties( 
            xinfo.display, xinfo.window, "Mad Birds", "Mad Birds", None,
            argv, argc, &hints 
            );

    /*
     * Get a graphics context and set the drawing colours.
     * Arguments to XCreateGC
     *           display - which uses this GC
     *           window - which uses this GC
     *           GCflags - flags which determine which parts of the GC are used
     *           GCdata - a struct that fills in GC data at initialization.
     */
    xinfo.gc = XCreateGC (xinfo.display, xinfo.window, 0, 0 );
    XSetBackground( xinfo.display, xinfo.gc, background );
    XSetForeground( xinfo.display, xinfo.gc, foreground );

    /*
     * Tell the window manager what input you want.
    */

    XSelectInput( 
            xinfo.display, xinfo.window,
            ButtonPressMask | ButtonReleaseMask | ButtonMotionMask
            | ExposureMask | KeyPressMask
            );

    /*
     * Put the window on the screen.
     */
    XMapRaised( xinfo.display, xinfo.window );

    /* flush all pending requests to the X server, and wait until */
    /* they are processed by the X server.                        */
    XSync(xinfo.display, False);
    sleep(1);
}
//======================================================================
//  Parse Command Line Arguments Helper Function
//======================================================================

char* getCmdOption(char ** begin, char ** end, const std::string & option){
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option){
    return std::find(begin, end, option) != end;
}


/*
 * Start executing here.
 *   First initialize window.
 *   Next loop responding to events.
 *   Exit forcing window manager to clean up - cheesy, but easy.
 */


int main ( int argc, char *argv[] ) 
{	
    string usage = "See ./madBird -h for help\n";
    string help = "./madBird\n-vp   to specify velocity of the flying pig in level 2, should be a positive integer (default 1)\n-f    to specify frame/s, shoule be a positive integer (default 50)\n-vm   to specify velocity multiplier, shoule be a positive floating number (default 0.1)\n-n1   to specify number of birds in level 1, should be a positive integer (default 2)\n-n2   to specify number of birds in level 2, should be a positive integer(default 3)\n-help see help message\n";

    if(cmdOptionExists(argv, argv+argc, "-help"))
    {
        cout << help;
        exit( EXIT_SUCCESS );
    }

    char * pigV = getCmdOption(argv, argv + argc, "-vp");

    if(pigV){
        int pigVelocity = atoi(pigV);
        
        if (pigVelocity <= 0){
            cerr << usage; 
            exit( EXIT_FAILURE );
        }

        velocityBall_3 = pigVelocity;
    }

    char * sframePerSecond = getCmdOption(argv, argv + argc, "-f");

    if(sframePerSecond){
        int framePerSecond = atoi(sframePerSecond);
        
        if (framePerSecond <= 0){
            cerr << usage; 
            exit( EXIT_FAILURE );
        }

        usleepTime = 1000000/framePerSecond;
    }

    char * sV = getCmdOption(argv, argv + argc, "-vm");

    if(sV){
        double v;
        v = atof(sV);

        if (v <= 0){
            cerr << usage; 
            exit( EXIT_FAILURE );
        }

        uVelocity = v;
    }

    char * nBird_1 = getCmdOption(argv, argv + argc, "-n1");

    if(nBird_1){
        int birdN_1 = atoi(nBird_1);

        if (birdN_1 <= 0){
            cerr << usage; 
            exit(EXIT_FAILURE);
        }

        birdsLeft_1 = birdN_1;

    }

    char * nBird_2 = getCmdOption(argv, argv + argc, "-n2");

    if(nBird_2){
        int birdN_2 = atoi(nBird_2);

        if (birdN_2 <= 0){
            cerr << usage; 
            exit(EXIT_FAILURE);
        }

        birdsLeft_2 = birdN_2;

    }

    XInfo xinfo;
    initX(argc, argv, xinfo);
    setupGC(xinfo);
    setup(1);
    repaint(dVector, xinfo);

    /* make a delay for a short period. */
    eventLoop(xinfo);

}
