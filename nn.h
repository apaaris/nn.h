#ifndef nn_h
#define nn_h

#include "linAlg.h"

template <class T>
class nn {
    
private:
    matrix<T> _x;
    matrix<T> _y;
    matrix<T> _res;
    
    matrix<T> * _weights;
    matrix<T> * _layers;
    matrix<T> * _deri;
    
    size_t* _arch;
    size_t _len;
    size_t* _act;
    
    
public:
        
    nn(matrix<T>& input, matrix<T>& output, matrix<T>* layers, matrix<T>* inters, size_t n, size_t* act): _x(input), _y(output), _res(output), _weights(layers), _layers(inters), _len(n), _act(act){
        
        randomize();
        
    }
    
    nn(matrix<T>& input, matrix<T>& output, size_t* arch, size_t n, size_t* act): _x(input), _y(output), _res(output), _arch(arch), _len(n), _act(act){
        
        _weights = new matrix<T>[_len]();
        for(size_t i = 0; i < _len; ++i){
            if(i == 0){
                matrix<T> tmpL(_x.getCols(), _arch[i], 0.0);
                _weights[i].setCols(tmpL.getCols());
                _weights[i].setRows(tmpL.getRows());
                _weights[i].setVal(tmpL.getVals());
            } else if (i == _len - 1){
                matrix<T> tmpL(_weights[i-1].getCols(), 1, 0.0);
                _weights[i].setCols(tmpL.getCols());
                _weights[i].setRows(tmpL.getRows());
                _weights[i].setVal(tmpL.getVals());
            } else {
                matrix<T> tmpL(_weights[i-1].getCols(), _arch[i], 0.0);
                _weights[i].setCols(tmpL.getCols());
                _weights[i].setRows(tmpL.getRows());
                _weights[i].setVal(tmpL.getVals());
            }
        }
        
        _layers = new matrix<T>[_len]();
        for(size_t i = 0; i < _len; ++i){
            if(i == 0){
                matrix<T> tmpI(1, _weights[i].getCols(), 0.0);
                _layers[i].setCols(tmpI.getCols());
                _layers[i].setRows(tmpI.getRows());
                _layers[i].setVal(tmpI.getVals());
                
            } else if (i == _len - 1){
                matrix<T> tmpI(_layers[i-1].getRows(), 1, 0.0);
                _layers[i].setCols(tmpI.getCols());
                _layers[i].setRows(tmpI.getRows());
                _layers[i].setVal(tmpI.getVals());
            } else {
                matrix<T> tmpI(_layers[i-1].getRows(), _weights[i].getCols(), 0.0);
                _layers[i].setCols(tmpI.getCols());
                _layers[i].setRows(tmpI.getRows());
                _layers[i].setVal(tmpI.getVals());
            }

        }
        
        _deri = new matrix<T>[_len]();
        for(size_t i = 0; i < _len; ++i){
            if(i == 0){
                matrix<T> tmpI(1, _weights[i].getCols(), 0.0);
                _deri[i].setCols(tmpI.getCols());
                _deri[i].setRows(tmpI.getRows());
                _deri[i].setVal(tmpI.getVals());
                
            } else if (i == _len - 1){
                matrix<T> tmpI(_layers[i-1].getRows(), 1, 0.0);
                _deri[i].setCols(tmpI.getCols());
                _deri[i].setRows(tmpI.getRows());
                _deri[i].setVal(tmpI.getVals());
            } else {
                matrix<T> tmpI(_layers[i-1].getRows(), _weights[i].getCols(), 0.0);
                _deri[i].setCols(tmpI.getCols());
                _deri[i].setRows(tmpI.getRows());
                _deri[i].setVal(tmpI.getVals());
            }

        }
        
        randomize();
        
    }
    
    static matrix<T> sigmoid(matrix<T>& mat);
    static matrix<T> sigmoid_derivative(matrix<T>& mat);
    static matrix<T> reLU(matrix<T>& mat);
    static matrix<T> linear(matrix<T>& mat);
    static matrix<T> tanh(matrix<T>& mat);
    
    ~nn(){
        
        delete[] _weights;
        delete[] _layers;
    }
    
    matrix<T> res(){ return _res;}

    void forwardPass(){
        std::cout << "Forward pass" << std::endl;
        
        for(size_t j = 0; j < _x.getRows(); ++j){
            matrix<T> pass = (_x.rowMatrix(j) * _weights[0]);
            _deri[0].setVal(pass.getVals());
            _layers[0] = acti(pass, _act[0]);
            for(size_t i = 1; i < _len; ++i){
                matrix<T> pass = (_layers[i-1] * _weights[i]);
                _deri[i].setVal(pass.getVals());
                _layers[i] = acti(pass, _act[i]);
            }
            _res.setVal(j, _layers[_len-1].getVal(0));
        }
    }
    
