#include "rotate.h"
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>

// BEGIN SECRET
#include <string>
// END SECRET

static const int M = 50000; // 180 degrees

namespace Rotating_Lines{
    template<typename T>
    struct fenwick{
        std::vector<T> f;
        fenwick(int n){
            f.resize(n + 2);
        }
        fenwick(){};

        void add(int i, T val){
            i++;
            while (i < (int)f.size()){
                f[i] += val;
                i += -i & i;
            }
        }

        T get(int i){
            i++;
            T res = 0;
            while (i > 0){
                res += f[i];
                i -= -i & i;
            }
            return res;
        }

        T get(int l, int r){
            if (l > r) return 0;
            return get(r) - get(l - 1);
        }
    };

    struct energy_calculator{
        fenwick<int> cnt;
        fenwick<long long> sum;
        long long total;
        
        long long contribution(int x){
            if (x >= M) x -= M;

            long long nrg = 0;

            if (0 <= x && x < M / 2){ // 0 <= x < 90
                int y = x + M / 2;
                nrg += sum.get(x + 1, y) - 1LL * x * cnt.get(x + 1, y);
                nrg += 1LL * x * cnt.get(0, x) - sum.get(0, x);
                nrg += 1LL * (x + M) * cnt.get(y + 1, M) - sum.get(y + 1, M);
            }

            if (M / 2 <= x && x < M){ // 90 <= x < 180
                int y = x - M / 2;
                nrg += sum.get(x + 1, M) - 1LL * x * cnt.get(x + 1, M);
                nrg += sum.get(0, y) - 1LL * (x - M) * cnt.get(0, y);
                nrg += 1LL * x * cnt.get(y + 1, x) - sum.get(y + 1, x);
            }

            return nrg;
        }

        void add(int x){
            total += contribution(x);
            cnt.add(x, 1);
            sum.add(x, x);
        }

        void del(int x){
            cnt.add(x, -1);
            sum.add(x, -x);
            total -= contribution(x);
        }

        void init(const std::vector<int> &v){
            cnt = fenwick<int>(M);
            sum = fenwick<long long>(M);
            total = 0;

            for (int x : v){
                if (x >= M) x -= M;
                add(x);
            }
        }

    } machine;
}

static void exit_mes(std::string s){
	const std::string output_secret = "Kk1WryViMzuiNVd5LmhgkRlqQl5ntK4r";
    printf("%s\n", output_secret.c_str());
	printf("WA\n");
    printf("%s\n", s.c_str());
    exit(0);
}

static std::vector<int> v;

static int total_cost = 0;

static long long last_energy;

void rotate(std::vector<int> t, int x){
    int k = (int) t.size();
    total_cost += k;

    if (total_cost > 2000000){
        exit_mes("Too many rotations");
    }
    
    std::sort(t.begin(), t.end());

    for (int i = 0; i < k; i++){
        if (t[i] < 0 || t[i] >= (int) v.size()){
            exit_mes("Invalid index");
        }
        if (i > 0 && t[i] == t[i - 1]){
            exit_mes("t has duplicate element");
        }

        Rotating_Lines::machine.del(v[t[i]]);
        v[t[i]] = (v[t[i]] + x) % 50000;
        Rotating_Lines::machine.add(v[t[i]]);
    }

    long long cur_energy = Rotating_Lines::machine.total;

    if (cur_energy < last_energy){
        exit_mes("Energy decreased");
    }

    last_energy = cur_energy;
}

int main() {
	// BEGIN SECRET
	const std::string input_secret = "tXwyMZV19lL68RlQwHgOG0E5XaXD61x1";
	const std::string output_secret = "Kk1WryViMzuiNVd5LmhgkRlqQl5ntK4r";
	char secret[1000];
	assert(1 == scanf("%999s", secret));
	if (std::string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("PV\n");
		printf("Possible tampering with the input\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
    
	int n;
	assert(1 == scanf("%d", &n));
    v.resize(n);
    for (int i = 0; i < n; i++){
        assert(1 == scanf("%d", &v[i]));
    }

	fclose(stdin);

    Rotating_Lines::machine.init(v);
    last_energy = Rotating_Lines::machine.total;

	energy(n, v);

    long long correct_answer = (long long)25000 * (n / 2) * (n - n / 2);

    if (last_energy != correct_answer){
        exit_mes("Not optimal energy efficiency");
    }

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	fclose(stdout);
	return 0;
}
