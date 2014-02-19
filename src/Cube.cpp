#include "Cube.h"

Cube::Cube() {
}

Cube::Cube(uint size) {
}

Cube::Cube(uint size, const char* cp, const char* ep, const char* co, const char* eo) :
    _corners(cp, co),
    _edges(ep, eo) {
}

Cube::Cube(uint size, AXIS axis, uint layer) :
    _corners(axis),
    _edges(axis) {
}

void Cube::mult(const Cube& rhs, Cube& out) const {
    _corners.mult(rhs._corners, out._corners);
    _edges.mult(rhs._edges, out._edges);
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

