#include "PositionCoordinate.h"
#include "Utils.h"

PositionCoordinate::PositionCoordinate(uint nb, std::unordered_set<char> pieces, std::function<Cube (const char*)> permToCube, std::function<void (const Cube&, char*)> cubeToPerm) :
    Coordinate(Utils::cnk(nb, pieces.size())),
    _nb(nb),
    _pieces(pieces),
    _permToCube(permToCube),
    _cubeToPerm(cubeToPerm),
    _tempPerm(new char[_nb]) {

    _init([this](uint coord) {
        return toCube(coord);
    }, [this](const Cube& cube) {
        return fromCube(cube);
    });
}

PositionCoordinate::~PositionCoordinate() {
    delete[] _tempPerm;
}

uint PositionCoordinate::fromCube(const Cube& cube) {
    _cubeToPerm(cube, _tempPerm);
    return Utils::posToInt(_tempPerm, _nb, _pieces);
}

Cube PositionCoordinate::toCube(uint coord) {
    Utils::intToPos(coord, _tempPerm, _nb, _pieces);
    return _permToCube(_tempPerm);
}

