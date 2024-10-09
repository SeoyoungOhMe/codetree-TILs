#include <bits/stdc++.h>

using namespace std;

int k, m;
vector<vector<int>> board;

vector<vector<int>> rotateBoard;
vector<vector<int>> eachMiddleValue;
vector<vector<int>> board2;
vector<int> wall;

isRange(int x, int y) {
    return (0 <= x && x < 5 && 0 <= y && y < 5);
}

vector<vector<int>> rotate(vector<vector<int>> board, int angle, int x, int y) {
    board2.assign(5, vector<int>(5, 0));
    if (angle == 0) {
        for (int i=x-1; i<=x+1; i++) {
            for (int j=y-1; j<=y+1; j++) {
                board2[j][2-i] = board[i][j];
            }
        }
    } else if (angle == 1) {
        for (int i=x-1; i<=x+1; i++) {
            for (int j=y-1; j<=y+1; j++) {
                board2[2-i][2-j] = board[i][j];
            }
        }
    } else if (angle == 2) {
        for (int i=x-1; i<=x+1; i++) {
            for (int j=y-1; j<=y+1; j++) {
                board2[2-j][i] = board[i][j];
            }
        }
    }

    return board2;
}

void calculateValue(int x, int y) {
    eachMiddleValue.assign(9, vector<int>(3, 0)); // 각 중심축의 유물 1차 획득 가치를 저장하는 곳(90,180, 270도로 회전)

    for (int i=0; i<9; i++) {
        rotateBoard = rotate(board, 0, x, y);
        eachMiddleValue[i][0] = bfs(rotateBoard); // 90도 회전

        rotateBoard = rotate(board, 1, x, y);
        eachMiddleValue[i][1] = bfs(rotateBoard); // 180도 회전

        rotateBoard = rotate(board, 2, x, y);
        eachMiddleValue[i][2] = bfs(rotateBoard); // 270도 회전
    }
}

void changeBoard(vector<vector<int>>& board) {
    int max_cnt=0, max_angle=0, max_value=0; 
    for (int i=0; i<9; i++) {
        for (int j=0; j<3; j++) {
            if (eachMiddleValue[i][j] > max_value) {
                max_cnt = i;
                max_angle = j;
                max_value = eachMiddleValue[i][j];
            } else if (eachMiddleValue[i][j] == max_value) {
                if (j < max_angle) {
                    max_cnt = i;
                    max_angle = j;
                    max_value = eachMiddleValue[i][j];
                } else if (j == max_angle) {
                    if (i < max_cnt) {
                        max_cnt = i;
                    }
                }
            }
        }
    }

    int x=max_cnt/3;
    int y=max_cnt%3;
    board = rotate(board, max_angle, x, y);
}

vector<vector<bool>> visited;
int bfs(vector<vector<int>>& board) { // 연결된 유물의 총 개수를 반환
    visited.assign(5, vector<bool>(5,false));
    vector<int> dx = {0, 1, 0, -1};
    vector<int> dy = {1, 0, -1, 0};
    int answer=0;
    stack<int> s;

    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            if (board[i][j] != 0 && !visited[i][j]) {
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i, j] = true;
                int cnt = 0;
                s.push({i, j});

                while(!q.empty()) {
                    int current_v = q.front();
                    int x = current_v.first;
                    int y = current_v.second;
                
                    for (int i=0; i<4; i++) {
                        int nx = x + dx[i];
                        int ny = y + dy[i];

                        if (isRange(nx, ny) && board[x][y] == board[nx][ny] && !visited[nx][ny]) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                            s.push({nx, ny});
                        }
                    }
                }

                if (cnt >= 3) {
                    answer += cnt;
                    for (int k=0; k<cnt; k++) {
                        int x, y = s.top();
                        s.pop();
                        board[x][y] = 0;
                    }
                }
            }
        }
    }
    
    return answer;
}

void fillBlank(vector<vector<int>>& board) {
    for (int i=4; i>=0; i--) { // 열
        for (int j=4; j>=0; j--) { // 행
            if (board[j][i] == 0) {
                for (int k=0; k<m; k++) {
                    if (wall[k] != 0) {
                        board[j][i] = wall[k];
                        wall[k] = 0;
                    }
                }
            }
        }
    }
}

void solution(vector<vector<int>>& board, vector<int>& wall) {
    for (int turn=0; turn < k; turn++) {
        int answer = 0;
        // 탐사 진행
        for (int i=1; i<=3; i++) { // 중심 : (1, 1) ~ (3, 3)
            for (int j=1; j<=3; j++) {
                calculateValue(i, j); // 중심좌표:(i, j)
            }
        }

        // 최대 유물이 되는 경우의 결과로 board의 중심좌표 근처 회전시켜 갱신하기
        changeBoard(board);

        // 유물 획득
        int value = bfs(board); // 서로 연결돼있는 유물의 개수 구하기
        answer += value;

        fillBlank(board);

        // 탐사 반복
        while(true) {
            if (bfs(board) == 0) { // 유물이 더 이상 존재하지 않으면 탐사 중단
                break;
            }

            int value = bfs(board); // 서로 연결돼있는 유물 구하기
            answer += value;

            fillBlank();
        }


        cout << answer << " ";
    }
}

int main() {

    cin >> k >> m;
    board.assign(5, vector<int>(5, 0));

    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            cin >> board[i][j];
        }
    }

    wall.assign(m, 0);
    for (int k=0; k<m; k++) {
        cin >> wall[k];
    }

    solution(board, wall);
    
    return 0;
}