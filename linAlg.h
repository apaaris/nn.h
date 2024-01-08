#ifndef linAlg_h
#define linAlg_h

#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>


template <class T>
class matrix {

private:
    
    size_t _rows, _cols;
    T* _val;
    
public:
    
    matrix() {}
    
    matrix(size_t rows, size_t cols){
        
        _rows = rows;
        _cols = cols;
        _val = new T[_rows * _cols]();
        
        for(size_t i = 0; i < _rows * _cols; ++i){
            _val[i] = 0;
        }
        
    }
    
    matrix(std::string filename, bool header){
        
        
        
        _rows = rowsCsv(filename, header);
        
        _cols = featCsv(filename, _rows, header);
        _val = new T[_rows * _cols]();
        
        parseCsv(filename, _rows, _cols, header);
        
        
        
    }
    
    matrix(size_t rows, size_t cols, T init){
        
        _rows = rows;
        _cols = cols;
        _val = new T[_rows * _cols];
        for(size_t i = 0; i < _rows * _cols; ++i){
            _val[i] = init;
        }
        
    }
    
    matrix(size_t rows, size_t cols, T* init){
        
        _rows = rows;
        _cols = cols;
        _val = init;

    }
    
    matrix(size_t size){
        
        _rows = size;
        _cols = size;
        _val = new T[_rows*_cols]();
        
    }
    
    
    matrix(const matrix &m) : _rows(m._rows), _cols(m._cols){

        _val = new T[_rows * _cols];
        for (size_t i = 0; i < _rows * _cols; ++i) {
            _val[i] = m._val[i];
        }
        
    }
    
    ~matrix(){
        
        //delete[] _val;
    }
    
    matrix load(T* values){
        _val = values;
    }
    
    matrix sum(const matrix &m){
        
        if (_rows != m._rows or _cols != m._cols) {
            throw std::invalid_argument("Matrix dimensions do not match");
        }
        
        matrix ret(_rows, _cols);
        
        for (size_t i = 0; i < _rows * _cols; ++i) {
            ret._val[i] = _val[i] + m._val[i];
        }
        
        return ret;
        
    }
    
    matrix sub(const matrix &m){
        
        if (_rows != m._rows or _cols != m._cols) {
            throw std::invalid_argument("Matrix dimensions do not match");
        }
        matrix ret(_rows, _cols);
        
        for (size_t i = 0; i < _rows * _cols; ++i) {
            ret._val[i] = _val[i] - m._val[i];
        }
        
        return ret;
    }
    
    matrix t(){
        
        matrix t(_cols,_rows);
        for(size_t i = 0; i < _rows; i++){
            for(size_t j = 0; j < _cols; j++){
                t._val[j * t._cols + i] = _val[i * _cols + j];
            }
        }
        return t;
    }
    
    matrix I(){
        
        for(size_t i = 0; i < _rows; i++){
            for(size_t j = 0; j < _rows; j++){
                if (i == j){
                    _val[i * _cols + j] = 1.0;
                } else {
                    _val[i * _cols + j] = 0.0;
                }
            }
        }
        return (*this);
    }
    
    void setVal(size_t idx, T val){
        _val[idx] = val; 
    }
    
    void setVal(T* vals){
        _val = vals;
    }
    
   void setRows(size_t rows){
        _rows = rows;
    }
    
    void setCols(size_t cols){
        _cols = cols;
    }
    
    T getVal(size_t idx){
        return _val[idx];
    }
    
    T* getVal(){
        return _val;
    }
    
    size_t rowsCsv(std::string filename, bool header){
        
        size_t rows = 0;
        std::string line;
        std::ifstream file;
        
        file.open(filename);
        
        if (file.fail()){
            file.clear();
            throw std::invalid_argument("File not found.");
        }
        
        
        if(header){
            std::getline(file, line);
        }
        
        while (std::getline(file, line)) {
            ++rows;
        }
        
        file.close();
        file.open(filename);
                
        
        return rows;
        
    }
    
    size_t featCsv(std::string filename, size_t rows, bool header){
        
        std::string line;
        std::ifstream file;
        
        file.open(filename);
        
        size_t entries = 0;
        
        if(header){
            std::getline(file, line);
        }
        
        while(std::getline(file,line, ',')){
            ++entries;
        }
        
        
        if (!file && line.empty()){
            std::cout << "done, if you are reading this you might want to double check your dataset." << std::endl;
        }
            
        file.close();
        if (rows == 1){
            return entries / rows;
        }
        return (entries + 2) / rows;
        
    }
    
    void parseCsv(std::string filename, size_t rows, size_t features, bool header){
        
        std::string line;
        std::ifstream file;
                
        file.open(filename);
        
        if(header){
            std::getline(file, line);
        }
        
        for(size_t i = 0; i < rows; ++i){

            getline(file, line);
            if(file.bad() || file.eof()){
                break;
            }

            std::stringstream lineStream(line);

            std::string cell;
            std::string::size_type sz;


            for(int j = 0; j < features; ++j){
                std::getline(lineStream, cell, ',');
                if (!cell.empty()){
                    this -> _val[i * features + j] = std::stod(cell,&sz);
                }
            }
        }


        
    }
    
    matrix rowMatrix(size_t row){
        matrix tmp(1, _cols);
        for(size_t i = 0; i < _cols; ++i){
            tmp._val[i] = _val[row * _cols + i];
        }
        return tmp;
    }
    
