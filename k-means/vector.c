#include "vector.h"

void init_random_m() {
    srand(time(NULL));
}

double get_random_norms() {
    return (double)(sin(rand()));
}

vector_t *get_random_norm(int vector_size) {
    vector_t *vector = NULL;
    int i, j;
    vector = allocatem(vector_size);
    for(i = 0; i < vector_size; i++) {
        vector->vector[i] = (double)(sin(rand()));
    }
    return vector;
}

vector_t *allocatem(int vector_size) {
    int i;
    vector_t *vector = (vector_t *)malloc(sizeof(vector_t));
    vector->vector = (double *)malloc(sizeof(double)*vector_size);
    vector->vector_size = vector_size;
    return vector;
}

void releasem(vector_t *v) {
    free(v->vector);
    free(v);
}

void copy(vector_t *v1, vector_t *v2) {
    int i;
    if(v1->vector_size != v2->vector_size)
        return;
    for(i = 0; i < v1->vector_size; i++) {
        v1->vector[i] = v2->vector[i];
    }
}

void printv(vector_t *vector) {
    int i;
    for(i = 0; i < vector->vector_size; i = i+1) {
            printf("|%lf|", vector->vector[i]);
    }
    printf("\n");
}

vector_t *zeroes(int vector_size) {
    int i;
    vector_t *vector = allocatem(vector_size);
    for(i = 0; i < vector_size; i++) {
        vector->vector[i] = 0.0f;
    }
    return vector;
}

vector_t *ones(int vector_size) {
    int i;
    vector_t *vector = allocatem(vector_size);
    for(i = 0; i < vector_size; i++) {
        vector->vector[i] = 1.0f;
    }
    return vector;
}

double dot(vector_t *v1, vector_t *v2) {
    double dot_product = 0.0f;
    for(int i = 0; i < v1->vector_size; i++) {
        dot_product += v1->vector[i]*v2->vector[i];
    }
    return dot_product;
}

vector_t *sub(vector_t *v1, vector_t *v2) {
    int i;
    vector_t *v_r = NULL;
    if(v1->vector_size != v2->vector_size) {
        printf("Vector size miss-match\n");
        exit(1);
    }
    v_r = zeroes(v1->vector_size);
    for(i = 0; i < v1->vector_size; i++) {
        v_r->vector[i] = v1->vector[i] - v2->vector[i];
    }
    return v_r;
}

vector_t *map(double (*function)(), vector_t *v) {
    int i;
    for(i = 0; i < v->vector_size; i++) {
        v->vector[i] = (function)(v->vector[i]);
    }
    return v;
}

vector_t *map2(double (*function)(), vector_t *v1, vector_t *v2) {
    int i;
    for(i = 0; i < v1->vector_size; i++) {
        v1->vector[i] = (function)(v1->vector[i], v2->vector[i]);
    }
    return v1;
}

vector_t *map2_s(double (*function)(), vector_t *v1, double s) {
    int i;
    for(i = 0; i < v1->vector_size; i++) {
        v1->vector[i] = (function)(v1->vector[i], s);
    }
    return v1;
}

double sum(vector_t *v) {
    int i;
    double s = 0;
    for(i = 0; i < v->vector_size; i++) {
        s += v->vector[i];
    }
    return s;
}

double mult(double n1, double n2) {
    return n1*n2;
}

double add(double n1, double n2) {
    return n1+n2;
}

int max_index(vector_t *v) {
    int i = 0;
    double max = v->vector[0];
    int max_i = 0;
    for(i = 0; i < v->vector_size; i++) {
        if (v->vector[i] > max) {
            max = v->vector[i];
            max_i = i;
        }
    }
    return max_i;
}

double euclidean_distance(vector_t *v1, vector_t *v2) {
    double distance = 0;
    int i;
    for(i = 0; i < v1->vector_size; i++) {
        distance += pow(v1->vector[i] - v2->vector[i], 2.0f);
    }
    return sqrt(distance);
}