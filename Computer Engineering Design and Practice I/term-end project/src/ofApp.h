#pragma once

#include "ofMain.h"
#include "structObj.h"
#include "waterObj.h"

#define MODE_CNT	7
#define UNDO		0
#define REDO		1
#define DOT			2
#define LINE		3
#define ERASE		4
#define	FAUCET		5
#define DISCHARGE	6

#define MAX_STRUCT_SET 20

class ofApp : public ofBaseApp {
	public:
		// menu & icon
		ofRectangle menuBox;
		ofImage icon[MODE_CNT];
		ofRectangle iconBox[MODE_CNT];

		// click
		bool isIconClicked = false;
		int iconIndex_clicked;
		bool isClicked;
		ofVec2f mousePressedPos;
		ofVec2f mouseReleasedPos;

		// mode
		bool mode[MODE_CNT] = { false, };
		bool step[3] = { false, };
		float waterRadius;
		bool waterFlowing = false;
		bool waterDraining = false;

		// error message
		ofTrueTypeFont errorFont;
		string errorMessage;
		ofRectangle errorBox;
		float errorOffsetX;
		int errorFontBright = 255;
		int errorTimer;

		// structure objects
		structSet* structQueue_front = NULL;
		structSet* structQueue_rear = NULL;
		int structSetCnt = 0;
		structSet* structQueue_cur = NULL;
		dotObj tempDot;
		lineObj tempLine;

		// water object
		float waterFlowSpeed;
		int waterDischargeCycle;
		waterObj* water = NULL;
		waterIndexNode** waterIndexQueue_front = NULL;
		waterIndexNode** waterIndexQueue_rear = NULL;

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

		// setting
		void setMenu();
		void setStructSet();

		// Draw
		void drawGrid(int interval);
		void drawWater(float radius);
		void drawDots();
		void drawLines();
		void drawMenu();

		// Handle the mode
		void handleIconClick();
		void changeMode(ofVec2f mousePos);
		void inactivateModes();

		// Work according to the mode
		void runMode();
		void undo();
		void redo();
		void addDot(ofVec2f pos);
		void addLine(ofVec2f pos1, ofVec2f pos2);
		void deleteStruct(int sort, int indexToDel);
		void editFaucet(int dotIndex);
		void updateWaterIndex(waterIndexNode** front, waterIndexNode** rear, int maxPathIndex);

		// Check if the location of the structure is appropriate or not
		bool checkPos(ofVec2f pos);
		bool checkPos(ofVec2f pos1, ofVec2f pos2);

		// Copy a structSet
		void copyStructSet(int sort, structSet* dest, structSet* src);
		void addSetInQueue(structSet* newSet);

		// Control error message output
		void setErrorMessage(string message);
		void printErrorMessage();
};