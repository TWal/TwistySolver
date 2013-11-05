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
    printf("\n");
}

int main() {
    printf("Building move tables and pruning tables...\n");
    Solver333 solver;
    printf("Done.\n");

    Cube cube = Parser333::queryCube();

    std::vector<uint> solution = solver.solve(cube);
    printSolution(solution);

    return 0;
}
