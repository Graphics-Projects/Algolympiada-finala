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
        return ((x % mod_val) + mod_val) % mod_val;
    }

    ll mod_add_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        return (norm_mod(a, mod_val) + norm_mod(b, mod_val)) % mod_val;
    }

    ll mod_add_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll rez = 0;
        for (size_t idx = 0; idx < A.size(); ++idx) {
            rez = (rez + norm_mod(A[idx], mod_val)) % mod_val;
        }
        return rez;
    }

    ll mod_subtract_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll x = norm_mod(a, mod_val);
        ll y = norm_mod(b, mod_val);
        return (x - y + mod_val) % mod_val;
    }

    ll mod_subtract_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0 || A.empty()) return 0;
        ll rez = norm_mod(A[0], mod_val);
        for (size_t idx = 1; idx < A.size(); ++idx) {
            rez = (rez - norm_mod(A[idx], mod_val) + mod_val) % mod_val;
        }
        return rez;
    }

    ll mod_multiply_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return 0;
        return (norm_mod(a, mod_val) * norm_mod(b, mod_val)) % mod_val;
    }

    ll mod_multiply_n(const v<ll>& A, ll mod_val) {
        if (mod_val <= 0) return 0;
        ll rez = 1 % mod_val;
        for (size_t idx = 0; idx < A.size(); ++idx) {
            rez = (rez * norm_mod(A[idx], mod_val)) % mod_val;
        }
        return rez;
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
        b = norm_mod(b, mod_val);
        ll x, y;
        ll g = gcdExtended(b, mod_val, x, y);
        if (g != 1) return -1;
        return norm_mod(x, mod_val);
    }

    ll mod_divide_2(ll a, ll b, ll mod_val) {
        if (mod_val <= 0) return -1;
        a = norm_mod(a, mod_val);
        ll inv = modInverse(b, mod_val);
        if (inv == -1) return -1;
        return (a * inv) % mod_val;
    }

    ll mod_factorial(ll a, ll mod_val) {
        if (mod_val <= 0 || a < 0) return 0;
        if (mod_val == 1 || a >= mod_val) return 0;

        ll rez = 1 % mod_val;
        for (ll k = 2; k <= a; ++k) {
            rez = mod_multiply_2(rez, k, mod_val);
        }
        return rez;
    }

    ll mod_exponent(ll a, ll b, ll mod_val) {
        if (mod_val <= 0 || b < 0) return 0;
        if (b == 0) return 1 % mod_val;

        ll base = norm_mod(a, mod_val);
        if (b % 2 == 0) {
            ll half = mod_exponent(base, b / 2, mod_val);
            return mod_multiply_2(half, half, mod_val);
        }
        return mod_multiply_2(base, mod_exponent(base, b - 1, mod_val), mod_val);
    }
} // namespace Modular_Arithmetics

namespace Number_Theory {
    vp<ll, i> get_prime_factorization(ll X) {
        vp<ll, i> factors;
        if (X <= 1) return factors;

        ll d = 2;
        while (d <= X / d) {
            i p_cnt = 0;
            while (X % d == 0) {
                ++p_cnt;
                X /= d;
            }
            if (p_cnt > 0) {
                factors.push_back({d, p_cnt});
            }
            ++d;
        }
        if (X > 1) {
            factors.push_back({X, 1});
        }
        return factors;
    }

    ll phi(ll X) {
        if (X <= 0) return 0;
        if (X == 1) return 1;
        vp<ll, i> factors = get_prime_factorization(X);
        ll rez = 1;
        for (size_t idx = 0; idx < factors.size(); ++idx) {
            ll prime = factors[idx].first;
            i exp_val = factors[idx].second;

            ll prime_pow = 1;
            for (i step = 1; step < exp_val; ++step) {
                prime_pow *= prime;
            }

            rez *= prime_pow * (prime - 1);
        }
        return rez;
    }

    bool is_prime(ll X)
    {
        if(X < 2) return false;
        else if(X == 2) return true;
        else if(X % 2 == 0) return false;

        for(ll d = 3; d * d <= X; d++)
        {
            if(X % d == 0) return false;
        }
        return true;
    }
} // namespace Number_Theory

namespace Combinatorics {
    template <typename OkFn, typename SolFn, typename ShowFn>
    void BackTracking(ll k, ll n, v<ll>& X, OkFn OK, SolFn Solution, ShowFn Show) {
        if (n <= 0 || k < 1 || k > n) {
            return;
        }

        for (ll val = 1; val <= n; ++val) {
            X[k] = val;
            if (OK(X, k)) {
                if (Solution(k)) {
                    Show(X, k);
                }
                if (k < n) {
                    BackTracking(k + 1, n, X, OK, Solution, Show);
                }
            }
        }
    }

