#include "Coordinate.h"
#include "Utils.h"
#include <algorithm>

Coordinate::Coordinate(uint size) :
    _size(size),
    _moveTable(nullptr) {
}

Coordinate::~Coordinate() {
    delete _moveTable;
}

void Coordinate::buildMoveTable(const std::vector<uint>& allowedMoves, const CubeProperties& props, const std::string& path) {
    _nbMove = allowedMoves.size();
    _moveTable = new uint[_size * _nbMove];
    _transposedMoves.assign(*std::max_element(allowedMoves.begin(), allowedMoves.end()) + 1,  -1);
    for(uint i = 0; i < _nbMove; ++i) {
        _transposedMoves[allowedMoves[i]] = i;
    }

    if(!path.empty()) {
        FILE* file = fopen(path.c_str(), "rb");
        if(file) {
            fseek(file, 0, SEEK_END);
            if(ftell(file) != sizeof(uint) * _size * _nbMove) {
                fprintf(stderr, "Size of file \"%s\" does not correspond to the size of the movetable\n", path.c_str());
            } else {
                rewind(file);
                fread(_moveTable, sizeof(uint), _size * _nbMove, file);
                return;
            }
        } else {
            fprintf(stderr, "Couldn't read file \"%s\"\n", path.c_str());
        }
    }

    for(uint i = 0; i < _size; ++i) {
        Cube c = toCube(i);
        for(uint move : allowedMoves) {
            uint layer = Utils::getLayer(move);
            uint axis = Utils::getAxis(move);
            uint nb = Utils::getNb(move);
            Cube cubeMove = props.makeCube((AXIS)axis, layer);
            Cube c2 = c;
            for(uint i = 0; i <= nb; ++i) {
                c2.applyMult(cubeMove);
            }
            _moveTable[i*_nbMove + _transposedMoves[move]] = fromCube(c2);
        }
    }

    if(!path.empty()) {
        FILE* file = fopen(path.c_str(), "wb");
        if(!file) {
            fprintf(stderr, "Couldn't write to file \"%s\"\n", path.c_str());
            return;
        }
        fwrite(_moveTable, sizeof(uint), _size * _nbMove, file);
        fclose(file);
    }
}

