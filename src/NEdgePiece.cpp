#include "NEdgePiece.h"
#include <cstring>

const char NEdgePiece::permMove[2][6][24] = {
    {
        // UF      UL      UB      UR        DF     DL      DB      DR      FL      FR      BR      BL
        //0   1   2   3   4   5   6   7    8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
        { 6,  7,  0,  1,  2,  3,  4,  5,  -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1}, //U
        {16, 17, -1, -1, -1, -1, -1, -1,  18, 19, -1, -1, -1, -1, -1, -1,   9,  8,  1,  0, -1, -1, -1, -1}, //F
        {-1, -1, -1, -1, -1, -1, 18, 19,  -1, -1, -1, -1, -1, -1, 20, 21,  -1, -1, 15, 14,  7,  6, -1, -1}, //R
        {-1, -1, -1, -1, -1, -1, -1, -1,  10, 11, 12, 13, 14, 15,  8,  9,  -1, -1, -1, -1, -1, -1, -1, -1}, //D
        {-1, -1, -1, -1, 20, 21, -1, -1,  -1, -1, -1, -1, 22, 23, -1, -1,  -1, -1, -1, -1, 13, 12,  5,  4}, //B
        {-1, -1, 22, 23, -1, -1, -1, -1,  -1, -1, 16, 17, -1, -1, -1, -1,   3,  2, -1, -1, -1, -1, 11, 10}  //L
    },
    {
        {-1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,  18, -1, 20, -1, 22, -1, 16, -1}, //U
        {-1, -1, 10, -1, -1, -1, -1,  2,  -1, -1, 15, -1, -1, -1, -1,  7,  -1, -1, -1, -1, -1, -1, -1, -1}, //F
        { 8, -1, -1, -1, -1,  0, -1, -1,  13, -1, -1, -1, -1,  5, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1}, //R
        {-1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,  -1, 23, -1, 17, -1, 19, -1, 21}, //D
        {-1, -1, -1,  6, -1, -1, 14, -1,  -1, -1, -1,  3, -1, -1, 11, -1,  -1, -1, -1, -1, -1, -1, -1, -1}, //B
        {-1,  4, -1, -1, 12, -1, -1, -1,  -1,  1, -1, -1,  9, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1}  //L
    }
};

NEdgePiece::NEdgePiece() {
    for(int i = 0; i < 24; ++i) {
        _perm[i] = i;
    }
}

NEdgePiece::NEdgePiece(const char* perm) {
    memcpy(_perm, perm, 24);
}

NEdgePiece::NEdgePiece(AXIS axis, uint layer, uint pos) {
    for(int i = 0; i < 24; ++i) {
        _perm[i] = -1;
    }

    for(int i = 0; i < (layer >= pos ? 2 : 1); ++i) {
        for(int j = 0; j < 24; ++j) {
            if(permMove[i][(int)axis][j] != -1) {
                _perm[j] = permMove[i][(int)axis][j];
            }
        }
    }

    for(int i = 0; i < 24; ++i) {
        if(_perm[i] == -1) {
            _perm[i] = i;
        }
    }
}


void NEdgePiece::mult(const NEdgePiece& rhs, NEdgePiece& out) const {
    for(uint i = 0; i < 24; ++i) {
        out._perm[i] = _perm[(uint)rhs._perm[i]];
    }
}

void NEdgePiece::makeIdentity() {
    for(int i = 0; i < 24; ++i) {
        _perm[i] = i;
    }
}

const char* NEdgePiece::getPerm() const {
    return _perm;
}

void NEdgePiece::setPerm(const char* perm) {
    memcpy(_perm, perm, 24);
}

void NEdgePiece::convertToGroupedEdges(char* perm) const {
    //Suppose the edges are grouped and oriented
    for(int i = 0; i < 12; ++i) {
        perm[i] = _perm[2*i]/2;
    }
}

