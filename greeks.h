#ifndef GREEKS_H
#define GREEKS_H

#include "linalg.h"

/* Must match your C-side Asset definition */
typedef struct {
    char   ticker[16];
    double mu;
    double sigma;
    double weight;
} Asset;

/* Core: run MC and return 95% VaR (or any p you like) */
double mc_var_percentile(const Asset *assets,
                         int n,
                         const Matrix *L,
                         int num_sims,
                         double p);

/* Delta wrt mu (per-asset): d VaR / d mu_i via bump-and-revalue */
void greeks_delta_mu(const Asset *assets,
                     int n,
                     const Matrix *L,
                     int num_sims,
                     double bump,
                     double p,
                     double *out_delta); /* length n */

/* Vega wrt sigma (per-asset): d VaR / d sigma_i */
void greeks_vega_sigma(const Asset *assets,
                       int n,
                       const Matrix *L,
                       int num_sims,
                       double bump,
                       double p,
                       double *out_vega); /* length n */

/* Optional: Gamma wrt mu (per-asset): d^2 VaR / d mu_i^2 */
void greeks_gamma_mu(const Asset *assets,
                     int n,
                     const Matrix *L,
                     int num_sims,
                     double bump,
                     double p,
                     double *out_gamma); /* length n */

#endif /* GREEKS_H */
