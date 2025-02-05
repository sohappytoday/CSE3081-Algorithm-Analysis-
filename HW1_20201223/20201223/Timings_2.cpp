# define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include "measure_cpu_time.h"
#include<string.h>
#include<stdlib.h>
#define MIN_MAX -2000000000
#define CMAX 1000000000

int Al3(int**, int, int);
int Al4(int**, int, int);
int Al5(int**, int, int);

int calculateSum(int** sumarr, int top, int bottom, int left, int right);
int hello(int** sumarr, int sizem, int sizen);
int middlesum(int** sumarr, int top, int bottom, int left, int right, int* localX1, int* localY1, int* localX2, int* localY2);
int kadane(int* temp, int sizem, int row, int col);

int x1, x2, y1, y2;   // �������� i,j,k,l�� �ǹ�
int x1temp, x2temp, y1temp, y2temp;

int main(void) {
    FILE* fp;
    fopen_s(&fp, "Data/HW1_config.txt", "r");  // file open

    if (fp == NULL) {
        perror("������ �ȿ����±�");  // perror�� ������ ����� �ҷ��帮�� ���� ��� ��� �� ����
        return 1;
    }

    char line[128];  // line�� ũ��� 128����Ʈ�� ����
    fgets(line, sizeof(line), fp);
    int i = 0, j, row, col;
    int NumTc = 0;
    while (line[i] != '\0' && line[i] != '\n') {  // ���Ϳ� null ���ڰ� �ƴ� ������ �޴´�
        NumTc += line[i] - '0';
        NumTc *= 10;
        i++;
    }
    NumTc = NumTc / 10;

    int Alnum;  // �˰��� ��ȣ
    char InputFileName[64];  // �Է� ���� ���� �̸�
    char InputAvgFileName[64];  // �Է� ���� ��հ� ���� �̸�
    char OutputFileName[64];  // ����� ��� ���� �̸�

    for (i = 0; i < NumTc; i++) {
        memset(InputFileName, 0, sizeof(char) * 64);
        memset(InputAvgFileName, 0, sizeof(char) * 64);
        memset(OutputFileName, 0, sizeof(char) * 64);

        fscanf_s(fp, "%d %s %s %s", &Alnum, InputFileName, sizeof(InputFileName), InputAvgFileName, sizeof(InputAvgFileName), OutputFileName, sizeof(OutputFileName));

        FILE* fp1;
        char filePath1[256];  
        sprintf(filePath1, "Data/%s", InputFileName); 
        fopen_s(&fp1, filePath1, "r");
        if (fp1 == NULL) {
            perror("InputFileName�� �� �����±�");
            return 1;
        }

        FILE* fp2;
        char filePath2[256]; 
        sprintf(filePath2, "Data/%s", InputAvgFileName); 

        fopen_s(&fp2, filePath2, "r");
        if (fp2 == NULL) {
            perror("InputAvgFileName�� �߸��Ƴ�����");
            return 1;
        }

        int avgnum;  // ��հ�
        fscanf_s(fp2, "%d", &avgnum);

        char name[20];
        int sizem, sizen;
        int mgv;
        fscanf_s(fp1, "%s", name, sizeof(name));  // "P2" ���� ������ �о ����
        fscanf_s(fp1, "%d %d", &sizem, &sizen);  // 2D Array�� m x n ������ �б�

        int** arr = (int**)malloc(sizeof(int*) * sizem);
        for (j = 0; j < sizem; j++)
            arr[j] = (int*)malloc(sizeof(int) * sizen);

        fscanf_s(fp1, "%d", &mgv);  // ���ʿ��� ù��° ���� ����

        for (row = 0; row < sizem; row++) {
            for (col = 0; col < sizen; col++) {
                fscanf_s(fp1, "%d", &arr[row][col]);
                arr[row][col] -= avgnum;  // ��հ� ���ֱ�
            }
        }

        int max;
        fprintf_s(stdout, "*****************************\n");

        switch (Alnum) {
        case 3:
            CHECK_TIME_START(_start, _freq);
            max = Al3(arr, sizem, sizen);
            CHECK_TIME_END(_start, _end, _freq, _compute_time);
            fprintf_s(stdout, "Algorithm 3 *** n = %u --> ", sizem);
            fprintf_s(stdout, " run time = %.3fms (%s)\n", _compute_time, InputFileName);
            break;
        case 4:
            CHECK_TIME_START(_start, _freq);
            max = Al4(arr, sizem, sizen);
            CHECK_TIME_END(_start, _end, _freq, _compute_time);
            fprintf_s(stdout, "Algorithm 4 *** n = %u --> ", sizem);
            fprintf_s(stdout, " run time = %.3fms (%s)\n", _compute_time, InputFileName);
            break;
        case 5:
            CHECK_TIME_START(_start, _freq);
            max = Al5(arr, sizem, sizen);
            CHECK_TIME_END(_start, _end, _freq, _compute_time);
            fprintf_s(stdout, "Algorithm 5 *** n = %u --> ", sizem);
            fprintf_s(stdout, " run time = %.3fms (%s)\n", _compute_time, InputFileName);
            break;
        default:
            printf_s("�˰��� �ѹ��� �߸��Ǿ����ϴ�.");
            return 1;
        }
        printf_s("%d %d %d %d %d\n", max, x1, y1, x2, y2);
        FILE* fp3;
        char filePath3[256];
        sprintf(filePath3, "Data/%s", OutputFileName);
        fopen_s(&fp3, filePath3, "w");
        fprintf_s(fp3, "%d %d %d %d %d", max, x1, y1, x2, y2);
    }

    fclose(fp);
    return 0;
}



