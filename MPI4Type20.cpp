#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI4Type20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;

    if (rank)
    {
        int P;
        pt >> P;

        std::vector<int> B(ptin_iterator<int>(3 * N - 2), ptin_iterator<int>());
        MPI_Send(B.data(), 3 * N - 2, MPI_INT, 0, P, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status s;

        const int K = size - 1;
        const int width = 3 * N;
        const int height = N * (K / 3);

        int offset = 0;
        const int elemCounts = 2 * N - 1;
        std::vector<int> blocks(elemCounts);
        std::vector<int> offsets(elemCounts);
        for (int i = 0; i < N; ++i)
        {
            if (i == N - 1)
            {
                blocks[i * 2] = N;
                offsets[i * 2] = offset;
            }
            else
            {
                blocks[i * 2 + 0] = 1;
                blocks[i * 2 + 1] = 1;
        
                offsets[i * 2 + 0] = offset;
                offsets[i * 2 + 1] = offset + N - 1;
            }
        
            offset += width;
        }
        
        MPI_Datatype Mat;
        MPI_Type_indexed(elemCounts, blocks.data(), offsets.data(), MPI_INT, &Mat);
        MPI_Type_commit(&Mat);

        std::vector<int> A(width * height);
        for (int i = 1; i < size; ++i)
        {
            MPI_Probe(i, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
            MPI_Recv(&A[s.MPI_TAG], 1, Mat, i, s.MPI_TAG, MPI_COMM_WORLD, &s);
        }

        std::copy(A.begin(), A.end(), ptout_iterator<int>());
    }
}
