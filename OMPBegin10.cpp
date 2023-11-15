#include "pt4.h"
#include <cmath>
#include <omp.h>

#include <chrono>

template <int NumThreads>
class Threads
{
public:
	Threads() = default;
	virtual ~Threads() = default;

public:
	double RunF(double X, int N, bool parallel = true)
	{
		double res;

		if (parallel)
		{
			StartProfile(mStartTime);

			res = F_Parallel(X, N);

			EndProfile(mStartTime, mEndTime, mTotalParallelTime);
		}
		else
		{
			StartProfile(mStartTime);

			res = F_NonParallel(X, N);

			EndProfile(mStartTime, mEndTime, mNonParallelTime);
		}

		return res;
	}

	void ShowInfos()
	{
		UpdateInfos();

		ShowLine("Non-parallel time: ", mNonParallelTime);
		ShowLine("num_procs: ", mNumProcs);
		ShowLine("num_threads: ", mNumThreads);

		for (int i = 0; i < NumThreads; ++i)
		{
			Show("thread_num: ", mThreadInfos[i].threadNum);
			Show("Count: ", mThreadInfos[i].count);
			Show("Thread Time: ", mThreadInfos[i].threadTime);
			ShowLine();
		}

		ShowLine("Total parallel time: ", mTotalParallelTime);
		ShowLine("Rate: ", mRate);
	}

protected:
	virtual double F_NonParallel(double X, int N) = 0;
	virtual double F_Parallel(double X, int N) = 0;

protected:
	void StartThreadProfile(int threadNum = omp_get_thread_num())
	{
		mThreadInfos[threadNum].threadNum = threadNum;
		mThreadInfos[threadNum].count = 0;

		StartProfile(mThreadInfos[threadNum].startTime);
	}

	void EndThreadProfile(int threadNum = omp_get_thread_num())
	{
		EndProfile(mThreadInfos[threadNum].startTime, mThreadInfos[threadNum].endTime, mThreadInfos[threadNum].threadTime);
	}

	void CountThread(int threadNum = omp_get_thread_num())
	{
		++mThreadInfos[threadNum].count;
	}

	void RecordCountThread(int count, int threadNum = omp_get_thread_num())
	{
		mThreadInfos[threadNum].count = count;
	}

private:
	void UpdateInfos()
	{
		if (mNonParallelTime > 0.0 && mTotalParallelTime > 0.0)
		{
			mRate = mNonParallelTime / mTotalParallelTime;
		}
	}

	void StartProfile(double& startTime)
	{
		startTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count() * 0.001;
	}

	void EndProfile(const double& startTime, double& endTime, double& deltaTime)
	{
		endTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count() * 0.001;

		deltaTime = endTime - startTime;
	}

private:
	struct ThreadInfo
	{
		int threadNum = 0;
		int count = 0;
		double threadTime = 0.0;

		double startTime = 0.0;
		double endTime = 0.0;
	};
private:
	double mNonParallelTime = 0.0;
	int mNumProcs = omp_get_num_procs();
	int mNumThreads = NumThreads;
	ThreadInfo mThreadInfos[NumThreads];
	double mTotalParallelTime = 0.0;
	double mRate = 1.0;

	double mStartTime = 0.0;
	double mEndTime = 0.0;
};

class ThreadBegin10 : public Threads<2>
{
public:
	double F_NonParallel(double X, int N) override
	{
		double res = 0.0;
		for (int I = 1; I <= N; ++I)
		{
			double tmp1 = 0.0;
			for (int J = 1; J <= I + N; ++J)
			{
				const double tmp2 = J + log(1 + X + J);
				const double tmp3 = 2 * I * J - 1;
				tmp1 += tmp2 / tmp3;
			}
			res += 1.0 / tmp1;
		}

		return res;
	}

	double F_Parallel(double X, int N) override
	{
		double res = 0.0;
		int K = GetK(1 + N, N + N, N);

#pragma omp parallel sections num_threads(2) reduction(+:res)
		{
#pragma omp section
			{
				int count = 0;
				const int threadNum = omp_get_thread_num();
				StartThreadProfile(threadNum);
				
				for (int I = 1; I < K; ++I)
				{
					double tmp1 = 0.0;
					for (int J = 1; J <= I + N; ++J)
					{
						count++;

						const double tmp2 = J + log(1 + X + J);
						const double tmp3 = 2 * I * J - 1;
						tmp1 += tmp2 / tmp3;
					}
					res += 1.0 / tmp1;
				}

				RecordCountThread(count, threadNum);
				EndThreadProfile(threadNum);
			}
#pragma omp section
			{
				int count = 0;
				const int threadNum = omp_get_thread_num();
				StartThreadProfile(threadNum);

				for (int I = K; I <= N; ++I)
				{
					double tmp1 = 0.0;
					for (int J = 1; J <= I + N; ++J)
					{
						count++;

						const double tmp2 = J + log(1 + X + J);
						const double tmp3 = 2 * I * J - 1;
						tmp1 += tmp2 / tmp3;
					}
					res += 1.0 / tmp1;
				}

				RecordCountThread(count, threadNum);
				EndThreadProfile(threadNum);
			}
		}

		return res;
	}

private:
	int GetK(int start, int end, int loop)
	{
		double a = 2.0 * static_cast<double>(end - start) / loop;
		double b = 4.0 * start;
		double c = -(start + end) * loop;

		return static_cast<int>((-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a));
	}
};

void Solve()
{
    Task("OMPBegin10");

	ThreadBegin10 t;

	double X;
	int N;

	pt >> X >> N;
	pt << t.RunF(X, N, false);

	pt >> X >> N;
	pt << t.RunF(X, N, true);

	t.ShowInfos();
}
