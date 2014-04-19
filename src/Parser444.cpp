#include "Parser444.h"
#include "Parser333.h"
#include <unordered_map>
#include <unordered_set>
#include "myconio.h"

static const std::array<char, 12> lowEdges = {{
    0, 2, 4, 6, 9, 11, 13, 15, 16, 19, 20, 23
}};

static const std::array<char, 12> highEdges = {{
    1, 3, 5, 7, 8, 10, 12, 14, 17, 18, 21, 22
}};

//Face + Sticker to Index
#define FS2I(axis, index) (((uint)(axis))*16+(index))

//0 1 2 3
//4 5 6 7
//8 9 1011
//12131415

static const uint edgeStickers[24*2] = {
    FS2I(U, 14), FS2I(F, 2),
    FS2I(U, 13), FS2I(F, 1),
    FS2I(U, 8), FS2I(L, 2),
    FS2I(U, 4), FS2I(L, 1),
    FS2I(U, 1), FS2I(B, 2),
    FS2I(U, 2), FS2I(B, 1),
    FS2I(U, 7), FS2I(R, 2),
    FS2I(U, 11), FS2I(R, 1),
    FS2I(D, 2), FS2I(F, 14),
    FS2I(D, 1), FS2I(F, 13),
    FS2I(D, 4), FS2I(L, 14),
    FS2I(D, 8), FS2I(L, 13),
    FS2I(D, 13), FS2I(B, 14),
    FS2I(D, 14), FS2I(B, 13),
    FS2I(D, 11), FS2I(R, 14),
    FS2I(D, 7), FS2I(R, 13),
    FS2I(L, 7), FS2I(F, 4),
    FS2I(L, 11), FS2I(F, 8),
    FS2I(R, 4), FS2I(F, 7),
    FS2I(R, 8), FS2I(F, 11),
    FS2I(R, 7), FS2I(B, 4),
    FS2I(R, 11), FS2I(B, 8),
    FS2I(L, 4), FS2I(B, 7),
    FS2I(L, 8), FS2I(B, 11)
};

Cube Parser444::parse(const std::vector<std::string>& strings, AXIS axisOrder[6]) {
    std::vector<std::string> def;
    Parser333::cycleFaces(strings, def, axisOrder);
    std::vector<AXIS> faces;
    colorToFace(def, faces);

    std::vector<AXIS> corners;

    for(int i = 0; i < 6; ++i) {
        corners.push_back(faces[16*i+0]);
        corners.push_back(faces[16*i+3]);
        corners.push_back(faces[16*i+12]);
        corners.push_back(faces[16*i+15]);
    }

    std::vector<char> cperm;
    std::vector<char> cori;

    for(uint i = 0; i < 8; ++i) {
        char c;
        if(!Parser333::searchPiece(corners, Parser333::cornerStickers, i, 3, 8, 4, c)) {
            fprintf(stderr, "Could not determine what is the corner %u\n", i);
            exit(-1);
        }
        cperm.push_back(c);
        cori.push_back(Parser333::getCornerOrient(corners, (CORNERS)i));
    }

    std::vector<AXIS> centers;

    for(int i = 0; i < 6; ++i) {
        centers.push_back(faces[16*i+5]);
        centers.push_back(faces[16*i+6]);
        centers.push_back(faces[16*i+10]);
        centers.push_back(faces[16*i+9]);
    }

    int centerNb[6] = {0};

    std::vector<char> centerPerm;

    for(int i = 0; i < 24; ++i) {
        centerPerm.push_back(((int)centers[i])*4 + centerNb[centers[i]]++);
        if(centerNb[centers[i]] > 4) {
            fprintf(stderr, "Too many centers of axis %d\n", centers[i]);
            exit(-1);
        }
    }

    std::vector<char> edgePerm;

    for(int i = 0; i < 12; ++i) {
        for(int j = 0; j < 2; ++j) {
            int id = 2*i+j;
            char c;
            if(!Parser333::searchPiece(faces, edgeStickers, id, 2, 24, 16, c)) {
                fprintf(stderr, "Could not determine what is the edge %d\n", id);
                exit(-1);
            }
            uint orient = getEdgeOrient(faces, id);
            if((lowEdges[i] == id && orient == 0) || (highEdges[i] == id && orient == 1)) {
                edgePerm.push_back(lowEdges[c/2]);
            } else {
                edgePerm.push_back(highEdges[c/2]);
            }
        }
    }

    Cube cube(4);
    cube.setCenterPerm(0, 0, centerPerm.data());
    cube.setCornerOri(cori.data());
    cube.setCornerPerm(cperm.data());
    cube.setNEdgePerm(0, edgePerm.data());

    return cube;
}

