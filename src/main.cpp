#include <algorithm>
#include <bits/stdc++.h>
#include <numeric>
#include <optional>
#include <cstring>

using namespace std;

/* Data types */
using i = int;
using i8 = int8_t;
using ui8 = uint8_t;
using ui64 = uint64_t;
using ll = long long;
using ull = unsigned long long;
using c = char;
using cchar = const char*;
using s = string;
using d = double;
using ld = long double;

/* Data structures */
template <typename T> using v = vector<T>;
template <typename T> using m = vector<vector<T>>;
template <typename A, typename B> using p = pair<A, B>;
template <typename A, typename B> using vp = vector<pair<A, B>>;

/* Debug & output helpers */
#define PRINT_FILE_MODE 1
#define PRINT_CONSOLE_MODE 0

template <typename T>
void print_msg(std::optional<cchar> name,
               const T& value,
               std::ofstream& fout,
               std::optional<c> endline_char = '\n') {
#if PRINT_CONSOLE_MODE || PRINT_FILE_MODE
    const c sep = endline_char.value_or('\n');

    auto write_to = [&](std::ostream& os) {
        if (name.has_value()) {
            os << *name << " = " << value << sep;
        } else {
            os << value << sep;
        }
    };

#if PRINT_CONSOLE_MODE
    write_to(std::cout);
#endif

#if PRINT_FILE_MODE
    if (fout.is_open()) {
        write_to(fout);
    }
#if !PRINT_CONSOLE_MODE
    else {
        write_to(std::cout);
    }
#endif
#endif
#else
    (void)name;
    (void)value;
    (void)fout;
    (void)endline_char;
#endif
}

#ifdef LOCAL_DEBUG
#define DBG_PRINT(fout, x)        print_msg(#x, (x), (fout))
#define DBG_VAL_SEP(fout, x, sep) print_msg(std::nullopt, (x), (fout), (sep))
#else
#define DBG_PRINT(fout, x)        ((void)0)
#define DBG_VAL_SEP(fout, x, sep) ((void)0)
#endif

#define FAST_IO do { \
    ios_base::sync_with_stdio(false); \
    cin.tie(nullptr); \
} while (0)

namespace Modular_Arithmetics {
    const ll MOD = 100000007LL;

    ll norm_mod(ll x, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll result = x % mod_val;
        return result < 0 ? result + mod_val : result;
    }

    ll mod_add_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        a = norm_mod(a, mod_val);
        b = norm_mod(b, mod_val);
        return a >= mod_val - b ? a - (mod_val - b) : a + b;
    }

    ll mod_add_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll result = 0;
        for (ll value : A) {
            result = mod_add_2(result, value, mod_val);
        }
        return result;
    }

    ll mod_subtract_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        a = norm_mod(a, mod_val);
        b = norm_mod(b, mod_val);
        return a >= b ? a - b : mod_val - (b - a);
    }

    ll mod_subtract_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0 || A.empty()) return 0;
        ll result = norm_mod(A[0], mod_val);
        for (size_t idx = 1; idx < A.size(); ++idx) {
            result = mod_subtract_2(result, A[idx], mod_val);
        }
        return result;
    }

    ll mod_multiply_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        a = norm_mod(a, mod_val);
        b = norm_mod(b, mod_val);
        return static_cast<ll>((__int128)a * b % mod_val);
    }

    ll mod_multiply_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll result = 1 % mod_val;
        for (ll value : A) {
            result = mod_multiply_2(result, value, mod_val);
        }
        return result;
    }

    ll gcdExtended(ll a, ll b, ll& x, ll& y) {
        if (a == 0) {
            x = 0;
            y = 1;
            return b;
        }

        ll x1, y1;
        ll gcd = gcdExtended(b % a, a, x1, y1);
        x = y1 - (b / a) * x1;
        y = x1;
        return gcd;
    }

    ll modInverse(ll b, ll mod_val) {
        if (mod_val <= 0) return -1;

        __int128 old_r = norm_mod(b, mod_val);
        __int128 r = mod_val;
        __int128 old_s = 1;
        __int128 s = 0;

        while (r != 0) {
            __int128 quotient = old_r / r;

            __int128 next_r = old_r - quotient * r;
            old_r = r;
            r = next_r;

            __int128 next_s = old_s - quotient * s;
            old_s = s;
            s = next_s;
        }

        if (old_r != 1) return -1;

        old_s %= mod_val;
        if (old_s < 0) old_s += mod_val;
        return static_cast<ll>(old_s);
    }

    ll mod_divide_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return -1;
        a = norm_mod(a, mod_val);
        ll inv = modInverse(b, mod_val);
        if (inv == -1) return -1;
        return mod_multiply_2(a, inv, mod_val);
    }

    ll mod_factorial(ll a, ll mod_val) {
        if (mod_val <= 0 || a < 0) return 0;
        if (mod_val == 1 || a >= mod_val) return 0;

        ll result = 1 % mod_val;
        for (ll k = 2; k <= a; ++k) {
            result = mod_multiply_2(result, k, mod_val);
        }
        return result;
    }

    ll mod_exponent(ll a, ll b, ll mod_val) {
        if (mod_val <= 0 || b < 0) return 0;
        if (mod_val == 1) return 0;

        ll result = 1 % mod_val;
        ll base = norm_mod(a, mod_val);

        while (b > 0) {
            if (b & 1LL) {
                result = mod_multiply_2(result, base, mod_val);
            }
            base = mod_multiply_2(base, base, mod_val);
            b >>= 1LL;
        }
        return result;
    }
} // namespace Modular_Arithmetics

