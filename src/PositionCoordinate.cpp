#include "PositionCoordinate.h"
#include "Utils.h"

PositionCoordinate::PositionCoordinate(uint nb, std::unordered_set<char> pieces, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*)) :
    _nb(nb), _pieces(pieces), _buildCube(buildCube), _unbuildCube(unbuildCube), _moveTable(nullptr), _size(Utils::cnk(nb, pieces.size())) {
}

PositionCoordinate::~PositionCoordinate() {
    delete[] _moveTable;
}

void PositionCoordinate::buildMoveTable() {
    _moveTable = new uint[_size * 6 * 3];
    char* perm = new char[_nb];

    for(uint i = 0; i < _size; ++i) {
        Utils::intToPos(i, perm, _nb, _pieces);
        Cube c = _buildCube(perm);
        for(uint j = 0; j < 6; ++j) {
            Cube move = Cube((AXIS)j);
            c.applyMult(move);
            for(uint k = 0; k < 3; ++k) {
                _unbuildCube(c, perm);
                _moveTable[i*6*3 + j*3 + k] = Utils::posToInt(perm, _nb, _pieces);
                c.applyMult(move);
            }
        }
    }
    delete[] perm;
}
