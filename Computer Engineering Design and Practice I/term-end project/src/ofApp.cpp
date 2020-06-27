#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(255);
	ofSetFrameRate(60);

	setMenu();
	setStructSet();

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
			mousePressedPos = ofVec2f(x, y);
			isClicked = true;
		}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if(isClicked)
		if (button == 0) {
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

void ofApp::drawDots() {
	if (mode[DOT]) {
		if (!isClicked)
			ofSetColor(210);
		else
			ofSetColor(180);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempDot.radius);
	}

	if (structQueue_cur)
		for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
			structQueue_cur->dot[dotIndex].draw();
}

void ofApp::drawLines() {
	if (mode[LINE]) {
		// TODO
		if (!step[0]) {
			ofSetColor(180);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}

		if (step[0] && !step[1]) {
			ofSetColor(150);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}

		if (step[1] && !step[2]) {
			ofSetColor(150);
			ofDrawCircle(tempLine.pos1, tempLine.width * 2 / 3);
			ofSetLineWidth(tempLine.width);
			ofDrawLine(tempLine.pos1.x, tempLine.pos1.y, ofGetMouseX(), ofGetMouseY());

			ofSetColor(180);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}

		if (step[2]) {
			ofSetColor(150);
			ofDrawCircle(tempLine.pos1, tempLine.width * 2 / 3);
			ofSetLineWidth(tempLine.width);
			ofDrawLine(tempLine.pos1.x, tempLine.pos1.y, ofGetMouseX(), ofGetMouseY());

			ofSetColor(150);
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), tempLine.width);
		}
	}

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
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		if (iconBox[modeIndex].inside(mousePos)) {
			if (!waterFlowing && !waterDraining) {
				step[0] = step[1] = step[2] = false;

				if (mode[modeIndex])
					mode[modeIndex] = false;

				else {
					inactivateModes();
					mode[modeIndex] = true;
				}
			}
			else if (waterFlowing)
				setErrorMessage("Please turn off the faucet first.");
			else if (waterDraining)
				setErrorMessage("Please wait for the water to drain.");
			return;
		}

	if (iconBox[DISCHARGE].inside(mousePos)) {
		if (!mode[DISCHARGE]) {
			inactivateModes();
			// TO DO
			waterFlowing = true;
			mode[DISCHARGE] = true;
		}

		else {
			//TO DO
			waterFlowing = false;
			mode[DISCHARGE] = false;
		}
	}
}

// Inactivate all modes
void ofApp::inactivateModes() {
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		mode[modeIndex] = false;
}

void ofApp::runMode() {
	if (mode[UNDO]) {
		undo();
		mode[UNDO] = false;
	}

	if (mode[REDO]) {
		redo();
		mode[REDO] = false;
	}

	if (mode[DOT]) {
		if (!step[0])
			if (isClicked)
				step[0] = true;

		if(step[0])
			if (!isClicked) {
				if (checkPos(mouseReleasedPos))
					addDot(mouseReleasedPos);
				step[0] = false;
			}
	}

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
			for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++)
				if (structQueue_cur->dot[dotIndex].distance(ofVec2f(ofGetMouseX(), ofGetMouseY()))
					< (float)tempDot.radius * 2)
					deleteStruct(DOT, dotIndex);

			for (int lineIndex = 0; lineIndex < structQueue_cur->lineCnt; lineIndex++)
				if (structQueue_cur->line[lineIndex].distance(ofVec2f(ofGetMouseX(), ofGetMouseY()))
					< (float)tempLine.width)
					deleteStruct(LINE, lineIndex);
		}

	if (mode[FAUCET]) {
		if (!step[0])
			if (isClicked)
				step[0] = true;

		if (step[0]) {
			if (!isClicked) {
				for (int dotIndex = 0; dotIndex < structQueue_cur->dotCnt; dotIndex++) {
					if (structQueue_cur->dot[dotIndex].distance(mousePressedPos) < (float)tempDot.radius * 2) {
						if (structQueue_cur->dot[dotIndex].distance(mouseReleasedPos)
							< (float)tempDot.radius * 2) {
							if(!structQueue_cur->dot[dotIndex].faucet)
								structQueue_cur->dot[dotIndex].faucet = true;
							else
								structQueue_cur->dot[dotIndex].faucet = false;
							step[0] = false;
						}
					}
				}
			}
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
	structSet* newSet = new structSet;
	structSet* toDel;

	structSet* temp = structQueue_cur->next;
	while (temp) {
		toDel = temp;
		temp = temp->next;
		delete toDel;
		structSetCnt--;
	}
	structQueue_front = structQueue_cur;
	structQueue_front->next = NULL;

	if (structSetCnt > MAX_STRUCT_SET) {
		toDel = structQueue_rear;
		structQueue_rear = structQueue_rear->next;
		delete toDel;

		structQueue_rear->prev = NULL;
		structSetCnt--;
	}

	newSet->dotCnt = structQueue_cur->dotCnt + 1;
	newSet->lineCnt = structQueue_cur->lineCnt;

	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];
	
	copyStructSet(DOT, newSet, structQueue_cur);
	copyStructSet(LINE, newSet, structQueue_cur);
	newSet->dot[newSet->dotCnt - 1].pos = pos;

	structQueue_front->next = newSet;
	newSet->prev = structQueue_front;
	newSet->next = NULL;
	structQueue_front = newSet;

	structSetCnt++;
	structQueue_cur = newSet;
}

