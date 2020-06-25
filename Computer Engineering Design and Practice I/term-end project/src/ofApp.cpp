#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(255);
	ofSetFrameRate(60);

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
	ofDrawRectangle(0, 0, 16 + (32 + 16) * MODE_CNT, 64);

	// Deepen the background color of the icon the cursor points to
	if (16 - 4 <= ofGetMouseY() && ofGetMouseY() <= 64 - 16 + 4)
		for(int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
			if (getIconBoxX(iconIndex).x <= ofGetMouseX() && ofGetMouseX() <= getIconBoxX(iconIndex).y) {
				ofSetColor(210);
				ofDrawRectangle(16 + (32 + 16) * iconIndex - 8, 8, 48, 48);
				break;
			}

	// More deepen the background color of the icon corresponding to the active mode or left-clicked
	// when activated
	ofSetColor(180);
	for(int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		if (mode[iconIndex]) {
			ofDrawRectangle(16 + (32 + 16) * iconIndex - 8, 8, 48, 48);
		}

	// when left-clicked
	if (iconClicked)
		ofDrawRectangle(16 + (32 + 16) * iconIndex_clicked - 8, 8, 48, 48);

	// Draw the icons
	ofSetColor(210);		// Set the brightness of the icons to (210/255) * 100%
	for (int iconIndex = UNDO; iconIndex <= DISCHARGE; iconIndex++)
		icon[iconIndex].draw(16 + (32 + 16) * iconIndex, 16);
}

// Get the left-end X coordinate and the right-end Y coordinate of the icon box
// Member variable x, y of the returned instance is actually each of the above
ofVec2f ofApp::getIconBoxX(int iconIndex) {
	return ofVec2f(16 + (32 + 16) * iconIndex - 4, 16 + (32 + 16) * iconIndex + 32 + 4);
}

void ofApp::updateIconClick() {
	// If an icon is left-clicked,
	// then set iconIndex_clicked to the index of the clicked icon and iconClicked to true
	if (!iconClicked)
		if (16 - 4 <= ofGetMouseY() && ofGetMouseY() <= 64 - 16 + 4)
			for (iconIndex_clicked = UNDO; iconIndex_clicked <= DISCHARGE; iconIndex_clicked++)
				if (getIconBoxX(iconIndex_clicked).x <= ofGetMouseX() &&
					ofGetMouseX() <= getIconBoxX(iconIndex_clicked).y)
					if (ofGetMousePressed(0)) {
						iconClicked = true;
						break;
					}

	// If the click is off in the location of the clicked icon box,
	// then try changing the mode, and set iconClicked to false
	if (iconClicked && !ofGetMousePressed(0)) {
		int x = ofGetMouseX(), y = ofGetMouseY();
		changeMode(x, y);
		iconClicked = false;
	}
}

void ofApp::changeMode(int x, int y) {
	if (16 - 4 <= y && y <= 64 - 16 + 4) {
		for (int modeIndex = UNDO; modeIndex <= FAUCET; modeIndex++)
			if (getIconBoxX(modeIndex).x <= x && x <= getIconBoxX(modeIndex).y) {
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

		if (getIconBoxX(DISCHARGE).x <= x && x <= getIconBoxX(DISCHARGE).y) {
			if (!mode[DISCHARGE]) {
				inactivateModes();
				mode[ROTATE_DOT] = mode[ROTATE_LINE] = false;
				// TO DO
				mode[DISCHARGE] = true;
			}
			
			else {
				//TO DO
				mode[DISCHARGE] = false;
			}
		}
	}
}

// Inactivate all modes except the rotate modes
void ofApp::inactivateModes() {
	for (int modeIndex = UNDO; modeIndex <= DISCHARGE; modeIndex++)
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