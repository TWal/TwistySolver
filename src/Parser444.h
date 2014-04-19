#ifndef PARSER444_H
#define PARSER444_H

#include <vector>
#include <string>
#include "Cube.h"

class Parser444 {
    public:
        static Cube parse(const std::vector<std::string>& strings, AXIS axisOrder[6]);
        static void colorToFace(const std::vector<std::string>& colors, std::vector<AXIS>& faces);
        static uint getEdgeOrient(const std::vector<AXIS>& def, uint e);
        static Cube queryCube();
        static void queryFace(std::string& s);
        static Cube parseArgs(int argc, char** argv);
};

#endif
