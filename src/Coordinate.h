#ifndef COORDINATE_H
#define COORDINATE_H

#include <functional>
#include <vector>
#include "Cube.h"
#include "types.h"
#include "CubeProperties.h"

class Coordinate {
    public:
        Coordinate(uint size);
        virtual ~Coordinate();
        void buildMoveTable(const std::vector<uint>& allowedMoves, const CubeProperties& props, const std::string& path = "");

        inline uint moveTableLookup(uint current, uint move) const {
            return _moveTable[current*_nbMove + _transposedMoves[move]];
        }
        inline uint moveTableLookup(uint current, AXIS axis, uint nb) const {
            return moveTableLookup(current, (uint)axis*3 + nb - 1);
        }
        inline uint size() const {
            return _size;
        }
        virtual uint fromCube(const Cube& cube) const = 0;
        virtual Cube toCube(uint coord) const = 0;

    protected:
        uint _size;
        uint* _moveTable;
        uint _nbMove;
        std::vector<uint> _transposedMoves;
};

#endif

