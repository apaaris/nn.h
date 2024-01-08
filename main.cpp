#include <iostream>
#include "linAlg.h"
#include "nn.h"

int main(int argc, const char * argv[]) {
    
    std::cout << "Build complete!\n";
    
    matrix<double> in("samples/sampleIn.txt", true);
    matrix<double> out("samples/sampleOut.txt", true);
    // reLU, reLU, reLU, sigmoid
    size_t fun[] = {1,1,1,1};
    
    // layers of neurons with size
    size_t layrs[] = {2, 5, 2};
    
    nn<double> d(in, out, layrs, sizeof(layrs) / sizeof(size_t) + 1, fun); // +1 takes into account output layer
    d.forwardPass();
    d.backwardPass();
    //d.printLayers();
    //d.errorRMSE();
    std::cout << d;
    //std::cout << d.res();
    return 0;
}
