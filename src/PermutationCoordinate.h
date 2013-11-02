#ifndef PERMUTATIONCOORDINATE
#define PERMUTATIONCOORDINATE

#include "types.h"
#include "Cube.h"
#include "Coordinate.h"

class PermutationCoordinate : public Coordinate {
    public:
        PermutationCoordinate(uint nb, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        virtual ~PermutationCoordinate();
        virtual uint fromCube(const Cube& cube) const;
        virtual Cube toCube(uint coord) const;
        static uint permToInt(const char* perm, int nb);
        static void intToPerm(uint in, char* perm, int nb);
    private:
        uint _nb;
        std::function<Cube (const char*)> _permToCube;
        std::function<void (const Cube&, char*)> _cubeToPerm;
        char* _tempPerm;
};

#endif

