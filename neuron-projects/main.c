#include "neuron.h"
#include "vector.h"

enum FUNCTION{TANH = 0, RELU = 1, LOGI = 2};
typedef double (*g)(double);

/* input size */
#define d   2
/* number of categories */
#define K   4
/* number of neurons at H1 (hidden layer 1) */
#define H1  16
/* number of neurons at H2 */
#define H2  16
/* number of neurons at H3 (if needed) */
#define H3  16
/* activation function to use at hidden layers */
#define F   TANH
/* activation function to use at the outer layer H+1 */
#define O_F TANH
/* batch size (must be divisor of N) */
#define B   400
/* epoch size */
#define N   4000
/* number of hidden layers */
#define CH  3
/* learning rate */
#define ETA 0.1
/* max accepted error */
#define ERR 0.01
/* min error difference between 2 epochs */
#define ERM 0.000001

int depth_per_layer[] = {H1,H2,H3,K};   /* amount of neurons per layer (edit this to change the order of layers) */
g activation_functions[3] = {&tanh, &relu, &logistic};          /* list of activation functions as pointers */
g d_activation_functions[3] = {&d_tanh, &d_relu, &d_logistic};  /* list of activation function's derivatives as pointers */

void read_data_file(int offset);

void init_categories();
void release_categories();

double train(vector_t **epoch_data, int *category_per_data);
void forward_pass(vector_t *input_vector, vector_t *output_vector);
void backprop(vector_t *input_vector, vector_t *expected_output);

double loss(vector_t *actual_output, vector_t *expected_output);
void d_loss(vector_t *actual_output, vector_t *expected_output);

double eta = ETA;                       /* learning rate */
int L = CH;       /* number of hidden layers */
int batch_size = B;                     /* batch size */

network_t *network;     /* network stucture global pointer */
vector_t **categories;  /* one-hot table for categories */
vector_t **epoch_data;  /* epoch data */
int *category_per_data; /* category per data */

void test_and_close() {
    int i;
    FILE *fp = fopen("test.txt", "w"); /* empty the txt file */
    vector_t *output_vector = allocatem(K);
    read_data_file(N);
    for(i = 0; i < N; i++) {
        forward_pass(epoch_data[i], output_vector);
        fprintf(fp, "%lf,%lf,%d\n", epoch_data[i]->vector[0], epoch_data[i]->vector[1], max_index(output_vector)+1);
    }
    fclose(fp);
    /* release all allocated memory! */
    for(i = 0; i < N; i++) {
        releasem(epoch_data[i]);
    }
    free(epoch_data);
    free(category_per_data);
    release_network(network);
    release_categories(K);
    releasem(output_vector);
}

int main() {
    init_random_m(); /* enable random values */
    init_categories(K); /* create one-hot table for categories */
    network = init_network(L, depth_per_layer, d);
    FILE *fp1 = fopen("bob.txt", "w");
    /* train the network with an epoch worth of data */
    double e0;
    double e_old = 100.0f;
    int epoch = 0;
    read_data_file(0);
    loop:    
        e0 = train(epoch_data, category_per_data);
        printf("epoch: %d, error: %lf\n", epoch, e0);
        fprintf(fp1, "%lf\n", e0);
        if((ERM >= fabs(e0 - e_old) || e0 <= ERR) && epoch >= 700) /* if the error increases or passes the threshhold */
            goto exit; /* then exit */
        e_old = e0; /* update old error value */
        epoch++;    /* increase epoch counter */
        goto loop;  /* loop */
    exit:
    fclose(fp1);
    test_and_close();
}

double train(vector_t **epoch_data, int *category_per_data) {
    int i, j, L = network->number_of_layers-1;;
    double error = 0;
    vector_t *output_vector = allocatem(K);

    for(i = 0; i <= N-batch_size; i+=batch_size) {

        /* reset all deltas */
        for(int p = 0; p < network->number_of_layers; p++) {
            for(int q = 0; q < network->layers[p].e->vector_size; q++) {
                network->layers[p].e->vector[q] = 0.0f;
            }
        }

        for(j = 0; j < batch_size; j++) { /* for every input in the batch data set */

            forward_pass(epoch_data[i+j], output_vector); /* forward pass the data through the network */
        
            backprop(epoch_data[i+j], categories[category_per_data[i+j] - 1]); /* the backpropagate the error and adjust the weights */

            error += loss(output_vector, categories[category_per_data[i+j] - 1]); /* add the mean square error created for the selected batch size */
            /* the final result is the epoch error */
        }

    }
    /* release all allocated memory! */
    releasem(output_vector);
    return error/(double)N;
}

