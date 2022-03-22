#include "vector.h"

#define K 3    /* number of clusters */
#define N 1200  /* number of data points */
#define P 2     /* vector size for coordinates */

struct point_s {
    vector_t *cords;
    int cluster;
};
typedef struct point_s point_t;

void print_points(point_t *points, int d);

FILE *fp;
point_t centroids[K]; /* centroids list */
point_t data[N]; /* data list */

double k_means() {
    fp = fopen("data.txt", "r");
    int i,j;
    double total_error;
    for(i = 0; i < N; i++) { /* for each point */
        data[i].cords = allocatem(P); /* allocate memory for the vector */
        data[i].cluster = 0; /* it doesn't belong to any cluster */
        fscanf(fp, "%lf,%lf\n", &data[i].cords->vector[0], &data[i].cords->vector[1]); /* read data from file */
    }
    fclose(fp);

    for(i = 0; i < K; i++) { /* for each cluster */
        centroids[i].cords = allocatem(P); /* allocate memory for the vector */
        centroids[i].cluster = i+1; /* assign cluster id */
        int r = rand() % N; /* pick a random point as cluser coordinates */
        centroids[i].cords->vector[0] = data[r].cords->vector[0];
        centroids[i].cords->vector[1] = data[r].cords->vector[1];
    }
loop:
    for(i = 0; i < N; i++) { /* for each data point */
        double min = euclidean_distance(data[i].cords, centroids[0].cords); /* calculate the euclidean distance from the first centroid */
        int min_c = 1; /* assign the id of the first centroid */  
        for(j = 0; j < K; j++) {
            double temp = euclidean_distance(data[i].cords, centroids[j].cords); /* calculate the distance from each centroid */
            if(min > temp) { /* if we find a better distance */
                min = temp; /* change min distance value */
                min_c = j+1; /* change cluser id for this point */
            }
        }
        data[i].cluster = min_c; /* assign the fina value to the data point */
    }
    int terminate = 0; /* create an integer */
    for(i = 0; i < K; i++) { /* for each centroid */
        double x1_mean = 0.0f;
        double x2_mean = 0.0f;
        int counter = 0;
        for(j = 0; j < N; j++) { /* calculate the new centroid position using mean data point coordinates */
            if(centroids[i].cluster == data[j].cluster) { /* if the data point belongs to the cluster */
                counter++;
                x1_mean = x1_mean + data[j].cords->vector[0];
                x2_mean = x2_mean + data[j].cords->vector[1];
            }
        }
        x1_mean = x1_mean/ (double)counter;
        x2_mean = x2_mean/ (double)counter;
        if(centroids[i].cords->vector[0] != x1_mean || centroids[i].cords->vector[1] != x2_mean) { /* if the data point changes position */
            terminate++; /* increase the counter */
        }
        centroids[i].cords->vector[0] = x1_mean;
        centroids[i].cords->vector[1] = x2_mean;
    }
    if(terminate > 0) { /* if any centroid had its position changed */
        goto loop; /* loop */
    }

    total_error = 0; /* calculate the clustering error for each cluster */
    /* and the total clustering error */
    for(i = 0; i < K; i++) {
        double temp = 0;
        for(j = 0; j < N; j++) {
            if(centroids[i].cluster == data[j].cluster) {
                temp += euclidean_distance(centroids[i].cords, data[j].cords);
            }
        }
        total_error += temp;
        printf("Clustering error for c = %d is c_error = %lf\n", i+1, temp);
    }
    return total_error;
}

void log_results() {
    int i,j;
    /* then write the data points and their assigned cluster to a file  for testing */ 
    fp = fopen("test.txt", "w");
    for(i = 0; i < N; i++) {
        fprintf(fp, "%lf,%lf,%d\n", data[i].cords->vector[0], data[i].cords->vector[1], data[i].cluster);
    }
    fclose(fp);
    /* also write the cluster coordinates and id's */
    fp = fopen("test2.txt", "w");
    for(i = 0; i < K; i++) {
        fprintf(fp, "%lf,%lf,%d\n", centroids[i].cords->vector[0], centroids[i].cords->vector[1], centroids[i].cluster);
    }
}

void release_points() {
    int i,j;
    /* release all allocated memory! */
    for(i = 0; i < K; i++) {
        releasem(centroids[i].cords);
    }
    
    for(i = 0; i < N; i++) {
        releasem(data[i].cords);
    }
}

int main() {
    int i,j;
    init_random_m(); /* to get random values */
    double err = 10000.0f;
    for(i = 0; i < 20; i++) {
        double temp = k_means();
        if(temp < err) {
            err = temp;
            log_results();
        }
        release_points();
    }
    printf("Total min clustering error c-total = %lf\n", err);
}

void print_points(point_t *points, int d) {
    int i, k;
    k = (d == 0)? K : N;
    for(i = 0; i < k; i++) {
        printf("cluster: %d\t", points[i].cluster); printv(points[i].cords);
    }
}