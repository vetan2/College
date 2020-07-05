#pragma once

#include "ofMain.h"

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

		void createMaze(int, int);
		void digVerticalWall(int);
		void digHorizonWall(int);
		void mergeRoomSet(int, int);
		void mergeInt(int*, int*);
		void printMaze();

		// 미로의 가로와 세로의 길이
		int N, M;

		/*
			미로의 구성 정보
			양수	:  각 방이 속해있는 집합 번호.
				  maze의 메모리가 할당된 후, 미로를 구축하기 전,
				  상단 좌측부터 우측 하단까지 1부터 N * M의 값을 가지게 됨
			0	:  뚫린 공간(벽이 없음)
			-1	:  가로 벽(-)
			-2	:  세로 벽(|)
			-3	:  십자 벽(+)
		*/
		int** maze = NULL;
};
