#include "linalg.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

Matrix mat_alloc(int rows, int cols) {
    Matrix M;
    M.rows = rows;
    M.cols = cols;
    M.data = (double *)malloc(rows * cols * sizeof(double));
    return M;
}

void mat_free(Matrix *M) {
    free(M->data);
    M->data = NULL;
    M->rows = 0;
    M->cols = 0;
}

void mat_zero(Matrix *M) {
    if(!M || !M -> data) return;
    memset(M->data, 0, M->rows * M->cols * sizeof(double));
}

void mat_copy(const Matrix *src, Matrix *dst) {
    if (!src || !dst) return;
    if(src->rows != dst->rows || src->cols != dst->cols) return;
    memcpy(dst->data, src->data, (size_t)src->rows * (size_t)src->cols * sizeof(double));
}

int mat_mul(const Matrix *B, const Matrix *C, Matrix *A) {
    if(!B || !C || !A) return -1;
    if(B->cols != C->rows) return -2;
    if(A->rows != B->rows || A->cols != C->cols) return -3;

    mat_zero(A);

    int r = B->rows;
    int k = B->cols;
    int c = C->cols;

    for(int i = 0; i < r; ++i) {
        for(int j = 0; j < c; ++j) {
            double sum = 0.0;
            for(int t = 0; t < k; ++t) {
                sum += mat_get(B, i, t) * mat_get(C, t, j);
            }
            mat_set(A, i, j, sum);
        }
    }
    return 0;
}

int mat_vec_mul(const Matrix *A, const double *x, double *y) {
    if(!A || !x || !y) return -1;
    if(A->rows != A->cols) return -2;

    int n = A->rows;
    for(int i = 0; i < n; ++i) {
        double sum = 0.0;
        for(int j = 0; j < n; ++j) {
            sum += mat_get(A, i, j) * x[j];
        }
        y[i] = sum;
    }
    return 0;
}

int cholesky(const Matrix *A, Matrix *L) {
    if(!A || !L) return -1;
    if(A->rows != A->cols) return -2;
    if(L->rows != A->rows || L->cols != A->cols) return -3;

    mat_zero(L);
    int n = A->rows;

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j <= i; ++j) {
            double sum = mat_get(A, i, j);
            for(int k = 0; k < j; ++k) {
                sum -= mat_get(L, i, k) * mat_get(L, j, k);
            }
            if(i == j) {
                if(sum <= 0.0) return -4; // Not positive definite
                mat_set(L, i, j, sqrt(sum));
            } else {
                double Ljj = mat_get(L, j, j);
                if(Ljj == 0.0) return -5; // Zero diagonal element
                mat_set(L, i, j, sum / mat_get(L, j, j));
            }
        }
    }
    return 0;
}

