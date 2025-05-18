#include "hack.h"
#include <vector>

int hack(){
    int n = 0;
    while (++n){
        if (collisions({1, n + 1})) return n;
    }
    return 42;
}