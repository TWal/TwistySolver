#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <vector>
#include <unordered_set>

class Utils {
    public:
        static uint orientToInt(const char* orients, int nb, char base);
        static void intToOrient(uint in, char* orients, int nb, char base);
        static uint permToInt(const char* perm, int nb);
        static void intToPerm(uint in, char* perm, int nb);
        static uint posToInt(const char* perm, int nb, const std::unordered_set<char>& pieces);
        static void intToPos(uint in, char* perm, int nb, const std::unordered_set<char>& pieces);

        static uint pow(uint n, uint m);
        static uint fac(uint n);
        static uint cnk(uint n, uint k);

    private:
        static std::vector<uint> _fac;
        static std::vector<std::vector<uint>> _cnk;
};

#endif

