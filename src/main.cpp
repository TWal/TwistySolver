#include <cstdio>

#include "Cube.h"
#include "Solver333.h"
#include "Solver444.h"
#include "Parser333.h"
#include "Parser444.h"
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

template<typename Solver, typename Parser>
void queryAndSolve(int argc, char** argv) {
    printf("Building move tables and pruning tables...\n");
    Solver solver;
    printf("Done.\n");
    Cube cube;

    if(argc == 1) {
        printf("Use OpenCV? ");
        int result = _getche();
        printf("\n");
        if(result == 'y' || result == 'Y' || result == 'o' || result == 'O') {
            std::vector<std::string> output;
            AXIS axisOrder[6] = {U, F, R, B, L, D};
            Vision viz(1, true);
            viz.scanCube(output);
            cube = Parser::parse(output, axisOrder);
        } else {
            cube = Parser::queryCube();
        }
    } else {
        cube = Parser::parseArgs(argc, argv);
    }

    solver.solve(cube, [](const std::vector<uint>& solution) {
        printSolution(solution);
        return true;
    });
}

int main(int argc, char** argv) {
    printf("Cube size? ");
    int result = _getche() - '0';
    printf("\n");
    if(result == 3) {
        queryAndSolve<Solver333, Parser333>(argc, argv);
    } else if(result == 4) {
        queryAndSolve<Solver444, Parser444>(argc, argv);
    }
    return 0;
}
