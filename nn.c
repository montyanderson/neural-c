#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double bias;
	double input;
	double output;
	double *weight;
} neuron_t;

typedef struct {
	neuron_t *neuron;
	size_t neurons;
} layer_t;

typedef struct {
	layer_t *layer;
	size_t layers;
} network_t;

network_t *network_create(size_t layers) {
	network_t *net = calloc(1, sizeof(network_t));

	net->layers = layers;

	if(net->layers > 0) {
		net->layer = calloc(net->layers, sizeof(layer_t));
	}

	return net;
}

void network_neuron_add(network_t *net, size_t layer_index, double bias) {
	layer_t *layer = &net->layer[layer_index];
	neuron_t neuron;

	neuron.bias = bias;
	neuron.input = 0;
	neuron.output = 0;

	if(layer_index > 0) {
		neuron.weight = calloc(net->layer[layer_index - 1].neurons, sizeof(double));
	}

	layer->neurons++;
	layer->neuron = realloc(layer->neuron, layer->neurons * sizeof(neuron_t));

	layer->neuron[layer->neurons - 1] = neuron;
}

void network_weight_add(network_t *net, size_t to_layer_index, size_t from_neuron_index, size_t to_neuron_index, double weight) {
	net->layer[to_layer_index].neuron[to_neuron_index].weight[from_neuron_index] = weight;
}

void network_activate(network_t *net) {
	size_t l, n, i;
	layer_t *layer, *previous_layer;
	neuron_t *neuron;

	for(l = 1; l < net->layers; l++) {
		layer = &net->layer[l];
		previous_layer = &net->layer[l - 1];

		for(n = 0; n < layer->neurons; n++) {
			neuron = &layer->neuron[n];
			neuron->input = neuron->bias;

			for(i = 0; i < previous_layer->neurons; i++) {
				neuron->input += previous_layer->neuron[i].output * neuron->weight[i];
			}

			neuron->output = 1.0 / (1.0 + exp(neuron->input));
		}
	}
}

int main() {
	network_t *net = network_create(3);

	network_neuron_add(net, 0, 0);
	network_neuron_add(net, 0, 0);
	network_neuron_add(net, 1, -2.8299873220539724);
	network_neuron_add(net, 1, -2.553442160702158);
	network_neuron_add(net, 1, -3.9735421882861974);
	network_neuron_add(net, 2, -2.882093229195121);;

	net->layer[0].neuron[0].output = 0;
	net->layer[0].neuron[1].output = 1;

	network_activate(net);

	printf("%f\n", net->layer[2].neuron[0].output);
}
