#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {                                                                        // Edge 구조체 
    long long start;
    long long end;
    long long weight;
} Edge;

typedef struct {                                                                        // Min-Heap 구조체
    Edge* edges;
    long long size;
    long long capacity;
} MinHeap;


typedef struct {                                                                        // Component 구조체
    long long size;
    long long weight;
} Component;

MinHeap* createMinHeap(long long capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->edges = (Edge*)malloc(sizeof(Edge) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapEdges(Edge* a, Edge* b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, long long idx) {
    long long smallest = idx;
    long long left = 2 * idx + 1;
    long long right = 2 * idx + 2;

    if (left < heap->size && heap->edges[left].weight < heap->edges[smallest].weight)
        smallest = left;
    if (right < heap->size && heap->edges[right].weight < heap->edges[smallest].weight)
        smallest = right;
    if (smallest != idx) {
        swapEdges(&heap->edges[idx], &heap->edges[smallest]);
        minHeapify(heap, smallest);
    }
}

Edge extractMin(MinHeap* heap) {
    if (heap->size <= 0)
        return (Edge) { -1, -1, -1 };
    if (heap->size == 1) {
        heap->size--;
        return heap->edges[0];
    }

    Edge root = heap->edges[0];
    heap->edges[0] = heap->edges[--heap->size];
    minHeapify(heap, 0);

    return root;
}

void insertMinHeap(MinHeap* heap, Edge edge) {
    if (heap->size == heap->capacity)
        return;

    long long i = heap->size++;
    heap->edges[i] = edge;

    while (i != 0 && heap->edges[(i - 1) / 2].weight > heap->edges[i].weight) {
        swapEdges(&heap->edges[i], &heap->edges[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void buildMinHeap(MinHeap* heap, Edge edges[], long long n) {
    heap->size = n;
    for (long long i = 0; i < n; i++) {
        heap->edges[i] = edges[i];
    }

    for (long long i = (n / 2) - 1; i >= 0; i--) {
        minHeapify(heap, i);
    }
}

long long find(long long* parent, long long vertex) {
    if (parent[vertex] != vertex)
        parent[vertex] = find(parent, parent[vertex]);
    return parent[vertex];
}

void unionSets(long long parent[], long long rank[], long long root1, long long root2) {
    if (rank[root1] > rank[root2]) {
        parent[root2] = root1;
    }
    else if (rank[root1] < rank[root2]) {
        parent[root1] = root2;
    }
    else {
        parent[root2] = root1;
        rank[root1]++;
    }
}

int compareComponents(const void* a, const void* b) {
    const Component* compA = (const Component*)a;
    const Component* compB = (const Component*)b;
    return (compA->size > compB->size) - (compA->size < compB->size);
}

void kruskal(long long n_vertex, long long n_edge, MinHeap* heap, long long* component_count, long long* kscanned, long long* mst_weight, Component** components) {
    long long* parent = malloc(sizeof(long long) * n_vertex);
    long long* rank = calloc(n_vertex, sizeof(long long));
    long long* sizes = calloc(n_vertex, sizeof(long long));
    long long* weights = calloc(n_vertex, sizeof(long long));

    for (long long i = 0; i < n_vertex; i++) {
        parent[i] = i;
    }

    long long mst_edges = 0;
    *kscanned = 0;
    *mst_weight = 0;

    while (heap->size > 0) {
        Edge minEdge = extractMin(heap);
        (*kscanned)++;

        long long root1 = find(parent, minEdge.start);
        long long root2 = find(parent, minEdge.end);

        if (root1 != root2) {
            unionSets(parent, rank, root1, root2);
            long long new_root = find(parent, root1); 
            *mst_weight += minEdge.weight; 
            weights[new_root] = weights[root1] + weights[root2] + minEdge.weight; 
            mst_edges++;

            if (mst_edges == n_vertex - 1)
                break;
        }
    }

    *component_count = 0;
    for (long long i = 0; i < n_vertex; i++) {
        long long root = find(parent, i);
        if (sizes[root] == 0) {
            (*component_count)++;
        }
        sizes[root]++;
    }

    *components = malloc((*component_count) * sizeof(Component));
    long long idx = 0;
    for (long long i = 0; i < n_vertex; i++) {
        if (sizes[i] > 0) {
            (*components)[idx].size = sizes[i];
            (*components)[idx].weight = weights[i];
            idx++;
        }
    }

    qsort(*components, *component_count, sizeof(Component), compareComponents);

    free(parent);
    free(rank);
    free(sizes);
    free(weights);
}

int main() {
    FILE* fp1 = fopen("commands.txt", "r");
    if (fp1 == NULL) {
        perror("Command 파일 없음");
        return 1;
    }

    char* path = calloc(400, sizeof(char));
    char* input_file = calloc(200, sizeof(char));
    char* output_file = calloc(200, sizeof(char));

    if (fgets(path, 400, fp1) != NULL) path[strcspn(path, "\n")] = '\0';
    if (fgets(input_file, 200, fp1) != NULL) input_file[strcspn(input_file, "\n")] = '\0';
    if (fgets(output_file, 200, fp1) != NULL) output_file[strcspn(output_file, "\n")] = '\0';

    fclose(fp1);

    strcat(path, "\\");
    strcat(path, input_file);

    FILE* fp2 = fopen(path, "r");
    if (fp2 == NULL) {
        perror("입력 파일 열기 실패\n");
        free(path);
        free(input_file);
        free(output_file);
        return 1;
    }

    long long n_vertex, n_edge, max_weight;
    fscanf(fp2, "%lld %lld %lld", &n_vertex, &n_edge, &max_weight);
    if (n_vertex == 0 || n_edge == 0) {
        printf("작동 여부 : NO\n");
        return 1;
    }
    Edge* edges = malloc(sizeof(Edge) * n_edge);
    for (long long i = 0; i < n_edge; i++) {
        fscanf(fp2, "%lld %lld %lld", &edges[i].start, &edges[i].end, &edges[i].weight);
    }
    fclose(fp2);

    MinHeap* heap = createMinHeap(n_edge);
    buildMinHeap(heap, edges, n_edge);

    clock_t start_time = clock();

    long long component_count;
    long long kscanned = 0;
    long long mst_weight = 0;
    Component* components = NULL;

    kruskal(n_vertex, n_edge, heap, &component_count, &kscanned, &mst_weight, &components);

    clock_t end_time = clock();

    FILE* output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        perror("출력 파일 열기 실패");
        free(heap->edges);
        free(heap);
        free(edges);
        free(path);
        free(input_file);
        free(output_file);
        free(components);
        return 1;
    }

    fprintf(output_fp, "%lld\n", component_count);

    for (long long i = 0; i < component_count; i++) {
        fprintf(output_fp, "%lld %lld\n", components[i].size, components[i].weight);
    }

    fclose(output_fp);

    printf("작동 여부: YES\n");
    printf("Component Count: %lld\n", component_count);
    printf("MST Weight: %lld\n", mst_weight);
    printf("MST Scanned Edges: %lld\n", kscanned);
    printf("수행 시간: %lf 초\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    free(heap->edges);
    free(heap);
    free(edges);
    free(path);
    free(input_file);
    free(output_file);
    free(components);

    return 0;
}
