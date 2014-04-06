#include <cstdio>

#include "Cube.h"
#include "Solver333.h"
#include "Solver444.h"
#include "Parser333.h"
#include "Utils.h"
#include "Vision.h"
#include "myconio.h"

void printSolution(const std::vector<uint>& solution) {
    for(uint i : solution) {
        if(Utils::getLayer(i) == 0) {
            switch((AXIS)Utils::getAxis(i)) {
                case U:
                    printf("U");
                    break;
                case D:
                    printf("D");
                    break;
                case F:
                    printf("F");
                    break;
                case B:
                    printf("B");
                    break;
                case R:
                    printf("R");
                    break;
                case L:
                    printf("L");
                    break;
                default:
                    break;
            }
        } else {
            if(Utils::getLayer(i) != 1) {
                printf("%d", Utils::getLayer(i));
            }
            switch((AXIS)Utils::getAxis(i)) {
                case U:
                    printf("u");
                    break;
                case D:
                    printf("d");
                    break;
                case F:
                    printf("f");
                    break;
                case B:
                    printf("b");
                    break;
                case R:
                    printf("r");
                    break;
                case L:
                    printf("l");
                    break;
                default:
                    break;
            }
        }
        switch(Utils::getNb(i)) {
            case 1:
                printf("2");
                break;
            case 2:
                printf("'");
                break;
            default:
                break;
        }
        printf(" ");
    }
    printf(" (%zd)\n", solution.size());
}

//#define USE_444

int main(int argc, char** argv) {
    printf("Building move tables and pruning tables...\n");
#ifndef USE_444
    Solver333 solver;
#else
    Solver444 solver;
#endif
    printf("Done.\n");

#ifndef USE_444
    Cube cube(3);
    if(argc == 1) {
        printf("Use OpenCV? ");
        int result = _getche();
        printf("\n");
        if(result == 'y' || result == 'Y' || result == 'o' || result == 'O') {
            std::vector<std::string> output;
            AXIS axisOrder[6] = {U, F, R, B, L, D};
            Vision viz(1, true);
            viz.scanCube(output);
            cube = Parser333::parse(output, axisOrder);
        } else {
            cube = Parser333::queryCube();
        }
    } else {
        cube = Parser333::parseArgs(argc, argv);
    }
#else
    Cube cube(4);
    cube.makeIdentity();
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 6; ++j) {
            cube.applyMult(Cube(4, (AXIS)j, 0));
            cube.applyMult(Cube(4, (AXIS)j, 1));
        }
    }

#endif

    solver.solve(cube, [](const std::vector<uint>& solution) {
        printSolution(solution);
        return true;
    });

    return 0;
}
