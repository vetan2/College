#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(255);
	ofSetFrameRate(60);

	setMenu();

	// Set the error message font
	errorFont.load("verdana.ttf", 20, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){
	updateIconClick();
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawGrid(16);
	// draw lines
	// draw dots
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
	icon[DELETE].load("image/icon_delete.png");
	icon[ROTATE_DOT].load("image/icon_rotate_dot.png");
	icon[ROTATE_LINE].load("image/icon_rotate_line.png");
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
	if (iconClicked)
		ofDrawRectangle(iconBox[iconIndex_clicked]);

	// Draw the icons
	ofSetColor(210);		// Set the brightness of the icons to (210/255) * 100%
	for (int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		icon[iconIndex].draw(16 + (32 + 16) * iconIndex, 16);
}

void ofApp::updateIconClick() {
	// If an icon is left-clicked,
	// then set iconIndex_clicked to the index of the clicked icon and iconClicked to true
	if (!iconClicked)
		for (iconIndex_clicked = UNDO; iconIndex_clicked <= DISCHARGE; iconIndex_clicked++)
			if (iconBox[iconIndex_clicked].inside(ofGetMouseX(), ofGetMouseY()) && ofGetMousePressed(0)) {
				iconClicked = true;
				break;
			}

	// If the click is off in the location of the clicked icon box,
	// then try changing the mode, and set iconClicked to false
	if (iconClicked && !ofGetMousePressed(0)) {
		int x = ofGetMouseX(), y = ofGetMouseY();
		if(iconBox[iconIndex_clicked].inside(x, y))
			changeMode(x, y);
		iconClicked = false;
	}
}

void ofApp::changeMode(int x, int y) {
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		if (iconBox[modeIndex].inside(x, y)) {
			if (!waterFlowing && !waterDraining)
				if (mode[modeIndex])
					mode[modeIndex] = false;
				else {
					inactivateModes();
					if (modeIndex != ROTATE_DOT && modeIndex != ROTATE_LINE)
						mode[ROTATE_DOT] = mode[ROTATE_LINE] = false;
					mode[modeIndex] = true;
				}
			else if (waterFlowing)
				setErrorMessage("Please turn off the faucet first.");
			else if (waterDraining)
				setErrorMessage("Please wait for the water to drain.");
			return;
		}

	if (iconBox[DISCHARGE].inside(x, y)) {
		if (!mode[DISCHARGE]) {
			inactivateModes();
			mode[ROTATE_DOT] = mode[ROTATE_LINE] = false;
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

// Inactivate all modes except the rotate modes
void ofApp::inactivateModes() {
	for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
		if (modeIndex != ROTATE_DOT && modeIndex != ROTATE_LINE)
			mode[modeIndex] = false;
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

		// 30 frames after this function is called, the error message slowly disappears.
		if (ofGetFrameNum() - errorTimer >= 30)
			errorFontBright += 4;
	}
}