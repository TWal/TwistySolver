#include "CenterPiece.h"
#include <cstring>

//U:  0  1  2  3
//F:  4  5  6  7
//R:  8  9 10 11
//D: 12 13 14 15
//B: 16 17 18 19
//L: 20 21 22 23
const char CenterPiece::permMove[3][6][24] = {
    //Outer face
    {
        //UUUUUUUUUUUUU   FFFFFFFFFFFFFF   RRRRRRRRRRRRRR   DDDDDDDDDDDDDD   BBBBBBBBBBBBBB   LLLLLLLLLLLLLL
        //0   1   2   3    4   5   6   7    8   9  10  11   12  13  14  15   16  17  18  19   20  21  22  23
        { 3,  0,  1,  2,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1}, //U
        {-1, -1, -1, -1,   7,  4,  5,  6,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1}, //F
        {-1, -1, -1, -1,  -1, -1, -1, -1,  11 , 8 , 9, 10,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1}, //R
        {-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  15, 12, 13, 14,  -1, -1, -1, -1,  -1, -1, -1, -1}, //D
        {-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  19, 16, 17, 18,  -1, -1, -1, -1}, //B
        {-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  23, 20, 21, 22}  //L
    },
    //First center
    {
        //UUUUUUUUUUUUU   FFFFFFFFFFFFFF   RRRRRRRRRRRRRR   DDDDDDDDDDDDDD   BBBBBBBBBBBBBB   LLLLLLLLLLLLLL
        //0   1   2   3    4   5   6   7    8   9  10  11   12  13  14  15   16  17  18  19   20  21  22  23
        {-1, -1, -1, -1,  -1,  9, -1, -1,  -1, 17, -1, -1,  -1, -1, -1, -1,  -1, 21, -1, -1,  -1,  5, -1, -1}, //U
        {-1, -1, -1, 22,  -1, -1, -1, -1,   3, -1, -1, -1,  -1,  8, -1, -1,  -1, -1, -1, -1,  -1, -1, 13, -1}, //F
        {-1, -1,  6, -1,  -1, -1, 14, -1,  -1, -1, -1, -1,  -1, -1, 16, -1,   2, -1, -1, -1,  -1, -1, -1, -1}, //R
        {-1, -1, -1, -1,  -1, -1, -1, 23,  -1, -1, -1,  7,  -1, -1, -1, -1,  -1, -1, -1, 11,  -1, -1, -1, 19}, //D
        {-1, 10, -1, -1,  -1, -1, -1, -1,  -1, -1, 15, -1,  -1, -1, -1, 20,  -1, -1, -1, -1,   1, -1, -1, -1}, //B
        {18, -1, -1, -1,   0, -1, -1, -1,  -1, -1, -1, -1,   4, -1, -1, -1,  -1, -1, 12, -1,  -1, -1, -1, -1}  //L
    },
    //Second center
    {
        //UUUUUUUUUUUUU   FFFFFFFFFFFFFF   RRRRRRRRRRRRRR   DDDDDDDDDDDDDD   BBBBBBBBBBBBBB   LLLLLLLLLLLLLL
        //0   1   2   3    4   5   6   7    8   9  10  11   12  13  14  15   16  17  18  19   20  21  22  23
        {-1, -1, -1, -1,   8, -1, -1, -1,  16, -1, -1, -1,  -1, -1, -1, -1,  20, -1, -1, -1,   4, -1, -1, -1}, //U
        {-1, -1, 21, -1,  -1, -1, -1, -1,  -1, -1, -1,  2,  11, -1, -1, -1,  -1, -1, -1, -1,  -1, 12, -1, -1}, //F
        {-1,  5, -1, -1,  -1, 13, -1, -1,  -1, -1, -1, -1,  -1, 19, -1, -1,  -1, -1, -1,  1,  -1, -1, -1, -1}, //R
        {-1, -1, -1, -1,  -1, -1, 22, -1,  -1, -1,  6, -1,  -1, -1, -1, -1,  -1, -1, 10, -1,  -1, -1, 18, -1}, //D
        { 9, -1, -1, -1,  -1, -1, -1, -1,  -1, 14, -1, -1,  -1, -1, 23, -1,  -1, -1, -1, -1,  -1, -1, -1,  0}, //B
        {-1, -1, -1, 17,  -1, -1, -1,  3,  -1, -1, -1, -1,  -1, -1, -1,  7,  -1, 15, -1, -1,  -1, -1, -1, -1}  //L
    }
};

CenterPiece::CenterPiece() {
    for(int i = 0; i < 24; ++i) {
        _perm[i] = i;
    }
}

CenterPiece::CenterPiece(const char* perm) {
    memcpy(_perm, perm, 24);
}

CenterPiece::CenterPiece(AXIS axis, uint layer, uint x, uint y) {
    char perms = 1;
    if(x <= layer) {
        perms |= (1 << 1);
    }
    if(y <= layer) {
        perms |= (1 << 2);
    }

    for(int i = 0; i < 24; ++i) {
        _perm[i] = -1;
    }

    for(int i = 0; i < 3; ++i) {
        if(perms & (1 << i)) {
            for(int j = 0; j < 24; ++j) {
                if(permMove[i][(int)axis][j] != -1) {
                    _perm[j] = permMove[i][(int)axis][j];
                }
            }
        }
    }

    for(int i = 0; i < 24; ++i) {
        if(_perm[i] == -1) {
            _perm[i] = i;
        }
    }
}

void CenterPiece::mult(const CenterPiece& rhs, CenterPiece& out) const {
    for(uint i = 0; i < 24; ++i) {
        out._perm[i] = _perm[(uint)rhs._perm[i]];
    }
}

void CenterPiece::makeIdentity() {
    for(int i = 0; i < 24; ++i) {
        _perm[i] = i;
    }
}

const char* CenterPiece::getPerm() const {
    return _perm;
}

void CenterPiece::setPerm(const char* perm) {
    memcpy(_perm, perm, 24);
}

