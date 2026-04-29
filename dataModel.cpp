#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>

std::unordered_map<std::string, double> load_ticker_value(const std::string& path);

struct Asset {
    std::string ticker;
    double mu;
    double sigma;
    double weight;
};

using Matrix = std::vector<std::vector<double>>;


Matrix load_corr_matrix(const std::string& path) {
    std::cout << "Loading correlation matrix from: " << path << std::endl;
    std::ifstream file(path);
    Matrix M;
    std::string line;
    
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::getline(ss, cell, ',');
        std::vector<double> row;
        while(std::getline(ss, cell, ',')) {
        if (cell.empty()) {
            std::cerr << "Empty cell encountered in correlation matrix\n";
            throw std::runtime_error("Empty cell in correlation matrix");
        }
        try {
            row.push_back(std::stod(cell));
        } catch (const std::exception& e) {
            std::cerr << "Failed to parse cell: '" << cell << "'\n";
            throw;
            }
        }
        M.push_back(std::move(row));
    }
    return M;
}



/*std::unordered_map<std::string, double> load_mu_sigma(const std::string& path) {
    std::ifstream file(path);
    std::unordered_map<std::string, double> data;
    std::string line;

    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker;
        std::getline(ss, ticker, ',');
        std::string mu_str;
        std::getline(ss, mu_str, ',');
        std::string sigma_str;
        std::getline(ss, sigma_str, ',');

        data[ticker] = std::stod(mu_str);
    }
    return data;
} */

std::unordered_map<std::string, double> load_sigma(const std::string& path) {
    std::ifstream file(path);
    std::unordered_map<std::string, double> data;

    std::string line;

    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker;
        std::getline(ss, ticker, ',');
        std::string sigma_str;
        std::getline(ss, sigma_str, ',');

        data[ticker] = std::stod(sigma_str);
    }
    return data;
}

std::unordered_map<std::string, double> load_weights(const std::string& path) {
    std::ifstream file(path);
    std::unordered_map<std::string, double> data;
    std::string line;

    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker;
        std::getline(ss, ticker, ',');
        std::string weight_str;
        std::getline(ss, weight_str, ',');

        data[ticker] = std::stod(weight_str);
    }
    return data;
}
/*
std::vector<Asset> load_assets(const std::string& mu_sigma_path, const std::string& weights_path) {
    auto mu_data = load_mu_sigma(mu_sigma_path);
    auto sigma_data = load_sigma(mu_sigma_path);
    auto weights_data = load_weights(weights_path);

    std::vector<Asset> assets;
    for (const auto& [ticker, mu] : mu_data) {
        double sigma = sigma_data[ticker];
        double weight = weights_data[ticker];
        assets.push_back({ticker, mu, sigma, weight});
    }
    return assets;
} */

/* std::vector<Asset> load_assets(const std::string& consituents_csv, const std::string& momentum_csv, const std::string& volatility_csv) {
    auto mu_map = load_ticker_value(momentum_csv);
    auto sig_map = load_ticker_value(volatility_csv);
    std::ifstream file(consituents_csv);
    std::string line;
    std::getline(file, line);

    std::vector<Asset> assets;
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker, weight_str;
        std::getline(ss, ticker, ','); // Ticker
        std::getline(ss, weight_str, ','); // Weight **** IF THERE IS AN ERROR IT IS THIS LINE MISMATCHING DATA SHAPE ****
        // python might want to make a ticker weight table so it can be parsed to C++ correctly

        if(ticker.empty() || weight_str.empty()) continue;

        Asset a;
        a.ticker = ticker;
        a.weight = std::stod(weight_str);
        a.mu = mu_map.at(ticker);
        a.sigma = sig_map.at(ticker);
        assets.push_back(std::move(a));
    }

    return assets;
} */


//THIS ASSET LOADER LEAVES OUT MARKET CAPITALIZATION WEIGHTS, FIX PYTHON TO INCLUDE THOSE WEIGHTS AND USE THE ABOVE METHOD WHEN FINISHING PROJECT.....
std::vector<Asset> load_assets(const std::string& constituents_csv,
                               const std::string& momentum_csv,
                               const std::string& volatility_csv)
{
    auto mu_map  = load_ticker_value(momentum_csv);
    auto sig_map = load_ticker_value(volatility_csv);

    std::ifstream file(constituents_csv);
    std::string line;
    std::getline(file, line); // skip header

    std::unordered_set<std::string> unique_tickers;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, ticker;

        std::getline(ss, date, ',');    // Date
        std::getline(ss, ticker, ',');  // Ticker

        if (!ticker.empty())
            unique_tickers.insert(ticker);
    }

    std::vector<std::string> tickers(unique_tickers.begin(), unique_tickers.end());
    double equal_weight = 1.0 / tickers.size();

    std::vector<Asset> assets;
    assets.reserve(tickers.size());

    for (auto& t : tickers) {
        Asset a;
        a.ticker = t;
        a.weight = equal_weight;
        a.mu     = mu_map.at(t);
        a.sigma  = sig_map.at(t);
        assets.push_back(a);
    }

    return assets;
}


