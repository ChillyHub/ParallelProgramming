#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI6File10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[20];

    if (!rank)
    {
        pt >> name;
    }

    MPI_Bcast(name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_File f;

    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    int intSize;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Offset offset = intSize * (rank - 1) * rank / 2;
    MPI_File_seek(f, offset, MPI_SEEK_SET);

    std::vector<int> src;
    if (rank)
    {
        src = std::vector<int>(ptin_iterator<int>(rank), ptin_iterator<int>());
    }

    MPI_File_write_all(f, src.data(), rank, MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
