#pragma once

struct BandedMatrix
{
         BandedMatrix(uint size, uint numLBands, uint numUBands);
         BandedMatrix(BandedMatrixCR);
        ~BandedMatrix();

        BandedMatrix& operator=(BandedMatrixCR); // Assignment

        uint   get_size     ()     const; // Member access
        uint   GetnumLBands ()     const;
        uint   GetnumUBands ()     const;
        real*  GetDBand     ();
        realCP GetDBand     ()     const;
        uint   GetLBandMax  (uint) const;  // LBand(i):  0 <= index < LBandMax
        real*  GetLBand     (uint);
        realCP GetLBand     (uint) const;
        uint   GetUBandMax  (uint) const;  // UBand(i):  0 <= index < UBandMax
        real*  GetUBand     (uint);
        realCP GetUBand     (uint) const;

        real& operator()    (uint r, uint c);
        real  operator()    (uint r, uint c) const;

        void set_zero       (); // Convenience functions for initializing matrices
        void set_identity   ();

        bool cholesky_factor(); // Factor the square banded matrix A into A = L*L^T, where L is a lower-triangular matrix (L^T is an upper-triangular matrix).  This is an LU decomposition that allows for stable inversion of A to solve A*X = B.  The return val is 'true' iff the factorizing is successful (L is invertible).  If successful, A contains the Cholesky factorization (L in the lower-triangular part of A and/ L^T in the upper-triangular part of A).
        bool solve_system   (real* bVector); // Solve the linear system A*X = B, where A is an NxN banded matrix and B is an Nx1 vector.  The unknown X is also Nx1.  The input to this function is B.  The output X is computed and stored in B.  The return val is 'true' iff the system has a solution.  The matrix A and the vector B are both modified by this function.  If successful, A contains the Cholesky factorization (L in the lower-triangular part of A and L^T in the upper-triangular part of A).
        bool solve_system   (real** bMatrix, uint numBColumns); // Solve the linear system A*X = B, where A is an NxN banded matrix and B is an NxM matrix.  The unknown X is also NxM.  The input to this function is B.  The output X is computed and stored in B.  The return val is 'true' iff the system has a solution.  The matrix A and the vector B are both modified by this function.  If successful, A contains the Cholesky factorization (L in the lower-triangular part of A and L^T in the upper-triangular part of A).

private:
        bool solve_lower    (real* dataVector)             const; // linear system is L*U*X = B, where A = L*U and U = L^T,  Reduce this to U*X = L^{-1}*B.  The return val is 'true' iff the operation is successful
        bool solve_upper    (real* dataVector)             const; // linear system is U*X = L^{-1}*B.  Reduce this to X = U^{-1}*L^{-1}*B.  The return val is 'true' iff the operation is successful
        bool solve_lower    (real** dataMatrix, uint cols) const; // linear system is L*U*X = B, where A = L*U and U = L^T,  Reduce this to U*X = L^{-1}*B.  The return val is 'true' iff the operation is successful
        bool solve_upper    (real** dataMatrix, uint cols) const; // linear system is U*X = L^{-1}*B.  Reduce this to X = U^{-1}*L^{-1}*B.  The return val is 'true' iff the operation is successful

        void allocate       ();
        void deallocate     ();

        uint   size, num_LBands, num_UBands;
        real*  DBand;
        real** LBand;
        real** UBand;
};