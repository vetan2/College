#pragma once

#include "ofMain.h"
#include <fstream>
#include <iostream>

enum state { vertical, vertical_tillEnd, oblique};

class waterObj{
	public:
		state getState(float* curCor);	
		void getPath();					
		void verticalMove();			
		void vertical_tillEndMove();	
		void obliqueMove();

		bool quit = FALSE;

		int** lineCor;
		int lineCnt;

		float curCor[2];
		float pathCor[1500][2];
		int pathCnt = 0;
		float tempCor[2];

		float speed = 15;

		float upsilon = 1;
};