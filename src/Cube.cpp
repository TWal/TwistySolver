#include "Cube.h"

#include <string.h>
#include "Utils.h"
#include "types.h"

  //{UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR}  //ID
const char Cube::cpermMove[6][8] = {
    {UFR, UFL, UBL, UBR, DFL, DBL, DBR, DFR}, //U
    {DFL, UBL, UBR, UFL, DFR, DBL, DBR, UFR}, //F
    {UFL, UBL, UFR, DFR, DFL, DBL, UBR, DBR}, //R
    {UFL, UBL, UBR, UFR, DBL, DBR, DFR, DFL}, //D
    {UFL, UBR, DBR, UFR, DFL, UBL, DBL, DFR}, //B
    {UBL, DBL, UBR, UFR, UFL, DFL, DBR, DFR}  //L
};

  //{UF, UL, UB, UR, DF, DL, DB, DR, FL, FR, BR, BL}  //ID
const char Cube::epermMove[6][12] = {
    {UR, UF, UL, UB, DF, DL, DB, DR, FL, FR, BR, BL}, //U
    {FL, UL, UB, UR, FR, DL, DB, DR, DF, UF, BR, BL}, //F
    {UF, UL, UB, FR, DF, DL, DB, BR, FL, DR, UR, BL}, //R
    {UF, UL, UB, UR, DL, DB, DR, DF, FL, FR, BR, BL}, //D
    {UF, UL, BR, UR, DF, DL, BL, DR, FL, FR, DB, UB}, //B
    {UF, BL, UB, UR, DF, FL, DB, DR, UL, FR, BR, DL}  //L
};

const char Cube::coriMove[6][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, //U
    {2, 0, 0, 1, 1, 0, 0, 2}, //F
    {0, 0, 1, 2, 0, 0, 2, 1}, //R
    {0, 0, 0, 0, 0, 0, 0, 0}, //D
    {0, 1, 2, 0, 0, 2, 1, 0}, //B
    {1, 2, 0, 0, 2, 1, 0, 0}  //L
};

const char Cube::eoriMove[6][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //U
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0}, //F
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //R
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //D
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1}, //B
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  //L
};

Cube::Cube() {
    for(int i = 0; i < 8; ++i) {
        _cperm[i] = i;
        _cori[i] = 0;
    }

    for(int i = 0; i < 12; ++i) {
        _eperm[i] = i;
        _eori[i] = 0;
    }
}

Cube::Cube(uint cp, uint ep, uint co, uint eo) {
    Utils::intToPerm(cp, _cperm, 8);
    Utils::intToPerm(ep, _eperm, 12);
    Utils::intToOrient(co, _cori, 8, 3);
    Utils::intToOrient(eo, _eori, 12, 2);
}

Cube::Cube(const char* cp, const char* ep, const char* co, const char* eo) {
    memcpy(_cperm, cp, 8);
    memcpy(_eperm, ep, 12);
    memcpy(_cori, co, 8);
    memcpy(_eori, eo, 12);
}

Cube::Cube(AXIS axis) : Cube(cpermMove[axis], epermMove[axis], coriMove[axis], eoriMove[axis]) {
}

void Cube::cMult(const Cube& rhs, Cube& out) const {
    for(uint i = 0; i < 8; ++i) {
        out._cperm[i] = _cperm[(uint)rhs._cperm[i]];
        out._cori[i] = (_cori[(uint)rhs._cperm[i]] + rhs._cori[i]) % 3;
    }
}

void Cube::eMult(const Cube& rhs, Cube& out) const {
    for(uint i = 0; i < 12; ++i) {
        out._eperm[i] = _eperm[(uint)rhs._eperm[i]];
        out._eori[i] = (_eori[(uint)rhs._eperm[i]] + rhs._eori[i]) % 2;
    }
}

void Cube::mult(const Cube& rhs, Cube& out) const {
    cMult(rhs, out);
    eMult(rhs, out);
}

void Cube::applyCMult(const Cube& rhs) {
    Cube out = *this;
    cMult(rhs, out);
    *this = out;
}

void Cube::applyEMult(const Cube& rhs) {
    Cube out = *this;
    eMult(rhs, out);
    *this = out;
}

void Cube::applyMult(const Cube& rhs) {
    Cube out = *this;
    mult(rhs, out);
    *this = out;
}

const char* Cube::getCornerPerm() const {
    return _cperm;
}

const char* Cube::getEdgePerm() const {
    return _eperm;
}

const char* Cube::getCornerOri() const {
    return _cori;
}

const char* Cube::getEdgeOri() const {
    return _eori;
}

