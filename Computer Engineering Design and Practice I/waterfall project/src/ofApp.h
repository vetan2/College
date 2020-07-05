#pragma once

#include "waterObj.h"
#include "ofMain.h"
#include <fstream>
#include <iostream>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void processOpenFileSelection(ofFileDialogResult openFileResult);
		waterObj initWater(waterObj);

		/*
			draw_flag : d key가 눌렸는지의 여부, 눌렸으면 1, 아니면 0

			load_flag : input file에서 선분과 점의 정보를 제대로 받았는지의 여부.
			input file을 찾고, 정보들을 제대로 받았으면 1, 아니면 0

			flow_flag_update : 물의 경로를 업데이트하는가의 여부
			하면 1, 아니면 0

			flow_flag_draw : 물의 흐름을 그리는가의 여부
			그리면 1, 아니면 0

			flow_flag_erase : 물의 흐름을 지우는가의 여부
			지우면 1, 아니면 0
		*/
		bool draw_flag = FALSE;
		bool load_flag = FALSE;
		bool flow_flag_draw = FALSE;
		bool flow_flag_finish = TRUE;
		bool flow_flag_erase = FALSE;

		/*
			Line
			lineCor : 선분을 잇는 두 점의 좌표를 저장할 포인터
			lineCnt : 선분의 개수
		*/
		int** lineCor = NULL;
		int lineCnt;
		int waterLineIndex = 0;

		/*
			Dot
			dotCor : 점의 좌표를 저장할 포인터
			dotCnt : 점의 개수
		*/
		int** dotCor = NULL;
		int dotCnt;

		// 물이 나오는 점의 index, 점의 정보가 제대로 받아지지 않았을 경우를 대비해 -1로 초기화
		int faucet = -1;
};