/*
 * Triangle Count algorithm implementation in GraphBLAS.
 */

#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>

#include "utils.h"
#include "graphio.h"
#include "computation_timer.hpp"

/*
 * Result serializer function
 */
void SerializeTCResult(
    int64_t *ntri,
    const std::vector<GrB_Index> &mapping,
    const BenchmarkParameters &parameters
) {
    std::ofstream file{parameters.output_file};
    if (!file.is_open()) {
        std::cerr << "File " << parameters.output_file << " does not exists" << std::endl;
        exit(-1);
    }
    file.precision(16);
    file << std::scientific;

    size_t n = mapping.size();
    for (size_t i = 0; i < n; i++) {
        size_t original_index = mapping[i];

        file << original_index << " " << ntri[i] << std::endl;
    }
}

int64_t *LA_TC(GrB_Matrix A, bool symmetric) {
    GrB_Info info;

    GrB_Index nvals;
    OK(GrB_Matrix_nrows(&nvals, A))

    int64_t *ntri = NULL;
    ntri = (int64_t*) LAGraph_malloc ((nvals + 1), sizeof (int64_t));

    {
        ComputationTimer timer{"TC"};
        double timing[2];
        OK(LAGraph_tricount(ntri, 1, A, NULL, NULL, NULL, timing))
    }

    return ntri;
}

int main(int argc, char **argv) {
    BenchmarkParameters parameters = ParseBenchmarkParameters(argc, argv);

    LAGraph_init();
    GxB_Global_Option_set(GxB_GLOBAL_NTHREADS, parameters.thread_num);

    GrB_Matrix A = ReadMatrixMarket(parameters);
    std::vector<GrB_Index> mapping = ReadMapping(parameters);

    std::cout << "Processing starts at: " << GetCurrentMilliseconds() << std::endl;
    int64_t *ntri = LA_TC(A, !parameters.directed);
    std::cout << "Processing ends at: " << GetCurrentMilliseconds() << std::endl;

    SerializeTCResult(ntri, mapping, parameters);

    GrB_Matrix_free(&A);
    LAGraph_free(ntri);
}
