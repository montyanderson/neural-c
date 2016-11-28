#include <stdio.h>
#include <stdlib.h>

typedef struct {
	double bias, input, activation;
} neuron_t;

typedef struct {
	neuron_t *neuron;
	size_t neurons;
	double *weight;
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
	neuron.activation = 0;

	layer->neurons++;
	layer->neuron = realloc(layer->neuron, layer->neurons * sizeof(neuron_t));

	layer->neuron[layer->neurons - 1] = neuron;
}

void network_weights_generate(network_t *net) {
	size_t i;

	for(i = net->layers + 1; i < net->layers; i++) {
		net->layer[i].weight = calloc(net->layer[i - 1].neurons, net->layer[i].neurons * sizeof(double));
	}
}

void network_weight_add(network_t *net, size_t to_layer_index, size_t from_neuron_index, size_t to_neuron_index, double weight) {
	net->layer[to_layer_index].weight[from_neuron_index][to_neuron_index] = weight;
}

int main() {
	network_t *net = network_create(3);

	network_neuron_add(net, 0, 0);
	network_neuron_add(net, 0, 0);
	network_neuron_add(net, 1, -2.8299873220539724);
	network_neuron_add(net, 1, -2.553442160702158);
	network_neuron_add(net, 1, -3.9735421882861974);
	network_neuron_add(net, 2, -2.882093229195121);

	network_weights_generate(net);


	printf("%lu %f\n", net->layer[1].neurons, net->layer[1].neuron[0].bias);
}
