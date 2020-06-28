#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("WaterFall");
	ofSetBackgroundColor(255);
	ofSetFrameRate(60);

	setMenu();
	setStructSet();
	waterFlowSpeed = 10;
	waterDischargeCycle = 10;

	// Set the error message font
	errorFont.load("verdana.ttf", 20, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){
	handleIconClick();
	runMode();
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawGrid(16);
	// If there is any water object to draw, then draw them
	if (waterFlowing || waterDraining)
		drawWater(tempDot.radius * 2 / 3);
	drawDots();
	drawLines();
	drawMenu();
	printErrorMessage();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'q') {
		if (!waterFlowing && !waterDraining) {
			structSet* toDel;

			while (structQueue_rear) {
				toDel = structQueue_rear;
				structQueue_rear = structQueue_rear->next;

				delete toDel->dot;
				delete toDel->line;
				delete toDel;
			}

			ofExit(0);
		}

		else if (waterFlowing)
			setErrorMessage("Please turn off the faucet first.");

		else if (waterDraining)
			setErrorMessage("Please wait for the water to drain.");
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (!isClicked)
		if (button == 0) {
			// Store position where the button was first pressed
			mousePressedPos = ofVec2f(x, y);
			isClicked = true;
		}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if(isClicked)
		if (button == 0) {
			// Store position where the button was released
			mouseReleasedPos = ofVec2f(x, y);
			isClicked = false;
		}
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

// Set the variables associated with the menu
void ofApp::setMenu() {
	// Set the menu box
	menuBox.x = 0;
	menuBox.y = 0;
	menuBox.width = 16 + (32 + 16) * MODE_CNT;
	menuBox.height = 64;

	// Load the icon images
	icon[UNDO].load("image/icon_undo.png");
	icon[REDO].load("image/icon_redo.png");
	icon[DOT].load("image/icon_dot.png");
	icon[LINE].load("image/icon_line.png");
	icon[ERASE].load("image/icon_erase.png");
	icon[FAUCET].load("image/icon_faucet.png");
	icon[DISCHARGE].load("image/icon_discharge.png");

	// Set the icon boxes
	for (int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++) {
		iconBox[iconIndex].x = 16 + (32 + 16) * iconIndex - 8;
		iconBox[iconIndex].y = 16 - 8;
		iconBox[iconIndex].width = 48;
		iconBox[iconIndex].height = 48;
	}
}

void ofApp::drawGrid(int interval) {
	int i;
	
	// Set the color and width of gird lines
	ofSetColor(240);
	ofSetLineWidth(1);
	
	// Draw vertical lines
	for (i = interval; i < ofGetWidth(); i += interval)
		ofDrawLine(i, 0, i, ofGetHeight());

	// Draw horizontal lines
	for (i = interval; i < ofGetHeight(); i += interval)
		ofDrawLine(0, i, ofGetWidth(), i);
}

void ofApp::drawWater(float radius) {
	for (int waterIndex = 0; waterIndex < structQueue_cur->faucetCnt; waterIndex++) {
		waterIndexNode* cur = waterIndexQueue_rear[waterIndex];

		ofSetColor(18, 115, 235);
		while (cur) {
			ofDrawCircle(water[waterIndex].path[(int)floor(cur->index)], radius);
			cur = cur->link;
		}
	}
}

void ofApp::drawDots() {
	// If DOT mode is activated, then draw a dot at the position of the cursor
	if (mode[DOT]) {
		if (!isClicked)
			ofSetColor(210);
		else
			ofSetColor(180);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempDot.radius);
	}

	// Draw stored dot objects
	if (structQueue_cur)
		for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
			structQueue_cur->dot[dotIndex].draw();
}

