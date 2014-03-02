#include "CubeProperties.h"

CubeProperties::CubeProperties(uint size) :
    _size(size) {
}

Cube CubeProperties::identity() {
    Cube cube(_size);
    cube.makeIdentity();
    return cube;
}

