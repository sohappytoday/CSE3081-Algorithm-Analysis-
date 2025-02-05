#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAX_VAL 1000000000
#define MAX_EDGE_NUM 200
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

double get_distance(double x1, double y1,double x2,double y2) {					//�� �� �Ÿ� ���ϴ� �Լ�
	double value = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return value;
}
int cnt = 0;
int sort_trace[MAX_EDGE_NUM-3][2];					//trace���� sort ��Ű�� ����

void init_sort_trace(void) {						//trace initialize
	for (int i = 0; i < MAX_EDGE_NUM - 3; i++) {
		for (int j = 0; j < 2; j++) {
			sort_trace[i][j] = MAX_EDGE_NUM;
		}
	}
	cnt = 0;
}

void traceback(int** trace, int i, int j) {
	if (trace[i][j] == -1) return;
	int k = trace[i][j];
	if (k - i != 1) {										//���ʿ� �� �پ��ִ°� �ƴ϶��
		traceback(trace, i, k);								// ���� �ﰢ�� 
		sort_trace[cnt][0] = i;
		sort_trace[cnt++][1] = k;
	}
	if (j - k != 1) {
		sort_trace[cnt][0] = k;
		sort_trace[cnt++][1] = j;
		traceback(trace, k, j);
	}														// ������ �ﰢ�� ����
}


double min_tri(int num, double** vertex) {				//num�� vertex ����, vertex�� x,y��ǥ ������ �ִ� �迭
	int i,j,size;										//size�� i vertex�� j vertex�� ������ j-i�� �˻��ϰ� ����
	/*for (i = 0; i < num; i++) {						//�� �޾Ҵ��� �˻�
		for (j = 0; j < 2; j++)
			printf("%lf ", vertex[i][j]);
		printf("\n");
	}*/

	double** dp = malloc(sizeof(double*) * num);
	for (i = 0; i < num; i++)
		dp[i] = malloc(sizeof(double) * num);
	
	int** trace = malloc(sizeof(int*) * num);            // edge ���ϴ� �Լ�
	for (i = 0; i < num; i++) {
		trace[i] = malloc(sizeof(int) * num);
		for (j = 0; j < num; j++) {
			trace[i][j] = -1;                            
		}
	}


	for (i = 0; i < num-1; i++) {						//�� ���� vertex�� �ﰢ���� ���鶧 ���� edge �ȳ��ö�
		for (j = i+1; j < num; j++) {
			if ((i+1 == j) || (i+2== j))
			{					
				dp[i][j] = 0;
			}
		}
	}
	double temp;
	for (size = 3; size < num; size++) {
		for (i = 0; i < num - 2; i++) {
			if (i + size < num) {											//�Ѿ�� �ȵ�
				dp[i][i + size] = MAX_VAL;
				for (j = i + 1; j < i + size; j++) { 
					temp = dp[i][i + size];
					if ((j + 1 != size + i) &&(j!=i+1)) {
						dp[i][i + size] = min(dp[i][i + size], dp[i][j] + dp[j][i + size] + get_distance(vertex[i][0], vertex[i][1],
							vertex[j][0], vertex[j][1]) + get_distance(vertex[j][0], vertex[j][1], vertex[i + size][0], vertex[i + size][1]));
					}
					else if (j + 1 == size + i) {	//�����ʿ� �� �پ��ִ� ���
						dp[i][i + size] = min(dp[i][i + size], dp[i][j] + dp[j][i + size] + get_distance(vertex[i][0], vertex[i][1],
							vertex[j][0], vertex[j][1]));

					}
					else if (j = i + 1) {				//���ʿ� �� �پ��ִ� ���
						dp[i][i + size] = min(dp[i][i + size], dp[i][j] + dp[j][i + size] + get_distance(vertex[j][0], vertex[j][1],
							vertex[i + size][0], vertex[i + size][1]));
					}
					if (temp != dp[i][i + size])
						trace[i][i + size] = j;
				}
			}
		}
	}
	/*
	for (i = 0; i < num - 1; i++) {
		for (j = i+1; j < num; j++)
			printf("%.3lf ", dp[i][j]);
		printf("\n");
	}*/								
	traceback(trace,0, num - 1);
	double result = dp[0][num - 1];
	for (i = 0; i < num; i++)
		free(dp[i]);
	free(dp);
	return result;
}

int compare(const void* a, const void* b) {
	int* arr1 = (int*)a;
	int* arr2 = (int*)b;
	if (arr1[0] != arr2[0]) 
		return arr1[0] - arr2[0]; 
	else 
		return arr1[1] - arr2[1];  
}

void read_txt(char* input_file, char* output_file) {
	FILE* fp1 = fopen(input_file, "r");
	if (fp1 == NULL) {
		perror("txt ���� ����");
		return;
	}
	FILE* fp2 = fopen(output_file, "w");
	init_sort_trace();
	int i, num_of_vertex;
	fscanf(fp1, "%d", &num_of_vertex);
	double** vertex = malloc(sizeof(double*) * num_of_vertex);
	for (i = 0; i < num_of_vertex; i++)
		vertex[i] = malloc(sizeof(double) * 2);
	for (i = 0; i < num_of_vertex; i++) {
		fscanf(fp1, "%lf %lf", &vertex[i][0], &vertex[i][1]);
	}
	double result = min_tri(num_of_vertex, vertex);								//dp
	
	qsort(sort_trace, num_of_vertex - 3, sizeof(sort_trace[0]), compare);		//sort_trace�� sort��Ű�� ����
	
	fprintf(fp2, "%.3lf\n", result);							//output ���Ͽ� ���
	for (i = 0; i < num_of_vertex - 3; i++){
		fprintf(fp2, "%d %d\n", sort_trace[i][0], sort_trace[i][1]);
	}
	for (i = 0; i < num_of_vertex; i++)							//free
		free(vertex[i]);
	free(vertex);
	fclose(fp2);
	fclose(fp1);
}

int main() {
	FILE* fp = fopen("MT_command.txt", "r");
	if (fp == NULL) {
		perror("���� ����");
		return 1;
	}
	int num_of_txt;												//txt���� ����
	fscanf(fp, "%d", &num_of_txt);
	int i;										
	for (i = 0; i < num_of_txt; i++) {
		char* input_file = malloc(32 * sizeof(char));					//input file �����͵�
		char* output_file = malloc(32 * sizeof(char));					//output file �����͵�
		fscanf(fp, "%s %s", input_file, output_file);
		read_txt(input_file, output_file);
		free(output_file);
		free(input_file);
	}
	fclose(fp);
	return 0;
}