void ofApp::drawLines() {
	// If LINE mode is activated,
	//	then draw dots and lines according to each step and the position of the cursor
	// The description of each step is given in the RunMode() function
	if (mode[LINE]) {
		if (!step[0]) {
			ofSetColor(180);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}

		if (step[0] && !step[1]) {
			ofSetColor(150);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}

		if (step[1]) {
			// Draw a line that connects the dot created first and the cursor
			ofSetColor(150);
			ofDrawCircle(tempLine.pos1, tempLine.width * 2 / 3);
			ofSetLineWidth(tempLine.width);
			ofDrawLine(tempLine.pos1.x, tempLine.pos1.y, ofGetMouseX(), ofGetMouseY());

			// If you've entered step[2], then deepen the point of the cursor
			if (step[2])
				ofSetColor(150);
			else
				ofSetColor(180);

			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}
	}

	// Draw stored line objects
	if (structQueue_cur)
		for (int lineIndex = 0; lineIndex < structQueue_cur->lineCnt; lineIndex++)
			structQueue_cur->line[lineIndex].draw();
}

void ofApp::drawMenu() {

	// Set the color of the menu area
	ofSetColor(240);

	// Draw the menu area
	ofDrawRectangle(menuBox);

	// Deepen the background color of the icon the cursor points to
	for (int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		if (iconBox[iconIndex].inside(ofGetMouseX(), ofGetMouseY())) {
			ofSetColor(210);
			ofDrawRectangle(iconBox[iconIndex]);
			break;
		}

	// More deepen the background color of the icon corresponding to the active mode or left-clicked
	// when activated
	ofSetColor(180);
	for(int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		if (mode[iconIndex]) {
			ofDrawRectangle(iconBox[iconIndex]);
		}

	// when left-clicked
	if (isIconClicked)
		ofDrawRectangle(iconBox[iconIndex_clicked]);

	// Draw the icons
	ofSetColor(210);		// Set the brightness of the icons to (210/255) * 100%
	for (int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		icon[iconIndex].draw(16 + (32 + 16) * iconIndex, 16);
}

void ofApp::handleIconClick() {
	// If an icon is left-clicked,
	// then set iconIndex_clicked to the index of the clicked icon and iconClicked to true
	if (isClicked)
		for (iconIndex_clicked = UNDO; iconIndex_clicked <= DISCHARGE; iconIndex_clicked++)
			if (iconBox[iconIndex_clicked].inside(mousePressedPos)) {
				isIconClicked = true;
				break;
			}

	// If the click is off in the location of the clicked icon box,
	// then try changing the mode, and set iconClicked to false
	if (isIconClicked && !isClicked) {
		if(iconBox[iconIndex_clicked].inside(mouseReleasedPos))
			changeMode(mouseReleasedPos);
		isIconClicked = false;
	}
}

void ofApp::changeMode(ofVec2f mousePos) {
	// Locate the icon box(UNDO~FAUCET) where the mousePos is located
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		if (iconBox[modeIndex].inside(mousePos)) {
			// If the corresponding icon box found,
			//	then turn on/off the mode only if water does not flow
			if (!waterFlowing && !waterDraining) {
				step[0] = step[1] = step[2] = false;

				if (mode[modeIndex])
					mode[modeIndex] = false;

				else {
					inactivateModes();
					mode[modeIndex] = true;
				}
			}

			// If the corresponding icon box found but water is flowing,
			//	then print proper error message
			else if (waterFlowing)
				setErrorMessage("Please turn off the faucet first.");
			else if (waterDraining)
				setErrorMessage("Please wait for the water to drain.");
			return;
		}

	// If mousePos is located at DISCHARGE icon box,
	//	then handle water objects and function that handle them
	if (iconBox[DISCHARGE].inside(mousePos)) {
		if (!mode[DISCHARGE]) {
			// If water is still draining,
			//	then print proper error message
			if (waterDraining)
				setErrorMessage("Please wait for the water to drain.");

			else {
				inactivateModes();
				mode[DISCHARGE] = true;
			}
		}

		else {
			waterFlowing = false;
			waterDraining = true;
			mode[DISCHARGE] = false;
		}
	}
}

void ofApp::inactivateModes() {
	// Inactivate all modes
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		mode[modeIndex] = false;
}

void ofApp::runMode() {
	// Perform undoing function and turn off UNDO mode
	if (mode[UNDO]) {
		undo();
		mode[UNDO] = false;
	}

	// Perform redoing function and turn off REDO mode
	if (mode[REDO]) {
		redo();
		mode[REDO] = false;
	}

	// step[0] : The left button of the mouse is pressed
	if (mode[DOT]) {
		if (!step[0])
			if (isClicked)
				step[0] = true;

		if(step[0])
			// In step[0], if the left button of the mouse is released,
			//	then try to add a new dot
			if (!isClicked) {
				if (checkPos(mouseReleasedPos))
					addDot(mouseReleasedPos);
				// Go back to the condition not in step[0]
				step[0] = false;
			}
	}

	// step[0] : The left button of the mouse is first pressed
	// step[1] : The left button of the mouse is first released
	// step[2] : The left button of the mouse is second pressed
	if (mode[LINE]) {
		if (!step[0])
			if (isClicked)
				step[0] = true;

		if (step[0] && !step[1])
			if (!isClicked) {
				if (checkPos(mouseReleasedPos)) {
					tempLine.pos1 = mouseReleasedPos;
					step[1] = true;
				}
				else
					step[0] = false;
			}

		if (step[1] && !step[2])
			if (isClicked)
				step[2] = true;

		if (step[2])
			if (!isClicked) {
				if (checkPos(mouseReleasedPos) && checkPos(tempLine.pos1, mouseReleasedPos)) {
					if (mouseReleasedPos.distance(tempLine.pos1) < tempLine.width * 2)
						setErrorMessage("The line is too short.");

					else {
						tempLine.pos2 = mouseReleasedPos;
						addLine(tempLine.pos1, tempLine.pos2);
						step[0] = step[1] = step[2] = false;
					}
				}
				else
					step[2] = false;
			}
	}

	if (mode[ERASE])
		if (isClicked) {
			// Locate the dot at the position of the cursor
			for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
				if (structQueue_cur->dot[dotIndex].distance(ofVec2f(ofGetMouseX(), ofGetMouseY()))
					< (float)tempDot.radius * 2)
						deleteStruct(DOT, dotIndex);
					
			// Locate the line at the position of the cursor
			for (int lineIndex = 0; lineIndex < structQueue_cur->lineCnt; lineIndex++)
				if (structQueue_cur->line[lineIndex].distance(ofVec2f(ofGetMouseX(), ofGetMouseY()))
					< (float)tempLine.width)
					deleteStruct(LINE, lineIndex);
		}

	// step[0] : The left button of the mouse is pressed
	if (mode[FAUCET]) {
		if (!step[0])
			if (isClicked)
				step[0] = true;

		if (step[0])
			if (!isClicked)
				// Locate the dot at the position of the cursor pressed
				for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
					if (structQueue_cur->dot[dotIndex].distance(mousePressedPos) < (float)tempDot.radius * 2)
						// Check whether the position of the cursor released is inside the same dot
						if (structQueue_cur->dot[dotIndex].distance(mouseReleasedPos)
							< (float)tempDot.radius * 2) {
							editFaucet(dotIndex);
								
							step[0] = false;
						}
	}

	if (mode[DISCHARGE])
		if (!waterFlowing && !waterDraining) {
			// Check whether there is any faucet
			for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++) {
				if (structQueue_cur->dot[dotIndex].faucet)
					break;

				// If not, then print proper error message
				if (dotIndex == structQueue_cur->dotCnt - 1) {
					setErrorMessage("There is no faucet.");
					mode[DISCHARGE] = false;
					return;
				}
			}

			// Allocate memories for water objects and waterIndexQueue
			water = new waterObj[structQueue_cur->faucetCnt];
			waterIndexQueue_front
				= (waterIndexNode * *)calloc(structQueue_cur->faucetCnt, sizeof(waterIndexNode*));
			waterIndexQueue_rear
				= (waterIndexNode * *)calloc(structQueue_cur->faucetCnt, sizeof(waterIndexNode*));

			// Calculate path of the water
			int waterIndex = -1;
			for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++) {
				if (structQueue_cur->dot[dotIndex].faucet) {
					water[++waterIndex].init(
						structQueue_cur->line,
						structQueue_cur->lineCnt,
						structQueue_cur->dot[dotIndex].pos
					);

					water[waterIndex].calcPath();
				}
			}

			// After the calculation, discharge water
			waterFlowing = true;
		}

	// If water is flowing, then update indexes used in drawWater() function
	if (waterFlowing || waterDraining)
		for (int waterIndex = 0; waterIndex < structQueue_cur->faucetCnt; waterIndex++)
			updateWaterIndex(
				waterIndexQueue_front + waterIndex,
				waterIndexQueue_rear + waterIndex,
				water[waterIndex].pathCnt
			);

	if (waterDraining)
		for (int waterIndex = 0; waterIndex < structQueue_cur->faucetCnt; waterIndex++) {
			if (waterIndexQueue_front[waterIndex])
				break;

			// If water has drained, then release allocated memory for water objects and waterIndexQueue
			if (waterIndex == structQueue_cur->faucetCnt - 1) {
				waterObj* toDel = water;
				if (toDel)
					delete water;
				water = NULL;

				// Finish draining
				waterDraining = false;
			}
		}
}

void ofApp::undo() {
	if (structSetCnt == 0 || structQueue_rear == structQueue_cur)
		setErrorMessage("No more reversals.");

	else
		structQueue_cur = structQueue_cur->prev;
}

void ofApp::redo() {
	if (structSetCnt == 0 || structQueue_front == structQueue_cur)
		setErrorMessage("It's up to date.");

	else
		structQueue_cur = structQueue_cur->next;
}

void ofApp::addDot(ofVec2f pos) {
	structSet* newSet = (structSet*)calloc(1, sizeof(structSet));

	addSetInQueue(newSet);

	// Set count variables of newSet
	newSet->dotCnt = structQueue_cur->dotCnt + 1;
	newSet->lineCnt = structQueue_cur->lineCnt;

	// Allocate memories for structure objects of newSet
	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];
	
	// Fill allocated memories ofnewSet
	copyStructSet(DOT, newSet, structQueue_cur);
	copyStructSet(LINE, newSet, structQueue_cur);
	newSet->dot[newSet->dotCnt - 1].pos = pos;

	structQueue_cur = newSet;
}

