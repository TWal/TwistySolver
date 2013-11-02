#include "OrientationCoordinate.h"
#include "Utils.h"

OrientationCoordinate::OrientationCoordinate(uint nb, uint base, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(Utils::pow(base, nb - 1)),
    _nb(nb),
    _base(base),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempOrient(new char[_nb]) {

    _init([this](uint coord) {
        return toCube(coord);
    }, [this](const Cube& cube) {
        return fromCube(cube);
    });
}

OrientationCoordinate::~OrientationCoordinate() {
    delete[] _tempOrient;
}

uint OrientationCoordinate::fromCube(const Cube& cube) {
    _cubeToPerm(cube, _tempOrient);
    return orientToInt(_tempOrient, _nb, _base);
}

Cube OrientationCoordinate::toCube(uint coord) {
    intToOrient(coord, _tempOrient, _nb, _base);
    return _permToCube(_tempOrient);
}

uint OrientationCoordinate::orientToInt(const char* orients, int nb, char base) {
    uint r = 0;
    for(int i = 0; i < nb-1; ++i) {
        r = r * base + orients[i];
    }
    return r;
}

void OrientationCoordinate::intToOrient(uint in, char* orients, int nb, char base) {
    uint sum = 0;
    for(int i = nb-2; i >= 0; --i) {
        orients[i] = in % base;
        sum += orients[i];
        in /= base;
    }
    orients[nb-1] = (base*nb - sum) % base;
}

