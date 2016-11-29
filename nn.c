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

network_t *network_create() {
	return calloc(1, sizeof(network_t));
}

int network_layer_add(network_t *net, size_t neurons) {
	size_t i;
	size_t previous_layer_neurons;
	layer_t *layer;

	net->layers++;

	if(!(net->layer = realloc(net->layer, net->layers * sizeof(layer_t)))) {
		return 0;
	}

	layer = &net->layer[net->layers - 1];

	layer->neurons = neurons;

	if(!(layer->neuron = calloc(neurons, sizeof(neuron_t)))) {
		return 0;
	}

	if(net->layers > 1) {
		previous_layer_neurons = net->layer[net->layers - 2].neurons;

		for(i = 0; i < neurons; i++) {
			if(!(layer->neuron[i].weight = calloc(previous_layer_neurons, sizeof(double)))) {
				return 0;
			}
		}
	}

	return 1;
}

neuron_t *network_neuron_get(network_t *net, size_t layer_index, size_t neuron_index) {
	return &net->layer[layer_index].neuron[neuron_index];
}

void network_weight_set(network_t *net, size_t to_layer_index, size_t from_neuron_index, size_t to_neuron_index, double weight) {
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

			neuron->output = 1.0 / (1.0 + exp(0 - neuron->input));
			//neuron->output = neuron->input / (1.0 + abs(neuron->input));
		}
	}
}

void network_train(network_t *net) {
	neuron_t *neuron;

	neuron = &net->layer[0].neuron[0];
	printf("%f\n", neuron->bias);
}

int main() {
	network_t *net = network_create();

	network_layer_add(net, 2);
	network_layer_add(net, 3);
	network_layer_add(net, 1);

	network_neuron_get(net, 0, 0)->bias = 0;
	network_neuron_get(net, 0, 1)->bias = 0;
	network_neuron_get(net, 0, 2)->bias = 0;
	network_neuron_get(net, 1, 0)->bias = -2.8299873220539724;
	network_neuron_get(net, 1, 1)->bias = -2.553442160702158;
	network_neuron_get(net, 1, 2)->bias = -3.9735421882861974;
	network_neuron_get(net, 2, 0)->bias = -2.882093229195121;

	/* layer 0->1 */
	network_weight_set(net, 1, 0, 0, 1.8536219987667468);
	network_weight_set(net, 1, 0, 1, 6.4371950810352505);
	network_weight_set(net, 1, 0, 2, 2.6948043355090183);
	network_weight_set(net, 1, 1, 0, 1.9806931670574952);
	network_weight_set(net, 1, 1, 1, 6.4246236041469595);
	network_weight_set(net, 1, 1, 2, 2.5756603225575634);
	/* layer 1->2 */
	network_weight_set(net, 2, 0, 0, -4.473585881440039);
	network_weight_set(net, 2, 1, 0, 8.255999612040616);
	network_weight_set(net, 2, 2, 0, -5.7787873318156295);

	network_neuron_get(net, 0, 0)->output = 0;
	network_neuron_get(net, 0, 1)->output = 0;

	network_activate(net);

	network_train(net);

	size_t i;

	double inputs[4][2] = {
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
		{ 0, 0 }
	};

	double results[4] = {
		1,
		0,
		1,
		0
	};

	double output, predicted;

	for(i = 0; i < 4; i++) {
		network_neuron_get(net, 0, 0)->output = inputs[i][0];
		network_neuron_get(net, 0, 1)->output = inputs[i][1];
		network_activate(net);

		predicted = results[i];
		output = network_neuron_get(net, 2, 0)->output;

		printf("%f, %f => %f (%d)\n", inputs[i][0], inputs[i][1], output, round(output) == predicted);
	}
}
