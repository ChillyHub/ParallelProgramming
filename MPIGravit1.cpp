#include "pt4.h"

#include "mpi.h"

#include <cmath>

#include <vector>

#define gravity 10 // gravitational constant
#define dt 0.1     // time step
#define N 800      // number of particles
#define fmax 1     // max force value

int k;             // number of processes
int r;             // rank of the current process
int Niter;         // number of iterations

struct Particle
{
    double x, y, vx, vy;
};

struct Force
{
    double x, y;
};

Particle p[N];
Force f[N];
double m[N];

Force fTotal[N];

int pairsCount;

void Init()
{
    for (int i = 0; i < N; i++)
    {
        p[i].x = 20 * (i / 20 - 20) + 10;
        p[i].y = 20 * (i % 20 - 10) + 10;
        p[i].vx = p[i].y / 15;
        p[i].vy = -p[i].x / 50;

        m[i] = 100 + i % 100;

        f[i].x = 0;
        f[i].y = 0;
    }
}

// Implementation of a non-parallel algorithm

void CalcForces2()
{
    for (int i = 0; i < N - 1; i++)
    for (int j = i + 1; j < N; j++)
    {
        double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y,
        r_2 = 1 / (dx * dx + dy * dy),
        r_1 = sqrt(r_2),
        fabs = gravity * m[i] * m[j] * r_2;
        if (fabs > fmax) fabs = fmax;
        f[i].x += dx = fabs * dx * r_1;
        f[i].y += dy = fabs * dy * r_1;
        f[j].x -= dx;
        f[j].y -= dy;
    }
}

void MoveParticlesAndFreeForces()
{
    for (int i = 0; i < N; i++)
    {
        double dvx = f[i].x * dt / m[i],
            dvy = f[i].y * dt / m[i];
        p[i].x += (p[i].vx + dvx / 2) * dt;
        p[i].y += (p[i].vy + dvy / 2) * dt;
        p[i].vx += dvx;
        p[i].vy += dvy;
        f[i].x = 0;
        f[i].y = 0;
    }
}

void NonParallelCalc(int n)
{
    Init();
    for (int i = 0; i < n; i++)
    {
        CalcForces2();
        MoveParticlesAndFreeForces();
    }
}

// End of the non-parallel algorithm implementation

// Implementation of the parallel algorithm
// based on the "manager-worker" method

void CalcForcesPair(int wProc, int i, int j)
{
    const int offset = N / wProc;
    int ib = i * offset;
    int jb = i == j ? ib: j * offset;
    int ie = ib + offset;
    int je = jb + offset;

    for (int ii = ib; ii < ie; ++ii)
    {
        if (i == j)
        {
            jb++;
        }

	    for (int ji = jb; ji < je; ++ji)
	    {
            double dx = p[ji].x - p[ii].x;
            double dy = p[ji].y - p[ii].y;
            double r_2 = 1 / (dx * dx + dy * dy);
            double r_1 = sqrt(r_2);
            double fabs = gravity * m[ii] * m[ji] * r_2;

            if (fabs > fmax)
                fabs = fmax;

            f[ii].x += dx = fabs * dx * r_1;
            f[ii].y += dy = fabs * dy * r_1;
            f[ji].x -= dx;
            f[ji].y -= dy;

            pairsCount++;
	    }
    }
}

void MoveParticlesAndFreeForces2()
{
    for (int i = 0; i < N; i++)
    {
        double dvx = fTotal[i].x * dt / m[i];
        double dvy = fTotal[i].y * dt / m[i];
        p[i].x += (p[i].vx + dvx / 2) * dt;
        p[i].y += (p[i].vy + dvy / 2) * dt;
        p[i].vx += dvx;
        p[i].vy += dvy;
        f[i].x = 0;
        f[i].y = 0;
    }
}

