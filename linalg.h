#ifndef LINALG_H
#define LINALG_H

#include <stddef.h>

/* Flat row-major matrix: size = nrows * ncols */
typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

/* Constructors / destructors */
Matrix mat_alloc(int rows, int cols);
void   mat_free(Matrix *M);

/* Accessors */
static inline double mat_get(const Matrix *M, int i, int j) {
    return M->data[i * M->cols + j];
}

static inline void mat_set(Matrix *M, int i, int j, double v) {
    M->data[i * M->cols + j] = v;
}

/* Basic ops */
void mat_zero(Matrix *M);
void mat_copy(const Matrix *src, Matrix *dst);

/* A = B * C  (all row-major, dims: B[r x k], C[k x c], A[r x c]) */
int mat_mul(const Matrix *B, const Matrix *C, Matrix *A);

/* y = A * x  (A[n x n], x[n], y[n]) */
int mat_vec_mul(const Matrix *A, const double *x, double *y);

/* Cholesky: A (SPD, n x n) -> L (lower, n x n), returns 0 on success */
int cholesky(const Matrix *A, Matrix *L);

#endif /* LINALG_H */
