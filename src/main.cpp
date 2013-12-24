#include <cstdio>

#include "Cube.h"
#include "Solver333.h"
#include "Parser333.h"

void printSolution(const std::vector<uint>& solution) {
    for(uint i : solution) {
        switch((AXIS)(i/3)) {
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
        switch(i%3) {
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

int main(int argc, char** argv) {
    printf("Building move tables and pruning tables...\n");
    Solver333 solver;
    printf("Done.\n");

    Cube cube;
    if(argc == 1) {
        cube = Parser333::queryCube();
    } else {
        cube = Parser333::parseArgs(argc, argv);
    }

    solver.solve(cube, [](const std::vector<uint>& solution) {
        printSolution(solution);
        return true;
    });

    return 0;
}
