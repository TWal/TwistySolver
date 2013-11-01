#ifndef POSITIONCOORDINATE
#define POSITIONCOORDINATE

#include "Coordinate.h"
#include <unordered_set>

class PositionCoordinate : public Coordinate {
    public:
        PositionCoordinate(uint nb, std::unordered_set<char> pieces, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        virtual ~PositionCoordinate();
        virtual uint fromCube(const Cube& cube);
        virtual Cube toCube(uint coord);
    private:
        uint _nb;
        std::unordered_set<char> _pieces;
        std::function<Cube (const char*)> _permToCube;
        std::function<void (const Cube&, char*)> _cubeToPerm;
        char* _tempPerm;
};

#endif

