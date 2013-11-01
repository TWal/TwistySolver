#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <vector>
#include <unordered_set>

class Utils {
    public:
        static uint pow(uint n, uint m);
        static uint fac(uint n);
        static uint cnk(uint n, uint k);

    private:
        static std::vector<uint> _fac;
        static std::vector<std::vector<uint>> _cnk;
};

#endif