void forward_pass(vector_t *input_vector, vector_t *output_vector) {
    int i, j; /* iterate over each neuron in every layer */
    copy(network->layer0, input_vector); /* set layer0 inputs */
    for(i = 0; i < network->number_of_layers; i++) { /* for every layer */
        for(j = 0; j < network->layers[i].layer_depth; j++) { /* iterate over every neuron */
            if(i == network->number_of_layers-1) {
                network->layers[i].o->vector[j] = (*activation_functions[O_F])(u(network->layers[i].x, network->layers[i].neurons[j].w, network->layers[i].neurons[j].w0));
            }else {
                network->layers[i].o->vector[j] = (*activation_functions[F])(u(network->layers[i].x, network->layers[i].neurons[j].w, network->layers[i].neurons[j].w0));
            }
            /* and calculate the dot product
               then pass it through the desired activation function to check if the neuron fires or not! */
        }
    }
    copy(output_vector, network->layers[network->number_of_layers-1].o);
}

void backprop(vector_t *input_vector, vector_t *expected_output) {
    int i, j, L = network->number_of_layers-1;
    double xi;
    vector_t wji;
    int q;
    int n;

    d_loss(network->layers[L].o, expected_output); /* calculate delta for the outer H+1 layer */
    for(i = 0; i < network->layers[L].layer_depth; i++) { /* for each neuron in L=H+1 */
        for(j = 0; j < network->layers[L].neurons[i].w->vector_size; j++) { /* for each weight in this neuron */
            xi = network->layers[L].x->vector[j]; /* the xi that the wi was multiplied with */
            network->layers[L].neurons[i].w->vector[j] -= eta * network->layers[L].e->vector[i] * xi;
            network->layers[L].neurons[i].w0 -= eta * network->layers[L].e->vector[i]; /* xi for w0 (bias) is 1 */
        }
    }

    for(i = L-1; i >= 0; i--) { /* for each hidden layer */
        for(j = 0; j < network->layers[i].layer_depth; j++) { /* for each neuron in it */
            
            vector_t *wji = allocatem(network->layers[i+1].layer_depth);
            for(int n = 0; n < network->layers[i+1].layer_depth; n++) { /* neuron indexer for L = h+1 */
                wji->vector[n] = network->layers[i+1].neurons[n].w->vector[j]; /* get the n-th weight from each neuron */
            }
            network->layers[i].e->vector[j] = (*d_activation_functions[F])(network->layers[i].o->vector[j]) * dot(wji, network->layers[i+1].e);
            releasem(wji);

            for(int q = 0; q < network->layers[i].neurons[j].w->vector_size; q++) { /* for each weight in this neuron */
                xi = network->layers[i].x->vector[q];
                network->layers[i].neurons[j].w->vector[q] -= eta * network->layers[i].e->vector[j] * xi; /* xi * delta */
                network->layers[i].neurons[j].w0 -= eta * network->layers[i].e->vector[j]; /* xi for w0 (bias) is 1 */
            }
        }
    }
}

double loss(vector_t *actual_output, vector_t *expected_output) {
    vector_t *diff = sub(actual_output, expected_output);
    double c;
    map2_s(pow, diff, 2.0f);
    c = sum(diff)/2.0f;
    releasem(diff);
    return c;
}

void d_loss(vector_t *actual_output, vector_t *expected_output) {
    int i;
    vector_t *diff = sub(actual_output, expected_output);
    for(i = 0; i < diff->vector_size; i++) {
        diff->vector[i] *= (*d_activation_functions[O_F])(actual_output->vector[i]); /* (y_hat - y) * g'(y_hat) */
        network->layers[network->number_of_layers-1].e->vector[i] += diff->vector[i];
    }
    for(i = 0; i < network->layers[network->number_of_layers-1].e->vector_size; i++) {
        network->layers[network->number_of_layers-1].e->vector[i] /= (double)batch_size;
    }
    releasem(diff);
}

void init_categories() { /* initialize categories as one-hot vectors, for example c1 = {1,0,0,0}. c2 = {0,1,0,0} etc. */
    int i;
    categories = (vector_t **)malloc(sizeof(vector_t *) * K);
    for(i = 0; i < K; i++) {
        categories[i] = zeroes(K);
        categories[i]->vector[i] = 1;
    }
}

void release_categories() {
    int i;
    for(i = 0; i < K; i++) {
        releasem(categories[i]);
    }
    free(categories);
}

void read_data_file(int offset) {
    epoch_data = (vector_t **)malloc(sizeof(vector_t *) * N);
    category_per_data = (int *)malloc(sizeof(int) * N);
    int i;
    for(i = 0; i < N; i++) {
        epoch_data[i] = zeroes(d);
    }

    double x1, x2;
    int category;

    FILE *fp = fopen("data.txt", "r");
    for(i = 0; i < offset; i++)
        fscanf(fp, "%lf,%lf,%d\n", &x1, &x2, &category);

    i = 0;
    while(fscanf(fp, "%lf,%lf,%d\n", &x1, &x2, &category) != EOF && i < N) {
        epoch_data[i]->vector[0] = x1;
        epoch_data[i]->vector[1] = x2;
        category_per_data[i] = category;
        i++;
    }
    fclose(fp);
}