#include "PositionCoordinate.h"
#include "Utils.h"

PositionCoordinate::PositionCoordinate(uint nb, std::unordered_set<char> pieces, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(Utils::cnk(nb, pieces.size())),
    _nb(nb),
    _pieces(pieces),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempPerm(new char[_nb]) {

    _init();
}

PositionCoordinate::~PositionCoordinate() {
    delete[] _tempPerm;
}

uint PositionCoordinate::fromCube(const Cube& cube) const {
    _cubeToPerm(cube, _tempPerm);
    return posToInt(_tempPerm, _nb, _pieces);
}

Cube PositionCoordinate::toCube(uint coord) const {
    intToPos(coord, _tempPerm, _nb, _pieces);
    return _permToCube(_tempPerm);
}

uint PositionCoordinate::posToInt(const char* perm, int nb, const std::unordered_set<char>& pieces) {
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

void PositionCoordinate::intToPos(uint in, char* perm, int nb, const std::unordered_set<char>& pieces) {
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

