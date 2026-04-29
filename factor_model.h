#ifndef FACTOR_MODEL_H
#define FACTOR_MODEL_H

#include "linalg.h"

/*
 * Factor model:
 *  N assets, K factors
 *  B: N x K  (factor exposures)
 *  F: K x K  (factor covariance)
 *  D: N x 1  (idiosyncratic variances, diagonal of D)
 */

/* Build full covariance: Sigma = B F B^T + D */
int factor_build_covariance(const Matrix *B,
                            const Matrix *F,
                            const double *D_diag,
                            Matrix *Sigma);

/* Portfolio variance via factor model: w^T Sigma w */
double factor_portfolio_variance(const double *w,
                                 int n,
                                 const Matrix *B,
                                 const Matrix *F,
                                 const double *D_diag);

/* Factor contributions to variance: size K */
int factor_contributions(const double *w,
                         int n,
                         const Matrix *B,
                         const Matrix *F,
                         const double *D_diag,
                         double *out_contrib, /* length K */
                         double *out_total_var);

#endif /* FACTOR_MODEL_H */
