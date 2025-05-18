#include "hack.h"
#include <cassert>
#include <cstdio>
#include <cassert>
#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>


static int n = 0;
static int total_cost = 0;

static void exit_mes(double points, const std::string& s) {
    const std::string output_secret = "1eJ4ArSX1VUyR4oqcU3vVcJ6xoN8jIYs";
    printf("%s\n", output_secret.c_str());
    printf("%lf\n", points);
    printf("%s\n", s.c_str());
    fclose(stdout);
    exit(0);
}

long long collisions(std::vector<long long> x){
    total_cost += (int) x.size();
    if (total_cost > 1000000){
        exit_mes(0, "Total cost exceeded 1,000,000");
    }
    
    sort(x.begin(), x.end());
    for(int i = 0; i < (int) x.size(); ++i) {
        if (x[i] < 1 || x[i] > (long long)1e18) {
            exit_mes(0, "x[i] is out of range\n");
        }

        if(i > 0 && x[i] == x[i - 1]) {
            exit_mes(0, "x has a duplicate element\n");
        }
    }
    
    for(int i = 0; i < (int) x.size(); ++i) x[i] %= n;
    
    sort(x.begin(), x.end());

    long long res = 0;
    for(int i = 0; i < (int) x.size(); ++i) {
        int j = i;
        while(j + 1 < (int) x.size() && x[j + 1] == x[i]) j++;
        res += 1LL * (j - i + 1) * (j - i) / 2;
        i = j;
    }

    return res;
}

int main() {
    const std::string input_secret = "gcxAq5ZVsD9pstWtmGwYDYtXCn4B88dd";
    const std::string output_secret = "1eJ4ArSX1VUyR4oqcU3vVcJ6xoN8jIYs";
    char secret[1000];
    assert(1 == scanf("%999s", secret));
    if (std::string(secret) != input_secret) {
        exit_mes(0, "Possible tampering with input");
        return 0;
    }
 
    int t, mode;
    assert(2 == scanf("%d %d", &t, &mode));

    std::vector<int> ns(t);
    for (int i = 0; i < t; i++){
        assert(1 == scanf("%d", &ns[i]));
    }

    int max_total_cost = 0;
    for(int i = 0; i < t; ++i){
        total_cost = 0;
        n = ns[i];
        
        int ans = hack();
        if(ans != n) {
            exit_mes(0, "Wrong guess");
        }

        max_total_cost = std::max(max_total_cost, total_cost);
    }

    std::cerr << "Max used total cost: " << max_total_cost << std::endl;
    if(mode == 1) {
        if(max_total_cost <= 110'000) exit_mes(1, "All good");
        else {
            double points = (logl(1'000'000) - logl(max_total_cost - 90'000)) / logl(50);
            exit_mes(points, "Partial points");
        }
    } else {
        exit_mes(1, "All good");
    }
    
}
