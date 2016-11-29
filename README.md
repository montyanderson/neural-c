# neural-c
In-progress neural-network library in C.

```
$ gcc nn.c -lm && ./a.out
1.000000, 0.000000 => 0.938487 (1)
1.000000, 1.000000 => 0.080261 (1)
0.000000, 1.000000 => 0.938204 (1)
0.000000, 0.000000 => 0.066483 (1)
```

## Speed

On a simple network (2 inputs, three hidden neurons in a single layer, one output) trained to XOR, the library managed over *3278688 ops/second* on my i7 4770k, with each operation doing a complete activation of the network, e.g. getting an output from two inputs.
