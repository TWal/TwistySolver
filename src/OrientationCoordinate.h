#ifndef ORIENTATIONCOORDINATE
#define ORIENTATIONCOORDINATE

#include "Coordinate.h"

class OrientationCoordinate : public Coordinate {
    public:
        OrientationCoordinate(uint nb, uint base, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        OrientationCoordinate(uint nb, uint base, const std::vector<uint>& allowedMoves, const CubeProperties& props, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        virtual ~OrientationCoordinate();
        virtual uint fromCube(const Cube& cube) const;
        virtual Cube toCube(uint coord) const;
        static uint orientToInt(const char* orients, int nb, char base);
        static void intToOrient(uint in, char* orients, int nb, char base);
    private:
        uint _nb;
        uint _base;
        std::function<Cube (const char*)> _permToCube;
        std::function<void (const Cube&, char*)> _cubeToPerm;
        char* _tempOrient;
};

#endif

