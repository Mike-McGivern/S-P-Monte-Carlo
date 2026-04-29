#include "greeks.h"
#include <stdlib.h>
#include <math.h>

/* You’ll need these from your MC engine */
extern void run_monte_carlo(int num_sims,
                            const Asset *assets,
                            int n,
                            const Matrix *L,
                            double *out_portfolio_returns);

static int cmp_double(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da < db) ? -1 : (da > db);
}

double mc_var_percentile(const Asset *assets,
                         int n,
                         const Matrix *L,
                         int num_sims,
                         double p)
{
    double *rets = (double*)malloc((size_t)num_sims * sizeof(double));
    run_monte_carlo(num_sims, assets, n, L, rets);

    qsort(rets, num_sims, sizeof(double), cmp_double);
    int idx = (int)(p * (num_sims - 1));
    double var = -rets[idx];

    free(rets);
    return var;
}

/* Delta wrt mu_i: central difference on VaR(mu_i ± bump) */
void greeks_delta_mu(const Asset *assets,
                     int n,
                     const Matrix *L,
                     int num_sims,
                     double bump,
                     double p,
                     double *out_delta)
{
    Asset *tmp_plus  = (Asset*)malloc((size_t)n * sizeof(Asset));
    Asset *tmp_minus = (Asset*)malloc((size_t)n * sizeof(Asset));

    for (int i = 0; i < n; ++i) {
        /* copy base */
        for (int j = 0; j < n; ++j) {
            tmp_plus[j]  = assets[j];
            tmp_minus[j] = assets[j];
        }

        tmp_plus[i].mu  += bump;
        tmp_minus[i].mu -= bump;

        double var_plus  = mc_var_percentile(tmp_plus,  n, L, num_sims, p);
        double var_minus = mc_var_percentile(tmp_minus, n, L, num_sims, p);

        out_delta[i] = (var_plus - var_minus) / (2.0 * bump);
    }

    free(tmp_plus);
    free(tmp_minus);
}

/* Vega wrt sigma_i: central difference on VaR(sigma_i ± bump) */
void greeks_vega_sigma(const Asset *assets,
                       int n,
                       const Matrix *L,
                       int num_sims,
                       double bump,
                       double p,
                       double *out_vega)
{
    Asset *tmp_plus  = (Asset*)malloc((size_t)n * sizeof(Asset));
    Asset *tmp_minus = (Asset*)malloc((size_t)n * sizeof(Asset));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            tmp_plus[j]  = assets[j];
            tmp_minus[j] = assets[j];
        }

        tmp_plus[i].sigma  += bump;
        tmp_minus[i].sigma -= bump;

        double var_plus  = mc_var_percentile(tmp_plus,  n, L, num_sims, p);
        double var_minus = mc_var_percentile(tmp_minus, n, L, num_sims, p);

        out_vega[i] = (var_plus - var_minus) / (2.0 * bump);
    }

    free(tmp_plus);
    free(tmp_minus);
}

/* Gamma wrt mu_i: second derivative via central difference */
void greeks_gamma_mu(const Asset *assets,
                     int n,
                     const Matrix *L,
                     int num_sims,
                     double bump,
                     double p,
                     double *out_gamma)
{
    Asset *tmp_plus  = (Asset*)malloc((size_t)n * sizeof(Asset));
    Asset *tmp_minus = (Asset*)malloc((size_t)n * sizeof(Asset));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            tmp_plus[j]  = assets[j];
            tmp_minus[j] = assets[j];
        }

        tmp_plus[i].mu  += bump;
        tmp_minus[i].mu -= bump;

        double var_plus  = mc_var_percentile(tmp_plus,  n, L, num_sims, p);
        double var_minus = mc_var_percentile(tmp_minus, n, L, num_sims, p);
        double var_0     = mc_var_percentile(assets,   n, L, num_sims, p);

        out_gamma[i] = (var_plus - 2.0 * var_0 + var_minus) / (bump * bump);
    }

    free(tmp_plus);
    free(tmp_minus);
}