namespace Number_Theory {
    vp<ll, i> get_prime_factorization(ll X) {
        vp<ll, i> factors;
        if (X <= 1) return factors;

        if (X % 2 == 0) {
            i count = 0;
            while (X % 2 == 0) {
                ++count;
                X /= 2;
            }
            factors.push_back({2, count});
        }

        for (ll d = 3; d <= X / d; d += 2) {
            i count = 0;
            while (X % d == 0) {
                ++count;
                X /= d;
            }
            if (count > 0) {
                factors.push_back({d, count});
            }
        }

        if (X > 1) {
            factors.push_back({X, 1});
        }
        return factors;
    }

    ll phi(ll X) {
        if (X <= 0) return 0;
        if (X == 1) return 1;

        ll result = X;
        vp<ll, i> factors = get_prime_factorization(X);
        for (const auto& factor : factors) {
            ll prime = factor.first;
            result -= result / prime;
        }
        return result;
    }

    bool is_prime(ll X) {
        if (X < 2) return false;
        if (X == 2) return true;
        if (X % 2 == 0) return false;

        for (ll d = 3; d <= X / d; d += 2) {
            if (X % d == 0) return false;
        }
        return true;
    }
} // namespace Number_Theory

namespace Combinatorics {
    template <typename OkFn, typename SolFn, typename ShowFn>
    void BackTracking(ll k, ll n, v<ll>& X, OkFn OK, SolFn Solution, ShowFn Show) {
        if (n <= 0 || k < 1 || k > n) return;

        for (ll val = 1; val <= n; ++val) {
            X[k] = val;
            if (OK(X, k)) {
                if (Solution(k)) Show(X, k);
                if (k < n) BackTracking(k + 1, n, X, OK, Solution, Show);
            }
        }
    }