void ofApp::addLine(ofVec2f pos1, ofVec2f pos2) {
	structSet* newSet = (structSet*)calloc(1, sizeof(structSet));
	addSetInQueue(newSet);

	// Set count variables of newSet
	newSet->dotCnt = structQueue_cur->dotCnt;
	newSet->lineCnt = structQueue_cur->lineCnt + 1;

	// Allocate memories for structure objects of newSet
	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];

	// Fill allocated memories ofnewSet
	copyStructSet(DOT, newSet, structQueue_cur);
	copyStructSet(LINE, newSet, structQueue_cur);
	newSet->line[newSet->lineCnt - 1].pos1 = pos1;
	newSet->line[newSet->lineCnt - 1].pos2 = pos2;

	structQueue_cur = newSet;
}

void ofApp::deleteStruct(int sort, int indexToDel) {
	structSet* newSet = (structSet*)calloc(1, sizeof(structSet));
	
	addSetInQueue(newSet);

	// Set count variables of newSet
	newSet->dotCnt = structQueue_cur->dotCnt;
	newSet->lineCnt = structQueue_cur->lineCnt;
	if (sort == DOT)
		newSet->dotCnt--;
	else if(sort == LINE)
		newSet->lineCnt--;

	// Allocate memories for structure objects of newSet
	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];

	// Fill allocated memories ofnewSet
	if (sort == DOT) {
		copyStructSet(LINE, newSet, structQueue_cur);
		for (int dotIndex = 0; dotIndex < indexToDel; dotIndex++) {
			newSet->dot[dotIndex] = structQueue_cur->dot[dotIndex];
			if (newSet->dot[dotIndex].faucet)
				newSet->faucetCnt++;
		}
		
		for (int dotIndex = indexToDel; dotIndex < newSet->dotCnt; dotIndex++) {
			newSet->dot[dotIndex] = structQueue_cur->dot[dotIndex + 1];
			if (newSet->dot[dotIndex].faucet)
				newSet->faucetCnt++;
		}
	}
	else if (sort == LINE) {
		copyStructSet(DOT, newSet, structQueue_cur);
		for (int lineIndex = 0; lineIndex < indexToDel; lineIndex++)
			newSet->line[lineIndex] = structQueue_cur->line[lineIndex];
		for (int lineIndex = indexToDel; lineIndex < newSet->lineCnt; lineIndex++)
			newSet->line[lineIndex] = structQueue_cur->line[lineIndex + 1];
	}

	structQueue_cur = newSet;
}

