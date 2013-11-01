#ifndef ORIENTATIONCOORDINATE
#define ORIENTATIONCOORDINATE

#include "types.h"
#include "Cube.h"

class OrientationCoordinate {
    public:
        OrientationCoordinate(uint nb, uint base, Cube(*buildCube)(const char*), void(*unbuildCube)(const Cube&, char*));
        ~OrientationCoordinate();
        void buildMoveTable();
        inline uint moveTableLookup(uint orient, AXIS axis, uint n) const {
            return _moveTable[orient*6*3 + (uint)axis*3 + n-1];
        }
        inline uint moveTableLookup(uint orient, uint n) const {
            return _moveTable[orient*6*3 + n];
        }
        inline uint size() const {
            return _size;
        }

    private:
        uint _nb;
        uint _base;
        Cube(*_buildCube)(const char*);
        void(*_unbuildCube)(const Cube&, char*);
        uint* _moveTable;
        uint _size;
};

#endif

