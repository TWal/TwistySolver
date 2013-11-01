#include "Coordinate.h"

Coordinate::Coordinate(uint size) : _size(size), _moveTable(nullptr) {
}

Coordinate::~Coordinate() {
    delete _moveTable;
}

//We cannot call virtual functions in the constructor, so this is a little hack to avoid this.
void Coordinate::_init(std::function<Cube (uint)> buildCube, std::function<uint (const Cube&)> unbuildCube) {
    _moveTable = new uint[_size * 6 * 3];

    for(uint i = 0; i < _size; ++i) {
        Cube c = buildCube(i);
        for(uint j = 0; j < 6; ++j) {
            Cube move = Cube((AXIS)j);
            c.applyMult(move);
            for(uint k = 0; k < 3; ++k) {
                _moveTable[i*6*3 + j*3 + k] = unbuildCube(c);
                c.applyMult(move);
            }
        }
    }
}