    void GenerateArrangements(ll n, ll p, ofstream& fout) {
        if (n <= 0 || p <= 0 || p > n) return;
        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx)
                if (arr[pos] == arr[idx]) return false;
            return true;
        };
        auto Solution = [p](ll pos) { return pos == p; };
        auto Show = [p, &fout](const v<ll>& arr, ll) {
            for (ll idx = 1; idx <= p; ++idx)
                fout << arr[idx] << (idx == p ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    void GenerateArrangementsFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) return;
        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx)
                if (arr[pos] == arr[idx]) return false;
            return true;
        };
        auto Solution = [p](ll pos) { return pos == p; };
        auto Show = [p, &vals, &fout](const v<ll>& arr, ll) {
            for (ll idx = 1; idx <= p; ++idx)
                fout << vals[arr[idx] - 1] << (idx == p ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    void GenerateAllArrangements(const v<ll>& vals, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0) return;
        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx)
                if (arr[pos] == arr[idx]) return false;
            return true;
        };
        auto Solution = [](ll) { return true; };
        auto Show = [&vals, &fout](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx <= pos; ++idx)
                fout << vals[arr[idx] - 1] << (idx == pos ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    void StoreCombinationSums(const v<ll>& vals, ll k, v<ll>& out_sums) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || k <= 0 || k > n) return;
        v<ll> X(k + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [k](ll pos) { return pos == k; };
        auto Collect = [k, &vals, &out_sums](const v<ll>& arr, ll) {
            ll sum = 0;
            for (ll idx = 1; idx <= k; ++idx)
                sum += vals[arr[idx] - 1];
            out_sums.push_back(sum);
        };

        BackTracking(1, n, X, OK, Solution, Collect);
    }

    void PrintCombinationSums(const v<ll>& vals, ll k, ofstream& fout) {
        v<ll> sums;
        StoreCombinationSums(vals, k, sums);
        for (size_t idx = 0; idx < sums.size(); ++idx) {
            if (idx > 0) fout << ' ';
            fout << sums[idx];
        }
        fout << '\n';
    }

    void GenerateCombinations(ll n, ll p, ofstream& fout) {
        if (n <= 0 || p <= 0 || p > n) return;
        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [p](ll pos) { return pos == p; };
        auto Show = [p, &fout](const v<ll>& arr, ll) {
            for (ll idx = 1; idx <= p; ++idx)
                fout << arr[idx] << (idx == p ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    void GenerateCombinationsFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) return;
        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [p](ll pos) { return pos == p; };
        auto Show = [p, &vals, &fout](const v<ll>& arr, ll) {
            for (ll idx = 1; idx <= p; ++idx)
                fout << vals[arr[idx] - 1] << (idx == p ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    void StoreCombinationsFromSet(const v<ll>& vals, ll p, m<ll>& out_subsets) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) return;
        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [p](ll pos) { return pos == p; };
        auto Collect = [p, &vals, &out_subsets](const v<ll>& arr, ll) {
            v<ll> cur;
            cur.reserve(static_cast<size_t>(p));
            for (ll idx = 1; idx <= p; ++idx)
                cur.push_back(vals[arr[idx] - 1]);
            out_subsets.push_back(move(cur));
        };

        BackTracking(1, n, X, OK, Solution, Collect);
    }

    void GenerateSubsetsOfSize(ll n, ll p, ofstream& fout) {
        GenerateCombinations(n, p, fout);
    }

    void GenerateAllSubsets(ll n, ofstream& fout) {
        for (ll p = 1; p <= n; ++p)
            GenerateSubsetsOfSize(n, p, fout);
    }

    void GenerateSubsetsOfSizeFromSet(const v<ll>& A, ll p, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        if (p < 0 || p > n) return;
        if (p == 0) {
            fout << '\n';
            return;
        }
        GenerateCombinationsFromSet(A, p, fout);
    }

    void StoreSubsetsOfSizeFromSet(const v<ll>& A, ll p, m<ll>& out_subsets) {
        ll n = static_cast<ll>(A.size());
        if (p < 0 || p > n) return;
        if (p == 0) {
            out_subsets.push_back({});
            return;
        }
        StoreCombinationsFromSet(A, p, out_subsets);
    }

    void GenerateAllSubsetsFromSet(const v<ll>& A, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        for (ll p = 0; p <= n; ++p)
            GenerateSubsetsOfSizeFromSet(A, p, fout);
    }

    void GenerateAllCombinations(const v<ll>& A, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        if (n <= 0) return;
        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [](ll) { return true; };
        auto Show = [&A, &fout](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx <= pos; ++idx)
                fout << A[arr[idx] - 1] << (idx == pos ? '\n' : ' ');
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    m<ll> StoreAllCombinations(const v<ll>& A) {
        m<ll> subsets;
        ll n = static_cast<ll>(A.size());
        if (n <= 0) return subsets;
        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            return pos == 1 || arr[pos] > arr[pos - 1];
        };
        auto Solution = [](ll) { return true; };
        auto Collect = [&A, &subsets](const v<ll>& arr, ll pos) {
            v<ll> cur;
            cur.reserve(static_cast<size_t>(pos));
            for (ll idx = 1; idx <= pos; ++idx)
                cur.push_back(A[arr[idx] - 1]);
            subsets.push_back(move(cur));
        };

        BackTracking(1, n, X, OK, Solution, Collect);
        return subsets;
    }

    m<ll> StoreAllSubsetsFromSet(const v<ll>& A) {
        m<ll> subsets;
        ll n = static_cast<ll>(A.size());
        for (ll p = 0; p <= n; ++p)
            StoreSubsetsOfSizeFromSet(A, p, subsets);
        return subsets;
    }

    void PrintSubsets(const m<ll>& subsets, ofstream& fout) {
        for (size_t s_idx = 0; s_idx < subsets.size(); ++s_idx) {
            for (size_t e_idx = 0; e_idx < subsets[s_idx].size(); ++e_idx) {
                if (e_idx > 0) fout << ' ';
                fout << subsets[s_idx][e_idx];
            }
            fout << '\n';
        }
    }

    void GeneratePermutationsOfN(ll n, ofstream& fout) {
        GenerateArrangements(n, n, fout);
    }

    void GeneratePermutationsFromSet(const v<ll>& vals, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0) return;
        GenerateArrangementsFromSet(vals, n, fout);
    }

    template <typename T, typename Condition, typename Action>
    void GeneratePermutations(const v<T>& arr, Condition valid, Action act) {
        v<T> perm = arr;
        sort(perm.begin(), perm.end());
        do {
            if (valid(perm)) act(perm);
        } while (next_permutation(perm.begin(), perm.end()));
    }

    template <typename T>
    vp<T, T> GenerateCartesianProduct(const v<T>& A, const v<T>& B) {
        vp<T, T> C;
        for (const T& first_val : A)
            for (const T& second_val : B)
                C.push_back({first_val, second_val});
        return C;
    }

    template <typename T>
    void PrintCartesianProduct(const v<T>& A, const v<T>& B, ofstream& fout) {
        for (const T& first_val : A)
            for (const T& second_val : B)
                fout << first_val << ' ' << second_val << '\n';
    }

    void GenerateCartesianPowerFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0) return;

        v<ll> tuple(p, 0);
        function<void(ll)> generate = [&](ll pos) {
            if (pos == p) {
                for (ll idx = 0; idx < p; ++idx)
                    fout << vals[tuple[idx]] << (idx == p - 1 ? '\n' : ' ');
                return;
            }

            for (ll idx = 0; idx < n; ++idx) {
                tuple[pos] = idx;
                generate(pos + 1);
            }
        };

        generate(0);
    }
} // namespace Combinatorics

namespace Data_Structure {
    struct SubarrayResult {
        ll max_len;
    };

    SubarrayResult longest_equal_subarray_1based(const vector<ll>& A) {
        ll n = static_cast<ll>(A.size()) - 1;
        if (n <= 0) return {0};

        ll max_len = 1, cur_len = 1;
        for (ll i = 2; i <= n; ++i) {
            if (A[i] == A[i - 1]) ++cur_len;
            else cur_len = 1;
            max_len = max(max_len, cur_len);
        }
        return {max_len};
    }

    SubarrayResult longest_element_subarray_1based_target(const vector<ll>& A, ll target) {
        ll n = static_cast<ll>(A.size()) - 1;
        if (n <= 0) return {0};

        ll max_len = 0, cur_len = 0;
        for (ll i = 1; i <= n; ++i) {
            if (A[i] == target) {
                ++cur_len;
                max_len = max(max_len, cur_len);
            } else {
                cur_len = 0;
            }
        }
        return {max_len};
    }
} // namespace Data_Structure

ifstream fin("files/cufere.in");
ofstream fout("files/cufere.out");

int main() {
    FAST_IO;
    return 0;
}
