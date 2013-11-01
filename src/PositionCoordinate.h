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
        static uint posToInt(const char* perm, int nb, const std::unordered_set<char>& pieces);
        static void intToPos(uint in, char* perm, int nb, const std::unordered_set<char>& pieces);
    private:
        uint _nb;
        std::unordered_set<char> _pieces;
        std::function<Cube (const char*)> _permToCube;
        std::function<void (const Cube&, char*)> _cubeToPerm;
        char* _tempPerm;
};

#endif

