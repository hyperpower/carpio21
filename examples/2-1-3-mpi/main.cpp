#include "mpi.h"
#include <stdio.h>

#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(nullptr, nullptr);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // std::cout << "world_size = " << world_size << std::endl;

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // std::cout << "world_rank = " << world_rank << std::endl;

    char pro_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(pro_name, &name_len);
    std::cout << "processor " << pro_name << " rank "<< world_rank << " of " << world_size << std::endl;

    MPI_Finalize();
}