int Al3(int** arr, int sizem, int sizen) {
    int i, j, k, l;
    int** sumarr = (int**)malloc(sizeof(int*) * sizem);
    for (i = 0; i < sizem; i++)
        sumarr[i] = (int*)malloc(sizeof(int) * sizen);
    for (i = 0; i < sizem; i++) {
        for (j = 0; j < sizen; j++) {
            if (i == 0 && j == 0)
                sumarr[i][j] = arr[0][0];
            else {
                if (i == 0)
                    sumarr[i][j] = sumarr[i][j - 1] + arr[i][j];  //����
                else if (j == 0)
                    sumarr[i][j] = sumarr[i - 1][j] + arr[i][j];  //�Ͽ�
                else
                    sumarr[i][j] = sumarr[i - 1][j] + sumarr[i][j - 1] - sumarr[i - 1][j - 1] + arr[i][j];
            }
        }
    }

    int max = MIN_MAX;
    for (i = 0; i < sizem; i++) {
        for (j = 0; j < sizen; j++) {                              //(i,j)�� ������
            for (k = 0; k <= i; k++) {
                for (l = 0; l <= j; l++) {                           //(k,l)������ �ڽ��� max�� ���Ұ��̴�.
                    int sum = sumarr[i][j];
                    if (k > 0)
                        sum -= sumarr[k - 1][j];
                    if (l > 0)
                        sum -= sumarr[i][l - 1];
                    if (k > 0 && l > 0)
                        sum += sumarr[k - 1][l - 1];
                    if (sum > max) {
                        max = sum;
                        x2 = i;
                        y2 = j;
                        x1 = k;
                        y1 = l;
                    }
                }
            }
        }
    }
    for (i = 0; i < sizem; i++)
        free(sumarr[i]);
    free(sumarr);
    return max;
}

int middlesum(int** sumarr, int top, int bottom, int left, int right, int* x1temp, int* y1temp, int* x2temp, int* y2temp) {
    if (left == right) {
        int sum = calculateSum(sumarr, top, bottom, left, right);
        *x1temp = top;
        *y1temp = left;
        *x2temp = bottom;
        *y2temp = right;
        return sum;
    }

    int center = (left + right) / 2;
    int leftMax = middlesum(sumarr, top, bottom, left, center, x1temp, y1temp, x2temp, y2temp);
    int rightMax = middlesum(sumarr, top, bottom, center + 1, right, x1temp, y1temp, x2temp, y2temp);

    int maxLeftSum = MIN_MAX, maxRightSum = MIN_MAX;
    int leftSum = 0, rightSum = 0;

    int bestX1temp = top, bestY1temp = center;
    int bestX2temp = bottom, bestY2temp = center + 1;

    // Maximum sum for left side
    for (int i = center; i >= left; i--) {
        leftSum += calculateSum(sumarr, top, bottom, i, i);
        if (leftSum > maxLeftSum) {
            maxLeftSum = leftSum;
            bestX1temp = top;
            bestY1temp = i;
        }
    }

    for (int i = center + 1; i <= right; i++) {
        rightSum += calculateSum(sumarr, top, bottom, i, i);
        if (rightSum > maxRightSum) {
            maxRightSum = rightSum;
            bestX2temp = bottom;
            bestY2temp = i;
        }
    }

    int maxMiddleSum = maxLeftSum + maxRightSum;
    if (leftMax >= rightMax && leftMax >= maxMiddleSum)
        return leftMax;
    else if (rightMax >= leftMax && rightMax >= maxMiddleSum)
        return rightMax;
    else {
        *x1temp = bestX1temp;
        *y1temp = bestY1temp;
        *x2temp = bestX2temp;
        *y2temp = bestY2temp;
        return maxMiddleSum;
    }
}

