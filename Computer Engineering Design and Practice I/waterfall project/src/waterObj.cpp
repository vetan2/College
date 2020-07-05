#include "waterObj.h"

using namespace std;

/*---------------------------------------------------------------------
	input : curCor
	output : 갱신된 pathCor
---------------------------------------------------------------------*/
void waterObj::getPath() {
	pathCor[0][0] = curCor[0], pathCor[0][1] = curCor[1];
	pathCnt = 1;

	while (1) {
		switch (getState(curCor)) {
		case vertical:
			verticalMove(); break;

		case vertical_tillEnd:
			vertical_tillEndMove(); break;

		case oblique:
			obliqueMove(); break;
		}

		if (quit == TRUE)
			return;
	}
}

/*---------------------------------------------------------------------
	input : curCor, lineCor, lineCnt
	output : 물이 어떻게 움직이는가에 대한 state, 갱신된 tempCor
---------------------------------------------------------------------*/
state waterObj::getState(float* curCor) {
	int i;
	int x1, x2, y1, y2;	// (x1, y1)이 위에 있는 점임
	float pointCor[2];
	float m;
	
	tempCor[0] = curCor[0], tempCor[1] = curCor[1];
	for (i = 0; i < lineCnt; i++) {
		// 선분을 이루는 두 점 중 아래에 있는 점(이하 아랫점)의 위치를 구함
		if (lineCor[i][1] > lineCor[i][3])
			x1 = lineCor[i][2], y1 = lineCor[i][3],
			x2 = lineCor[i][0], y2 = lineCor[i][1];

		else
			x1 = lineCor[i][0], y1 = lineCor[i][1],
			x2 = lineCor[i][2], y2 = lineCor[i][3];
			
		// 아랫점이 수도꼭지보다 위에 있으면 continue
		if (curCor[1] - y2 > upsilon)
			continue;

		// 선분 위에 x좌표가 수도꼭지의 x좌표인 점이 없으면 continue
		if (x1 > x2) {
			if (curCor[0] < x2 || curCor[0] > x1)
				continue;
		}
		else
			if (curCor[0] < x1 || curCor[0] > x2)
				continue;
			
		// 선분 위에서 x좌표가 수도꼭지의 x좌표인 점을 찾음
		m = (float)(y1 - y2) / (float)(x1 - x2);
		pointCor[0] = curCor[0];
		pointCor[1] = y1 + m * (curCor[0] - x1);

		// 찾은 점이 curCor과 일치하는가? (upsilon과의 비교)
		if(pointCor[1] - curCor[1] < upsilon && pointCor[1] - curCor[1] > -upsilon)

		// 일치하는 경우(upsilon과의 비교),
			if (!(pointCor[1] - y2 < upsilon && pointCor[1] - y2 > -upsilon)) {

				// 그 점이 아랫점이 아니면, temp에 아랫점을 저장, return oblique
				tempCor[0] = x2, tempCor[1] = y2;
				return oblique;
			}

		// 일치하지 않는 경우
		// 찾은 점이 현재 위치보다 아래에 있을 때
		if (pointCor[1] > curCor[1])

			// tempCor이 초기화상태 그대로라면, tempCor에 찾은 점을 저장
			if (tempCor[0] == curCor[0] && tempCor[1] == curCor[1])
				tempCor[1] = pointCor[1];

		// tempCor이 초기화상태가 아니라면, 찾은 점과 tempCor 중 더 위에 있는 점을 tempCor에 저장
			else
				if (tempCor[1] > pointCor[1])
					tempCor[1] = pointCor[1];
	}

	// 위의 for문을 통과했으면 현재 위치는 선분 위에 있는 것이 아님
	// tempCor이 초기화상태 그대로가 아니라면
	if(tempCor[1] != curCor[1])
		return vertical;

	// tempCor이 초기화상태 그대로라면 현재 물 위치 아래에 아무것도 없는 것
	return vertical_tillEnd;
}


