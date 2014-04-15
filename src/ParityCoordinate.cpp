#include "ParityCoordinate.h"

ParityCoordinate::ParityCoordinate(uint nb, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(2),
    _nb(nb),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempPerm(new char[_nb]) {
}

ParityCoordinate::ParityCoordinate(uint nb, const std::vector<uint>& allowedMoves, const CubeProperties& props, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm, const std::string& path) :
    ParityCoordinate(nb, permToCube, cubeToPerm) {
    buildMoveTable(allowedMoves, props, path);
}

ParityCoordinate::~ParityCoordinate() {
    delete[] _tempPerm;
}

uint ParityCoordinate::fromCube(const Cube& cube) const {
    _cubeToPerm(cube, _tempPerm);
    return parToInt(_tempPerm, _nb);
}

Cube ParityCoordinate::toCube(uint coord) const {
    intToPar(coord, _tempPerm, _nb);
    return _permToCube(_tempPerm);
}

uint ParityCoordinate::parToInt(const char* perm, int nb) {
    bool res = false;
    for(uint i = 0; i < nb; ++i) {
        for(uint j = i; j < nb; ++j) {
            if(perm[i] > perm[j]) {
                res = !res;
            }
        }
    }
    return (uint)res;
}

void ParityCoordinate::intToPar(uint in, char* perm, int nb) {
    for(int i = 0; i < nb; ++i) {
        perm[i] = i;
    }
    if(nb >= 2 && in == 1) {
        perm[0] = 1;
        perm[1] = 0;
    }
}

