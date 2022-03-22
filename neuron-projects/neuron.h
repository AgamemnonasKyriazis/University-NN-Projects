#ifndef __NEURON_C__
#define __NEURON_C__
#include "vector.h"

struct neuron_s {
    vector_t *w;
    double w0;
};
typedef struct neuron_s neuron_t;

struct layer_s
{
    int layer_id;
    neuron_t *neurons;
    vector_t *x;
    vector_t *o;
    vector_t *e;
    int layer_depth;
};
typedef struct layer_s layer_t;

struct network_s {
    vector_t *layer0;
    layer_t *layers;
    int number_of_layers;
};
typedef struct network_s network_t;

network_t *init_network(int number_of_hidden_layers, int *layer_depth, int input_size);
void release_network(network_t *network);
void print_network(network_t *network);

double u(vector_t *x, vector_t *w, double w0);

double logistic(double x);
double d_logistic(double lx);

/* tanh */
double d_tanh(double lx);

double relu(double x);
double d_relu(double x);

#endif