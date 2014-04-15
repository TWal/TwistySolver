#ifndef PARITYCOORDINATE_H
#define PARITYCOORDINATE_H

#include "types.h"
#include "Cube.h"
#include "Coordinate.h"

class ParityCoordinate : public Coordinate {
    public:
        ParityCoordinate(uint nb, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm);
        ParityCoordinate(uint nb, const std::vector<uint>& allowedMoves, const CubeProperties& props, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm, const std::string& path = "");
        virtual ~ParityCoordinate();
        virtual uint fromCube(const Cube& cube) const;
        virtual Cube toCube(uint coord) const;
        static uint parToInt(const char* perm, int nb);
        static void intToPar(uint in, char* perm, int nb);
    private:
        uint _nb;
        std::function<Cube (const char*)> _permToCube;
        std::function<void (const Cube&, char*)> _cubeToPerm;
        char* _tempPerm;
};

#endif

