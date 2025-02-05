#pragma once

double get_distance(double x1, double y1, double x2, double y2);
void init_sort_trace(void);
void traceback(int** trace, int i, int j);
double min_tri(int num, double** vertex);
int compare(const void* a, const void* b);
void read_txt(char* input_file, char* output_file);
