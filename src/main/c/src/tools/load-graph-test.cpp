/*
 * Load graph - to test handling XL graphs in GraphBLAS.
 */

#include "utils.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        throw std::runtime_error{"Please provide the Matrix Market files as argument"};
    }

    char *market_file_path = argv[1];
    FILE *mmfile = fopen(market_file_path, "r");
    if (mmfile == nullptr) {
        throw std::runtime_error{"Cannot open Matrix Market file"};
    }

    LAGraph_init();

    GrB_Matrix A ;
    LAGraph_mmread(&A, mmfile) ;

    GrB_Index n ;
    GrB_Matrix_nrows (&n, A) ;

    printf("%lu\n", n);

    GrB_Matrix_free(&A);
}
