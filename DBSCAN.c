#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 15
#define MAX_DIMENSIONS 2

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void get_neighbors(int points[][MAX_DIMENSIONS], int point_idx, int epsilon, int* neighbors, int* num_neighbors) {
    *num_neighbors = 0;
    int i;
    for (i = 0; i < MAX_POINTS; i++) {
        if (manhattan_distance(points[point_idx][0], points[point_idx][1], points[i][0], points[i][1]) <= epsilon) {
            neighbors[(*num_neighbors)++] = i;
        }
    }
}

void dbscan(int points[][MAX_DIMENSIONS], int epsilon, int min_points, int* labels, int* num_clusters) {
    int stack[MAX_POINTS],i,j;
    int stack_size = 0;
    *num_clusters = 0;

    for ( i = 0; i < MAX_POINTS; i++) {
        if (labels[i] != 0) continue;

        int neighbors[MAX_POINTS];
        int num_neighbors;
        get_neighbors(points, i, epsilon, neighbors, &num_neighbors);

        if (num_neighbors < min_points) {
            labels[i] = -1;  // Noise point
        } else {
            (*num_clusters)++;
            labels[i] = *num_clusters;  // Core point
            stack[stack_size++] = i;

            while (stack_size > 0) {
                int current = stack[--stack_size];
                get_neighbors(points, current, epsilon, neighbors, &num_neighbors);
                
                for ( j = 0; j < num_neighbors; j++) {
                    int neighbor = neighbors[j];
                    if (labels[neighbor] <= 0) {
                        if (labels[neighbor] == 0) {
                            stack[stack_size++] = neighbor;
                        }
                        labels[neighbor] = *num_clusters;
                    }
                }
            }
        }
    }
}

void print_points(int points[][MAX_DIMENSIONS], int* labels, int label_value, const char* point_type) {
    printf("%s points:\n", point_type);
    int i;
    for ( i = 0; i < MAX_POINTS; i++) {
        if (labels[i] == label_value) {
            printf("  (%d,%d)\n", points[i][0], points[i][1]);
        }
    }
    printf("\n");
}

int main() {
    int points[MAX_POINTS][MAX_DIMENSIONS] = {
        {1,1}, {1,2}, {5,7}, {3,1}, {3,3}, {6,6}, {18,4}, {18,3},
        {7,6}, {4,3}, {4,7}, {2,2}, {8,6}, {8,7}, {9,6}
    };

    int epsilon = 2,i,cluster;
    int min_points = 3;
    int labels[MAX_POINTS] = {0};
    int num_clusters;

    dbscan(points, epsilon, min_points, labels, &num_clusters);
//    scanf("%d",&i);
    // Print results
    print_points(points, labels, 1, "Core");
    print_points(points, labels, 2, "Border");
    print_points(points, labels, -1, "Noise");

    printf("Number of clusters discovered: %d\n", num_clusters);

    // Print clusters
    for ( cluster = 1; cluster <= num_clusters; cluster++) {
        printf("\nCluster %d:\n", cluster);
        for ( i = 0; i < MAX_POINTS; i++) {
            if (labels[i] == cluster) {
                printf("  (%d,%d)\n", points[i][0], points[i][1]);
            }
        }
    }

    return 0;
}
