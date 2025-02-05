#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int length;      // dp[0][num - 1] 
    char* lps;       // 리턴받을 문자열 저장
} LPSResult;


LPSResult solve_LPS(int n, char* str, int** visit) {
    int i, j;
    int** dp = malloc(sizeof(int*) * n);
    for (i = 0; i < n; i++)
        dp[i] = malloc(sizeof(int) * n);

    int size;
    for (size = 0; size < n; size++) {
        for (i = 0; i < n - size; i++) {
            j = i + size;
            if (size == 0) {                //길이 1
                dp[i][j] = 1;
                visit[i][j] = -1;
            }
            else if (size == 1) {           //길이 2
                if (str[i] == str[j]) {     //근데 그게 같아
                    dp[i][j] = 2;
                    visit[i][j] = -1;
                }
                else {                      //근데 그게 달라
                    dp[i][j] = 1;
                    //           visit[i][j] = (dp[i + 1][j] >= dp[i][j - 1]) ? i + 1 : j - 1;
                    visit[i][j] = i;
                }
            }
            else {
                if (str[i] == str[j]) {         //양 끝이 같아
                    dp[i][j] = 2 + dp[i + 1][j - 1];
                    visit[i][j] = -1;
                }
                else {                          //양 끝이 달라
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                    visit[i][j] = (dp[i + 1][j] >= dp[i][j - 1]) ? i + 1 : j - 1;
                }
            }
        }
    }

    char* result_str = malloc(sizeof(char) * (dp[0][n - 1] + 1));
    int start = 0, end = dp[0][n - 1] - 1;
    i = 0, j = n - 1;
    while (i <= j) {                        //양 끝이 같아질때
        if (visit[i][j] == -1) {
            result_str[start++] = str[i];
            result_str[end--] = str[j];
            i++;
            j--;
        }
        else if (visit[i][j] == i + 1) {
            i++;
        }
        else {
            j--;
        }
    }
    result_str[dp[0][n - 1]] = '\0';

    LPSResult result;
    result.length=dp[0][n - 1]; 
    result.lps = result_str;

    for (i = 0; i < n; i++)
        free(dp[i]);
    free(dp);

    return result;
}

void read_txt(char* input_file, char* output_file) {
    FILE* fp1 = fopen(input_file, "r");
    if (fp1 == NULL) {
        perror("txt 파일 없음");
        return;
    }
    FILE* fp2 = fopen(output_file, "w");

    int result, i, j, num, len_of_str;
    fscanf(fp1, "%d", &num);
    fprintf(fp2, "%d\n", num);
    for (i = 0; i < num; i++) {
        fscanf(fp1, "%d", &len_of_str);
        char* str = malloc(sizeof(char) * (len_of_str + 1));
        int** visit = malloc(sizeof(int*) * len_of_str);
        for (j = 0; j < len_of_str; j++)
            visit[j] = calloc(len_of_str, sizeof(int));
        fscanf(fp1, "%s", str);
        LPSResult res = solve_LPS(len_of_str, str, visit);
        fprintf(fp2, "%d\n%s\n", res.length, res.lps); 
        free(res.lps);

        // 메모리 해제
        for (j = 0; j < len_of_str; j++)
            free(visit[j]);
        free(visit);
        free(str);
    }

    fclose(fp2);
    fclose(fp1);
}

int main() {
    FILE* fp = fopen("LPS_command.txt", "r");
    if (fp == NULL) {
        perror("파일 없음");
        return 1;
    }
    int num_of_txt;
    fscanf(fp, "%d", &num_of_txt);

    for (int i = 0; i < num_of_txt; i++) {
        char* input_file = malloc(32 * sizeof(char));
        char* output_file = malloc(32 * sizeof(char));
        fscanf(fp, "%s %s", input_file, output_file);
        read_txt(input_file, output_file);
        free(output_file);
        free(input_file);
    }
    fclose(fp);
    return 0;
}