void ofApp::editFaucet(int dotIndex) {
	structSet* newSet = (structSet*)calloc(1, sizeof(structSet));

	addSetInQueue(newSet);

	// Set count variables of newSet
	newSet->dotCnt = structQueue_cur->dotCnt;
	newSet->lineCnt = structQueue_cur->lineCnt;

	// Allocate memories for structure objects of newSet
	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];

	// Fill allocated memories ofnewSet
	copyStructSet(DOT, newSet, structQueue_cur);
	copyStructSet(LINE, newSet, structQueue_cur);
	if (newSet->dot[dotIndex].faucet) {
		newSet->dot[dotIndex].faucet = false;
		newSet->faucetCnt--;
	}
	else {
		newSet->dot[dotIndex].faucet = true;
		newSet->faucetCnt++;
	}

	structQueue_cur = newSet;
}

void ofApp::updateWaterIndex(waterIndexNode** front, waterIndexNode** rear, int maxPathIndex) {
	waterIndexNode* curNode = *rear;

	// Increase all indexes of the Queue
	while (curNode) {
		curNode->index += waterFlowSpeed;
		curNode = curNode->link;
	}

	// If the index of the rear is at the end, then pop in Queue
	if (*rear && (*rear)->index > (float)maxPathIndex) {
		waterIndexNode* toDel = *rear;

		if (*rear == *front)
			*rear = *front = NULL;
		else
			*rear = (*rear)->link;

		delete toDel;
	}

	// If water is discharging, then add a new node with an index of 0 at waterDischargeCycle
	if (waterFlowing)
		if (ofGetFrameNum() % waterDischargeCycle == 0) {
			waterIndexNode* newNode = (waterIndexNode*)calloc(1, sizeof(waterIndexNode));

			if (!(*front))
				* rear = *front = newNode;

			else {
				(*front)->link = newNode;
				*front = newNode;
			}
		}
}

