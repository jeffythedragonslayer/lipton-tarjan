#define IT Matrix<T>

template<typename T> IT::Matrix(MatrixCR m) : rows(m.rows), cols(m.cols)
{
        uint s = size();
        data   = new T[s];
        memcpy(data, m.data, s*sizeof(T));
}

template<typename T> IT:: Matrix()                     : rows(0),    cols(0),    data(nullptr)          {}
template<typename T> IT:: Matrix(uint rows, uint cols) : rows(rows), cols(cols), data(new T[rows*cols]) {}
template<typename T> IT:: Matrix(Pt2uCR p)             : Matrix(p.x, p.y)                               {}
template<typename T> IT::~Matrix()                                                                      {delete data;}

template<typename T> void Matrix<T>::swap(Matrix& m)
{
        std::swap(rows, m.rows);
        std::swap(cols, m.cols);
        std::swap(data, m.data);
}

template<typename T> T& Matrix<T>::operator()(uint r, uint c)       {assert(r  <rows && c  <cols); return data[r*cols+c];}
template<typename T> T  Matrix<T>::operator()(uint r, uint c) const {assert(r  <rows && c  <cols); return data[r*cols+c];}
template<typename T> T& Matrix<T>::operator()(Pt2uCR p)             {assert(p.x<rows && p.y<cols); return data[p.x*cols+p.y];}
template<typename T> T  Matrix<T>::operator()(Pt2uCR p)       const {assert(p.x<rows && p.y<cols); return data[p.x*cols+p.y];}

template<typename T> IT& Matrix<T>::operator= (MatrixCR m)
{
        if( this == &m ) return self;
        assert(same_shape(m));

        uint s = size();

        delete data;
        data = new T[s];
        memcpy(data, m.data, s*sizeof(T));
        return self;
}

template<typename T> IT Matrix<T>::operator* (MatrixCR m) const
{
        assert(cols == m.rows);
        Matrix ret(rows, m.cols);
        DO_TIMES(rows, r) DO_TIMES(c, m.cols){
                ret(r, c) = 0;
                DO_TIMES(i, cols) ret(r, c) += self(r, i) * m(i, c);
        }
        return ret;
}

template<typename T> IT& Matrix<T>::operator*=(MatrixCR m)       {assert(same_shape(m));                                                                            return self = self * m;}
template<typename T> IT& Matrix<T>::operator+=(MatrixCR m)       {assert(same_shape(m));                       DO_TIMES(i, size()) data[i] += m.data[i];            return self;}
template<typename T> IT& Matrix<T>::operator-=(MatrixCR m)       {assert(same_shape(m));                       DO_TIMES(i, size()) data[i] -= m.data[i];            return self;}
template<typename T> IT& Matrix<T>::operator*=(T s)              {                                             DO_TIMES(i, size()) data[i] *= s;                    return self;}
template<typename T> IT& Matrix<T>::operator/=(T s)              {                                             DO_TIMES(i, size()) data[i] /= s;                    return self;}
template<typename T> IT  Matrix<T>::operator+ (MatrixCR m) const {assert(same_shape(m)); Matrix r(rows, cols); DO_TIMES(i, size()) r.data[i] = data[i] + m.data[i]; return r;}
template<typename T> IT  Matrix<T>::operator- (MatrixCR m) const {assert(same_shape(m)); Matrix r(rows, cols); DO_TIMES(i, size()) r.data[i] = data[i] - m.data[i]; return r;}
template<typename T> IT  Matrix<T>::operator* (T s)        const {                       Matrix r(rows, cols); DO_TIMES(i, size()) r.data[i] = data[i] * s;         return r;}
template<typename T> IT  Matrix<T>::operator/ (T s)        const {                       Matrix r(rows, cols); DO_TIMES(i, size()) r.data[i] = data[i] / s;         return r;}

