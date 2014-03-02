#include "Cube.h"

Cube::Cube() {
}

Cube::Cube(uint size) {
    if(size > 3) {
        int nbCenter;
        if(size % 2 == 0) {
            nbCenter = ((size-2)/2);
            nbCenter *= nbCenter;
        } else {
            nbCenter = ((size-3)/2);
            nbCenter *= nbCenter;
            nbCenter += (size-3)/2;
        }
        _centers.resize(nbCenter);
        _nedges.resize((size-2)/2);
    }
    _size = size;
}

Cube::Cube(uint size, const char* cp, const char* ep, const char* co, const char* eo) :
    _corners(cp, co),
    _edges(ep, eo),
    _size(size) {
}

Cube::Cube(uint size, AXIS axis, uint layer) :
    _corners(axis),
    _edges(axis),
    _size(size) {
    for(int i = 0; i < (_size-1)/2; ++i) {
        for(int j = 0; j < (_size-2)/2; ++j) {
            _centers.push_back(CenterPiece(axis, layer, i+1, j+1));
        }
    }
    for(int i = 0; i < (size-2)/2; ++i) {
        _nedges.push_back(NEdgePiece(axis, layer, i+1));
    }
}

void Cube::mult(const Cube& rhs, Cube& out) const {
    _corners.mult(rhs._corners, out._corners);
    if(_size % 2 == 1) {
        _edges.mult(rhs._edges, out._edges);
    }
    for(uint i = 0; i < _centers.size(); ++i) {
        _centers[i].mult(rhs._centers[i], out._centers[i]);
    }
    for(uint i = 0; i < _nedges.size(); ++i) {
        _nedges[i].mult(rhs._nedges[i], out._nedges[i]);
    }
}

void Cube::makeIdentity() {
    _corners.makeIdentity();
    _edges.makeIdentity();
    for(CenterPiece& center : _centers) {
        center.makeIdentity();
    }
    for(NEdgePiece& edge : _nedges) {
        edge.makeIdentity();
    }
}

void Cube::applyMult(const Cube& rhs) {
    Cube out = *this;
    mult(rhs, out);
    *this = out;
}

const char* Cube::getCornerPerm() const {
    return _corners.getPerm();
}

const char* Cube::getEdgePerm() const {
    return _edges.getPerm();
}

const char* Cube::getCornerOri() const {
    return _corners.getOri();
}

const char* Cube::getEdgeOri() const {
    return _edges.getOri();
}

const char* Cube::getCenterPerm(uint x, uint y) const {
    return _centers[((_size-2)/2)*x+y].getPerm();
}

const char* Cube::getNEdgePerm(uint x) const {
    return _nedges[x].getPerm();
}

Cube& Cube::setCornerPerm(const char* data) {
    _corners.setPerm(data);
    return *this;
}

Cube& Cube::setEdgePerm(const char* data) {
    _edges.setPerm(data);
    return *this;
}

Cube& Cube::setCornerOri(const char* data) {
    _corners.setOri(data);
    return *this;
}

Cube& Cube::setEdgeOri(const char* data) {
    _edges.setOri(data);
    return *this;
}

Cube& Cube::setCenterPerm(uint x, uint y, const char* data) {
    _centers[((_size-2)/2)*x+y].setPerm(data);
    return *this;
}

Cube& Cube::setNEdgePerm(uint x, const char* data) {
    _nedges[x].setPerm(data);
    return *this;
}

