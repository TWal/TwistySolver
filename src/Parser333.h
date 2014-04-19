#ifndef PARSER333_H
#define PARSER333_H

#include <vector>
#include <string>
#include "Cube.h"

class Parser333 {
    public:
        static Cube parse(const std::vector<std::string>& strings, AXIS axisOrder[6]);
        static void cycleFaces(const std::vector<std::string>& strings, std::vector<std::string>& output, AXIS axisOrder[6]);
        static void colorToFace(const std::vector<std::string>& colors, std::vector<AXIS>& faces);
        static bool searchPiece(const std::vector<AXIS>& def, const uint* pieces, uint pieceNb, uint n, uint searchLimit, uint nbPerFace, char& pieceOut);
        static uint getCornerOrient(const std::vector<AXIS>& def, CORNERS c);
        static uint getEdgeOrient(const std::vector<AXIS>& def, EDGES e);
        static bool doPieceExists(const std::vector<AXIS>& piece);
        static void sortAxis(std::vector<AXIS>& piece);
        static Cube queryCube();
        static void queryFace(std::string& s);
        static Cube parseArgs(int argc, char** argv);

        static const uint cornerStickers[8*3];
        static const uint edgeStickers[12*2];
        static const uint cornerTwistFB[8];
};

#endif