    /* ================= ARRANGEMENTS (VARIAȚIUNI) ================= */

    // Arrangements of size p from 1..n (no repeated elements, order matters)
    void GenerateArrangements(ll n, ll p, ofstream& fout) {
        if (n <= 0 || p <= 0 || p > n) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx) {
                if (arr[pos] == arr[idx]) {
                    return false;
                }
            }
            return true;
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Show = [p, &fout](const v<ll>& arr, ll /*pos*/) {
            for (ll idx = 1; idx <= p; ++idx) {
                fout << arr[idx] << (idx == p ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    // Arrangements of size p drawn from 0-indexed set vals
    void GenerateArrangementsFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx) {
                if (arr[pos] == arr[idx]) {
                    return false;
                }
            }
            return true;
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Show = [p, &vals, &fout](const v<ll>& arr, ll /*pos*/) {
            for (ll idx = 1; idx <= p; ++idx) {
                fout << vals[arr[idx] - 1] << (idx == p ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    // All non-empty arrangements of any length 1..n from 0-indexed set vals
    void GenerateAllArrangements(const v<ll>& vals, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0) {
            return;
        }

        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx < pos; ++idx) {
                if (arr[pos] == arr[idx]) {
                    return false;
                }
            }
            return true;
        };

        auto Solution = [](ll /*pos*/) {
            return true;
        };

        auto Show = [&vals, &fout](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx <= pos; ++idx) {
                fout << vals[arr[idx] - 1] << (idx == pos ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    /* ================= COMBINATIONS & SUBSETS ================= */
    /* ================= COMBINATION PARTIAL SUMS ================= */

    // Stores the sums of all k-element combinations from 0-indexed set vals
    void StoreCombinationSums(const v<ll>& vals, ll k, v<ll>& out_sums) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || k <= 0 || k > n) {
            return;
        }

        v<ll> X(k + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [k](ll pos) {
            return pos == k;
        };

        auto CollectSum = [k, &vals, &out_sums](const v<ll>& arr, ll /*pos*/) {
            ll sum = 0;
            for (ll idx = 1; idx <= k; ++idx) {
                sum += vals[arr[idx] - 1];
            }
            out_sums.push_back(sum);
        };

        BackTracking(1, n, X, OK, Solution, CollectSum);
    }

    // Prints the sums of all k-element combinations from 0-indexed set vals to fout
    void PrintCombinationSums(const v<ll>& vals, ll k, ofstream& fout) {
        v<ll> sums;
        StoreCombinationSums(vals, k, sums);
        for (size_t idx = 0; idx < sums.size(); ++idx) {
            if (idx > 0) fout << ' ';
            fout << sums[idx];
        }
        fout << '\n';
    }

    // Combinations of size p from 1..n (strictly increasing indices/values)
    void GenerateCombinations(ll n, ll p, ofstream& fout) {
        if (n <= 0 || p <= 0 || p > n) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Show = [p, &fout](const v<ll>& arr, ll /*pos*/) {
            for (ll idx = 1; idx <= p; ++idx) {
                fout << arr[idx] << (idx == p ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    // Combinations of size p drawn from 0-indexed set vals (printed to fout)
    void GenerateCombinationsFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Show = [p, &vals, &fout](const v<ll>& arr, ll /*pos*/) {
            for (ll idx = 1; idx <= p; ++idx) {
                fout << vals[arr[idx] - 1] << (idx == p ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    // Combinations of size p drawn from 0-indexed set vals (stored in out_subsets)
    void StoreCombinationsFromSet(const v<ll>& vals, ll p, m<ll>& out_subsets) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0 || p > n) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Collect = [p, &vals, &out_subsets](const v<ll>& arr, ll /*pos*/) {
            v<ll> cur;
            cur.reserve(static_cast<size_t>(p));
            for (ll idx = 1; idx <= p; ++idx) {
                cur.push_back(vals[arr[idx] - 1]);
            }
            out_subsets.push_back(move(cur));
        };

        BackTracking(1, n, X, OK, Solution, Collect);
    }

    // Subsets of exact size p from 1..n
    void GenerateSubsetsOfSize(ll n, ll p, ofstream& fout) {
        GenerateCombinations(n, p, fout);
    }

    // All non-empty subsets of size 1..n from 1..n
    void GenerateAllSubsets(ll n, ofstream& fout) {
        for (ll p = 1; p <= n; ++p) {
            GenerateSubsetsOfSize(n, p, fout);
        }
    }

    // Subsets of exact size p drawn from 0-indexed set A (p = 0 outputs a blank line)
    void GenerateSubsetsOfSizeFromSet(const v<ll>& A, ll p, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        if (p < 0 || p > n) {
            return;
        }
        if (p == 0) {
            fout << '\n';
            return;
        }
        GenerateCombinationsFromSet(A, p, fout);
    }

    // Store subsets of exact size p drawn from 0-indexed set A (p = 0 stores an empty subset {})
    void StoreSubsetsOfSizeFromSet(const v<ll>& A, ll p, m<ll>& out_subsets) {
        ll n = static_cast<ll>(A.size());
        if (p < 0 || p > n) {
            return;
        }
        if (p == 0) {
            out_subsets.push_back({});
            return;
        }
        StoreCombinationsFromSet(A, p, out_subsets);
    }

    // All subsets (sizes 0..n) drawn from 0-indexed set A (printed to fout)
    void GenerateAllSubsetsFromSet(const v<ll>& A, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        for (ll p = 0; p <= n; ++p) {
            GenerateSubsetsOfSizeFromSet(A, p, fout);
        }
    }

    // All non-empty subsets/combinations of elements from 0-indexed set A (printed to fout)
    void GenerateAllCombinations(const v<ll>& A, ofstream& fout) {
        ll n = static_cast<ll>(A.size());
        if (n <= 0) {
            return;
        }

        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [](ll /*pos*/) {
            return true;
        };

        auto Show = [&A, &fout](const v<ll>& arr, ll pos) {
            for (ll idx = 1; idx <= pos; ++idx) {
                fout << A[arr[idx] - 1] << (idx == pos ? '\n' : ' ');
            }
        };

        BackTracking(1, n, X, OK, Solution, Show);
    }

    // Store all non-empty subsets/combinations of elements from 0-indexed set A
    m<ll> StoreAllCombinations(const v<ll>& A) {
        m<ll> subsets;
        ll n = static_cast<ll>(A.size());
        if (n <= 0) {
            return subsets;
        }

        v<ll> X(n + 1, 0);

        auto OK = [](const v<ll>& arr, ll pos) {
            if (pos == 1) return true;
            return arr[pos] > arr[pos - 1];
        };

        auto Solution = [](ll /*pos*/) {
            return true;
        };

        auto Collect = [&A, &subsets](const v<ll>& arr, ll pos) {
            v<ll> cur;
            cur.reserve(static_cast<size_t>(pos));
            for (ll idx = 1; idx <= pos; ++idx) {
                cur.push_back(A[arr[idx] - 1]);
            }
            subsets.push_back(move(cur));
        };

        BackTracking(1, n, X, OK, Solution, Collect);
        return subsets;
    }

    // Store all subsets (sizes 0..n, including empty subset) from 0-indexed set A
    m<ll> StoreAllSubsetsFromSet(const v<ll>& A) {
        m<ll> subsets;
        ll n = static_cast<ll>(A.size());
        for (ll p = 0; p <= n; ++p) {
            StoreSubsetsOfSizeFromSet(A, p, subsets);
        }
        return subsets;
    }

    // Helper to print a stored collection of subsets to fout
    void PrintSubsets(const m<ll>& subsets, ofstream& fout) {
        for (size_t s_idx = 0; s_idx < subsets.size(); ++s_idx) {
            for (size_t e_idx = 0; e_idx < subsets[s_idx].size(); ++e_idx) {
                if (e_idx > 0) fout << ' ';
                fout << subsets[s_idx][e_idx];
            }
            fout << '\n';
        }
    }

    /* ================= PERMUTATIONS (PERMUTĂRI) ================= */

    // All permutations of 1..n via BackTracking
    void GeneratePermutationsOfN(ll n, ofstream& fout) {
        GenerateArrangements(n, n, fout);
    }

    // All permutations of 0-indexed set vals via BackTracking
    void GeneratePermutationsFromSet(const v<ll>& vals, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0) {
            return;
        }
        GenerateArrangementsFromSet(vals, n, fout);
    }

    // Filtered permutation generator using STL next_permutation on a copy
    template <typename T, typename Condition, typename Action>
    void GeneratePermutations(const v<T>& arr, Condition valid, Action act) {
        v<T> perm = arr;
        sort(perm.begin(), perm.end());
        do {
            if (valid(perm)) {
                act(perm);
            }
        } while (next_permutation(perm.begin(), perm.end()));
    }

    /* ================= CARTESIAN PRODUCT (PRODUS CARTEZIAN) ================= */

    // Pairwise Cartesian product A x B (all pairs (a, b))
    template <typename T>
    vp<T, T> GenerateCartesianProduct(const v<T>& A, const v<T>& B) {
        vp<T, T> C;
        for (const T& first_val : A) {
            for (const T& second_val : B) {
                C.push_back({first_val, second_val});
            }
        }
        return C;
    }

    // Write pairwise Cartesian product A x B directly to fout
    template <typename T>
    void PrintCartesianProduct(const v<T>& A, const v<T>& B, ofstream& fout) {
        for (const T& first_val : A) {
            for (const T& second_val : B) {
                fout << first_val << ' ' << second_val << '\n';
            }
        }
    }

    // Ordered tuples of length p with repetition allowed from 0-indexed set vals (vals^p)
    void GenerateCartesianPowerFromSet(const v<ll>& vals, ll p, ofstream& fout) {
        ll n = static_cast<ll>(vals.size());
        if (n <= 0 || p <= 0) {
            return;
        }

        v<ll> X(p + 1, 0);

        auto OK = [](const v<ll>& /*arr*/, ll /*pos*/) {
            return true; // repetition and any order allowed
        };

        auto Solution = [p](ll pos) {
            return pos == p;
        };

        auto Show = [p, &vals, &fout](const v<ll>& arr, ll /*pos*/) {
            for (ll idx = 1; idx <= p; ++idx) {
                fout << vals[arr[idx] - 1] << (idx == p ? '\n' : ' ');
            }
        };

        BackTracking(1, max(n, p), X, OK, Solution, Show);
    }
} // namespace Combinatorics

namespace Data_Structure {
    struct SubarrayResult {
        ll max_len;
    };

    SubarrayResult longest_equal_subarray_1based(const vector<ll>& A) {
        ll n = static_cast<ll>(A.size()) - 1;
        if (n <= 0) {
            return {0};
        }

        ll max_len = 1, cur_len = 1;

        for (ll i = 2; i <= n; ++i) {
            if (A[i] == A[i - 1]) {
                cur_len++;
            } else {
                cur_len = 1;
            }
            if (cur_len > max_len) {
                max_len = cur_len;
            }
        }

        return {max_len};
    }

    SubarrayResult longest_element_subarray_1based_target(const vector<ll>& A, ll target) {
        ll n = static_cast<ll>(A.size()) - 1;
        if (n <= 0) {
            return {0};
        }

        ll max_len = 0, cur_len = 0;

        for (ll i = 1; i <= n; ++i) {
            if (A[i] == target) {
                cur_len++;
                if (cur_len > max_len) {
                    max_len = cur_len;
                }
            } else {
                cur_len = 0;
            }
        }

        return {max_len};
    }
} // namespace Data_Structure

ifstream fin("files/cufere.in");
ofstream fout("files/cufere.out");

void Cer1();
void Cer2();
ll n;
m<ll> A;
struct Obiect
{
    ll eticheta, //ultimele 2 nr
       nr_obiecte; //primele 2 cifre
};
bool cmpEticheta(const Obiect& a, const Obiect& b) {
    if (a.eticheta != b.eticheta) {
        return a.eticheta < b.eticheta;
    }
    return a.nr_obiecte < b.nr_obiecte; // tie-breaker
}
int main()
{
    FAST_IO;
    int c;
    fin>>c;
    switch(c)
    {
        case 1: Cer1(); break;
        case 2: Cer2(); break;
        default: break;
    }
    return 0;
}

void Cer1()
{
    fin>>n;
    A.assign(3 * n, v<ll>(9, 0));
    v<Obiect> B;
    for(int i = 0; i < (int)A.size(); i++)
    {
        for(int j = 0; j < (int)A[i].size(); j++)
        {
            fin>>A[i][j];
        }
    }
    for(int i = 0; i < (int)A.size(); i++)
    {
        for(int j = 0; j < (int)A[i].size(); j++)
        {
            ll X = A[i][j];
            Obiect obj;
            obj.eticheta = X % 100;
            obj.nr_obiecte = X / 100;
            B.push_back(obj);
        }
    }
    std::sort(B.begin(), B.end(), cmpEticheta); 
    map<ll, ll> freq;
for (const auto& elem : B) {
    freq[elem.eticheta] += elem.nr_obiecte;
}
for (const auto& [element, total_obiecte] : freq) {
    fout << element << " " << total_obiecte << '\n';
}
}
void Cer2()
{

}
