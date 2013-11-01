#include "Utils.h"


uint Utils::orientToInt(const char* orients, int nb, char base) {
    uint r = 0;
    for(int i = 0; i < nb-1; ++i) {
        r = r * base + orients[i];
    }
    return r;
}

void Utils::intToOrient(uint in, char* orients, int nb, char base) {
    uint sum = 0;
    for(int i = nb-2; i >= 0; --i) {
        orients[i] = in % base;
        sum += orients[i];
        in /= base;
    }
    orients[nb-1] = base - (sum % base);
}

#if 0
void Utils::intToOrient(uint in, char* orients, int nb, char base) {
    for(int i = nb-1; i >= 0; --i) {
        orients[i] = in % base;
        in /= base;
    }
}
#endif

uint Utils::permToInt(const char* perm, int nb) {
    uint r = 0;
    for(int i = 0; i < nb; ++i) {
        r *= (nb - i);
        for(int j = i+1; j < nb; ++j)  {
            if(perm[j] < perm[i]) {
                ++r;
            }
        }
    }
    return r;
}

void Utils::intToPerm(uint in, char* perm, int nb) {
    perm[nb-1] = 0;
    for(int i = nb-2; i >= 0; --i) {
        perm[i] = in % (nb - i);
        in /= (nb - i);
        for(int j = i+1; j  < nb; ++j) {
            if(perm[j] >= perm[i]) {
                ++perm[j];
            }
        }
    }
}

#if 0
uint Utils::permToInt(const char* perm, int nb) {
    uint r = 0;
    for(int i = nb-1; i > 0; --i) {
        r *= (i+1);
        for(int j = 0; j < i; ++j) {
            if(perm[i] > perm[j]) {
                ++r;
            }
        }
    }
    return r;
}

void Utils::intToPerm(uint in, char* perm, int nb) {
    perm[0] = 0;
    for(int i = 1; i < nb; ++i) {
        perm[i] = in % (i+1);
        in /= (i+1);
        for(int j = 0; j < i; ++j) {
            if(perm[i] <= perm[j]) {
                ++perm[j];
            }
        }
    }
}
#endif

uint Utils::posToInt(const char* perm, int nb, const std::unordered_set<char>& pieces) {
    uint r = 0;
    int k = pieces.size() - 1;
    for(int i = nb - 1; i >= 0 && k >= 0; --i) {
        if(pieces.count(perm[i])) {
            --k;
        } else {
            r += Utils::cnk(i, k);
        }
    }
    return r;
}

void Utils::intToPos(uint in, char* perm, int nb, const std::unordered_set<char>& pieces) {
    for(int i = 0; i < nb; ++i) {
        perm[i] = -1;
    }

    int k = pieces.size()-1;
    for(int i = nb - 1; i >= 0 && k >= 0; --i) {
        int v = Utils::cnk(i, k);
        if(v > in) {
            perm[i] = 0;
            --k;
        } else {
            in -= v;
        }
    }

    int i = 0;
    for(char piece : pieces) {
        while(perm[i] == -1) {
            ++i;
        }
        perm[i] = piece;
        ++i;
    }
}

uint Utils::pow(uint n, uint m) {
    uint res = 1;
    while(m--) {
        res *= n;
    }
    return res;
}

std::vector<uint> Utils::_fac = {1};

uint Utils::fac(uint n) {
    if(n >= _fac.size()) {
        for(uint i = _fac.size(); i <= n; ++i) {
            _fac.push_back(i * _fac[i-1]);
        }
    }
    return _fac[n];
}

std::vector<std::vector<uint>> Utils::_cnk;

uint Utils::cnk(uint n, uint k) {
    if(n >= _cnk.size()) {
        for(uint i = _cnk.size(); i <= n; ++i) {
            _cnk.push_back(std::vector<uint>());
            _cnk[i].resize(i+1);
            _cnk[i][0] = 1;
            _cnk[i][i] = 1;
            for(uint j = 1; j < i; ++j) {
                _cnk[i][j] = _cnk[i-1][j] + _cnk[i-1][j-1];
            }
        }
    }
    return _cnk[n][k];
}