bool ofApp::checkPos(ofVec2f pos) {
	if (structSetCnt == 0)
		return 1;

	for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
		if (structQueue_cur->dot[dotIndex].distance(pos) < tempDot.radius * 2) {
			setErrorMessage("Too close to an existing point.");
			return 0;
		}

	for (int lineIndex = 0; lineIndex < structQueue_cur->lineCnt; lineIndex++)
		if (structQueue_cur->line[lineIndex].distance(pos) < tempDot.radius + tempLine.width) {
			setErrorMessage("Too close to an existing line.");
			return 0;
		}

	return 1;
}

bool ofApp::checkPos(ofVec2f pos1, ofVec2f pos2) {
	if (structSetCnt == 0)
		return 1;

	if (pos1.x == pos2.x) {
		setErrorMessage("A line should not be vertical.");
		return 0;
	}

	if (pos1.y == pos2.y) {
		setErrorMessage("A line should not be horizontal.");
		return 0;
	}

	for (int lineIndex = 0; lineIndex < structQueue_cur->lineCnt; lineIndex++) {
		if (structQueue_cur->line[lineIndex].intersectWith(pos1, pos2)) {
			setErrorMessage("Lines should not cross each other.");
			return 0;
		}

		dotObj lineEnd;

		lineEnd.pos = structQueue_cur->line[lineIndex].pos1;
		if (lineEnd.distance(pos1, pos2) < tempLine.width + tempDot.radius) {
			setErrorMessage("Too close to an existing line.");
			return 0;
		}

		lineEnd.pos = lineEnd.pos = structQueue_cur->line[lineIndex].pos2;
		if (lineEnd.distance(pos1, pos2) < tempLine.width + tempDot.radius) {
			setErrorMessage("Too close to an existing line.");
			return 0;
		}
	}
		
	for(int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
		if (structQueue_cur->dot[dotIndex].distance(pos1, pos2) < tempDot.radius + tempLine.width) {
			setErrorMessage("Too close to an existing point.");
			return 0;
		}

	return 1;
}

void ofApp::setStructSet() {
	structQueue_front = (structSet*)calloc(1, sizeof(structSet));
	structQueue_rear = structQueue_cur = structQueue_front;
	structSetCnt = 1;
}

void ofApp::copyStructSet(int sort, structSet* dest, structSet* src) {
	if (sort == DOT) {
		dest->faucetCnt = 0;

		for (int dotIndex = 0; dotIndex < MIN(dest->dotCnt, src->dotCnt); dotIndex++) {
			dest->dot[dotIndex] = src->dot[dotIndex];
			if (dest->dot[dotIndex].faucet)
				dest->faucetCnt++;
		}
	}

	else if(sort == LINE)
		for (int lineIndex = 0; lineIndex < MIN(dest->lineCnt, src->lineCnt); lineIndex++)
			dest->line[lineIndex] = src->line[lineIndex];
}

void ofApp::addSetInQueue(structSet* newSet) {
	structSet* toDel;

	// Make structQueue_cur into front of the queue
	structSet* temp = structQueue_cur->next;
	while (temp) {
		toDel = temp;
		temp = temp->next;
		delete toDel;
		structSetCnt--;
	}
	structQueue_front = structQueue_cur;
	structQueue_front->next = NULL;

	// If structQueue is full, then delete the oldest
	if (structSetCnt > MAX_STRUCT_SET) {
		toDel = structQueue_rear;
		structQueue_rear = structQueue_rear->next;
		delete toDel;

		structQueue_rear->prev = NULL;
		structSetCnt--;
	}

	// Connect newSet with structQueue
	structQueue_front->next = newSet;
	newSet->prev = structQueue_front;
	newSet->next = NULL;
	structQueue_front = newSet;
	structSetCnt++;
}

// After calling this function, error message is printed automatically on printErrorMessage()
void ofApp::setErrorMessage(string message) {
	errorMessage = message;
	errorTimer = ofGetFrameNum();	// current time
	errorFontBright = 100;

	// These variables are used to center the error message
	errorBox = errorFont.getStringBoundingBox(message, 0, 0);
	errorOffsetX = -errorBox.x - errorBox.width * 0.5f;
}

void ofApp::printErrorMessage() {
	if (errorFontBright < 255) {
		ofSetColor(errorFontBright);
		errorFont.drawString(errorMessage, ofGetWidth() / 2 + errorOffsetX, 600);

		// 90 frames after this function is called, the error message slowly disappears.
		if (ofGetFrameNum() - errorTimer >= 90)
			errorFontBright += 4;
	}
}