/*---------------------------------------------------------------------
	input : curCor, tempCor(물이 떨어질 선분의 위치를 담고 있음)
	output : 갱신된 curCor, 갱신된 pathCor, pathCnt
---------------------------------------------------------------------*/
void waterObj::verticalMove() {
	float y = curCor[1];
	
	while (1) {
		y += speed;
		// y와 tempCor[1]이 일치하지 않으면 (upsilon 비교)
		if (!(y - tempCor[1] < upsilon && y - tempCor[1] > -upsilon)) {

			// y가 tempCor[1]보다 아래에 있으면
			if (y > tempCor[1]) {

				// pathCor과 curCor에 tempCor을 저장 후 return
				pathCor[pathCnt][0] = tempCor[0];
				pathCor[pathCnt++][1] = tempCor[1];
				curCor[0] = tempCor[0], curCor[1] = tempCor[1];
				return;
			}

			// y가 tempCor[1]보다 위에 있으면
			else {

				// pathCor에 x = curCor[0], y = y인 점을 저장
				pathCor[pathCnt][0] = curCor[0];
				pathCor[pathCnt++][1] = y;
				curCor[0] = tempCor[0], curCor[1] = tempCor[1];
			}
		}
		
		// y와 tempCor[1]이 일치하면
		else {

			// pathCor에 tempCor을 저장 후 return
			pathCor[pathCnt][0] = tempCor[0];
			pathCor[pathCnt++][1] = tempCor[1];
			return;
		}
	}
}

/*---------------------------------------------------------------------
	input : curCor
	output : 갱신된 curCor, 갱신된 pathCor, pathCnt
---------------------------------------------------------------------*/
void waterObj::vertical_tillEndMove() {
	float y = curCor[1];

	while (1) {
		y += speed;
		if (y > 738) {
			pathCor[pathCnt][0] = curCor[0];
			pathCor[pathCnt++][1] = 738;
			break;
		}
		
		pathCor[pathCnt][0] = curCor[0];
		pathCor[pathCnt++][1] = y;
	}

	curCor[1] = y;
	quit = TRUE;
}

/*---------------------------------------------------------------------
	input : curCor, tempCor(따라 내려가는 선분의 끝 점을 담고 있음)
	output : 갱신된 curCor, 갱신된 pathCor, pathCnt
---------------------------------------------------------------------*/
void waterObj::obliqueMove() {
	float x, y, m, speed_x, speed_y;
	x = curCor[0], y = curCor[1];
	m = (curCor[1] - tempCor[1]) / (curCor[0] - tempCor[0]);

	if (curCor[0] < tempCor[0]) {
		speed_x = sqrt(speed * speed / (m * m + 1));
		speed_y = speed_x * m;
	}
		
	else {
		speed_x = -sqrt(speed * speed / (m * m + 1));
		speed_y = speed_x * m;
	}

	while (1) {
		x += speed_x;
		y += speed_y;
		// y와 tempCor[1]이 일치하지 않으면 (upsilon 비교)
		if (!(y - tempCor[1] < upsilon && y - tempCor[1] > -upsilon)) {

			// y가 tempCor[1]보다 아래에 있으면
			if (y > tempCor[1]) {

				// pathCor과 curCor에 tempCor을 저장 후 return
				pathCor[pathCnt][0] = tempCor[0];
				pathCor[pathCnt++][1] = tempCor[1];
				curCor[0] = tempCor[0], curCor[1] = tempCor[1];
				return;
			}

			// y가 tempCor[1]보다 위에 있으면
			else {

				// pathCor에 x = x, y = y인 점을 저장
				pathCor[pathCnt][0] = x;
				pathCor[pathCnt++][1] = y;
			}
		}

		// y와 tempCor[1]이 일치하면
		else {

			// pathCor과 curCor에 tempCor을 저장 후 return
			pathCor[pathCnt][0] = tempCor[0];
			pathCor[pathCnt++][1] = tempCor[1];
			curCor[0] = tempCor[0], curCor[1] = tempCor[1];
			return;
		}
	}
}