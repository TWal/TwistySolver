#include "PermutationCoordinate.h"
#include "Utils.h"

PermutationCoordinate::PermutationCoordinate(uint nb, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(Utils::fac(nb)),
    _nb(nb),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempPerm(new char[_nb]) {

    _init([this](uint coord) {
        return toCube(coord);
    }, [this](const Cube& cube) {
        return fromCube(cube);
    });
}

PermutationCoordinate::~PermutationCoordinate() {
    delete[] _tempPerm;
}

uint PermutationCoordinate::fromCube(const Cube& cube) {
    _cubeToPerm(cube, _tempPerm);
    return permToInt(_tempPerm, _nb);
}

Cube PermutationCoordinate::toCube(uint coord) {
    intToPerm(coord, _tempPerm, _nb);
    return _permToCube(_tempPerm);
}

uint PermutationCoordinate::permToInt(const char* perm, int nb) {
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

void PermutationCoordinate::intToPerm(uint in, char* perm, int nb) {
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

