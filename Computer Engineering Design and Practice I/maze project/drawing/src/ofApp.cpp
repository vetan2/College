/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// add code here
	if (input_flag == TRUE)
		for (i = 0; i <= 2 * M; i++) {
			for (j = 0; j <= 2 * N; j++)

				if (i % 2 == 0)
					for (j = 1; j <= 2 * N - 1; j++) {
						if (maze[i][j] == -1) {
							ofDrawLine((j - 1) * gridLen, i * gridLen,
								(j + 1) * gridLen, i * gridLen);
							ofDrawCircle((j - 1) * gridLen, i * gridLen, 2.5);
							ofDrawCircle((j + 1) * gridLen, i * gridLen, 2.5);
						}

					}
				else
					for (j = 0; j <= 2 * N; j++)
						if (maze[i][j] == -2) {
							ofDrawLine(j * gridLen, (i - 1) * gridLen,
								j * gridLen, (i + 1) * gridLen);
							ofDrawCircle(j * gridLen, (i - 1) * gridLen, 2.5);
							ofDrawCircle(j * gridLen, (i + 1) * gridLen, 2.5);
						}

		}

	if (isDFS)
	{
		if (isOpen) {
			ofSetLineWidth(5);

			node* temp;
			int num1, num2;

			ofSetColor(200);
			temp = path;
			while (temp->link) {
				num1 = temp->num - 1;
				num2 = temp->link->num - 1;

				ofDrawLine(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen,
					((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen);
				ofDrawCircle(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen, 2.5);
				ofDrawCircle(((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen, 2.5);
				temp = temp->link;
			}

			ofSetColor(39, 174, 96);
			temp = stack_bot;
			while (temp->link) {
				num1 = temp->num - 1;
				num2 = temp->link->num - 1;

				ofDrawLine(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen,
					((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen);
				ofDrawCircle(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen, 2.5);
				ofDrawCircle(((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen, 2.5);
				temp = temp->link;
			}
		}

		else
			cout << "You must open file first" << endl;
	}

	if (isBFS)
	{
		if (isOpen) {
			ofSetLineWidth(5);

			node* temp;
			int num1, num2;

			ofSetColor(200);
			temp = path;
			while (temp->link) {
				num1 = temp->num - 1;
				num2 = temp->link->num - 1;

				ofDrawLine(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen,
					((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen);
				ofDrawCircle(((num1 % N) * 2 + 1) * gridLen, ((num1 / N) * 2 + 1) * gridLen, 2.5);
				ofDrawCircle(((num2 % N) * 2 + 1) * gridLen, ((num2 / N) * 2 + 1) * gridLen, 2.5);
				temp = temp->link;
			}
		}

		else
			cout << "You must open file first" << endl;
	}

	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			
			// TO DO
			int i, j;

			if (!file.exists()) {
				cout << "Target file does not exist." << endl;
				return FALSE;
			}

			if (maze)
				freeMemory();

			ofBuffer::Line it = buffer.getLines().begin();
			string line = *it;

			if (size(line))
				col = size(line);

			else
				return FALSE;

			while (size(line)) {
				if (!maze) {

					// newly allocate maze's memory
					maze = new int* [1];
					maze[0] = new int[col];
					row++;
				}

				else {

					/*
						reallocate maze's memory

						먼저 maze의 값들을 temp에 임시로 저장한다.
						maze의 메모리를 모두 해제하고 행이 하나 많게 메모리를 재할당한다.
						maze에 temp의 값들을 저장한다.
						temp의 메모리를 해제한다.
					*/
					int** temp;
					temp = new int* [row];
					for (i = 0; i < row; i++) {
						temp[i] = new int[col];

						for (j = 0; j < col; j++)
							temp[i][j] = maze[i][j];
					}

					for (i = 0; i < row; i++)
						delete maze[i];
					delete maze;

					row++;
					maze = new int* [row];
					for (i = 0; i < row - 1; i++) {
						maze[i] = new int[col];

						for (j = 0; j < col; j++)
							maze[i][j] = temp[i][j];
					}
					maze[i] = new int[col];

					for (i = 0; i < row - 1; i++)
						delete temp[i];
					delete temp;
				}

				// save values in maze
				for (j = 0; j < col; j++) {
					switch (line[j]) {
					case '+': maze[row - 1][j] = -3; break;
					case '|': maze[row - 1][j] = -2; break;
					case '-': maze[row - 1][j] = -1; break;
					case ' ': maze[row - 1][j] = 0; break;
					default:
						for (i = 0; i < row; i++)
							delete maze[i];
						delete maze;
						maze = NULL;

						cout << "Wrong character in the file." << endl;
						return FALSE;
					}
				}

				// 한 줄 넘어가고 line은 넘어간 그 줄을 가리킨다.
				it++;
				line = *it;
			}

			M = (row - 1) / 2;
			N = (col - 1) / 2;
			createGraph();
			
			// 10은 화면에 여백을 남기기 위함
			if (windowHeight / (float)(row + 10) > windowWidth / (float)(col + 10))
				gridLen = windowWidth / (float)(col + 10);

			else
				gridLen = windowHeight / (float)(row + 10);

			input_flag = TRUE;
			isDFS = isBFS = FALSE;

			return TRUE;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return FALSE;
		}
	}
}

/*
	 graph와 visited를 생성한다.
	input : M, N, maze
	result : newly created graph and visited
*/
void ofApp::createGraph() {
	int i, j, num;
	node* temp;

	// graph 생성
	graph = new node * [M * N + 1];

	for (i = 1; i <= 2 * M - 1; i += 2)
		for (j = 1; j <= 2 * N - 1; j += 2) {
			num = N * ((i - 1) / 2) + (j + 1) / 2;
			graph[num] = new node;
			graph[num]->num = num, graph[num]->link = NULL;
			temp = graph[num];

			// 동쪽으로 진행할 수 있을 때(오른쪽에 벽이 없을 때)
			if (!maze[i][j + 1]) {
				temp->link = new node;
				temp = temp->link;
				temp->num = N * ((i - 1) / 2) + (j + 3) / 2;
				temp->link = NULL;
			}

			// 남쪽으로 진행할 수 있을 때(아래에 벽이 없을 때)
			if (!maze[i + 1][j]) {
				temp->link = new node;
				temp = temp->link;
				temp->num = N * ((i + 1) / 2) + (j + 1) / 2;
				temp->link = NULL;
			}

			// 서쪽으로 진행할 수 있을 때(왼쪽에 벽이 없을 때)
			if (!maze[i][j - 1]) {
				temp->link = new node;
				temp = temp->link;
				temp->num = N * ((i - 1) / 2) + (j - 1) / 2;
				temp->link = NULL;
			}

			// 북쪽으로 진행할 수 있을 때(위에 벽이 없을 때)
			if (!maze[i - 1][j]) {
				temp->link = new node;
				temp = temp->link;
				temp->num = N * ((i - 3) / 2) + (j + 1) / 2;
				temp->link = NULL;
			}
		}

	// visited 생성
	visited = new int[N * M + 1];
	for (i = 0; i <= N * M; i++)
		visited[i] = 0;
}

// 제거되는 메모리 : maze, graph, visited, stack, queue
void ofApp::freeMemory() {
	int i;
	node* temp, * toDel;
	for (i = 0; i < row; i++)
		delete maze[i];
	delete maze;
	maze = NULL;

	for (i = 1; i <= M * N; i++)
		delete graph[i];
	delete graph;
	graph = NULL;

	delete visited;
	visited = NULL;

	InitNode();
	row = 0, col = 0;
	M = 0, N = 0;
}

void ofApp::DFS() {
	isBFS = FALSE;
	InitNode();

	stack_bot = new node;
	stack_top = stack_bot;
	stack_top->num = 1, stack_top->link = NULL;

	visited[1] = 1;

	bool found = FALSE;
	int num, num_next;
	node* temp;
	node* path_top = NULL;

	while (stack_top && !found) {
		num = stackPop();
		temp = graph[num];

		pathPush(&path_top, num);

		while (temp->link && !found) {
			num_next = temp->link->num;

			if (num_next == M * N)
				found = TRUE;

			else if (!visited[num_next]) {
				visited[num_next] = 1;

				pathPush(&path_top, num_next);
				stackPush(num);
				num = num_next;
				temp = graph[num_next];
			}

			else
				temp = temp->link;
		}
	}

	if (found) {
		stackPush(num), stackPush(M * N);
		pathPush(&path_top, num), pathPush(&path_top, M * N);
	}

	/*
	temp = stack_bot;
	while (temp) {
		cout << temp->num << " " << endl;
		temp = temp->link;
	}
	*/

	isDFS = TRUE;
}

void ofApp::stackPush(int n) {
	node* temp = new node;
	temp->num = n, temp->link = NULL;
	
	if (!stack_bot)
		stack_bot = stack_top = temp;

	else {
		stack_top->link = temp;
		stack_top = temp;
	}
}

int ofApp::stackPop() {
	if (stack_top) {
		int num;

		if (stack_bot == stack_top) {
			num = stack_top->num;
			delete stack_top;
			stack_bot = stack_top = NULL;
			return num;
		}

		node* toDel = stack_top;
		for (stack_top = stack_bot; stack_top->link != toDel; stack_top = stack_top->link)
			;
		num = toDel->num;
		delete toDel;
		return num;
	}

	return 0;
}
void ofApp::BFS() {
	//TO DO
	isDFS = FALSE;
	InitNode();

	queue_front = new node;
	queue_rear = queue_front;
	queue_rear->num = 1, queue_rear->link = NULL;

	visited[1] = 1;

	bool found = FALSE;
	int num, num_next;
	node* temp;
	node* path_top = NULL;

	while (queue_rear && !found) {
		num = queuePop();
		temp = graph[num];

		pathPush(&path_top, num);

		while (temp->link && !found) {
			num_next = temp->link->num;

			if (num_next == M * N)
				found = TRUE;

			else if (!visited[num_next]) {
				visited[num_next] = 1;

				pathPush(&path_top, num_next);
				queuePush(num_next);
				temp = temp->link;
			}

			else
				temp = temp->link;
		}
	}

	if (found) {
		stackPush(num), stackPush(M * N);
		pathPush(&path_top, num), pathPush(&path_top, M * N);
	}

	/*
	temp = stack_bot;
	while (temp) {
		cout << temp->num << " " << endl;
		temp = temp->link;
	}
	*/

	isBFS = TRUE;
}

void ofApp::queuePush(int n) {
	node* temp = new node;
	temp->num = n, temp->link = NULL;

	if (!queue_rear)
		queue_rear = queue_front = temp;

	else {
		queue_rear->link = temp;
		queue_rear = temp;
	}
}

int ofApp::queuePop() {
	if (queue_front) {
		int num;

		if (queue_front == queue_rear) {
			num = queue_front->num;
			delete queue_front;
			queue_front = queue_rear = NULL;
			return num;
		}

		node* toDel = queue_front;
		queue_front = queue_front->link;
		num = toDel->num;
		delete toDel;
		return num;
	}

	return 0;
}

void ofApp::InitNode() {
	node* temp;
	node* toDel;

	temp = stack_bot;
	while (temp) {
		toDel = temp;
		temp = temp->link;
		delete toDel;
	}
	stack_bot = stack_top = NULL;

	temp = queue_front;
	while (temp) {
		toDel = temp;
		temp = temp->link;
		delete toDel;
	}
	queue_rear = queue_front = NULL;

	temp = path;
	while (temp) {
		toDel = temp;
		temp = temp->link;
		delete toDel;
	}
	path = NULL;

	if (visited) {
		int i;
		for (i = 0; i <= N * M; i++)
			visited[i] = 0;
	}
}

void ofApp::pathPush(node** path_top, int num) {
	if (*path_top) {
		(*path_top)->link = new node;
		(*path_top) = (*path_top)->link;
		(*path_top)->num = num, (*path_top)->link = NULL;
	}
	
	else {
		path = new node;
		path->num = num, path->link = NULL;
		(*path_top) = path;
	}
}