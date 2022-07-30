#include <iostream>
#include <math.h>
#include <iomanip>
#include <conio.h>

using namespace std;
#define PI 3.14159265

class Distribution
{
private:
	double* xr, * pract, * pract1, * Fi, * Fi_1;
	int* count;
	double Mx = 0, Mx2 = 0;


public:
	Distribution(int n)
	{


		xr = new double[n];
		pract1 = new double[n];
		count = new int[n];
		Fi_1 = new double[n];
		Fi = new double[n];
		pract = new double[n];
	}

	void null_mas(int n)
	{
		for (int i = 0; i < n; i++)
		{
			pract[i] = 0;
			pract1[i] = 0;
			Fi[i] = 0;
			count[i] = 0;

		}
	}

	void FI(double start, double end, int n)
	{
		double dx, p, SumM = 0, x = start, Dx, sko;

		dx = (end - start) / n;
		x = x - dx;
		for (int i = 0; i < n; i++)
		{
			x += dx;
			double arc = atan(x);

			Fi[i] = 0.5 + arc / PI;
			SumM += Fi[i];

			p = 1.0 / n;
			Mx += p * x;
			Mx2 = Mx2 + (x * x) * p;
			printf("i=  %i F[i]=%f\t %f \n", i, Fi[i], x);

			xr[i] = x;

		}
		Dx = Mx2 - (Mx * Mx);
		sko = sqrt(Dx);
		printf("\nМатематическое ожидание =%f\t Дисперисия =%f\tСКО=%f\n ", Mx, Dx, sko);
	}

	void NewFi(int n)
	{
		double width, min;
		min = Fi[0];
		width = Fi[49] - Fi[0];

		for (int i = 0; i < n; i++)
		{
			Fi_1[i] = (Fi[i] - min) / width;
		}
	}

	int play(int n)
	{
		double RandX;
		int result = 0;
		RandX = ((double)rand()) / (RAND_MAX);
		for (int i = 0; i < n; i++)
		{
			if (Fi_1[i] > RandX)
			{
				result = i;
				break;
			}
		}


		return result;
	}

	void start(int thous, int n)
	{
		srand(time(NULL));
		int result;
		for (int i = 0; i < thous; i++)
		{
			result = play(n);
			pract[result] = Fi[result];
			count[result]++;
		}
	}

	void ImprovePract(int n)
	{
		pract1[0] = Fi[0];
		for (int i = 1; i < n - 1; i++)
		{
			pract1[i] = (pract[i] + pract[i + 1]) / 2;
		}
		pract1[n - 1] = pract[n - 1];
	}


	void Print(int n)
	{
		printf("\nСередина интревала\tКоличество раз выпало\t Номер интервала ");

		for (int i = 0; i < n; i++)
		{
			printf("\n          %6.5f   \t%d                       \t%d    \t%f", pract1[i], count[i], i + 1, xr[i]);

		}
	}

	double* get_Fi()
	{
		return Fi;
	}

	double* get_xr() {
		return xr;
	}

	~Distribution()
	{
		delete[] Fi;
		delete[] pract;
		delete[] pract1;
		delete[] count;
		delete[] xr;
	}

};



class Characteristics
{

private:
	int n = 50;

	double R;
	int K;
	double h;
	double Mx = 0, Dx = 0;
	double* BigFEnd, * BigFBegin;



public:
	Characteristics(double* Fi, double* xr, int n, double start, double end)
	{

		double  p, sko, Mx2 = 0, dx, x = Fi[0];
		BigFBegin = new double[n];
		BigFEnd = new double[n];
		dx = (Fi[49] - Fi[0]) / n;
		for (int i = 0; i < n; i++)
		{

			p = 1.0 / n;
			Mx += p * xr[i];

			Mx2 = Mx2 + (xr[i] * xr[i]) * p;


		}
		Dx = Mx2 - (Mx * Mx);
		sko = sqrt(Dx);

		printf("\nМатематическое ожидание = %6.3f\tДисперсия = %6.3f\tСКО = %6.3f", Mx, Dx, sko);
		R = Fi[49] - Fi[0];
		K = round((1 + (3.32 * log10(1000))));
		h = R / K;
		cout << endl << "Кол-во интервалов: " << setw(3) << K;
		cout << endl << "Шаг: " << setw(22) << h << endl;
	}

	double H()
	{
		return h;
	}
	int k()
	{
		return K;
	}

	int  play(double** matr, int n)
	{
		double rand_x;
		int result = 0;

		rand_x = ((double)rand()) / (RAND_MAX);


		for (int i = 0; i < n; i++)
		{
			if (rand_x <= matr[i][1])
			{
				result = i;
				break;
			}
		}
		return result;
	}

