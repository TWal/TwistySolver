#include "PermutationCoordinate.h"
#include "Utils.h"

PermutationCoordinate::PermutationCoordinate(uint nb, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(Utils::fac(nb)),
    _nb(nb),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempPerm(new char[_nb]) {

    _init([this](uint coord) {
        return toCube(coord);
    }, [this](const Cube& cube) {
        return fromCube(cube);
    });
}

PermutationCoordinate::~PermutationCoordinate() {
    delete[] _tempPerm;
}

uint PermutationCoordinate::fromCube(const Cube& cube) {
    _cubeToPerm(cube, _tempPerm);
    return Utils::permToInt(_tempPerm, _nb);
}

Cube PermutationCoordinate::toCube(uint coord) {
    Utils::intToPerm(coord, _tempPerm, _nb);
    return _permToCube(_tempPerm);
}

