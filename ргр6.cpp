#include <iostream>
#include <fstream>

using namespace std;

const int razmer = 256;    //максимальное кол-во значений "х" и "у"

int main()
{
	
	double Y[razmer];             //правые части уравнений для внутренних точек отрезка
	double P[razmer];             //прогоночный коэффициент P
	double Q[razmer];             //прогоночный коэффициент Q
	setlocale(LC_ALL, "rus");
	ifstream fin("text.txt");   //файл для чтения значений
	double n = 0;               //число всех значений "х" и "у"
	double xmas[razmer] = { 0 };  //все х
	double ymas[razmer] = { 0 };  //все у
	double h[razmer];             //длина отрезка
	double m[razmer];             //внутренние точки
	//счет кол-ва элементов
	while (fin >> xmas[0])
		n++;

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	printf("x |");
	for (size_t i = 0; i < n / 2; i++)
	{
		fin >> xmas[i]; //считывание х
		printf("%.3lf\t|", xmas[i]);
	}

	printf("\ny |");
	for (size_t i = 0; i < n / 2; i++)
	{
		fin >> ymas[i]; //считывание у
		printf("%.3lf\t|", ymas[i]);
	}


	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//метод интерполяции сплайнами
	n /= 2;

	//вычисление и вывод длин отрезков
	printf("\n\n");
	for (size_t i = 1; i < n; i++)
	{
		h[i] = xmas[i] - xmas[i - 1];
		printf("h_%d = %.3lf\n", i, h[i]);
	}

	//вычисление и вывод правых частей уравнений
	printf("\n");
	for (size_t i = 1; i < n - 1; i++)
	{
		Y[i] = (ymas[i + 1] - ymas[i]) / h[i + 1] - (ymas[i] - ymas[i - 1]) / h[i];
		printf("Y_%d = %.3lf\n", i, Y[i]);
	}

	//вычисление и вывод прогоночных коэффициентов
	printf("\n");
	P[1] = Q[1] = 0;
	for (size_t i = 2; i < n; i++)
	{
		P[i] = -h[i] / (2 * (h[i] + h[i - 1]) + P[i - 1] * h[i - 1]);
		Q[i] = (6 * Y[i - 1] - h[i - 1] * Q[i - 1]) / (2 * (h[i - 1] + h[i]) + P[i - 1] * h[i - 1]);
		printf("P_%d = %.3lf\t", i, P[i]);
		printf("Q_%d = %.3lf\n", i, Q[i]);
	}

	//вычисление внутренних точек
	printf("\n");
	m[0] = m[int(n - 1)] = 0;
	m[int(n - 2)] = Q[int(n - 1)];
	for (size_t i = n - 3; i > 0; i--)
		m[i] = P[i + 1] * m[i + 1] + Q[i + 1];

	//вывод внутренних точек
	for (size_t i = 0; i < n; i++)
		printf("m_%d = %.3lf\n", i, m[i]);

	//вывод кубических сплайнов
	for (size_t i = 1; i < n; i++)
		printf("\nS_%d(x) = \n(%.3lf)*(x - (%.3lf))^3/(6*(%.3lf)) + \n(%.3lf)*((%.3lf) - x)^3/(6*(%.3lf)) + \n((%.3lf) - (%.3lf)*(%.3lf)^2/6)*(x - (%.3lf))/(%.3lf) + \n((%.3lf) - (%.3lf)*(%.3lf)^2/6)*((%.3lf) - x)/(%.3lf)\n\n", i,
			m[i], xmas[i - 1], h[i],                    //первый член сплайна
			m[i - 1], xmas[i], h[i],                    //второй член сплайна
			ymas[i], m[i], h[i], xmas[i - 1], h[i],     //третий член сплайна
			ymas[i - 1], m[i - 1], h[i], xmas[i], h[i]  //чет-ый член сплайна
		);

	//закрытие файла
	fin.close();
	system("pause>>void");
	return 0;
}