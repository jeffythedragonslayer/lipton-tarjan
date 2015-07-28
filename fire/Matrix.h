#pragma once
#include "style.h"
#include "Vec.h"

template<typename T> struct Matrix
{
        typedef Matrix const& MatrixCR;

                      Matrix             ();
                      Matrix             (MatrixCR);
                      Matrix             (uint, uint);
                      Matrix             (Pt2uCR);
                      ~Matrix            ();

        void          swap               (Matrix&);

        T&            operator()         (uint r, uint c);
        T             operator()         (uint r, uint c) const;
        T&            operator()         (Pt2uCR);
        T             operator()         (Pt2uCR)         const;

        Matrix&       operator=          (MatrixCR);
        Matrix&       operator*=         (MatrixCR);
        Matrix&       operator+=         (MatrixCR);
        Matrix&       operator-=         (MatrixCR);
        Matrix&       operator*=         (T);
        Matrix&       operator/=         (T);

        Matrix        operator+          ()               const;
        Matrix        operator-          ()               const;

        Matrix        operator*          (MatrixCR)       const;
        Matrix        operator+          (MatrixCR)       const;
        Matrix        operator-          (MatrixCR)       const;
        Matrix        operator*          (T)              const;
        Matrix        operator/          (T)              const;
        friend Matrix operator*          (T, MatrixCR);

        bool          operator==         (MatrixCR)       const;

        void          resize             (uint rows, uint cols);
        void          resize             (Pt2uCR dims);

        void          solve(Matrix& augmented);
        T             det                ()               const;
        T             rank               ()               const;
        T             trace              ()               const;

        bool          same_shape         (MatrixCR)       const;
        
        uint          size               ()               const;
        uint          num_rows           ()               const;
        uint          num_cols           ()               const;
        bool          is_real            ()               const;
        bool          is_square          ()               const;
        bool          is_diagonal        ()               const;
        bool          is_scalar          ()               const;
        bool          is_identity        ()               const;
        bool          is_lower_triangular()               const;
        bool          is_upper_triangular()               const;
        bool          is_symmetric       ()               const;
        bool          is_skew_symmetric  ()               const;
        
        void          transpose          ();
        Matrix        get_transpose      ()               const;

        void          invert             ();
        Matrix        get_inverse        ()               const;

        void          swap_rows          (uint a, uint b);
        void          scale_row          (uint r, T scale);
        void          add_rows           (uint target, uint source, T scale);

        uint rows, cols;
        T*   data;
};

#include "Matrix.inl"