	void start(double** matr, int n, int m)
	{
		srand(time(NULL));

		int result;
		for (int k = 0; k < 1000; k++)
		{
			result = play(matr, n);
			matr[result][3]++;

		}
	}
	void Matrix(double** matr, int n, int m)
	{
		for (int i = 1; i < n; i++)
		{


			matr[i][0] = matr[i - 1][1];
			matr[i][1] = matr[i][0] + h;

		}


		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				matr[i][2] = (matr[i][0] + matr[i][1]) / 2;
			}
		}


	}


	void fix_matr(double** matr, int n, int m, double* min, double* width)
	{
		*min = matr[0][1];

		*width = matr[n - 1][1] - matr[0][1];
		for (int i = 0; i < n; i++)
		{
			matr[i][1] = (matr[i][1] - *min) / *width;
		}
	}
	void teor_matr(double** matr, int n, int m)
	{
		for (int i = 0; i < n; i++)
		{
			matr[i][4] = matr[i][3] / 1000;
		}
	}

	void back_matr(double** matr, int n, int m, double min, double width)
	{
		for (int i = 0; i < n; i++)
		{
			matr[i][1] = (matr[i][1] * width);
			matr[i][1] += min;
		}
	}

	void printMatr(double** matr, int n, int m)
	{

		printf("Xл\tXп\tXcр\tNi\tP*i\tP\n");
		for (int i = 0; i < n; i++)
		{

			printf("%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.4f\n", matr[i][0], matr[i][1], matr[i][2], matr[i][3], matr[i][4], matr[i][5]);

		}
	}

	void NormMatrix(double** matr, int n, int m)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				matr[i][j] = (matr[i][j] - Mx) / Dx;
			}
		}
	}


	void fill(int n)
	{
		BigFBegin[0] = 0.3264;
		BigFBegin[1] = 0.3212;
		BigFBegin[2] = 0.3159;
		BigFBegin[3] = 0.3106;
		BigFBegin[4] = 0.3051;
		BigFBegin[5] = 0.2995;
		BigFBegin[6] = 0.2939;
		BigFBegin[7] = 0.2881;
		BigFBegin[8] = 0.2823;
		BigFBegin[9] = 0.2794;
		BigFBegin[10] = 0.2734;

		BigFEnd[0] = 0.3212;
		BigFEnd[1] = 0.3159;
		BigFEnd[2] = 0.3106;
		BigFEnd[3] = 0.3051;
		BigFEnd[4] = 0.2995;
		BigFEnd[5] = 0.2939;
		BigFEnd[6] = 0.2881;
		BigFEnd[7] = 0.2823;
		BigFEnd[8] = 0.2794;
		BigFEnd[9] = 0.2734;
		BigFEnd[10] = 0.2673;
	}


	void TV(double** matr, int n)
	{
		for (int i = 0; i < n; i++)
		{
			matr[i][5] = BigFBegin[i] - BigFEnd[i];
		}
	}

	double X2(double** matr, int n, int m)
	{
		double tmp = 0;
		double sum = 0;
		for (int i = 0; i < n; i++)
		{


			tmp = ((matr[i][4] - matr[i][5]) * (matr[i][4] - matr[i][5])) / matr[i][5];
			sum += tmp;

		}
		double Xi2 = sum;
		return Xi2;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	int n = 50, thous = 1000, start = 0, end = 5;
	double* Fi, * xr, h, Xi2, Xi2kr;
	int K;
	double width, min;
	Distribution Koshi(n);

	Koshi.null_mas(n);
	Koshi.FI(start, end, n);
	Koshi.NewFi(n);
	Koshi.start(thous, n);
	Koshi.ImprovePract(n);
	Koshi.Print(n);

	Fi = Koshi.get_Fi();
	xr = Koshi.get_xr();


	Characteristics charac(Fi, xr, n, 0, 5);

	h = charac.H();
	K = charac.k();

	double** matr = new double* [K];
	for (int i = 0; i < K; i++)
	{
		matr[i] = new double[6];
	}

	for (int i = 0; i < K; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			matr[i][j] = 0;
		}
	}
	matr[0][0] = Fi[0];
	matr[0][1] = Fi[0] + h;

	charac.Matrix(matr, K, 6);
	charac.fix_matr(matr, K, 6, &min, &width);
	charac.start(matr, K, 6);
	charac.teor_matr(matr, K, 6);
	cout << "Для вывода таблицы нажмите любую клавишу..." << endl;
	_getch();
	charac.back_matr(matr, K, 6, min, width);
	charac.printMatr(matr, K, 6);
	charac.NormMatrix(matr, K, 6);
	cout << "Для вывода нормированных значений нажмите любую клавишу.." << endl;
	_getch();
	charac.fill(K);
	charac.TV(matr, K);
	charac.printMatr(matr, K, 6);
	Xi2 = charac.X2(matr, K, 6);
	Xi2kr = 15.5;
	cout << "\nXi2 = " << Xi2 << "\t" << "Xi2kr = " << Xi2kr << endl;
	if (Xi2 > Xi2kr)
	{
		cout << "\nЗначение Xi2 больше Критического значения Xi2kr.Hence,Есть основания отвергать гипотезу о нормальном распределении.\n";

	}
	else
	{
		cout << "\nЗначение Xi2 меньше Критического значения Xi2kr.Hence,Нет оснований отвергать гипотезу о нормальном распределении.\n";
	}
}