void ofApp::addLine(ofVec2f pos1, ofVec2f pos2) {
	structSet* newSet = new structSet;
	structSet* toDel;

	structSet* temp = structQueue_cur->next;
	while (temp) {
		toDel = temp;
		temp = temp->next;
		delete toDel;
		structSetCnt--;
	}
	structQueue_front = structQueue_cur;
	structQueue_front->next = NULL;

	if (structSetCnt > MAX_STRUCT_SET) {
		toDel = structQueue_rear;
		structQueue_rear = structQueue_rear->next;
		delete toDel;

		structQueue_rear->prev = NULL;
		structSetCnt--;
	}

	newSet->dotCnt = structQueue_cur->dotCnt;
	newSet->lineCnt = structQueue_cur->lineCnt + 1;

	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];

	copyStructSet(DOT, newSet, structQueue_cur);
	copyStructSet(LINE, newSet, structQueue_cur);
	newSet->line[newSet->lineCnt - 1].pos1 = pos1;
	newSet->line[newSet->lineCnt - 1].pos2 = pos2;

	structQueue_front->next = newSet;
	newSet->prev = structQueue_front;
	newSet->next = NULL;
	structQueue_front = newSet;

	structSetCnt++;
	structQueue_cur = newSet;
}

void ofApp::deleteStruct(int sort, int indexToDel) {
	structSet* newSet = new structSet;
	structSet* toDel;

	structSet* temp = structQueue_cur->next;
	while (temp) {
		toDel = temp;
		temp = temp->next;
		delete toDel;
		structSetCnt--;
	}
	structQueue_front = structQueue_cur;
	structQueue_front->next = NULL;

	if (structSetCnt > MAX_STRUCT_SET) {
		toDel = structQueue_rear;
		structQueue_rear = structQueue_rear->next;
		delete toDel;

		structQueue_rear->prev = NULL;
		structSetCnt--;
	}

	newSet->dotCnt = structQueue_cur->dotCnt;
	newSet->lineCnt = structQueue_cur->lineCnt;
	if (sort == DOT)
		newSet->dotCnt--;
	else if(sort == LINE)
		newSet->lineCnt--;

	newSet->dot = new dotObj[newSet->dotCnt];
	newSet->line = new lineObj[newSet->lineCnt];

	if (sort == DOT) {
		copyStructSet(LINE, newSet, structQueue_cur);
		for (int dotIndex = 0; dotIndex < indexToDel; dotIndex++)
			newSet->dot[dotIndex] = structQueue_cur->dot[dotIndex];
		for (int dotIndex = indexToDel; dotIndex < newSet->dotCnt; dotIndex++)
			newSet->dot[dotIndex] = structQueue_cur->dot[dotIndex + 1];
	}
	else if (sort == LINE) {
		copyStructSet(DOT, newSet, structQueue_cur);
		for (int lineIndex = 0; lineIndex < indexToDel; lineIndex++)
			newSet->line[lineIndex] = structQueue_cur->line[lineIndex];
		for (int lineIndex = indexToDel; lineIndex < newSet->lineCnt; lineIndex++)
			newSet->line[lineIndex] = structQueue_cur->line[lineIndex + 1];
	}

	structQueue_front->next = newSet;
	newSet->prev = structQueue_front;
	newSet->next = NULL;
	structQueue_front = newSet;

	structSetCnt++;
	structQueue_cur = newSet;
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
	if(sort == DOT)
		for (int dotIndex = 0; dotIndex < MIN(dest->dotCnt, src->dotCnt); dotIndex++)
			dest->dot[dotIndex] = src->dot[dotIndex];

	else if(sort == LINE)
		for (int lineIndex = 0; lineIndex < MIN(dest->lineCnt, src->lineCnt); lineIndex++)
			dest->line[lineIndex] = src->line[lineIndex];
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