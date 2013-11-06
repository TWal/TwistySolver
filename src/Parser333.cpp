#include "Parser333.h"
#include <unordered_map>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
#include "myconio.h"

//U     012
//FRBL  345
//D     678

//Face + Sticker to Index
#define FS2I(axis, index) (((uint)(axis))*9+(index))

const uint cornerStickers[8*3] = {
    FS2I(U, 6), FS2I(F, 0), FS2I(L, 2), //UFL
    FS2I(U, 0), FS2I(B, 2), FS2I(L, 0), //UBL
    FS2I(U, 2), FS2I(B, 0), FS2I(R, 2), //UBR
    FS2I(U, 8), FS2I(F, 2), FS2I(R, 0), //UFR
    FS2I(D, 0), FS2I(F, 6), FS2I(L, 8), //DFL
    FS2I(D, 6), FS2I(B, 8), FS2I(L, 6), //DBL
    FS2I(D, 8), FS2I(B, 6), FS2I(R, 8), //DBR
    FS2I(D, 2), FS2I(F, 8), FS2I(R, 6)  //DFR
};

const uint edgeStickers[12*2] = {
    FS2I(U, 7), FS2I(F, 1), //UF
    FS2I(U, 3), FS2I(L, 1), //UL
    FS2I(U, 1), FS2I(B, 1), //UB
    FS2I(U, 5), FS2I(R, 1), //UR
    FS2I(D, 1), FS2I(F, 7), //DF
    FS2I(D, 3), FS2I(L, 7), //DL
    FS2I(D, 7), FS2I(B, 7), //DB
    FS2I(D, 5), FS2I(R, 7), //DR
    FS2I(F, 3), FS2I(L, 5), //FL
    FS2I(F, 5), FS2I(R, 3), //FR
    FS2I(B, 3), FS2I(R, 5), //BR
    FS2I(B, 5), FS2I(L, 3)  //BL
};

const uint cornerTwistFB[12] = {
    1, //UFL
    2, //UBL
    1, //UBR
    2, //UFR
    2, //DFL
    1, //DBL
    2, //DBR
    1  //DFR
};

Cube Parser333::parse(const std::vector<std::string>& strings) {
    std::vector<AXIS> faces;
    colorToFace(strings, faces);
    std::vector<char> cperm;
    std::vector<char> eperm;
    std::vector<char> cori;
    std::vector<char> eori;

    for(uint i = 0; i < 8; ++i) {
        char c;
        if(!searchPiece(faces, cornerStickers, i, 3, 8, c)) {
            fprintf(stderr, "Could not determine what is the corner %u\n", i);
            exit(-1);
        }
        cperm.push_back(c);
        cori.push_back(getCornerOrient(faces, (CORNERS)i));
    }

    for(uint i = 0; i < 12; ++i) {
        char e;
        if(!searchPiece(faces, edgeStickers, i, 2, 12, e)) {
            fprintf(stderr, "Could not determine what is the edge %u\n", i);
            exit(-1);
        }
        eperm.push_back(e);
        eori.push_back(getEdgeOrient(faces, (EDGES)i));
    }

    return Cube(cperm.data(), eperm.data(), cori.data(), eori.data());
}

void Parser333::colorToFace(const std::vector<std::string>& colors, std::vector<AXIS>& faces) {
    if(colors.size() != 6) {
        fprintf(stderr, "A cube has 6 sides, you have %zu\n", colors.size());
        exit(-1);
    }
    std::unordered_map<uint, char> colorToFaceMap;
    for(int i = 0; i < 6; ++i) {
        if(colors[i].size() != 9) {
            fprintf(stderr, "A face has 9 stickers, %c has %zu\n", (AXIS)i, colors[i].size());
            exit(-1);
        }
        colorToFaceMap[colors[i][4]] = (AXIS)i;
    }
    if(colorToFaceMap.size() != 6) {
        fprintf(stderr, "A cube has 6 different colors, you have %zu\n", colorToFaceMap.size());
        exit(-1);
    }
    for(const std::string& str : colors) {
        for(char c : str) {
            if(colorToFaceMap.count(c) == 0) {
                fprintf(stderr, "Unknown color: %c\n", c);
                exit(-1);
            }
            faces.push_back((AXIS)colorToFaceMap[c]);
        }
    }
}

