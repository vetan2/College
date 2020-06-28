#pragma once

#include "ofMain.h"

class dotObj {
	public:
		ofVec2f pos;
		int radius = 10;
		bool faucet = false;

	public:
		void draw();
		float distance(ofVec2f anotherPos);
		float distance(ofVec2f pos1, ofVec2f pos2);
};

class lineObj {
	public:
		ofVec2f pos1, pos2;
		int width = 10;

	public:
		void draw();
		float distance(ofVec2f pos);
		bool intersectWith(ofVec2f pos3, ofVec2f pos4);
};

class structSet {
	public:
		int dotCnt;
		dotObj* dot;
		int faucetCnt;

		int lineCnt;
		lineObj* line;

		struct structSet* next;
		struct structSet* prev;
};