#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	while (!maze) {
		cin >> N;
		cin >> M;
		createMaze(N, M);
	}

	ofFileDialogResult result = ofSystemSaveDialog("maze.maz", "Save");
	if (result.bSuccess) {
		string path = result.getPath();
		ofstream out(path);

		for (int i = 0; i <= 2 * M; i++) {
			for (int j = 0; j <= 2 * N; j++)
				switch (maze[i][j]) {
				case -1: out << '-'; break;
				case -2: out << '|'; break;
				case -3: out << '+'; break;
				default: out << ' '; break;
				}

			if (i != 2 * M)
				out << endl;
		}
	}

	ofExit();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/*--------------------------------------------------------------
	input : 미로의 가로(N)와 세로(M)
	output : x

	N, M에 따라 미로를 생성하고(Eller's algorithm)
	그 구성을 maze에 저장한다.
*/
void ofApp::createMaze(int N, int M) {
	if (N <= 0 || M <= 0) {
		cout << "wrong input" << endl;
		return;
	}

	int i, j;
	int curRow = 1;
	bool quit = FALSE;

	// 메모리 할당
	maze = new int* [2 * M + 1];
	for (i = 0; i <= 2 * M; i++)
		maze[i] = new int[2 * N + 1];

	/*-----------------------------------------
		할당된 메모리에 초기값을 저장
	*/
	for (i = 0; i <= 2 * M; i++)
		for (j = 0; j <= 2 * N; j++)

			// 방의 집합 번호
			if (i % 2 && j % 2)
				maze[i][j] = (i - 1) / 2 * N + (j + 1) / 2;

			// 가로 벽
			else if (!(i % 2) && j % 2)
				maze[i][j] = -1;

			// 세로 벽
			else if (i % 2 && !(j % 2))
				maze[i][j] = -2;

			// 십자 벽
			else
				maze[i][j] = -3;

	/*-----------------------------------------
		미로를 구축하기 위해 가로 벽, 세로 벽을 없애는 작업을 수행한다.
	*/
	while(!quit) {
		digVerticalWall(curRow);
		mergeRoomSet(1, curRow);
		mergeRoomSet(curRow, 1);

		// curRow가 마지막 줄이라면
		if (curRow == 2 * M - 1)
			quit = TRUE;
		
		// curRow가 마지막 줄이 아니라면
		else {
			curRow++;
			digHorizonWall(curRow);
			curRow++;
		}

		mergeRoomSet(1, curRow);
		mergeRoomSet(curRow, 1);
	}

	printMaze();
}

/*--------------------------------------------------------------
	input : 현재 행
	output : x

	 미로의 현재 행 curRow의 세로 벽을 무작위로 제거한다.
*/
void ofApp::digVerticalWall(int curRow) {
	int j;

	// maze[curRow][j]는 줄이 curRow인 줄의 세로 벽이다.
	for (j = 2; j <= 2 * (N - 1); j += 2) {

		// 벽의 양 옆의 방이 서로 다른 집합에 속해 있으면
		if (maze[curRow][j - 1] != maze[curRow][j + 1])
			if ((int)ofRandom(2) == 1) {	// 랜덤 변수

				// 벽을 없앰
				maze[curRow][j] = 0;

				// 벽 오른쪽의 방과 벽 왼쪽의 방을 같은 집합으로 연결함(숫자가 작은 쪽으로)
				mergeInt(maze[curRow] + (j - 1), maze[curRow] + (j + 1));

				mergeRoomSet(curRow, 1);
			}
	}

	/*
		 마지막 줄의 모든 집합은 집합 하나로 통일되어야 하므로
		벽 양 옆의 방이 서로 다른 집합에 속해 있으면 그 벽을 없앤다.
	*/
	if (curRow == 2 * M - 1) {
		for (j = 2; j <= 2 * (N - 1); j += 2) {

			// 벽의 양 옆의 방이 서로 다른 집합에 속해 있으면
			if (maze[curRow][j - 1] != maze[curRow][j + 1]) {

				// 벽을 없앰
				maze[curRow][j] = 0;

				// 벽 오른쪽의 방과 벽 왼쪽의 방을 같은 집합으로 연결함(숫자가 작은 쪽으로)
				mergeInt(maze[curRow] + (j - 1), maze[curRow] + (j + 1));

				mergeRoomSet(1, curRow);
				mergeRoomSet(curRow, 1);
			}
		}
	}
}

