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

        template<size_t N>
        static inline void zoom(const std::array<char, N>& pieces, const char* perm, char* out) {
            //TODO: optimize
            std::vector<char> reverse(pieces.back()+2, -1);
            for(uint i = 0; i < N; ++i) {
                reverse[pieces[i]+1] = i;
            }
            const char* reverseData = &reverse[1]; //reverseData[-1] = -1
            for(uint i = 0; i < N; ++i) {
                out[i] = reverseData[perm[pieces[i]]];
            }
        }

        template<size_t N>
        static inline void unzoom(uint nPerm, const std::array<char, N>& pieces, const char* in, char* perm) {
            for(uint i = 0; i < nPerm; ++i) {
                perm[i] = -1;
            }
            for(uint i = 0; i < N; ++i) {
                if(in[i] == -1) {
                    perm[pieces[i]] = -1;
                } else {
                    perm[pieces[i]] = pieces[in[i]];
                }
            }
        }

    private:
        static std::vector<uint> _fac;
        static std::vector<std::vector<uint>> _cnk;
};

#endif

