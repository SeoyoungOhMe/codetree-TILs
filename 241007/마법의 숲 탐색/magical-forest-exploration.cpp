#include <bits/stdc++.h>

using namespace std;

int r, c, k;
vector<vector<bool>> occupied;
                //상 우 하 좌
vector<int> dx = {-1, 0, 1, 0};
vector<int> dy = {0, 1, 0, -1};

int answer = 0;

int x, y; // 현재 골렘의 x, y 위치

bool isTop() {
    // 현재 가운데에서 장애물을 마주치거나 한 칸 내렸을 때 세 면 중 최소 하나에서 장애물을 마주치는 경우
    return (occupied[x+1][y] || occupied[x+1][y-1] || occupied[x+2][y] || occupied[x+1][y+1]);
}

bool isMovingDownValid(int d) {
    return (!occupied[x+1][y-1] && !occupied[x+2][y] && !occupied[x+1][y+1])
}

void moveDown() {
    x++; // row 내리기
}

bool isFloor() {
    return (x == r-2);
}

int moveChar() {
    
    
    return x;
}

bool isTurningLeftValid(int d) { 
    // 이미 방향이 서쪽으로 정해져 있거나, 현재 왼쪽 벽과 닿아있지 않거나, 왼->아 이동해도 장애물이 없을 때
    return (d == 0 || x != 1 || (!occupied[x][y-2] && !occupied[x+1][y-1] && isMovingDownValid(d)));
}

bool isTurningRightValid(int d) {
    // 이미 방향이 동쪽으로 정해져 있거나, 현재 오른쪽 벽과 닿아있지 않거나, 오->아 이동해도 장애물이 없을 때
    return (d == 1 || x != c-2 || (!occupied[x][y+2] && !occupied[x+1][y+1] && isMovingDownValid(d)))
}

void turnLeft(int& d) {
    if (d == 3) d = 0;
    else d++;
}

void turnRight(int& d) {
    if (d == 0) d = 3;
    else d--;
}

int main() {
    cin >> r >> c >> k;
    occupied.assign(r, vector<bool>(c, false));

    for (int i = 0; i < k; i++) { // 모든 골렘에 대하여
        int start, exit; // start:출발하는 열, d:출구 방향
        cin >> start >> exit;

        x = -1;
        y = start - 1; // 인덱스 0 기준으로 맞춤
        int d = -1; // 초기화 (0:left, 1:right)

        while(true) {
            if (isTop()) { // 처음부터 내려갈 수 없을 때
                occupied.assign(r, vector<bool>(c, false));
                break;
            }
            else if (isMovingDownValid(d)) {
                moveDown();
            } else if (isFloor()) {
                int last_row = moveChar();
                answer += last_row;
                break;
            } else { // 밑에 세 칸 중 하나라도 장애물이 있는 경우
                if (isTurningLeftValid(d)) { // 서쪽으로 돌 수 있는 경우 (이미 방향이 서쪽으로 정해져 있거나, 현재 왼쪽 벽과 닿아있지 않거나, 왼->아 이동해도 장애물이 없을 때)
                    turnLeft(d); // 출구 방향도 돌려야 함
                    moveDown();
                } else if (isTurningRightValid(d)) { // 동쪽으로 돌 수 있는 경우 (이미 방향이 동쪽으로 정해져 있거나, 현재 오른쪽 벽과 닿아있지 않거나, 오->아 이동해도 장애물이 없을 때)
                    turnRight(d);
                    moveDown();
                } else { // 왼쪽, 오른쪽 모두 돌아서 이동할 수 없는 완전히 막힌 경우
                    int last_row = moveChar();
                    answer += last_row;
                    break;
                }
            }
        }
    }

    cout << answer;
    
    return 0;
}