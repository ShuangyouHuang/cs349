// CS 349 Fall 2018
// A1: Breakout code sample
// You may use any or all of this code in your assignment!
// See makefile for compiling instructions

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

// X11 structures
Display* display;
Window window;

// fixed ball speed
int ballSpeed = 3;
long standardspeed;

// fixed frames per second animation
int FPS = 60;

// get current time
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

// build a specific color
int buildColor(double red, double green, double blue)
{
    return(
        (((int)(red*255)%256)<<16)+
        (((int)(green*255)%256)<<8)+
        (((int)(blue*255)%256)));
}

// entry point
int main( int argc, char *argv[] ) {

	string fps;
	string speed;
	if(argc > 2){
		fps = argv[1];
		speed = argv[2];
		istringstream iss (fps);
		iss >> FPS;
		standardspeed = 60/FPS;
		istringstream iss1 (speed);
		iss1 >> ballSpeed;
		ballSpeed = ballSpeed * standardspeed;

	}
	else if(argc > 1){
		fps = argv[1];
		istringstream iss (fps);
		iss >> FPS;
		ballSpeed = 180/FPS;

	}

	// create window
	display = XOpenDisplay("");
	if (display == NULL) exit (-1);
	int screennum = DefaultScreen(display);
	long background = WhitePixel(display, screennum);
	long foreground = BlackPixel(display, screennum);
	window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                            10, 10, 1280, 800, 2, foreground, background);

	// set events to monitor and display window
	XSelectInput(display, window, ButtonPressMask | KeyPressMask);
	XMapRaised(display, window);
	XFlush(display);

	// variables
	int lfast = 0;
	int rfast = 0;


	// ball postition, size, and velocity
	XPoint ballPos;
	ballPos.x = 640;
	ballPos.y = 700;
	int ballSize = 10;

	XPoint ballDir;
	
	ballDir.x = -ballSpeed;
	ballDir.y = -ballSpeed;

	// block position, size
	XPoint rectPos;
	rectPos.x = 560;
	rectPos.y = 760;
	int rectLen = 160;
	int rectWidth  = 5;

	// blocks position, size
	int block[6][10] = {0};
	block[0][0] = 1;
	block[0][1] = 2;
	block[0][5] = 4;
	block[0][9] = 1;
	block[1][0] = 2;
	block[1][1] = 4;
	block[1][4] = 2;
	block[1][5] = 4;
	block[1][6] = 2;
	block[1][9] = 2;
	block[2][2] = 2;
	block[2][8] = 1;
	block[3][0] = 3;
	block[3][1] = 1;
	block[3][3] = 3;
	block[3][5] = 3;
	block[3][7] = 3;
	block[3][9] = 4;
	block[4][2] = 2;
	block[4][3] = 2;
	block[4][4] = 1;
	block[4][6] = 4;
	block[4][7] = 2;
	block[4][8] = 1;
	block[5][0] = 2;
	block[5][3] = 2;
	block[5][4] = 4;
	block[5][6] = 2;
	block[5][7] = 3;
	block[5][9] = 3;

	int score = 0;
	string sscore;

	

	// create gc for drawing
	GC gc = XCreateGC(display, window, 0, 0);
	XWindowAttributes w;
	XGetWindowAttributes(display, window, &w);


	// create windowsize hint
	XSizeHints *sh;
	sh = XAllocSizeHints();
	sh->flags = PMinSize | PMaxSize;
	sh->min_width = sh->max_width = 1280;
	sh->min_height = sh->max_height = 800;
	XSetWMNormalHints(display, window, sh);
	//XSetWMSizeHints(d, w, sh, PMinSize | PMaxSize);
	XFree(sh);

	// save time of last window paint
	unsigned long lastRepaint = 0;

	// event handle for current event
	XEvent event;
	
	while (true){

	//setup
	block[0][0] = 1;
	block[0][1] = 2;
	block[0][5] = 4;
	block[0][9] = 1;
	block[1][0] = 2;
	block[1][1] = 4;
	block[1][4] = 2;
	block[1][5] = 4;
	block[1][6] = 2;
	block[1][9] = 2;
	block[2][2] = 2;
	block[2][8] = 1;
	block[3][0] = 3;
	block[3][1] = 1;
	block[3][3] = 3;
	block[3][5] = 3;
	block[3][7] = 3;
	block[3][9] = 4;
	block[4][2] = 2;
	block[4][3] = 2;
	block[4][4] = 1;
	block[4][6] = 4;
	block[4][7] = 2;
	block[4][8] = 1;
	block[5][0] = 2;
	block[5][3] = 2;
	block[5][4] = 4;
	block[5][6] = 2;
	block[5][7] = 3;
	block[5][9] = 3;

	ballPos.x = 640;
	ballPos.y = 700;
	rectPos.x = 560;
	rectPos.y = 760;
	ballDir.x = -ballSpeed;
	ballDir.y = -ballSpeed;
	score = 0;

	//text declaration
	string temp;
	string msg1;
	const char *msg = "GAME OVER";
	const char *msg3 = "LEVEL CLEAR!!!";
	const char *msg2 = "Press R to retry, press Q or Esc to exit game";
	//XFontStruct *font1;
	//font1 = XLoadQueryFont(display, "-misc-fixed-medium-r-normal--12-*-*-*-*-*-iso8859-15");








	int clearflag = 0;

	// event loop
	while ( true ) {
		





	
		// process if we have any events
		if (XPending(display) > 0) { 
			XNextEvent( display, &event ); 

			switch ( event.type ) {

				// mouse button press
				case ButtonPress:
					//cout << "CLICK" << endl;
					break;

				case KeyPress: // any keypress
					KeySym key;
					char text[10];
					int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0 );

					// move right
					if ( i == 1 && text[0] == 'd') {
						if(rectPos.x < 1280-rectLen){
							rectPos.x += 20;
						}
					}

					// move left
					if ( i == 1 && text[0] == 'a') {
						if(rectPos.x > 0){
							rectPos.x -= 20;
						}
					}

					// quit game
					if ( i == 1 && (text[0] == 'q' || text[0] == 27)) {
						XCloseDisplay(display);
						exit(0);
					}
					break;
				}
		}

		unsigned long end = now();	// get current time in microsecond

		if (end - lastRepaint > 1000000 / FPS) { 

			// clear background
			XClearWindow(display, window);

			// draw pad
			XDrawRectangle(display, window, gc, rectPos.x, rectPos.y, rectLen, rectWidth);

			// draw blocks
			for(int i=0;i<6;++i){
				for(int j=0;j<10;++j){
					if(block[i][j] == 4){
						XSetForeground(display, gc, buildColor(200.0, 200.0, 100.0));
						XFillRectangle(display, window, gc, j*128, i*60, 128, 60);
					}
					if(block[i][j] == 3){
						XSetForeground(display, gc, buildColor(1.0, 130.0, 223.0));
						XFillRectangle(display, window, gc, j*128, i*60, 128, 60);
					}
					if(block[i][j] == 2){
						XSetForeground(display, gc, buildColor(20.0, 100.0, 100.0));
						XFillRectangle(display, window, gc, j*128, i*60, 128, 60);
					}
					if(block[i][j] == 1){
						XSetForeground(display, gc, buildColor(20.0, 100.0, 20.0));
						XFillRectangle(display, window, gc, j*128, i*60, 128, 60);
					}


				}
			}

			XSetForeground(display, gc, buildColor(255.0, 255.0, 255.0));
			// draw ball from centre
			XFillArc(display, window, gc, 
				ballPos.x - ballSize/2, 
				ballPos.y - ballSize/2, 
				ballSize, ballSize,
				0, 360*64);

			// update ball position
			ballPos.x += ballDir.x;
			ballPos.y += ballDir.y;

			int skipflag = 0;
			// bounce ball
			if (ballPos.x + ballSize/2 > w.width && ballDir.x > 0){
				ballDir.x = -ballDir.x;
				skipflag = 1;
			}
			if (ballPos.x - ballSize/2 < 0 && ballDir.x < 0){
				ballDir.x = -ballDir.x;
				skipflag = 1;
			}
			if (ballPos.y - ballSize/2 < 0 && ballDir.y < 0){
				ballDir.y = -ballDir.y;
				skipflag = 1;
			}if (ballDir.y > 0 && ballPos.y + ballSize/2 >= 760 && ballPos.y + ballSize/2 < 760+ballSpeed && (ballPos.x - ballSize/2 >= rectPos.x && ballPos.x + ballSize/2 <= rectPos.x + rectLen)){
				ballDir.y = -ballDir.y;
				skipflag = 1;
			}

			if(skipflag == 0){
				for(int i=0;i<6;++i){
					for(int j=0;j<10;++j){
						if(block[i][j] > 0) {
							if(ballPos.x + ballSize/2 >= j*128 && ballPos.x - ballSize/2 <= (j+1)*128 
							&& ballPos.y + ballSize/2 >= i*60 && ballPos.y - ballSize/2 <= (i+1)*60){
								if(ballPos.x + ballSize/2 - j*128 < ballSpeed || (j+1)*128 - (ballPos.x - ballSize/2) < ballSpeed) {
									ballDir.x = -ballDir.x;
									block[i][j]--;
									score += 40;

									clearflag = 0;
									for(int i=0;i<6;++i){
										for(int j=0;j<10;++j){
											if(block[i][j] != 0){
												clearflag = 1;
												break; 
											}
										}
									}
									if(clearflag == 0){
										if (ballPos.y + ballSize/2 >= 800) {
											XClearWindow(display, window);
	
											//XSetFont(display, gc, font1);
											XDrawString(display, window, gc, 615, 300, msg3, strlen(msg3));
											stringstream ss;
											ss << score;
											temp = ss.str();
											msg1 = "Score:" + temp;
											XDrawString(display, window, gc, 620, 350, msg1.c_str(), strlen(msg1.c_str()));
											XDrawString(display, window, gc, 520, 400, msg2, strlen(msg2));


			
											break;
										}
									}
									
									break;
								}
								else if(ballPos.y + ballSize/2 - i*60 < ballSpeed || (i+1)*60 - (ballPos.y - ballSize/2)< ballSpeed) {
									ballDir.y = -ballDir.y;
									block[i][j]--;
									score += 40;
									if(clearflag == 0){
										if (ballPos.y + ballSize/2 >= 800) {
											XClearWindow(display, window);
	
											//XSetFont(display, gc, font1);
											XDrawString(display, window, gc, 615, 300, msg3, strlen(msg3));
											stringstream ss;
											ss << score;
											temp = ss.str();
											msg1 = "Score:" + temp;
											XDrawString(display, window, gc, 620, 350, msg1.c_str(), strlen(msg1.c_str()));
											XDrawString(display, window, gc, 520, 400, msg2, strlen(msg2));


			
											break;
										}
									}


									break;
								}
							}
						}
					}
				}
			}
			stringstream ss;
			ss << score;
			temp = ss.str();
			msg1 = "Score:" + temp;
			XDrawString(display, window, gc, 50, 50, msg1.c_str(), strlen(msg1.c_str()));

			XFlush( display );

			lastRepaint = now(); // remember when the paint happened
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(display) == 0) {
			usleep(1000000 / FPS - (now() - lastRepaint));
		}

		//GAMEOVER
		if (ballPos.y + ballSize/2 >= 800) {
			XClearWindow(display, window);

			//XSetFont(display, gc, font1);
			XDrawString(display, window, gc, 620, 300, msg, strlen(msg));
			stringstream ss;
			ss << score;
			temp = ss.str();
			msg1 = "Score:" + temp;
			XDrawString(display, window, gc, 620, 350, msg1.c_str(), strlen(msg1.c_str()));
			XDrawString(display, window, gc, 520, 400, msg2, strlen(msg2));


			
			break;
		}

	}

	while(true){
		XNextEvent( display, &event );
		if (event.type == KeyPress){
			KeySym key;
			char text[10];
			int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0 );
			
			// restart
			if ( i == 1 && text[0] == 'r') {
				break;
			}

			//quit game
			if ( i == 1 && (text[0] == 'q' || text[0] == 27)) {
				XCloseDisplay(display);
				exit(0);
			}
						
		}
	}
	










	
	}
	XCloseDisplay(display);
}