bool Parser333::searchPiece(const std::vector<AXIS>& def, const uint* pieces, uint pieceNb, uint n, uint searchLimit, char& pieceOut) {
    std::vector<AXIS> pieceAxis;
    for(uint i = 0; i < n; ++i) {
        pieceAxis.push_back(def[pieces[pieceNb*n+i]]);
    }
    if(!doPieceExists(pieceAxis)) {
        return false;
    }
    std::vector<AXIS> sortedPiece = pieceAxis;
    sortAxis(sortedPiece);

    for(uint i = 0; i < searchLimit; ++i) {
        bool found = true;
        for(uint j = 0; j < n; ++j) {
            if(sortedPiece[j] != (pieces[i*n+j]/9)) {
                found = false;
                break;
            }
        }
        if(found) {
            pieceOut = i;
            return true;
        }
    }
    return false;
}

uint Parser333::getCornerOrient(const std::vector<AXIS>& def, CORNERS c) {
    std::vector<uint> piece;
    for(uint i = 0; i < 3; ++i) {
        piece.push_back((uint)def[cornerStickers[c*3+i]]%3);
    }
    if(piece[0] == U) {
        return 0;
    } else if(piece[1] == U) {
        return cornerTwistFB[c];
    } else {
        return 3-cornerTwistFB[c];
    }
}

uint Parser333::getEdgeOrient(const std::vector<AXIS>& def, EDGES e) {
    std::vector<uint> piece;
    for(uint i = 0; i < 2; ++i) {
        piece.push_back((uint)def[edgeStickers[e*2+i]]%3);
    }
    if(piece[0] == U) {
        return 0;
    }
    if(piece[1] == U) {
        return 1;
    }
    if(piece[0] == F) {
        return 0;
    }
    return 1;
}

bool Parser333::doPieceExists(const std::vector<AXIS>& piece) {
    std::unordered_set<uint> existing;
    for(AXIS f : piece) {
        uint newEntry = (uint)f % 3;
        if(existing.count(newEntry) == 1) {
            return false;
        }
        existing.insert(newEntry);
    }
    return true;
}

void Parser333::sortAxis(std::vector<AXIS>& piece) {
    std::sort(piece.begin(), piece.end(), [](AXIS a, AXIS b) {
        return (a%3) < (b%3);
    });
}

Cube Parser333::queryCube() {
    std::vector<std::string> def(6, "");
    std::string instructions[6] = {
        "Enter what you see on the U face",
        "On the front face",
        "On the right face",
        "On the back face",
        "On the left face",
        "On the down face"
    };
    AXIS axisOrder[6] = {U, F, R, B, L, D};

    for(int i = 0; i < 6; ++i) {
        printf("%s\n", instructions[i].c_str());
        queryFace(def[axisOrder[i]]);
    }
    return parse(def);
}

void Parser333::queryFace(std::string& s) {
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            s.push_back(_getche());
        }
        printf("\n");
    }
}

Cube Parser333::parseArgs(int argc, char** argv) {
    AXIS axisOrder[6] = {U, F, R, B, L, D};
    std::vector<std::string> def(6, "");

    if(argc <= 6) {
        fprintf(stderr, "Six faces are needed, you have %d\n", argc-1);
        exit(-1);
    }

    for(int i = 0; i < 6; ++i) {
        def[axisOrder[i]] = argv[i+1];
        if(def[axisOrder[i]].size() != 9) {
            fprintf(stderr, "There a 9 stickers on a face, you have %zu on the face %d\n", def[axisOrder[i]].size(), i);
            exit(-1);
        }
    }
    return parse(def);
}

