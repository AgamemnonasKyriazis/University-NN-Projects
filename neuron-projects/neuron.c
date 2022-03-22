#include "neuron.h"

network_t *init_network(int number_of_hidden_layers, int *layer_depth, int input_size) {
    int i; /* layer indexer */
    int j; /* neuron indexer */
    int total_layers = number_of_hidden_layers+1; // calculate the total number of layers
    network_t *network = (network_t *)malloc(sizeof(network_t)); // the network is an array of layers
    network->number_of_layers = total_layers; // set the number of layers
    network->layers = (layer_t *)malloc(sizeof(layer_t)*total_layers); // initialize the array
    
    network->layer0 = allocatem(input_size); /* this is the input layer for the network */

    for(i = 0; i < total_layers; i++) {
        
        network->layers[i].layer_depth = layer_depth[i];
        network->layers[i].layer_id = i+1;
        
        /* set the input of neurons on layer 0 or connect the previous output with the current input vector */
        network->layers[i].x = i == 0? network->layer0 : network->layers[i-1].o;
        
        network->layers[i].o = zeroes(layer_depth[i]);
        network->layers[i].e = zeroes(layer_depth[i]);
        network->layers[i].neurons = (neuron_t *)malloc(sizeof(neuron_t)*layer_depth[i]);

        for(j = 0; j < layer_depth[i]; j++) {
            network->layers[i].neurons[j].w = get_random_norm(network->layers[i].x->vector_size); // and set the weights vector accordingly
            network->layers[i].neurons[j].w0 = get_random_norms();
        }
    }

    return network; // finaly return a pointer to the whole structure
}

void release_network(network_t *network) {
    int i, j; // we again iterate over every layer and each neuron inside it
    releasem(network->layers[0].x); network->layers[0].x = NULL; // firstly release the input vector of the first layer *we can't access it later* 
    for(i = network->number_of_layers-1; i >= 0; i--) { // then for each layer starting from the output layer
        releasem(network->layers[i].o); network->layers[i].o = NULL; // release the output vector *the n-th input vector is pointed by the n-th -1 output vector
        releasem(network->layers[i].e); network->layers[i].e = NULL; // release the error vector
        // so we free two vectors at once!*
        for(j = 0; j < network->layers[i].layer_depth; j++) { // for each neuron inside the layer
            releasem(network->layers[i].neurons[j].w); network->layers[i].neurons[j].w = NULL; // release the weights vector
        }
        free(network->layers[i].neurons); network->layers[i].neurons = NULL; // now release the whole neurons array
    }
    free(network->layers); // free the layers array
    network->layers = NULL;
    free(network); // and finally free the entire network pointer
    network = NULL; // NO DANGLING POINTERS! :)
}

void print_network(network_t *network) {
    int i, j;
    printv(network->layer0);
    for(i = 0; i < network->number_of_layers; i++) {
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        for(j = 0; j < network->layers[i].layer_depth; j++) {
            printf("w%d\n", j); printv(network->layers[i].neurons[j].w);
        }
        printf("\n");
        printf("o%d\n", network->layers[i].layer_id); printv(network->layers[i].o);
        printf("\n");
    }
}

double u(vector_t *x, vector_t *w, double w0) {
    return dot(x, w) + w0;
}

double logistic(double x) {
    return 1/ (1 + exp(-x));
}

double d_logistic(double lx) {
    return lx * (1 - lx); //where lx = logistic(x)
}

double d_tanh(double lx) { // where lx = tanh(x)
    return 1 - pow(lx, 2.0f); // 1 - tanh(x)^2
}

double relu(double x) {
    return x > 0? x : 0; // return max(x,0)
}

double d_relu(double x) {
    return x > 0? 1 : 0; // derivative of f(x) = x is 1
}