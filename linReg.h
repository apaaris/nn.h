//
//  linReg.h
//  leTeest
//
//  Created by Andrea Paris on 14.08.23.
//

#ifndef linReg_h
#define linReg_h
#include "linAlg.h"

template <class T>
class linearRegression {

private:
    
    matrix<T> _X;
    matrix<T> _Y;
    matrix<T> _W;
    
    
public:
    
    linearRegression(matrix<T>& x, matrix<T>& y,  matrix<T>& w) : _X(x), _Y(y), _W(w){}
    
    ~linearRegression(){ }
    
    void fitLS() { _W = (_X.t() * _X).inv()*_X.t()*_Y; }
    void fitRidge(const T lambda) {
        matrix _I = (_X.t() * _X).I();
        _W = (_X.t() * _X + _I * lambda).inv()*_X.t()*_Y;
        
    }
    //void fitLasso(double lambda) { _W = (_X.t() * _X).inv()*_X.t()*_Y; }
    
    matrix<T> getX() { return _X; }
    matrix<T> getY() { return _Y; }
    matrix<T> getW() { return _W; }
    
    matrix<T> predict() { return _X * _W; }
    
    T loss() { return (_X * _W - _Y).norm(); }
    
    
};



#endif /* linReg_h */
