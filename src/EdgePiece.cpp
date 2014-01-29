#include "EdgePiece.h"
#include <cstring>


  //{UF, UL, UB, UR, DF, DL, DB, DR, FL, FR, BR, BL}  //ID
const char EdgePiece::permMove[6][12] = {
    {UR, UF, UL, UB, DF, DL, DB, DR, FL, FR, BR, BL}, //U
    {FL, UL, UB, UR, FR, DL, DB, DR, DF, UF, BR, BL}, //F
    {UF, UL, UB, FR, DF, DL, DB, BR, FL, DR, UR, BL}, //R
    {UF, UL, UB, UR, DL, DB, DR, DF, FL, FR, BR, BL}, //D
    {UF, UL, BR, UR, DF, DL, BL, DR, FL, FR, DB, UB}, //B
    {UF, BL, UB, UR, DF, FL, DB, DR, UL, FR, BR, DL}  //L
};


const char EdgePiece::oriMove[6][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //U
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0}, //F
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //R
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //D
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1}, //B
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  //L
};

EdgePiece::EdgePiece() {
    for(int i = 0; i < 12; ++i) {
        _perm[i] = i;
        _ori[i] = 0;
    }
}

EdgePiece::EdgePiece(const char* perm, const char* ori) {
    memcpy(_perm, perm, 12);
    memcpy(_ori, ori, 12);
}

EdgePiece::EdgePiece(AXIS axis) : EdgePiece(permMove[(int)axis], oriMove[(int)axis]) {
}


void EdgePiece::mult(const EdgePiece& rhs, EdgePiece& out) const {
    for(uint i = 0; i < 12; ++i) {
        out._perm[i] = _perm[(uint)rhs._perm[i]];
        out._ori[i] = (_ori[(uint)rhs._perm[i]] + rhs._ori[i]) % 2;
    }
}

const char* EdgePiece::getOri() const {
    return _ori;
}

const char* EdgePiece::getPerm() const {
    return _perm;
}

