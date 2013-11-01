#include "Utils.h"

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

