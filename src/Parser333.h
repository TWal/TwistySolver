#ifndef PARSER333_H
#define PARSER333_H

#include <vector>
#include <string>
#include "Cube.h"

class Parser333 {
    public:
        static Cube parse(const std::vector<std::string>& strings);
        static void colorToFace(const std::vector<std::string>& colors, std::vector<AXIS>& faces);
        static bool searchPiece(const std::vector<AXIS>& def, const uint* pieces, uint pieceNb, uint n, uint searchLimit, char& pieceOut);
        static uint getCornerOrient(const std::vector<AXIS>& def, CORNERS c);
        static uint getEdgeOrient(const std::vector<AXIS>& def, EDGES e);
        static bool doPieceExists(const std::vector<AXIS>& piece);
        static void sortAxis(std::vector<AXIS>& piece);
        static Cube queryCube();
        static void queryFace(std::string& s);
};

#endif

