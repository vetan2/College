#include "waterObj.h"

void waterObj::init(lineObj* line, int lineCnt, ofVec2f dotPos) {
	this->lineCnt = lineCnt;
	modifiedLine = new lineObj[lineCnt];

	dotObj tempDot;
	lineObj tempLine;
	for (int lineIndex = 0; lineIndex < lineCnt; lineIndex++) {
		modifiedLine[lineIndex] = line[lineIndex];

		ofVec2f u(modifiedLine[lineIndex].pos2 - modifiedLine[lineIndex].pos1);
		u /= u.length();

		modifiedLine[lineIndex].pos1 -= u * (tempLine.width / 2);
		modifiedLine[lineIndex].pos2 += u * (tempLine.width / 2);
		modifiedLine[lineIndex].pos1.y -= (tempLine.width / 2) * (1 / abs(cos(ofVec2f(1, 0).angleRad(u))));
		modifiedLine[lineIndex].pos2.y -= (tempLine.width / 2) * (1 / abs(cos(ofVec2f(1, 0).angleRad(u))));
	}

	path[0] = dotPos;
	pathCnt = 1;
	velocity = ofVec2f(0, 0);
	g /= ofGetFrameRate() * 100;
}

void waterObj::calcPath() {
	while (1) {
		switch (getState(path[pathCnt - 1])) {
		case vertical:
			moveVertically(); break;

		case verticalTillEnd:
			moveVerticallyTillEnd(); break;

		case diagnal:
			moveDiagnally(); break;
		}

		if (calcPathDone == true)
			return;
	}
}

state waterObj::getState(ofVec2f pos) {
	ofVec2f linePos_upper, linePos_lower;
	ofVec2f tempPos;

	tempPos = destPos = pos;
	for (int lineIndex = 0; lineIndex < lineCnt; lineIndex++) {
		if (modifiedLine[lineIndex].pos1.y < modifiedLine[lineIndex].pos2.y) {
			linePos_upper = modifiedLine[lineIndex].pos1;
			linePos_lower = modifiedLine[lineIndex].pos2;
		}

		else if (modifiedLine[lineIndex].pos1.y > modifiedLine[lineIndex].pos2.y) {
			linePos_upper = modifiedLine[lineIndex].pos2;
			linePos_lower = modifiedLine[lineIndex].pos1;
		}

		else {
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dis(0, 1);

			if (dis(gen) == 0) {
				linePos_upper = modifiedLine[lineIndex].pos1;
				linePos_lower = modifiedLine[lineIndex].pos2;
			}

			else {
				linePos_upper = modifiedLine[lineIndex].pos2;
				linePos_lower = modifiedLine[lineIndex].pos1;
			}
		}

		if (pos.y - linePos_lower.y >= upsilon)
			continue;

		if (linePos_upper.x > linePos_lower.x) {
			if (linePos_lower.x - pos.x >= upsilon || pos.x - linePos_upper.x >= upsilon)
				continue;
		}
		else
			if (linePos_upper.x - pos.x >= upsilon || pos.x - linePos_lower.x >= upsilon)
				continue;

		ofVec2f u(linePos_lower.x - linePos_upper.x, linePos_lower.y - linePos_upper.y);
		float t = (pos.x - linePos_upper.x) / u.x;
		tempPos = ofVec2f(linePos_upper.x + u.x * t, linePos_upper.y + u.y * t);

		
		if (tempPos.distance(pos) < upsilon)
			if (tempPos.distance(linePos_lower) >= upsilon) {
				destPos = linePos_lower;
				return diagnal;
			}

		if (tempPos.y > pos.y) {
			if (destPos == pos)
				destPos = tempPos;
			else
				if (destPos.y > tempPos.y)
					destPos = tempPos;
		}
	}
	// If passed loop above, pos is not on any line

	if (destPos != pos)
		return vertical;

	// If destPos is in the initial state, there is nothing under pos
	return verticalTillEnd;
}

void waterObj::moveVertically() {
	ofVec2f nextPos(0, path[pathCnt - 1].y);
	velocity.y = velocity.length();
	velocity.x = 0;

	while (1) {
		velocity.y += g;
		nextPos.y += velocity.y;
		if (nextPos.distance(destPos) > upsilon) {
			if (nextPos.y > destPos.y) {
				path[pathCnt++] = destPos;
				return;
			}
			else {
				path[pathCnt] = path[pathCnt - 1];
				path[pathCnt++].y = nextPos.y;
			}
		}

		else {
			path[pathCnt++] = destPos;
			return;
		}
	}
}

void waterObj::moveVerticallyTillEnd() {
	ofVec2f nextPos(0, path[pathCnt - 1].y);
	velocity.y = velocity.length();
	velocity.x = 0;

	while (1) {
		velocity.y += g;
		nextPos.y += velocity.y;
		if (nextPos.y > ofGetHeight()) {
			path[pathCnt] = path[pathCnt - 1];
			path[pathCnt++].y = ofGetHeight();
			break;
		}

		path[pathCnt] = path[pathCnt - 1];
		path[pathCnt++].y = nextPos.y;
	}

	calcPathDone = true;
}

void waterObj::moveDiagnally() {
	ofVec2f nextPos = path[pathCnt - 1];
	ofVec2f u = (destPos - path[pathCnt - 1]);
	u /= u.length();

	velocity = u * velocity.length();

	while (1) {
		velocity *= ((velocity.length() + g * sin(ofVec2f(1, 0).angleRad(velocity))) / velocity.length());
		nextPos += velocity;

		if (nextPos.distance(destPos) >= upsilon) {
			if (nextPos.y > destPos.y) {
				path[pathCnt++] = destPos;
				return;
			}

			else
				path[pathCnt++] = nextPos;
		}

		else {
			path[pathCnt++] = destPos;
			return;
		}
	}
}