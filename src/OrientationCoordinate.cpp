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
    return Utils::orientToInt(_tempOrient, _nb, _base);
}

Cube OrientationCoordinate::toCube(uint coord) {
    Utils::intToOrient(coord, _tempOrient, _nb, _base);
    return _permToCube(_tempOrient);
}

