#include "structObj.h"

void dotObj::draw() {
	if (faucet)
		ofSetColor(235, 125, 22);
	else
		ofSetColor(128);

	ofDrawCircle(pos, radius);
}

float dotObj::distance(ofVec2f anotherPos) {
	return pos.distance(anotherPos);
}

float dotObj::distance(ofVec2f pos1, ofVec2f pos2) {
	ofVec2f u(pos2.x - pos1.x, pos2.y - pos1.y);
	lineObj sampleLine;
	float t = (u.x * (pos.x - pos1.x) + u.y * (pos.y - pos1.y)) / (pow(u.x, 2) + pow(u.y, 2));
	if (t < 0)
		return pos.distance(pos1);

	else if (t > 1)
		return pos.distance(pos2);

	else {
		ofVec2f H(pos1.x + u.x * t, pos1.y + u.y * t);
		return pos.distance(H);
	}
}

void lineObj::draw() {
	ofSetColor(80);
	ofSetLineWidth(width);

	ofDrawLine(pos1, pos2);
	ofDrawCircle(pos1, width / 2);
	ofDrawCircle(pos2, width / 2);
}

float lineObj::distance(ofVec2f pos) {
	ofVec2f u(pos2.x - pos1.x, pos2.y - pos1.y);
	lineObj sampleLine;
	float t = (u.x * (pos.x - pos1.x) + u.y * (pos.y - pos1.y)) / (pow(u.x, 2) + pow(u.y, 2));
	if (t < 0)
		return pos.distance(pos1);

	else if (t > 1)
		return pos.distance(pos2);

	else {
		ofVec2f H(pos1.x + u.x * t, pos1.y + u.y * t);
		return pos.distance(H);
	}
}

bool lineObj::intersectWith(ofVec2f pos3, ofVec2f pos4) {
	float under = (pos4.y - pos3.y) * (pos2.x - pos1.x) - (pos4.x - pos3.x) * (pos2.y - pos1.y);
	if (under == 0)
		return 0;

	float t = (pos4.x - pos3.x) * (pos1.y - pos3.y) - (pos4.y - pos3.y) * (pos1.x - pos3.x);
	float s = (pos2.x - pos1.x) * (pos1.y - pos3.y) - (pos2.y - pos1.y) * (pos1.x - pos3.x);
	t /= under, s /= under;

	if (t >= 0.0 && t <= 1.0 && s >= 0.0 && s <= 1.0)
		return 1;

	return 0;
}