std::unordered_map<std::string, double> load_ticker_value(const std::string& path) {
    std::ifstream file(path);
    std::unordered_map<std::string, double> m;
    std::string line;

    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker;
        std::getline(ss, ticker, ',');
        std::string value_str;
        std::getline(ss, value_str, ',');
        
        if(!ticker.empty() && !value_str.empty()) {
        m[ticker] = std::stod(value_str);
        }
    }
    return m;
}

//From correlation matrix, mu, sigma, and weights, we can compute the covariance matrix and run the cholesky algorithm

Matrix build_covariance(const Matrix& corr, const std::vector<Asset>& assets) {
    int n = assets.size();
    Matrix cov(n, std::vector<double>(n, 0.0));

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cov[i][j] = corr[i][j] * assets[i].sigma * assets[j].sigma;
        }
    }
    return cov;
}

Matrix cholesky(const Matrix& A) {
    int n = A.size();
    Matrix L(n, std::vector<double>(n, 0.0));

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j <= i; ++j) {
            double sum = A[i][j];
            for(int k = 0; k < j; ++k) {
                sum -= L[i][k] * L[j][k];
            }
            if(i == j) {
                L[i][j] = std::sqrt(std::max(sum, 0.0)); // Ensure non-negative for sqrt
            } else {
                L[i][j] = (A[i][j] - sum) / L[j][j];
            }
        }
    }
    return L;
}
//the above 2 functions run in O(n^3) time, which is the bottleneck of the entire algorithm. For large n, this will be very slow. We will need to optimize this part of the code to achieve better performance.
//below is the core loop that will need to be optimized via Assembly, GPU acceleration or FPGA circuity..... runtime complexity will be quartic or worse



std::mt19937 rng(std::random_device{}());
std::normal_distribution<double> dist(0.0, 1.0);

std::vector<double> simulate_once(const std::vector<Asset>& assets, const Matrix& L) {
    int n = assets.size();
    std::vector<double> Z(n), shock(n);

    for(int i = 0; i < n; ++i)
        Z[i] = dist(rng);
    
    for(int i = 0; i < n; ++i) {
        double s = 0.0;
        for(int j = 0; j <= i; ++j)
            s += L[i][j] * Z[j];
        shock[i] = s;
    }
    std::vector<double> returns(n);
    for(int i = 0; i < n; ++i)
        returns[i] = assets[i].mu + assets[i].sigma * shock[i];
    return returns;
}

double portfolio_return(const std::vector<Asset>& assets, const std::vector<double>& sim_returns) {
    double r = 0.0;
    for(int i = 0; i < assets.size(); ++i)
        r += sim_returns[i] * assets[i].weight;
    return r;
}

std::vector<double> run_monte_carlo(int num_sims, const std::vector<Asset>& assets, const Matrix& L) {
    std::vector<double> results;
    results.reserve(num_sims);

    for(int s = 0; s < num_sims; ++s) {
        auto sim = simulate_once(assets, L);
        results.push_back(portfolio_return(assets, sim));
    }
    return results;
}
double percentile(std::vector<double>&data, double p) {
    std::sort(data.begin(), data.end());
    double idx = p * (data.size() - 1);
    return data[static_cast<int>(idx)];
}
int main() {
    std::cout << "Program started\n";
    std::vector<Asset> assets;
try {
    assets = load_assets(
        "D:\\VB_learning\\Reports\\CSV_Exports\\sp500_constituents.csv",
        "D:\\VB_learning\\Reports\\CSV_Exports\\sp500_momentum.csv",
        "D:\\VB_learning\\Reports\\CSV_Exports\\sp500_volatility.csv"
    );
} catch (const std::exception& e) {
    std::cerr << "ERROR in load_assets: " << e.what() << "\n";
    return 1;
}

    std::cout << "Loaded " << assets.size() << " assets\n";
    std::cout << "loading correlation matrix\n";
    Matrix corr;
    try {
        corr = load_corr_matrix("D:\\VB_learning\\Reports\\CSV_Exports\\full_index_corr_matrix.csv");
    } catch (const std::exception& e) {
        std::cout << "error loading correlation matrix caught" << std::endl;
        std::cerr << "ERROR in load_corr_matrix: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Loaded correlation matrix of size " << corr.size() << "x" << corr[0].size() << std::endl;
    Matrix cov = build_covariance(corr, assets);
    std::cout << "Built covariance matrix\n";
    Matrix L = cholesky(cov);
    std::cout << "Computed Cholesky decomposition\n";

    int num_sims = 100000;
    auto portfolio_returns = run_monte_carlo(num_sims, assets, L);
    //compute VaR/CVaR, percentiles and other risk metrics from portfolio_returns
    std::cout << "Simulated " << portfolio_returns.size() << " portfolio returns\n";

    double var95 = -percentile(portfolio_returns, 0.05);
    double var99 = -percentile(portfolio_returns, 0.01);

    double cvar95 = -(std::accumulate(portfolio_returns.begin(), portfolio_returns.begin() + portfolio_returns.size() * 0.05, 0.0) / (portfolio_returns.size() * 0.05));

    std::cout << "95% VaR: " << var95 << std::endl;
    std::cout << "99% VaR: " << var99 << std::endl;
    std::cout << "95% CVaR: " << cvar95 << std::endl;

    return 0;
}

