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
		// By comparing the y-coordinates of two dots forming a line,
		//	the determine linePos_upper and linePos_lower
		if (modifiedLine[lineIndex].pos1.y < modifiedLine[lineIndex].pos2.y) {
			linePos_upper = modifiedLine[lineIndex].pos1;
			linePos_lower = modifiedLine[lineIndex].pos2;
		}

		else if (modifiedLine[lineIndex].pos1.y > modifiedLine[lineIndex].pos2.y) {
			linePos_upper = modifiedLine[lineIndex].pos2;
			linePos_lower = modifiedLine[lineIndex].pos1;
		}

		// If the y-coordinates of the two dots are the same,
		//	then determine linePos_upper and linePos_lower randomly
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

		// If linePos_lower is above pos, then compare pos with the next line
		if (pos.y - linePos_lower.y >= upsilon)
			continue;

		// 선분 위에 x좌표가 수도꼭지의 x좌표인 점이 없으면 continue
		if (linePos_upper.x > linePos_lower.x) {
			if (linePos_lower.x - pos.x >= upsilon || pos.x - linePos_upper.x >= upsilon)
				continue;
		}

		else
			if (linePos_upper.x - pos.x >= upsilon || pos.x - linePos_lower.x >= upsilon)
				continue;

		// 선분 위에서 x좌표가 수도꼭지의 x좌표인 점을 찾음
		ofVec2f u(linePos_lower.x - linePos_upper.x, linePos_lower.y - linePos_upper.y);
		float t = (pos.x - linePos_upper.x) / u.x;
		tempPos = ofVec2f(linePos_upper.x + u.x * t, linePos_upper.y + u.y * t);

		// 찾은 점이 curCor과 일치하는가? (upsilon과의 비교)
		if (tempPos.distance(pos) < upsilon)

			// 일치하는 경우(upsilon과의 비교),
			if (tempPos.distance(linePos_lower) >= upsilon) {

				// 그 점이 아랫점이 아니면, temp에 아랫점을 저장, return oblique
				destPos = linePos_lower;
				return diagnal;
			}

		// 일치하지 않는 경우
		// 찾은 점이 현재 위치보다 아래에 있을 때
		if (tempPos.y > pos.y)

			// tempCor이 초기화상태 그대로라면, tempCor에 찾은 점을 저장
			if (destPos == pos)
				destPos = tempPos;

		// tempCor이 초기화상태가 아니라면, 찾은 점과 tempCor 중 더 위에 있는 점을 tempCor에 저장
			else
				if (destPos.y > tempPos.y)
					destPos = tempPos;
	}

	// 위의 for문을 통과했으면 현재 위치는 선분 위에 있는 것이 아님
	// tempCor이 초기화상태 그대로가 아니라면
	if (destPos != pos)
		return vertical;

	// tempCor이 초기화상태 그대로라면 현재 물 위치 아래에 아무것도 없는 것
	return verticalTillEnd;
}

void waterObj::moveVertically() {
	ofVec2f nextPos(0, path[pathCnt - 1].y);
	velocity.y = velocity.length();
	velocity.x = 0;

	while (1) {
		velocity.y += g;
		nextPos.y += velocity.y;
		// y와 tempCor[1]이 일치하지 않으면 (upsilon 비교)
		if (nextPos.distance(destPos) > upsilon) {

			// y가 tempCor[1]보다 아래에 있으면
			if (nextPos.y > destPos.y) {

				// pathCor과 curCor에 tempCor을 저장 후 return
				path[pathCnt++] = destPos;
				return;
			}

			// y가 tempCor[1]보다 위에 있으면
			else {

				// pathCor에 x = curCor[0], y = y인 점을 저장
				path[pathCnt] = path[pathCnt - 1];
				path[pathCnt++].y = nextPos.y;
			}
		}

		// y와 tempCor[1]이 일치하면
		else {

			// pathCor에 tempCor을 저장 후 return
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

		// y와 tempCor[1]이 일치하지 않으면 (upsilon 비교)
		if (nextPos.distance(destPos) >= upsilon) {

			// y가 tempCor[1]보다 아래에 있으면
			if (nextPos.y > destPos.y) {

				// pathCor과 curCor에 tempCor을 저장 후 return
				path[pathCnt++] = destPos;
				return;
			}

			// y가 tempCor[1]보다 위에 있으면
			else {

				// pathCor에 x = x, y = y인 점을 저장
				path[pathCnt++] = nextPos;
			}
		}

		// y와 tempCor[1]이 일치하면
		else {

			// pathCor과 curCor에 tempCor을 저장 후 return
			path[pathCnt++] = destPos;
			return;
		}
	}
}