#ifndef ORIENTATIONCOORDINATE
#define ORIENTATIONCOORDINATE

#include "Coordinate.h"

class OrientationCoordinate : public Coordinate {
    public:
        OrientationCoordinate(uint nb, uint base, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        virtual ~OrientationCoordinate();
        virtual uint fromCube(const Cube& cube);
        virtual Cube toCube(uint coord);
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

