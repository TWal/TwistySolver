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

        static inline uint getLayer(uint move) {
            return move / 18;
        }

        static inline uint getAxis(uint move) {
            return (move % 18) / 3;
        }

        static inline uint getNb(uint move) {
            return move % 3;
        }

    private:
        static std::vector<uint> _fac;
        static std::vector<std::vector<uint>> _cnk;
};

#endif