int hello(int** sumarr, int sizem, int sizen) {
    int maxSum = MIN_MAX;
    int x1temp = 0, y1temp = 0, x2temp = 0, y2temp = 0;

    for (int top = 0; top < sizem; top++) {
        for (int bottom = top; bottom < sizem; bottom++) {
            int currentMax = middlesum(sumarr, top, bottom, 0, sizen - 1, &x1temp, &y1temp, &x2temp, &y2temp);
            if (currentMax > maxSum) {
                maxSum = currentMax;
                // Update final coordinates
                x1 = x1temp;
                y1 = y1temp;
                x2 = x2temp;
                y2 = y2temp;
            }
        }
    }

    return maxSum;
}

int calculateSum(int** sumarr, int top, int bottom, int left, int right) {
    int sum = sumarr[bottom][right];
    if (top > 0) sum -= sumarr[top - 1][right];
    if (left > 0) sum -= sumarr[bottom][left - 1];
    if (top > 0 && left > 0) sum += sumarr[top - 1][left - 1];
    return sum;                                                             //(top,left)���� (bottom,right)������ ��
}

int Al4(int** arr, int sizem, int sizen) {
    int** sumarr = (int**)malloc(sizeof(int*) * sizem);
    for (int i = 0; i < sizem; i++) {
        sumarr[i] = (int*)malloc(sizeof(int) * sizen);
    }

    sumarr[0][0] = arr[0][0];
    for (int i = 1; i < sizem; i++)
        sumarr[i][0] = arr[i][0] + sumarr[i - 1][0];
    for (int i = 1; i < sizen; i++)
        sumarr[0][i] = arr[0][i] + sumarr[0][i - 1];
    for (int i = 0; i < sizem; i++) {
        for (int j = 0; j < sizen; j++) {
            if (i != 0 && j != 0)
                sumarr[i][j] = arr[i][j] + sumarr[i - 1][j] + sumarr[i][j - 1] - sumarr[i - 1][j - 1];
        }
    }
    int result = hello(sumarr, sizem, sizen);

    for (int i = 0; i < sizem; i++)
        free(sumarr[i]);
    free(sumarr);

    return result;
}






int Al5(int** arr, int sizem, int sizen) {
    int* temp = (int*)malloc(sizeof(int) * sizem);
    int max = MIN_MAX;                     //���� -100000�� �ִ�� �ΰ� ū ���� ������ ����
    int maxtemp;
    int sum;
    int i, j, k;
    for (i = 0; i < sizen; i++) {
        memset(temp, 0, sizeof(int) * sizem);
        for (j = i; j < sizen; j++) {
            for (k = 0; k < sizem; k++)
                temp[k] += arr[k][j];               //k���� i��°~j��°���� �����ذ�
            maxtemp = kadane(temp, sizem, i, j);
            if (maxtemp > max) {
                max = maxtemp;
                x1 = x1temp;
                x2 = x2temp;
                y1 = y1temp;
                y2 = y2temp;
            }
        }
    }
    free(temp);
    return max;
}
int kadane(int* temp, int sizem, int row, int col) {
    int* dp = (int*)malloc(sizeof(int) * sizem);
    dp[0] = temp[0];
    int max = dp[0];
    y1temp = row;
    x1temp = 0;
    x2temp = 0;

    for (int i = 1; i < sizem; i++) {
        if (dp[i - 1] >= 0)
            dp[i] = dp[i - 1] + temp[i];
        else {
            dp[i] = temp[i];
            x1temp = i; // ���ο� ���� ��ġ
        }
        if (dp[i] > max) {
            max = dp[i];
            x2temp = i;  // ������ ��ġ
        }
    }

    y2temp = col; // x�� ������ ��ġ�� ����
    free(dp);
    return max;
}
