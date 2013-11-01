#ifndef PERMUTATIONCOORDINATE
#define PERMUTATIONCOORDINATE

#include "types.h"
#include "Cube.h"

class PermutationCoordinate {
    public:
        PermutationCoordinate(uint nb, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*));
        ~PermutationCoordinate();
        void buildMoveTable();
        inline uint moveTableLookup(uint perm, AXIS axis, uint n) const {
            return _moveTable[perm*6*3 + (uint)axis*3 + n-1];
        }
        inline uint moveTableLookup(uint perm, uint n) const {
            return _moveTable[perm*6*3 + n];
        }
        inline uint size() const {
            return _size;
        }

    private:
        uint _nb;
        Cube(*_buildCube)(const char*);
        void(*_unbuildCube)(const Cube&, char*);
        uint* _moveTable;
        uint _size;
};

#endif

