#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

enum CORNERS {
    UFL = 0, UBL, UBR, UFR, DFL, DBL, DBR, DFR
};
enum EDGES {
    UF = 0, UL, UB, UR, DF, DL, DB, DR, FL, FR, BR, BL
};
enum AXIS {
    U = 0, F, R, D, B, L
};

#endif

