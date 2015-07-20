#include "stdafx.h"
#include "BandedMatrix.h"

BandedMatrix::BandedMatrix(uint size, uint lBands, uint uBands)
{
        assert(size && lBands < size && uBands < size);
        self.size = size;
        num_LBands = lBands;
        num_UBands = uBands;
        allocate();
}

BandedMatrix::BandedMatrix(BandedMatrixCR mat)
{
        DBand = nullptr;
        LBand = UBand = nullptr;
        self  = mat;
}

BandedMatrix::~BandedMatrix() {deallocate();}

BandedMatrix& BandedMatrix::operator=(BandedMatrixCR m)
{
        deallocate();
        size       = m.size;
        num_LBands = m.num_LBands;
        num_UBands = m.num_UBands;
        allocate();
        size_t numBytes = size*sizeof(real);
        memcpy(DBand, m.DBand, numBytes);
        for( uint i = 0; i < num_LBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memcpy(LBand[i], m.LBand[i], numBytes);
        for( uint i = 0; i < num_UBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memcpy(UBand[i], m.UBand[i], numBytes);
        return self;
}

uint   BandedMatrix::get_size    () const {return size;}
uint   BandedMatrix::GetnumLBands() const {return num_LBands;}
uint   BandedMatrix::GetnumUBands() const {return num_UBands;}
real*  BandedMatrix::GetDBand    ()       {return DBand;}
realCP BandedMatrix::GetDBand    () const {return DBand;}

uint   BandedMatrix::GetLBandMax(uint i) const {            assert(i < num_LBands); return size - 1 - i; }
real*  BandedMatrix::GetLBand   (uint i)       {if( LBand ){assert(i < num_LBands); return LBand[i]; } return 0; }
realCP BandedMatrix::GetLBand   (uint i) const {if( LBand ){assert(i < num_LBands); return LBand[i]; } return 0; }
uint   BandedMatrix::GetUBandMax(uint i) const {            assert(i < num_UBands); return size - 1 - i;}
real*  BandedMatrix::GetUBand   (uint i)       {if( UBand ){assert(i < num_UBands); return UBand[i];} return 0;}
realCP BandedMatrix::GetUBand   (uint i) const {if( UBand ){assert(i < num_UBands); return UBand[i];} return 0;}

real& BandedMatrix::operator() (uint r, uint column)
{
        assert(r < size && column < size);
        int band = column - r;
        if(   band >  0 ) if( --band < num_UBands && r < size - 1 - band ) return UBand[band][r];
        elif( band >= 0 ) return DBand[r];
        band = -band;
        if( --band < num_LBands && column < size - 1 - band ) return LBand[band][column];
        static real blah = 0;
        return blah;
}

real BandedMatrix::operator() (uint r, uint column) const
{
        assert(r < size && column < size);
        int band = column - r;
        if( band > 0 ){
            if( --band < num_UBands && r < size - 1 - band ) return UBand[band][r];
        } elif( band >= 0 ) return DBand[r];
        band = -band;
        if( --band < num_LBands && column < size - 1 - band ) return LBand[band][column];
        return 0;
}

void BandedMatrix::set_zero()
{
        assert(size > 0);
        size_t numBytes = size*sizeof(real);
        memset(DBand, 0, numBytes);
        for( uint i = 0; i < num_LBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memset(LBand[i], 0, numBytes); 
        for( uint i = 0; i < num_UBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memset(UBand[i], 0, numBytes);
}

void BandedMatrix::set_identity()
{
        assert(size > 0);
        for( uint i = 0; i < size; ++i ) DBand[i] = 1;
        size_t numBytes;
        for( uint i = 0; i < num_LBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memset(LBand[i], 0, numBytes); 
        for( uint i = 0; i < num_UBands; ++i ) numBytes = (size - 1 - i)*sizeof(real), memset(UBand[i], 0, numBytes);
}

bool BandedMatrix::cholesky_factor()
{
        assert(num_LBands == num_UBands);
        if( num_LBands != num_UBands ) return false;
        int sizeM1 = size - 1;
        int k, kMax;
        for (int i = 0; i < size; ++i) {
                int jMin = i - num_LBands;
                if( jMin < 0 ) jMin = 0;
                int j;
                for( j = jMin; j < i; ++j ){
                        kMax = j + num_LBands;
                        if( kMax > sizeM1 ) kMax = sizeM1;
                        for( k = i; k <= kMax; ++k ) self(k,i) -= self(i,j)*self(k,j);
                }
                kMax = j + num_LBands;
                if( kMax > sizeM1 ) kMax = sizeM1;
                for( k = 0; k < i; ++k ) self(k,i) = self(i,k);
                real diagonal = self(i,i);
                if( diagonal <= 0 ) return false;
                real invSqrt = invsqrt(diagonal);
                for( k = i; k <= kMax; ++k ) self(k,i) *= invSqrt;
        }
        return true;
}

bool BandedMatrix::solve_system(real* bVector)                    {return cholesky_factor() && solve_lower(bVector)              && solve_upper(bVector);}
bool BandedMatrix::solve_system(real** bMatrix, uint numBColumns) {return cholesky_factor() && solve_lower(bMatrix, numBColumns) && solve_upper(bMatrix, numBColumns);}

bool BandedMatrix::solve_lower(real* dataVector) const
{
        for( uint r = 0; r < size; ++r ){
                real lowerRR = self(r,r);
                if( abs(lowerRR) < EP ) return false;
                for( uint column = 0; column < r; ++column ){
                        real lowerRC = self(r,column);
                        dataVector[r] -= lowerRC*dataVector[column];
                }
                dataVector[r] /= lowerRR;
        }
        return true;
}

bool BandedMatrix::solve_upper(real* dataVector) const
{
        for( int r = size - 1; r >= 0; --r ){
                real upperRR = self(r,r);
                if( abs(upperRR) < EP ) return false;
                for( uint column = r + 1; column < size; ++column ){
                        real upperRC = self(r,column);
                        dataVector[r] -= upperRC*dataVector[column];
                }
                dataVector[r] /= upperRR;
        }
        return true;
}

bool BandedMatrix::solve_lower(real** dataMatrix, uint numColumns) const
{
        for( uint r = 0; r < size; ++r ){
                real lowerRR = self(r,r);
                if( abs(lowerRR) < EP ) return false;
                int bCol;
                for( uint column = 0; column < r; ++column ){
                        real lowerRC = self(r,column);
                        for( bCol = 0; bCol < numColumns; ++bCol ) dataMatrix[r][bCol] -= lowerRC*dataMatrix[column][bCol];
                }
                real inverse = 1./lowerRR;
                for( bCol = 0; bCol < numColumns; ++bCol ) dataMatrix[r][bCol] *= inverse;
        }
        return true;
}

bool BandedMatrix::solve_upper(real** dataMatrix, uint numColumns) const
{
        for( int r = size - 1; r >= 0; --r ){
                real upperRR = self(r,r);
                if( abs(upperRR) < EP ) return false;
                int bCol;
                for( uint column = r + 1; column < size; ++column ){
                        real upperRC = self(r,column);
                        for( bCol = 0; bCol < numColumns; ++bCol ) dataMatrix[r][bCol] -= upperRC*dataMatrix[column][bCol];
                }
                real inverse = 1./upperRR;
                for( bCol = 0; bCol < numColumns; ++bCol ) dataMatrix[r][bCol] *= inverse;
        }
        return true;
}

void BandedMatrix::allocate()
{
        // assert:  size, mnumLBands, mnumUBands already set
        // assert:  mDBand, mLBand, mUBand all null
        DBand = new real[size];
        size_t numBytes = size*sizeof(real);
        memset(DBand, 0, numBytes);
        LBand = num_LBands > 0 ? new real*[num_LBands] : nullptr;
        UBand = num_UBands > 0 ? new real*[num_UBands] : nullptr;
        for( uint i = 0; i < num_LBands; ++i ){int lower_size = size - 1 - i; LBand[i] = new real[lower_size]; memset(LBand[i], 0, lower_size*sizeof(real));} 
        for( uint i = 0; i < num_UBands; ++i ){int upper_size = size - 1 - i; UBand[i] = new real[upper_size]; memset(UBand[i], 0, upper_size*sizeof(real));}
}

 void BandedMatrix::deallocate()
{
        delete [] DBand;
        if( LBand ){ for( uint i = 0; i < num_LBands; ++i ) delete [] LBand[i]; delete [] LBand; LBand = 0;}
        if( UBand ){ for( uint i = 0; i < num_UBands; ++i ) delete [] UBand[i]; delete [] UBand; UBand = 0;}
}
