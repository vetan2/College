#pragma once

#include "ofMain.h"
#include "structObj.h"
#include <random>

#define MAX_PATH_CNT 60000

enum state { vertical, verticalTillEnd, diagnal };

class waterObj{
	public:
		ofVec2f path[MAX_PATH_CNT];
		int pathCnt;

		lineObj* modifiedLine = NULL;
		int lineCnt;
		ofVec2f destPos;

		ofVec2f velocity;
		float g = 9.8;
		float upsilon = 1;
		bool calcPathDone = false;

	public:
		void init(lineObj* line, int lineCnt, ofVec2f dotPos);

		state getState(ofVec2f pos);
		void calcPath();
		void moveVertically();
		void moveVerticallyTillEnd();
		void moveDiagnally();
};

typedef struct waterIndexNode {
	float index;
	waterIndexNode* link;
};