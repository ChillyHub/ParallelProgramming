#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File5");
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

    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    if (rank)
    {
        int num;
        int count = 0;
        do
        {
            MPI_File_read(f, &num, 1, MPI_INT, MPI_STATUS_IGNORE);
            count++;

            pt << num;
        } while (num != rank);

        MPI_Offset offset;
        MPI_File_get_position(f, &offset);

        MPI_File_seek(f, -offset, MPI_SEEK_END);

        for (int i = 0; i < count; ++i)
        {
            MPI_File_read(f, &num, 1, MPI_INT, MPI_STATUS_IGNORE);

            pt << num;
        }
    }

    MPI_File_close(&f);
}