template<typename T> bool Matrix<T>::operator==(MatrixCR m) const
{
        if( !same_shape(m) ) return false;
        DO_TIMES(i, size()) if( data[i] != m.data[i] ) return false;
        return true;
}

template<typename T> void Matrix<T>::resize(uint rows, uint cols)
{
        delete data;
        self.rows = rows;
        self.cols = cols;
        data = new T[size()];
}

template<typename T> void Matrix<T>::resize     (Pt2uCR dims)       {resize(dims.x, dims.y);}
template<typename T> T    Matrix<T>::det        ()            const {fail; return 0;}
template<typename T> T    Matrix<T>::trace      ()            const {assert(is_square()); T sum = 0; DO_TIMES(i, rows) sum += self(i, i); return sum;}
template<typename T> T    Matrix<T>::rank       ()            const {return -1;}
template<typename T> bool Matrix<T>::same_shape (MatrixCR m)  const {return rows == m.rows && cols == m.cols;}
template<typename T> uint Matrix<T>::size       ()            const {return rows * cols;}
template<typename T> uint Matrix<T>::num_rows   ()            const {return rows;}
template<typename T> uint Matrix<T>::num_cols   ()            const {return cols;}
template<typename T> bool Matrix<T>::is_square  ()            const {return rows == cols;}

template<typename T> bool Matrix<T>::is_identity() const
{
        if( !is_square() ) return false;
        DO_TIMES(r, rows) DO_TIMES(c, cols){
                if( r == c ) if( self(r, c) != 1 ) return false;
                else         if( self(r, c)      ) return false;
        }
        return true;
}

template<typename T> bool Matrix<T>::is_real() const
{
        DO_TIMES(r, rows) DO_TIMES(c, cols) if( !::is_real(self(r, c))) return false;
        return true;
}

template<typename T> void Matrix<T>::transpose    ()       {                      DO_TIMES(r, rows) DO_TIMES(c, cols) if( r != c ) std::swap(self(r, c), self(c, r));}
template<typename T> IT   Matrix<T>::get_transpose() const {Matrix t(cols, rows); DO_TIMES(r, rows) DO_TIMES(c, cols) t(c, r) = self(r, c); return t;}

template<typename T> void Matrix<T>::swap_rows(uint a, uint b)
{
        T* row_a = data + a*cols;
        T* row_b = data + b*cols;
        swap_ranges(row_a, row_a+cols, row_b);
}

template<typename T> void Matrix<T>::scale_row(uint r, T s)                   {DO_TIMES(c, cols) self(r, c) *= s;}
template<typename T> void Matrix<T>::add_rows (uint target, uint source, T s) {DO_TIMES(c, cols) self(target, c) += self(source, c) * s;}

template<typename T> IT Matrix<T>::get_inverse() const
{
        T d = det();
        if( !d ) throw Zerodet();
        fail;
        return self;
}

template<typename T> void Matrix<T>::invert() {self = self.get_inverse();}

template<typename T> void Matrix<T>::solve(Matrix& augmented)
{
        for( uint r=0, uint lead=0; r<rows; ++r, ++lead){
                if( lead >= columns ) return;
                uint row2 = r;
                while( !self(row2, lead) ){
                        if( rows == ++row2 ){
                                row2 = r;
                                if( columns == ++lead ) return;
                        }
                }
                swaprows(r, row2); // put r in correct place
                augmented.swaprows(r, row2);
                T scale = 1 / (self(r, lead));  // make leading nonzero # 1
                scalerow(r, scale);
                augmented.scalerow(r, scale);
                for( row2=0; row2<rows; ++row2){ // zero out other #s in this column
                        if( r == row2 ) continue;
                        T scale = -self(row2, lead);
                        addrows(row2, r, scale);
                        augmented.addrows(row2, r, scale);
                }
        }
}

template<typename T> Matrix<T> operator* (T s, const Matrix<T>& m) {Matrix r(m.rows, m.cols); DO_TIMES(i, m.size()) r.data[i] = s * m.data[i]; return r;}