#include "alpha_beta.h"

// 评估函数，判断当前局面的得分
int evaluate(int board[3][3], int computer) {
    // 检查行
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == computer) {
                return 1;
            } else if (board[row][0] == 3-computer) {
                return -1;
            }
        }
    }

    // 检查列
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == computer) {
                return 1;
            } else if (board[0][col] == 3-computer) {
                return -1;
            }
        }
    }

    // 检查对角线
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == computer) {
            return 1;
        } else if (board[0][0] == 3-computer) {
            return -1;
        }
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == computer) {
            return 1;
        } else if (board[0][2] == 3-computer) {
            return -1;
        }
    }

    // 如果没有获胜者，返回 0
    return 0;
}

// Alpha-Beta 算法核心函数
int alphaBeta(int board[3][3], int depth, int alpha, int beta, int isMaximizing, int cnt, int computer) {
    int score = evaluate(board, computer);

    // 如果游戏结束或达到最大深度，返回得分
    if (score == 1 || score == -1 || depth == cnt) {
        return score;
    }

    if (isMaximizing) {
        int bestScore = INT_MIN;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == 0) {
                    board[row][col] = computer;
                    int score = alphaBeta(board, depth + 1, alpha, beta, 0, cnt, computer);
                    board[row][col] = 0;
                    bestScore = (score > bestScore)? score : bestScore;
                    alpha = (alpha > bestScore)? alpha : bestScore;
                    if (beta <= alpha) {
                        break;  // 剪枝
                    }
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == 0) {
                    board[row][col] = 3-computer;
                    int score = alphaBeta(board, depth + 1, alpha, beta, 1, cnt, computer);
                    board[row][col] = 0;
                    bestScore = (score < bestScore)? score : bestScore;
                    beta = (beta < bestScore)? beta : bestScore;
                    if (beta <= alpha) {
                        break;  // 剪枝
                    }
                }
            }
        }
        return bestScore;
    }
}

// 找到最优的下一步
void findBestMove(int board[3][3], int *row, int *col, int cnt, int computer) {
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                board[i][j] = computer;
                int score = alphaBeta(board, 1, INT_MIN, INT_MAX, 0, cnt, computer);
                board[i][j] = 0;
                if (score > bestScore) {
                    bestScore = score;
                    *row = i;
                    *col = j;
                }
            }
        }
    }
}

// 打印棋盘
void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int count(int board[3][3]) {
    int res = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 0) {
                res++;
            }
        }
    }
    return res;
}

// int now = 1;

// void playerMove() {
//     printf("请输入你的下一步：\n");
//     int x, y;
//     scanf("%d %d", &x, &y);
//     board[x][y] = 2;
//     printBoard(board);
// }

// void computerMove() {
//     if (cnt == 9) {
//         board[1][1] = 1;
//         printBoard(board);
//         return;
//     }
//     int row, col;
//     findBestMove(board, &row, &col);
//     printf("最优的下一步：%d %d\n", row, col);
//     board[row][col] = 1;
//     printBoard(board);
// }