    T errorRMSE(){
        T rmse = 0.0;
        for(size_t i = 0; i < _y.getRows(); ++i)
            rmse += (_res.getVal(i) - _y.getVal(i))*(_res.getVal(i) - _y.getVal(i));
        rmse = std::sqrt(rmse/_y.getRows());
        std::cout << "Rmse -> " << rmse << std::endl;
        return rmse;
    }
    
    matrix<T> errorRMSEvector(){
        matrix<T> rmse(_y.getRows(), 1 ,0.0);
        for(size_t i = 0; i < _y.getRows(); ++i){
            rmse.setVal(i, std::sqrt((_res.getVal(i) - _y.getVal(i))*(_res.getVal(i) - _y.getVal(i))));
        }
        return rmse;
    }
    
    

    void backwardPass(){
        std::cout << "Backpropagating error" << std::endl;
        // son 3 layer lo faccio a mano prima
        
        // Come funziona sta merda:
        // 1. Si calcola l'rmse per il primo layer
        // 2. Si moltiplica l'errore per l'inverse sigmoid dei pesi matrix B
        // 3. I pesi vengono aggiornati W + B * lr
        // 4. Si ripete fino all'input layer.
        
        auto rmse = errorRMSE();
        auto tmp0 = _weights[3].dotF(rmse);
        auto tmp1 = sigmoid_derivative(tmp0);
        std::cout << tmp1 << std::endl;
        auto tmp2 = (_weights[2].dotF(rmse));
        auto tmp3 = sigmoid_derivative(tmp2);
        std::cout << tmp3 << std::endl;
        auto tmp4 = (_weights[1].dotF(rmse));
        auto tmp5 = sigmoid_derivative(tmp4);
        std::cout << tmp5 << std::endl;
        auto tmp6 = (_weights[0].dotF(rmse));
        auto tmp7 = sigmoid_derivative(tmp6);
        std::cout << tmp7 << std::endl;
    }
    
    matrix<T> acti(matrix<T> mat,size_t fn){
        switch (fn) {
            case 0:
                return nn::reLU(mat);
            case 1:
                return nn::sigmoid(mat);
            case 2:
                return nn::tanh(mat);
            case 3:
                return nn::linear(mat);
            default:
                return nn::linear(mat);
        }
    }
    
    void randomize(){
        for(size_t i = 0; i < _len; ++i){
            _weights[i].rand();
        }
    }
    
    void printWeights(){
        for(size_t i = 0; i < _len; ++i){
            _weights[i].info();
        }
    }
    
    void printLayers(){
        for(size_t i = 0; i < _len; ++i){
            _layers[i].info();
        }
    }
    
    friend std::ostream& operator<<(std::ostream& out, const nn&net){
        
        if (out) {
            
            for(size_t i = 0; i < net._len; ++i){
                std::cout << net._weights[i] << std::endl;
            }
        }
        
        return out;
    }
};

template <class T>
matrix<T> nn<T>::sigmoid(matrix<T>& mat){
    matrix<T> res(mat);
    //std::cout << mat << std::endl;
    for(size_t i = 0; i < res.getRows() * res.getCols(); ++i){
        T sig = (T) 1 / (1 + expf(-res.getVal(i)));
        res.setVal(i, sig);
    }
    return res;
}

template <class T>
matrix<T> nn<T>::sigmoid_derivative(matrix<T>& mat) {
    matrix<T> s = nn<T>::sigmoid(mat);
    matrix<T> ones(s.getRows(), s.getCols(), 1.0);
    matrix<T> res = s;
    for(size_t i = 0; i < s.getRows(); ++i){
        for(size_t j = 0; j < s.getCols(); ++j){
            size_t idx = i * s.getCols() + j;
            res.setVal(idx, s.getVal(idx) * (1 - s.getVal(idx)));
        }
    }
    return res;
}


template <class T>
matrix<T> nn<T>::tanh(matrix<T>& mat){
    matrix<T> res(mat);
    //std::cout << mat << std::endl;
    for(size_t i = 0; i < res.getRows() * res.getCols(); ++i){
        T tan = (expf(res.getVal(i)) - expf(-res.getVal(i))) /
                (expf(res.getVal(i)) + expf(-res.getVal(i)));
        res.setVal(i, tan);
    }
    return res;
}

template <class T>
matrix<T> nn<T>::reLU(matrix<T>& mat){
    matrix<T> res(mat);
    for(size_t i = 0; i < res.getRows() * res.getCols(); ++i){
        res.setVal(i, (0 < res.getVal(i)) ? res.getVal(i) : 0);
    }
    return res;
}

template <class T>
matrix<T> nn<T>::linear(matrix<T>& mat){ return mat;}
// forward pass: dot, sum bias, act

#endif /* nn_h */
