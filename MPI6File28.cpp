#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File28");
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

    int N;
    pt >> N;

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Datatype t1, t2;
    MPI_Type_vector(size / 2, 1, size, MPI_DOUBLE, &t1);
    MPI_Type_create_resized(t1, 0, doubleSize * size * size / 2, &t2);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset = doubleSize * (N - 1);
    MPI_File_set_view(f, offset, MPI_DOUBLE, t2, "native", MPI_INFO_NULL);

    std::vector<double> src(ptin_iterator<double>(size / 2), ptin_iterator<double>());
    MPI_File_write_all(f, src.data(), size / 2, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
