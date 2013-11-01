#ifndef COORDINATE_H
#define COORDINATE_H

#include <functional>
#include "Cube.h"
#include "types.h"

class Coordinate {
    public:
        Coordinate(uint size);
        virtual ~Coordinate();
        inline uint moveTableLookup(uint current, AXIS axis, uint nb) {
            return _moveTable[current*6*3 + (uint)axis*3 + nb - 1];
        }
        inline uint moveTableLookup(uint current, uint move) {
            return _moveTable[current*6*3 + move];
        }
        inline uint size() {
            return _size;
        }
        virtual uint fromCube(const Cube& cube) = 0;
        virtual Cube toCube(uint coord) = 0;
    protected:
        void _init(std::function<Cube (uint)> buildCube, std::function<uint (const Cube&)> unbuildCube);
        uint _size;
        uint* _moveTable;
};

#endif