/*--------------------------------------------------------------
	input : 현재 행
	output : x

	 미로의 현재 행 curRow의 가로 벽을 무작위로 제거한다.
	단, curRow 위에 있는 방들 중, 같은 집합의 방들에 대해
	적어도 하나의 가로 벽이 제거되어야 한다.
*/
void ofApp::digHorizonWall(int curRow) {
	int j;

	/*
		 벽 위에 고립된 집합이 있으면 안 되므로,
		벽 위의 모든 집합이 벽 아래의 행과 연결되도록 하는 데 쓰이는 flag이다.
	*/
	bool dig_flag = TRUE;

	// maze[curRow][j]는 줄이 curRow인 점의 가로 벽이다.
	for (j = 1; j <= N * 2 - 1; j += 2) {

		// 벽의 위아래의 방이 서로 다른 집합에 속해 있으면
		if (maze[curRow - 1][j] != maze[curRow + 1][j]) {
			if ((int)ofRandom(2) == 1) {	// 랜덤 변수

				// 벽을 없앰
				maze[curRow][j] = 0;

				// 가로벽 아래의 방을 현재 방과 같은 집합으로 연결함(숫자가 작은 쪽으로)
				mergeInt(maze[curRow - 1] + j, maze[curRow + 1] + j);

				dig_flag = FALSE;
			}

			// maze[curRow][j]가 맨 오른쪽의 가로 벽이라면
			if (j == 2 * N - 1) {
				if (dig_flag == TRUE) {
					maze[curRow][j] = 0;

					mergeInt(maze[curRow - 1] + j, maze[curRow + 1] + j);
				}
			}

			// 벽 위의 방과 벽 우측 상단의 방이 서로 다른 집합에 속해있다면
			else if (maze[curRow - 1][j] != maze[curRow - 1][j + 2]) {
				if (dig_flag == TRUE) {
					maze[curRow][j] = 0;

					mergeInt(maze[curRow - 1] + j, maze[curRow + 1] + j);
				}

				dig_flag = TRUE;
			}
		}
	}
}

/*--------------------------------------------------------------
	input : 시작 행, 끝 행
	output : x

	  미로의 시작 행부터 끝 행까지,
	 연결된 방들끼리는 같은 집합끼리 연결한다.(숫자가 작은 쪽으로)
*/
void ofApp::mergeRoomSet(int startRow, int endRow) {
	int i = startRow, j, n;

	while (1) {

		// 맨 왼쪽 방에서 맨 오른쪽 방으로 한 번
		for (j = 1; j <= 2 * N - 1; j += 2) {
			if (!maze[i - 1][j] && i != 1)
				mergeInt(maze[i - 2] + j, maze[i] + j);

			if (!maze[i][j + 1] && j != 2 * N - 1)
				mergeInt(maze[i] + (j + 2), maze[i] + j);

			if (!maze[i + 1][j] && i != 2 * M - 1)
				mergeInt(maze[i + 2] + j, maze[i] + j);

			if (!maze[i][j - 1] && j != 1)
				mergeInt(maze[i] + (j - 2), maze[i] + j);
		}

		// 맨 오른쪽 방에서 맨 왼쪽 방으로 한 번
		for (j = 2 * N - 1; j >= 1; j -= 2) {
			if (!maze[i - 1][j] && i != 1)
				mergeInt(maze[i - 2] + j, maze[i] + j);

			if (!maze[i][j + 1] && j != 2 * N - 1)
				mergeInt(maze[i] + (j + 2), maze[i] + j);

			if (!maze[i + 1][j] && i != 2 * M - 1)
				mergeInt(maze[i + 2] + j, maze[i] + j);

			if (!maze[i][j - 1] && j != 1)
				mergeInt(maze[i] + (j - 2), maze[i] + j);
		}

		if (i == endRow)
			break;

		// 현재 행에서 위로
		if (startRow > endRow)
			i -= 2;

		// 현재 행에서 아래로
		else
			i += 2;
	}
}

/*--------------------------------------------------------------
	input : 두 정수의 포인터
	output : x

	 두 정수의 값을 두 정수 중 작은 값으로 통일한다.
*/
void ofApp::mergeInt(int* a, int* b) {
	if (*a < *b)
		* b = *a;

	else if (*a > * b)
		* a = *b;
}

void ofApp::printMaze() {
	int i, j;
	for (i = 0; i <= 2 * M; i++) {
		for (j = 0; j <= 2 * N; j++)
			switch (maze[i][j]) {
			case 0: cout.width(2); cout << left << ' '; break;
			case -1: cout.width(2); cout << left << '-'; break;
			case -2: cout.width(2); cout << left << '|'; break;
			case -3: cout.width(2); cout << left << '+'; break;
			default: cout.width(2); cout << left << maze[i][j];
			}

		cout << endl;
	}
	cout << endl;
}