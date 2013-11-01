#include "PermutationCoordinate.h"
#include "Utils.h"

PermutationCoordinate::PermutationCoordinate(uint nb, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*)) :
    _nb(nb), _buildCube(buildCube), _unbuildCube(unbuildCube), _moveTable(nullptr), _size(Utils::fac(_nb)) {
}

PermutationCoordinate::~PermutationCoordinate() {
    delete[] _moveTable;
}

void PermutationCoordinate::buildMoveTable() {
    _moveTable = new uint[_size * 6 * 3];
    char* perm = new char[_nb];

    for(uint i = 0; i < _size; ++i) {
        Utils::intToPerm(i, perm, _nb);
        Cube c = _buildCube(perm);
        for(uint j = 0; j < 6; ++j) {
            Cube move = Cube((AXIS)j);
            c.applyMult(move);
            for(uint k = 0; k < 3; ++k) {
                _unbuildCube(c, perm);
                _moveTable[i*6*3 + j*3 + k] = Utils::permToInt(perm, _nb);
                c.applyMult(move);
            }
        }
    }

    delete[] perm;
}
