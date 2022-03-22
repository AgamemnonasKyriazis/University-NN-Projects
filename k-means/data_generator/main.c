#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>


FILE *fp;
double scale;

double rand_range(double low, double high) {
    // [0 - 1]
    double random = ((double) rand() / RAND_MAX);
    return low + (random * (high - low)) ;
}

void gen_data(double low1, double high1, double low2, double high2, int count) {
    double x1, x2;
    for(int i = 0; i < count; i++) {
        x1 = rand_range(low1, high1);
        x2 = rand_range(low2, high2);
        fprintf(fp, "%lf,%lf\n", x1, x2);
    }
}

int main() {
    srand(time(NULL));
    fp = fopen("data.txt", "w");
    int i;
    double high1[] = {1.25f, 0.5f, 0.5f, 2.0f, 2.0f, 0.8f, 0.8f, 1.4f, 1.4f, 2.0f};
    double low1[] =  {0.75f, 0.0f, 0.0f, 1.5f, 1.5f, 0.6f, 0.6f, 1.2f, 1.2f, 0.0f};

    double high2[] = {1.25f, 0.5f, 2.0f, 0.5f, 2.0f, 0.4f, 2.0f, 0.4f, 2.0f, 2.0f};
    double low2[] =  {0.75f, 0.0f, 1.5f, 0.0f, 1.5f, 0.0f, 1.6f, 0.0f, 1.6f, 0.0f};

    int count[] = {150, 150, 150, 150, 150, 75, 75, 75, 75, 150};

    for(i = 0; i < 10; i++)
        gen_data(low1[i], high1[i], low2[i], high2[i], count[i]);

    fclose(fp);
}