#pragma once

#include "ofMain.h"

#define MODE_CNT	9

#define UNDO		0
#define REDO		1
#define DOT			2
#define LINE		3
#define DELETE		4
#define ROTATE_DOT	5
#define ROTATE_LINE	6
#define	FAUCET		7
#define DISCHARGE	8

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void setMenu();

		void drawGrid(int interval);
		void drawLines();
		void drawDots();
		void drawMenu();

		void updateIconClick();
		void changeMode(int x, int y);
		void inactivateModes();

		void undo();
		void redo();

		void setErrorMessage(string message);
		void printErrorMessage();

		// menu & icon
		ofRectangle menuBox;
		ofImage icon[MODE_CNT];
		ofRectangle iconBox[MODE_CNT];
		// faucet RGB : 235, 125, 22
		// water RGB : 18, 115, 235

		// icon click
		bool iconClicked = FALSE;
		int iconIndex_clicked;

		// mode
		bool mode[MODE_CNT] = { false, };
		bool waterFlowing = false;
		bool waterDraining = false;

		// error message
		ofTrueTypeFont errorFont;
		string errorMessage;
		ofRectangle errorBox;
		float errorOffsetX;
		int errorFontBright = 255;
		int errorTimer;
};
