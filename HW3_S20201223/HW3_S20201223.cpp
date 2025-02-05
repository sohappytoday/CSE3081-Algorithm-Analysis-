#include "records.h"
#include <cmath>										//intro sort의 log함수 이용
#define SWAP(a,b,temp) {temp=a; a=b; b=temp;}


void RECORDS::sort_records_insertion(int start_index, int end_index) {
	// Insertion sort
	for (int i = start_index + 1; i <= end_index; i++) {
		RECORD tmp = records[i];
		int j = i;
		while ((j > start_index) && (compare_keys((const void*)&tmp, (const void*)(&records[j - 1])) < 0)) {
			records[j] = records[j - 1];
			j--;
		}
		records[j] = tmp;
	}
}

void adjustmax(RECORD* heap, int i, int n) {
	
	while (2 * i <= n) {
		int leftchild = 2 * i;
		if ((leftchild < n) && (heap[leftchild + 1].key > heap[leftchild].key))
		{
			leftchild++;					//Then it is right child
		}
		if (heap[leftchild].key <= heap[i].key)
			break;
		else {
			RECORD temp;
			SWAP(heap[leftchild], heap[i], temp);
			i = leftchild;
		}
	}
}
void adjustmin(RECORD* heap, int i, int n) {

	while (2 * i <= n) {
		int leftchild = 2 * i;
		if ((leftchild < n) && (heap[leftchild + 1].key < heap[leftchild].key))
		{
			leftchild++;					//Then it is right child
		}
		if (heap[leftchild].key >= heap[i].key)
			break;
		else {
			RECORD temp;
			SWAP(heap[leftchild], heap[i], temp);
			i = leftchild;
		}
	}
}

void RECORDS::sort_records_heap(int start_index, int end_index) {
	int i;
	int size = end_index - start_index+1;
	RECORD temp1;
	RECORD* heap = (RECORD*)malloc(sizeof(RECORD) * (size + 1));
	int idx = start_index;
	for (i = 1; i <= size; i++){
		heap[i] = records[idx++];
	}

	for (i = size/2; i > 0; i--) {
		adjustmax(heap,i, size);
	}
	
	for (int i = size-1; i > 0; i--) {
		SWAP(heap[1], heap[i+1], temp1); 		
		adjustmax(heap, 1, i);
	}

	idx = start_index;
	for (i = 1; i <= size; i++) {
		records[idx++] = heap[i];
	}
	free(heap);
}



void RECORDS::sort_records_weird(int start_index, int end_index) {
	int i;
	int size = end_index - start_index + 1;
	RECORD* heap = (RECORD*)malloc(sizeof(RECORD) * (size + 1));
	int idx = start_index;
	for (i = 1; i <= size; i++) {
		heap[i] = records[idx++];
	}

	for (i = size / 2; i > 0; i--) {						//민희 시켜주기
		adjustmin(heap, i, size);
	}

	idx = start_index;										//가져오기
	for (i = 1; i <= size; i++) {
		records[idx++] = heap[i];
	}
	sort_records_insertion(start_index, end_index);
	free(heap);
}

int partition(RECORD* record, int start, int end) {
	int i, pivot;
	pivot = start;
	RECORD temp;
	for (i = start; i < end; i++) {
		if (record[i].key < record[end].key){
			SWAP(record[i], record[pivot],temp);
			pivot++;
		}
		
	}
	SWAP(record[end], record[pivot], temp);
	return pivot;
}

void RECORDS::sort_records_quick_classic(int start_index, int end_index) {
	int pivot;

	if (end_index > start_index) {
		pivot = partition(records, start_index, end_index);
		sort_records_quick_classic(start_index, pivot-1);
		sort_records_quick_classic(pivot+1, end_index);
	}

}

void RECORDS::sort_records_intro(int start_index, int end_index) {
	int size = end_index - start_index + 1;
	int maxdepth = 2*(int)std::log2(size);
	intro_sort(start_index, end_index, maxdepth);
}

void RECORDS::intro_sort(int start_index, int end_index, int maxdepth){
	int pivot;
	int size = end_index - start_index + 1;
	if (size < 16)
		sort_records_insertion(start_index, end_index);
	else if (maxdepth == 0)
		sort_records_heap(start_index, end_index);
	else {
		pivot = partition(records, start_index, end_index);
		intro_sort(start_index, pivot - 1,maxdepth-1);
		intro_sort(pivot + 1, end_index,maxdepth-1);
	}
}

void RECORDS::sort_records_merge_with_insertion(int start_index, int end_index) {
	int size = end_index - start_index + 1;
	if (size < 16)
		sort_records_insertion(start_index, end_index);
	else {
		int middle_index = (start_index + end_index) / 2;
		sort_records_merge_with_insertion(start_index, middle_index);
		sort_records_merge_with_insertion(middle_index + 1, end_index);
		merge(start_index, middle_index,end_index);
	}
}

void RECORDS::merge(int start, int middle, int end) {
	int i = 0, j = 0, k = start;
	int left_size = middle - start + 1;
	int right_size = end - middle;
	RECORD* left_arr = (RECORD*)malloc(left_size * sizeof(RECORD));
	RECORD* right_arr = (RECORD*)malloc(right_size * sizeof(RECORD));
	memcpy(left_arr, &records[start], left_size * sizeof(RECORD));
	memcpy(right_arr, &records[middle + 1], right_size * sizeof(RECORD));

	while (i < left_size && j < right_size) {
		if (left_arr[i].key <= right_arr[j].key) 
			records[k++] = left_arr[i++];
		else
			records[k++] = right_arr[j++];
	}
	while (i < left_size) {
		records[k++] = left_arr[i++];
	}
	while (j < right_size) {
		records[k++] = right_arr[j++];
	}
	free(left_arr);
	free(right_arr);
}
