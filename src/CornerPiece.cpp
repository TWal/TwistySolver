#include "CornerPiece.h"
#include <cstring>

  //{UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR}  //ID
const char CornerPiece::permMove[6][8] = {
    {UFR, UFL, UBL, UBR, DFL, DBL, DBR, DFR}, //U
    {DFL, UBL, UBR, UFL, DFR, DBL, DBR, UFR}, //F
    {UFL, UBL, UFR, DFR, DFL, DBL, UBR, DBR}, //R
    {UFL, UBL, UBR, UFR, DBL, DBR, DFR, DFL}, //D
    {UFL, UBR, DBR, UFR, DFL, UBL, DBL, DFR}, //B
    {UBL, DBL, UBR, UFR, UFL, DFL, DBR, DFR}  //L
};


const char CornerPiece::oriMove[6][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, //U
    {2, 0, 0, 1, 1, 0, 0, 2}, //F
    {0, 0, 1, 2, 0, 0, 2, 1}, //R
    {0, 0, 0, 0, 0, 0, 0, 0}, //D
    {0, 1, 2, 0, 0, 2, 1, 0}, //B
    {1, 2, 0, 0, 2, 1, 0, 0}  //L
};


CornerPiece::CornerPiece() {
    for(int i = 0; i < 8; ++i) {
        _perm[i] = i;
        _ori[i] = 0;
    }
}

CornerPiece::CornerPiece(const char* perm, const char* ori) {
    memcpy(_perm, perm, 8);
    memcpy(_ori, ori, 8);
}

CornerPiece::CornerPiece(AXIS axis) : CornerPiece(permMove[(int)axis], oriMove[(int)axis]) {
}

void CornerPiece::mult(const CornerPiece& rhs, CornerPiece& out) const {
    for(uint i = 0; i < 8; ++i) {
        out._perm[i] = _perm[(uint)rhs._perm[i]];
        out._ori[i] = (_ori[(uint)rhs._perm[i]] + rhs._ori[i]) % 3;
    }
}

const char* CornerPiece::getOri() const {
    return _ori;
}

const char* CornerPiece::getPerm() const {
    return _perm;
}

