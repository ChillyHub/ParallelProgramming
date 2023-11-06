#include "pt4.h"

#include "mpi.h"

#include <vector>
#include <algorithm>

void Solve()
{
    Task("MPI6File20");
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

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset = doubleSize * (size + rank + 2) * (size - rank - 1) / 2;
    MPI_File_set_view(f, offset, MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    std::vector<double> src(ptin_iterator<double>(rank + 1), ptin_iterator<double>());
    std::reverse(src.begin(), src.end());
    MPI_File_write_all(f, src.data(), src.size(), MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
