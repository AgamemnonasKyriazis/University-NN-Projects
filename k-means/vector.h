#ifndef __VECTOR_C__
#define __VECTOR_C__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct vector_s {
    double *vector;
    int vector_size;
};
typedef struct vector_s vector_t;

vector_t *allocatem(int vector_size);
void releasem(vector_t *v);
void copy(vector_t *v1, vector_t *v2);
void init_random_m();
vector_t *get_random_norm(int vector_size);
double get_random_norms();
void printv(vector_t *v);
vector_t *zeroes(int vector_size);
vector_t *ones(int vector_size);
double dot(vector_t *v1, vector_t *v2);
vector_t *sub(vector_t *v1, vector_t *v2);
vector_t *map(double (*function)(), vector_t *v);
vector_t *map2(double (*function)(), vector_t *v1, vector_t *v2);
vector_t *map2_s(double (*function)(), vector_t *v1, double s);
double sum(vector_t *v);
double mult(double n1, double n2);
double add(double n1, double n2);
double euclidean_distance(vector_t *v1, vector_t *v2);
int max_index(vector_t *v);

#endif