void CalcForceManager(int wProc)
{
    int worker;
    int pairs[2];

    MPI_Status s;

    for (int i = 0; i < wProc; ++i)
    {
	    for (int j = i; j < wProc; ++j)
	    {
            MPI_Recv(&worker, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &s);

            pairs[0] = i;
            pairs[1] = j;

            MPI_Send(pairs, 2, MPI_INT, worker, 0, MPI_COMM_WORLD);
	    }
    }

    pairs[0] = -1;
    pairs[1] = -1;

    for (int i = 0; i < wProc; ++i)
    {
        MPI_Recv(&worker, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &s);
        MPI_Send(pairs, 2, MPI_INT, worker, 0, MPI_COMM_WORLD);
    }
}

void CalcForceWorker(int wProc, int rank, int managerId)
{
    int worker = rank;
    int pairs[2];

    MPI_Status s;

	while (true)
	{
        MPI_Send(&worker, 1, MPI_INT, managerId, 0, MPI_COMM_WORLD);

        MPI_Recv(pairs, 2, MPI_INT, managerId, 0, MPI_COMM_WORLD, &s);

        if (pairs[0] == -1 && pairs[1] == -1)
        {
            MPI_Send(&worker, 1, MPI_INT, managerId, 0, MPI_COMM_WORLD);
	        break;
        }

        CalcForcesPair(wProc, pairs[0], pairs[1]);
	}
}

void CalcForcesManagerWorker(int wProc, int rank)
{
    const int color = rank == wProc ? 1 : 0;

    MPI_Comm MPI_COMM_WORKERS;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &MPI_COMM_WORKERS);

	if (color)
	{
        CalcForceManager(wProc);
	}
	else
	{
        CalcForceWorker(wProc, rank, wProc);
	}

    MPI_Allreduce(f, fTotal, N * 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

void ParallelCalc(int n, int k, int r)
{
    Init();
    for (int i = 0; i < n; ++i)
    {
        CalcForcesManagerWorker(k - 1, r);
        MoveParticlesAndFreeForces2();
    }
}

// End of parallel algorithm implementation

void Solve()
{
    Task("MPIGravit1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
    if (r == 0)
    {
        pt >> Niter;
    // Testing the non-parallel algorithm:

        ShowLine("NON-PARALLEL ALGORITHM");
        NonParallelCalc(1);

        ShowLine("After one iteration");
        SetPrecision(12);
        Show("    Coordinates of the point 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Coordinates of the point 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        double t = MPI_Wtime();
        NonParallelCalc(Niter);
        t = MPI_Wtime() - t;

        ShowLine("After the required number of iterations");
        Show("    Coordinates of the point 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Coordinates of the point 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        SetPrecision(2);
        ShowLine("Non-parallel algorithm running time: ", t * 1000);
    }
    MPI_Bcast(&Niter, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Testing the parallel algorithm:

    if (r == 0)
    {
        ShowLine("PARALLEL ALGORITHM");
    }

	ParallelCalc(1, k, r);

    if (r == 0)
    {
        ShowLine("After one iteration");
        SetPrecision(12);
        Show("    Coordinates of the point 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Coordinates of the point 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        pt << p[0].x << p[0].y;
    }

    if (r == k - 2)
    {
        pt << p[799].x << p[799].y;
    }

    pairsCount = 0;

    double t = MPI_Wtime();
    ParallelCalc(Niter, k, r);
    t = MPI_Wtime() - t;

    if (r == 0)
    {
        ShowLine("After the required number of iterations");
        Show("    Coordinates of the point 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Coordinates of the point 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        SetPrecision(2);

        pt << p[0].x << p[0].y;
    }

    if (r == k - 2)
    {
        pt << p[799].x << p[799].y;
    }

    // Debug show
    if (r == k - 1)
    {
        Show("Manager Time = ", t * 1000);
    }
    else
    {
        Show("Time = ", t * 1000);
        Show("PairsCount = ", pairsCount);
    }
}