void Parser444::colorToFace(const std::vector<std::string>& colors, std::vector<AXIS>& faces) {
    std::string corners;
    for(const std::string& face : colors) {
        corners.push_back(face[0]);
        corners.push_back(face[3]);
        corners.push_back(face[12]);
        corners.push_back(face[15]);
    }

    std::unordered_map<char, std::unordered_set<char>> opposites;
    std::unordered_set<char> allChars(corners.begin(), corners.end());

    for(int i = 0; i < 8; ++i) {
        char corner[3];
        for(int j = 0; j < 3; ++j) {
            corner[j] = corners[Parser333::cornerStickers[3*i+j]];
            if(!opposites.count(corner[j])) {
                opposites[corner[j]] = allChars;
                opposites[corner[j]].erase(corner[j]);
            }
        }
        for(int j = 0; j < 3; ++j) {
            opposites[corner[j]].erase(corner[(j+1)%3]);
            opposites[corner[j]].erase(corner[(j+2)%3]);
        }
    }

    for(const auto& opposite : opposites) {
        if(opposite.second.size() != 1) {
            fprintf(stderr, "Couldn't determine opposite colors: %d\n", opposite.second.size());
            exit(-1);
        }
    }

    std::unordered_map<char, AXIS> colorToFaceMap;
    colorToFaceMap[corners[Parser333::cornerStickers[0]]] = U;
    colorToFaceMap[corners[Parser333::cornerStickers[1]]] = F;
    colorToFaceMap[corners[Parser333::cornerStickers[2]]] = L;
    colorToFaceMap[*(opposites[corners[Parser333::cornerStickers[0]]].begin())] = D;
    colorToFaceMap[*(opposites[corners[Parser333::cornerStickers[1]]].begin())] = B;
    colorToFaceMap[*(opposites[corners[Parser333::cornerStickers[2]]].begin())] = R;

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

uint Parser444::getEdgeOrient(const std::vector<AXIS>& def, uint e) {
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
    if(piece[0] == R) {
        return 0;
    }
    return 1;
}


Cube Parser444::queryCube() {
    std::vector<std::string> def(6, "");
    std::string instructions[6] = {
        "Enter what you see on the upper face",
        "On the front face",
        "On the right face",
        "On the back face",
        "On the left face",
        "On the down face"
    };
    AXIS axisOrder[6] = {U, F, R, B, L, D};

    for(int i = 0; i < 6; ++i) {
        printf("%s\n", instructions[i].c_str());
        queryFace(def[i]);
    }
    return parse(def, axisOrder);
}

void Parser444::queryFace(std::string& s) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            s.push_back(_getche());
        }
        printf("\n");
    }
}

Cube Parser444::parseArgs(int argc, char** argv) {
    AXIS axisOrder[6] = {U, F, R, B, L, D};
    std::vector<std::string> def(6, "");

    if(argc <= 6) {
        fprintf(stderr, "Six faces are needed, you have %d\n", argc-1);
        exit(-1);
    }

    for(int i = 0; i < 6; ++i) {
        def[i] = argv[i+1];
        if(def[i].size() != 16) {
            fprintf(stderr, "There are 16 stickers on a face, you have %zu on the face %d\n", def[i].size(), i);
            exit(-1);
        }
    }
    return parse(def, axisOrder);
}


