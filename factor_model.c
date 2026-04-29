#include "factor_model.h"
#include <stdlib.h>

/* Sigma = B F B^T + D */
int factor_build_covariance(const Matrix *B,
                            const Matrix *F,
                            const double *D_diag,
                            Matrix *Sigma)
{
    if (!B || !F || !D_diag || !Sigma) return -1;
    int N = B->rows;
    int K = B->cols;
    if (F->rows != K || F->cols != K) return -2;
    if (Sigma->rows != N || Sigma->cols != N) return -3;

    Matrix BF  = mat_alloc(N, K);
    Matrix BFBt = mat_alloc(N, N);

    /* BF = B * F */
    if (mat_mul(B, F, &BF) != 0) {
        mat_free(&BF);
        mat_free(&BFBt);
        return -4;
    }

    /* BFBt = BF * B^T
       Implement B^T on the fly: (BF)[i,k] * B[j,k]
    */
    mat_zero(&BFBt);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < K; ++k) {
                double bfik = mat_get(&BF, i, k);
                double bjk  = mat_get(B, j, k); /* B[j,k] */
                sum += bfik * bjk;
            }
            mat_set(&BFBt, i, j, sum);
        }
    }

    /* Sigma = BFBt + D (diagonal) */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double v = mat_get(&BFBt, i, j);
            if (i == j) v += D_diag[i];
            mat_set(Sigma, i, j, v);
        }
    }

    mat_free(&BF);
    mat_free(&BFBt);
    return 0;
}

/* w^T Sigma w via factor decomposition:
   Var = w^T B F B^T w + w^T D w
*/
double factor_portfolio_variance(const double *w,
                                 int N,
                                 const Matrix *B,
                                 const Matrix *F,
                                 const double *D_diag)
{
    int K = B->cols;

    /* y = B^T w  (K x 1) */
    double *y = (double*)calloc(K, sizeof(double));
    for (int k = 0; k < K; ++k) {
        double s = 0.0;
        for (int i = 0; i < N; ++i) {
            s += mat_get(B, i, k) * w[i];
        }
        y[k] = s;
    }

    /* z = F y  (K x 1) */
    double *z = (double*)calloc(K, sizeof(double));
    for (int k = 0; k < K; ++k) {
        double s = 0.0;
        for (int j = 0; j < K; ++j) {
            s += mat_get(F, k, j) * y[j];
        }
        z[k] = s;
    }

    /* factor variance = y^T z */
    double var_factor = 0.0;
    for (int k = 0; k < K; ++k)
        var_factor += y[k] * z[k];

    /* idiosyncratic variance = sum_i w_i^2 * D_i */
    double var_idio = 0.0;
    for (int i = 0; i < N; ++i)
        var_idio += w[i] * w[i] * D_diag[i];

    free(y);
    free(z);

    return var_factor + var_idio;
}

/* Factor contributions: contribution_k = (B_k w)^2 * F_kk in spirit,
   but we do it via y = B^T w and F.
*/
int factor_contributions(const double *w,
                         int N,
                         const Matrix *B,
                         const Matrix *F,
                         const double *D_diag,
                         double *out_contrib,
                         double *out_total_var)
{
    if (!w || !B || !F || !D_diag || !out_contrib || !out_total_var) return -1;
    int K = B->cols;

    /* y = B^T w */
    double *y = (double*)calloc(K, sizeof(double));
    for (int k = 0; k < K; ++k) {
        double s = 0.0;
        for (int i = 0; i < N; ++i) {
            s += mat_get(B, i, k) * w[i];
        }
        y[k] = s;
    }

    /* z = F y */
    double *z = (double*)calloc(K, sizeof(double));
    for (int k = 0; k < K; ++k) {
        double s = 0.0;
        for (int j = 0; j < K; ++j) {
            s += mat_get(F, k, j) * y[j];
        }
        z[k] = s;
    }

    /* factor contributions: y_k * z_k */
    double var_factor = 0.0;
    for (int k = 0; k < K; ++k) {
        out_contrib[k] = y[k] * z[k];
        var_factor += out_contrib[k];
    }

    /* idiosyncratic variance */
    double var_idio = 0.0;
    for (int i = 0; i < N; ++i)
        var_idio += w[i] * w[i] * D_diag[i];

    *out_total_var = var_factor + var_idio;

    free(y);
    free(z);
    return 0;
}
