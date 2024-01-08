//
//  trash.h
//  leTeest
//
//  Created by Andrea Paris on 14.08.23.
//

#ifndef trash_h
#define trash_h
#include "linAlg.h"


weight -= nabla * lr

nabla_wT = [np.zeros(wT.shape) for wT in weightsT]
auto nabla_wT = _layers;

### Implement here
# feedforward
# Here you need to store all the activations of all the units
# by feedforward pass
a = []
activations = []
activations.append(x)
for layer in range(1, num_layers):
    a.append(np.dot(weightsT[layer-1], activations[layer-1]))
    activations.append(sigmoid(a[layer-1]))
###
// feedforward, salviamo in

# compute the gradient of error respect to output
# activations[-1] is the list of activations of the output layer
// delta = (cost).df_wrt_a(activations[-1], y)
// rmse

### Implement here
# backward pass
# Here you need to implement the backward pass to compute the
# gradient for each weight and bias
//nabla_wT[-1] = np.dot(delta, activations[-2].transpose())
nabla_wT[_len-1] = delta * _deri[_len-2].T();


for layer in range(2, num_layers):
    delta = np.dot(weightsT[-layer+1].transpose(), delta) * sigmoid_prime(a[-layer])
    nabla_wT[-layer] = np.dot(delta, activations[-layer-1].transpose())
###

for (size_t i = _len-1; i > 0; --i){
    delta = (_layer[i-1].T * delta) * sigmoid_prime(a[idx])
    nabla_wT[i] = delta * _acti[i-1].T();
}








matrix<double> l0(in.getCols(), 3);
matrix<double> l1(l0.getCols(), 5);
matrix<double> l2(l1.getCols(), 5);
matrix<double> l3(l2.getCols(), 3);
matrix<double> l4(l3.getCols(), out.getCols());

matrix<double> i0(in.getRows(), l0.getCols()); // in * l0
matrix<double> i1(i0.getRows(), l1.getCols()); // i0 * l1 -> 3x3 * 3 x 5 - > 3x5
matrix<double> i2(i1.getRows(), l2.getCols()); // i1 * l2 -> 3x5 * 5x5 -> 3 x 5
matrix<double> i3(i2.getRows(), l3.getCols()); // i2 * l3 -> 3x5 * 5 x 3 -> 3 x 3
matrix<double> i4(i3.getRows(), out.getCols()); // i3 * l4 -> 3x3 * 3x1 -> 3x1

matrix<double> layers[] = {l0, l1, l2, l3, l4};
matrix<double> inters[] = {i0, i1, i2, i3, i4};

// 0 -> reLU
// 1 -> sigmoid
// 2 -> tanh

nn<double> b(in, out, layers, inters, 5, fun);
std::cout << b.forward() << std::endl;


#endif /* trash_h */
