#ifndef CUBEPROPERTIES_H
#define CUBEPROPERTIES_H

#include "types.h"
#include "Cube.h"

class CubeProperties {
    public:
        CubeProperties(uint size);
        template<typename ... T>
        Cube makeCube(T ... args) const {
            return Cube(_size, args ...);
        }

        Cube identity();

    private:
        uint _size;
};

#endif

