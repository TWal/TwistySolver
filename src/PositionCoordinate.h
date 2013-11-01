#ifndef POSITIONCOORDINATE
#define POSITIONCOORDINATE

#include "types.h"
#include "Cube.h"
#include <unordered_set>

class PositionCoordinate {
    public:
        PositionCoordinate(uint nb, std::unordered_set<char> pieces, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*));
        ~PositionCoordinate();
        void buildMoveTable();
        inline uint moveTableLookup(uint pos, AXIS axis, uint n) const {
            return _moveTable[pos*6*3 + (uint)axis*3 + n-1];
        }
        inline uint moveTableLookup(uint pos, uint n) const {
            return _moveTable[pos*6*3 + n];
        }
        inline uint size() const {
            return _size;
        }

    private:
        uint _nb;
        std::unordered_set<char> _pieces;
        Cube(*_buildCube)(const char*);
        void(*_unbuildCube)(const Cube&, char*);
        uint* _moveTable;
        uint _size;
};

#endif

