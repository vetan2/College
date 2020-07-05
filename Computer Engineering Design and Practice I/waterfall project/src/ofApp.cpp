#include "ofApp.h"
#include "waterObj.h"

using namespace std;
waterObj water;

//--------------------------------------------------------------
void ofApp::setup(){
	// 배경 색을 자주색으로, 선분의 굵기를 5로 설정한다.
	ofBackground(144, 12, 63);
	ofSetLineWidth(5);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (flow_flag_draw == TRUE) {
		if (waterLineIndex != water.pathCnt - 2) {
			waterLineIndex++;
		}

		else
			flow_flag_finish = TRUE;
	}

	if (flow_flag_erase == TRUE) {
		if (waterLineIndex != water.pathCnt - 1) {
			waterLineIndex++;
		}

		else
			flow_flag_finish = TRUE;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	int i, j;
	// 수업자료와 유사하게 만들기 위해 배경 위에 추가로 큰 흰색 직사각형을 하나 그린다.
	ofSetColor(255);
	ofDrawRectangle(0, 30, 1024, 708);

	// d key가 눌렸다면 그림을 그린다.
	if (draw_flag == TRUE) {
		
		// 선분을 그린다.
		ofSetColor(27, 38, 49);
		for (i = 0; i < lineCnt; i++)
			ofDrawLine(lineCor[i][0], lineCor[i][1], lineCor[i][2], lineCor[i][3]);

		// 점을 그린다. 물이 나오는 점은 빨간색으로 그린다.
		ofSetColor(77, 86, 86);
		for (i = 0; i < dotCnt; i++) {
			if (i == faucet)
				ofSetColor(255, 87, 51);

			ofDrawCircle(dotCor[i][0], dotCor[i][1], 10);
			ofSetColor(77, 86, 86);
		}

		if (flow_flag_draw == TRUE) {
			ofSetColor(52, 152, 219);
			for (i = 0; i <= waterLineIndex; i++)
				ofDrawLine(water.pathCor[i][0], water.pathCor[i][1],
					water.pathCor[i + 1][0], water.pathCor[i + 1][1]);
		}

		if (flow_flag_erase == TRUE) {
			ofSetColor(52, 152, 219);
			for (i = waterLineIndex; i <= water.pathCnt - 2; i++)
				ofDrawLine(water.pathCor[i][0], water.pathCor[i][1],
					water.pathCor[i + 1][0], water.pathCor[i + 1][1]);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	/*
		d key가 눌렸고, 선분과 점의 정보를 제대로 받았을 경우,
		→, ← key를 누름에 따라 물이 나오는 점의 위치를 변경한다.
	*/
	if (draw_flag == TRUE && faucet >= 0 && flow_flag_draw == FALSE) {
		int i;

		// →
		if (key == OF_KEY_RIGHT) {
			// 물이 나오는 점이 맨 오른쪽이면 맨 왼쪽으로
			if (faucet == dotCnt - 1)
				faucet = 0;

			else
				faucet++;

			cout << "Selected Dot Coordinate is (" << dotCor[faucet][0] << ", " << dotCor[faucet][1]
				<< ")" << endl;
		}

		// ←
		if (key == OF_KEY_LEFT) {
			// 물이 나오는 점이 맨 왼쪽이면 맨 오른쪽으로
			if (faucet == 0)
				faucet = dotCnt - 1;

			else
				faucet--;

			cout << "Selected Dot Coordinate is (" << dotCor[faucet][0] << ", " << dotCor[faucet][1]
				<< ")" << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	/*
		input file으로부터 선과 점의 정보를 받은 후,
		처리 결과에 따라 메시지를 출력한다.
	*/
	if (key == 'l') {

		// input file으로부터 선과 점의 정보를 받는다.
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Load file");

		if (openFileResult.bSuccess)
			processOpenFileSelection(openFileResult);

		// 처리 결과에 따라 메시지를 출력한다.
		if (load_flag == FALSE)
			cout << "Fail to read the data" << endl;

		if (load_flag == TRUE) {
			cout << "The number of line is: " << lineCnt << endl;
			cout << "The number of dot is " << dotCnt << endl;
		}
	}

	if (key == 'd') {
		if (load_flag == TRUE) {
			draw_flag = TRUE;

			// 물이 나오는 점의 좌표를 출력
			cout << "Selected Dot Coordinate is (" << dotCor[faucet][0] << ", " << dotCor[faucet][1]
				<< ")" << endl;
		}

	}

	if (key == 's') {
		if (draw_flag == TRUE && flow_flag_draw == FALSE && flow_flag_finish == TRUE) {
			flow_flag_erase = FALSE;

			// 물의 경로를 계산 후 저장
			water = initWater(water);
			water.getPath();

			waterLineIndex = 0;
			flow_flag_finish = FALSE;
			flow_flag_draw = TRUE;
		}
	}

	if (key == 'e') {
		if (flow_flag_draw == TRUE && flow_flag_finish == TRUE) {
			waterLineIndex = 0;
			flow_flag_draw = FALSE;
			flow_flag_finish = FALSE;
			flow_flag_erase = TRUE;
		}
	}

	// q key가 눌리면, 할당된 배열들의 메모리를 해제하고 프로그램을 종료한다.
	if (key == 'q') {
		int i;

		if (lineCor != NULL) {
			for (i = 0; i < lineCnt; i++)
				delete lineCor[i];
			delete lineCor;
			cout << "Memory for line segment has been freed." << endl;
		}

		if (dotCor != NULL) {
			for (i = 0; i < dotCnt; i++)
				delete dotCor[i];
			delete dotCor;
			cout << "Memory for dot has been freed." << endl;
		}

		ofExit();
	}
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	string fileName = openFileResult.getName();
	ofFile file(fileName);

	load_flag = FALSE;

	// input file을 못 찾았으면 에러 메시지를 출력
	if (!file.exists())
		cout << "Target file does not exists." << endl;

	// input file을 찾았으면,
	else {
		cout << "We found the target file." << endl;

		// 버퍼 및 반복문에 쓰일 변수 설정
		ofBuffer buffer(file);
		int i, j;

		ofBuffer::Line it = buffer.getLines().begin();
		string line = *it;

		// 선분의 정보를 읽어들이는 과정
		lineCnt = stoi(line);
		if (lineCnt) {

			// 배열 생성
			lineCor = new int* [lineCnt];

			for (i = 0; i < lineCnt; i++)
				lineCor[i] = new int[4];

			/*
				lineCnt만큼 한 줄 한 줄 읽어나가면서 선분을 이루는 점들의 좌표값을 lineCor에 저장
				좌표값이 화면 범위를 넘어가면 load_flag가 0인 채로 함수에서 빠져나간다.
			*/
			for (i = 0; i < lineCnt; i++) {
				it++;
				line = *it;

				vector<string> nums = ofSplitString(line, " ");
				for (j = 0; j < nums.size() || j < 4; j++)
					if (j % 2)
						if (atoi(nums[j].c_str()) >= 30 && atoi(nums[j].c_str()) <= 738)
							lineCor[i][j] = atoi(nums[j].c_str());

						else
							return;

					else
						if (atoi(nums[j].c_str()) >= 0 && atoi(nums[j].c_str()) <= 1024)
							lineCor[i][j] = atoi(nums[j].c_str());

						else
							return;
					
			}
		}
		
		// 버퍼를 다음 줄로 이동
		it++;
		line = *it;

		// 점의 정보를 읽어들이는 과정
		dotCnt = stoi(line);
		if (dotCnt) {

			// 배열 생성
			dotCor = new int* [dotCnt];
			for (i = 0; i < dotCnt; i++)
				dotCor[i] = new int[2];

			/*
				dotCnt만큼 한 줄 한 줄 읽어나가면서 점의 좌표값을 dotCor에 저장
				좌표값이 화면 범위를 넘어가면 load_flag가 0인 채로 함수에서 빠져나간다.
			*/
			for (i = 0; i < dotCnt; i++) {
				it++;
				line = *it;

				vector<string> nums = ofSplitString(line, " ");
				for (j = 0; j < nums.size() || j < 2; j++)
					if (j % 2)
						if (atoi(nums[j].c_str()) >= 50 && atoi(nums[j].c_str()) <= 725)
							dotCor[i][j] = atoi(nums[j].c_str());

						else
							return;

					else
						if (atoi(nums[j].c_str()) >= 0 && atoi(nums[j].c_str()) <= 1050)
							dotCor[i][j] = atoi(nums[j].c_str());

						else
							return;
			}

			// dotCor을 x 좌표의 오름차순으로 정렬한다. (insert sort)
			for (i = 1; i < dotCnt; i++) {
				int key = dotCor[i][0], j = i - 1;
				while (j >= 0 && key < dotCor[j][0]) {
					int temp_x, temp_y;
					temp_x = dotCor[j][0], temp_y = dotCor[j][1];
					dotCor[j][0] = dotCor[j + 1][0], dotCor[j][1] = dotCor[j + 1][1];
					dotCor[j + 1][0] = temp_x, dotCor[j + 1][1] = temp_y;
					j--;
				}
				dotCor[j + 1][0] = key;
			}

			// -1이었던 faucet을 0으로 바꿔준다.
			faucet = 0;
		}

		// 선분과 점의 정보까지 무사히 읽어들였으면 load_flag = 1
		load_flag = TRUE;
	}
}

waterObj ofApp::initWater(waterObj water) {
	water.curCor[0] = dotCor[faucet][0];
	water.curCor[1] = dotCor[faucet][1];
	water.lineCor = lineCor;
	water.lineCnt = lineCnt;
	water.pathCnt = 0;
	water.quit = FALSE;
	return water;
}