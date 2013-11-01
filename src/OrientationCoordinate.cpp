#include "OrientationCoordinate.h"
#include "Utils.h"

OrientationCoordinate::OrientationCoordinate(uint nb, uint base, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*)) :
    _nb(nb), _base(base), _buildCube(buildCube), _unbuildCube(unbuildCube), _moveTable(nullptr), _size(Utils::pow(_base, _nb - 1))  {
}

OrientationCoordinate::~OrientationCoordinate() {
    delete[] _moveTable;
}

void OrientationCoordinate::buildMoveTable() {
    _moveTable = new uint[_size * 6 * 3];
    char* orient = new char[_nb];

    for(uint i = 0; i < _size; ++i) {
        Utils::intToOrient(i, orient, _nb, _base);
        Cube c = _buildCube(orient);
        for(uint j = 0; j < 6; ++j) {
            Cube move = Cube((AXIS)j);
            c.applyMult(move);
            for(uint k = 0; k < 3; ++k) {
                _unbuildCube(c, orient);
                _moveTable[i*6*3 + j*3 + k] = Utils::orientToInt(orient, _nb, _base);
                c.applyMult(move);
            }
        }
    }

    delete[] orient;
}