    matrix subMatrix(const size_t row, const size_t column) const {
        
        if (row >= _rows || column >= _cols){
            
            throw std::out_of_range("incorrect function arguments");
        }
        
        matrix res(_rows - 1, _cols - 1);

        size_t subRow = 0;
        size_t subCol = 0;

        for (size_t i = 0; i < _rows; ++i){
            
            if (i == row){
                --subRow;
            }

            for (size_t j = 0; j < _cols; ++j){
               
                if (j == column){
                    --subCol;
                }

                if (i != row && j != column){
                    
                    res._val[subRow * res._rows + subCol] = _val[i * _cols + j];
                }
                
                ++subCol;
            }

            subCol = 0;
            ++subRow;
        }

        return res;
    }
    
    T norm(){
        T total = 0.0;
        for(size_t i = 0; i < _rows * _cols; ++i){
            total += _val[i] * _val[i];
        }
        total /= _rows * _cols;
        return std::sqrt(total);
    }
    
    size_t getRows(){
        return _rows;
    }
    
    size_t getCols(){
        return _cols;
    }
    
    T* getVals(){
        return _val;
    }
    
    T minor(const size_t row, const size_t column) const {
        
        return subMatrix(row, column).det();
    }
    
    T algoC(const size_t row, const size_t column) const {
        
        return (((row + column) % 2 == 0) ? 1 : -1) * minor(row, column);
    }
    
    T det() const {
        
        if (_rows != _cols){
            throw std::out_of_range("matrix is not square");
        }
        
        if (_rows == 1){
        
            return _val[0];
            
        } else {
            T det = T();

            for (size_t i = 0; i < _rows; i++){
                det += _val[i * _cols + 0] * algoC(i, 0);
            }

            return det;
        }
    }
    
    matrix inv() const {
        
        T det = this->det();

        if (det == T()){
       
            throw std::out_of_range("matrix is not invertible");
        }
        
        if (this->_rows == 1 && this->_cols == 1){
            matrix res(1, 1, 1 / det);
            return res;
        }
        
        matrix res(_cols, _rows);

        for (size_t i = 0; i < _rows; ++i){
            for (size_t j = 0; j < _cols; ++j){
                
                res._val[i * res._cols + j] = algoC(j, i);
            }
        }

        res = res / det;
        
        return res;
    }
    
    matrix dotF(const T a){
        
        matrix ret(_rows, _cols);
        
        for(size_t i = 0; i < _rows*_cols; ++i) {
            ret._val[i] = _val[i] * a;
        }
        
        return ret;
    }
    
    matrix divF(const T a){
        
        matrix ret(_rows, _cols);
        
        for(size_t i = 0; i < _rows*_cols; ++i) {
            ret._val[i] = _val[i] / a;
        }
        
        return ret;
    }
    
    matrix dot(const matrix &m){
        
        if(_cols != m._rows) {
            throw std::invalid_argument("Matrix dimensions do not match");
        }
                
        matrix ret(_rows, m._cols);
        
        for(size_t i = 0; i < _rows; ++i) {
            for(size_t j = 0; j < m._cols; ++j) {
                ret._val[i * ret._cols + j] = 0.0;
                for(size_t k = 0; k < _cols; ++k) {
                    ret._val[i * ret._cols + j] += _val[i * _cols + k] * m._val[k * m._cols + j];
                }
            }
        }
        return ret;
    }
    
    matrix dotV(const matrix &m){
        if (_cols != m._cols && _rows != m._rows && m._cols != 1){
            throw std::invalid_argument("Not a valid scalar product");
        }
        T res = 0.0;
        for(size_t i = 0; i < _rows; ++i){
            res += _val[i] * m._val[i];
        }
        matrix resM(1, 1, res);
        return resM;
        
    }
    
    void rand(){
        
        srand((uint32_t)time(NULL));
        for(size_t i = 0; i < _rows * _cols; ++i){
            _val[i] = std::rand() / (T) RAND_MAX;
        }
    }
    
    matrix operator= (const matrix &m){
        
        if (this != &m) {
            if (_rows != m._rows or _cols != m._cols) {
                throw std::out_of_range("Cannot assign matrix with different dimensions");
            }
            setCols(m._cols);
            setRows(m._rows);
            
            for (size_t i = 0; i < _rows * _cols; ++i) {
                setVal(i, m._val[i]);
            }
        }

        return *this;
    }
    
    matrix operator* (const matrix& m){
        if(_cols == m._cols && _rows == m._rows && m._cols == 1){
            return dotV(m);
        }
        return dot(m);}
    
    matrix operator* (T a){ return dotF(a);}
    
    //matrix operator*= (const matrix& m){ return dot(m);}
    
    matrix operator/ (T a){ return divF(a);}
    
    //matrix operator/= (T a){ return divF(a);}
    
    matrix operator+ (const matrix& m){ return sum(m);}
    
    matrix operator- (const matrix& m){ return sub(m);}
    
    void info(){
        
        std::cout << "-------------\n";
        std::cout << "Type -> " << typeid(_val[0]).name() << "\n";
        std::cout << "Rows -> " << _rows << "\n";
        std::cout << "Cols -> " << _cols << "\n";
        std::cout << "-------------\n";
        std::cout << *this;
        std::cout << "-------------\n";
        
        
    }
    
    friend std::ostream& operator<<(std::ostream& out, const matrix&m){
        
        if (out) {
            out << std::fixed << std::setprecision(3);
            for (int i = 0; i < m._rows; ++i) {
                for (int j = 0; j < m._cols; ++j) {
                    out << (j > 0 ? " " : "[") << std::setw(4);
                    out << m._val[i * m._cols + j];
                 }
                 out << "]\n";
            }
        }
        
        return out;
    }
    
};


#endif /* linAlg_h */
