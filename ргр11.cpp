#pragma warning(disable:4996)
#include <iostream>
#define pi  3.14159265359
#define e   2.71828182846

using namespace std;

const int razmer = 240;   //максимальное кол-во значений "х" и "у"
const double h = 0.1; //длина шага

// K(x)y'' + L(x)y' + M(x)y = F(x)
double K(double x);
double L(double x);
double M(double x);
double F(double x);

int main()
{
	setlocale(LC_ALL, "rus");               //русский язык
	int n;                                  //число всех значений "х" и "у"
	double massiveX[razmer] = { 0 };              //все х
	double massiveY[razmer] = { 0 };              //все у
	double progONKA[razmer][4] = { 0 };           //массив для метода прогонки
	double P[razmer], Q[razmer];                //прогоночные коэффициенты
	double R = 0, S = 0.5, T = 0.5;             // Ry'(a) + Sy(a) = T
	double V = 1.1, W = 1.1, Z = 1.3;  // Vy'(b) + Wy(b) = Z

/******************************************************************************************************************************************************************
******************************************************************************************************************************************************************/

	printf("\tРешение краевой задачи для ОДУ методом прогонки\n\t");

	//начальные условия
	massiveX[0] = 1;
	massiveX[1] = 10;

	//кол-во отрезков (строк + 1 прогоночной матрицы)
	n = (massiveX[1] - massiveX[0]) / h;

	for (size_t i = 1; i <= n; i++)
		massiveX[i] = massiveX[i - 1] + h;

	//Метод прогоночных коэффициентов
	progONKA[0][0] = 0;             //коэффициент а_1
	progONKA[0][1] = -R / h + S;    //коэффициент b_1
	progONKA[0][2] = R / h;         //коэффициент c_1
	progONKA[0][3] = T;             //коэффициент d_1

	for (size_t i = 1; i < n; i++)
	{
		progONKA[i][0] = K(massiveX[i]) / h / h - L(massiveX[i]) / 2 / h;   //коэффициент а_i
		progONKA[i][1] = (-2.0) * K(massiveX[i]) / h / h + M(massiveX[i]);  //коэффициент b_i
		progONKA[i][2] = K(massiveX[i]) / h / h + L(massiveX[i]) / 2 / h;   //коэффициент c_i
		progONKA[i][3] = F(massiveX[i]);                                //коэффициент d_i
	}

	progONKA[n][0] = V / h;         //коэффициент а_n
	progONKA[n][1] = V / h + W;     //коэффициент b_n
	progONKA[n][2] = 0;             //коэффициент c_n
	progONKA[n][3] = Z;             //коэффициент d_n

	printf("Таблица коэффициентов\n");
	printf("\ta\t\tb\t\tc\t\td\n");
	for (size_t i = 0; i < n + 1; i++)
		printf("\t%8.4lf\t%8.4lf\t%8.4lf\t%8.4lf\n", progONKA[i][0], progONKA[i][1], progONKA[i][2], progONKA[i][3]);

	P[0] = Q[0] = 0;

	//вывод таблицы прогоночных коэффициентов
	cout << "\n\tТаблица прогоночных коэффициентов:\n\t";
	cout << "№\tP\tQ\n\t";
	printf("%d\t%.4lf\t%.4lf\n\t", 0, P[0], Q[0]);  //вывод значений
	for (size_t i = 0; i < n + 1; i++)
	{
		P[i + 1] = -progONKA[i][2] / (progONKA[i][1] + progONKA[i][0] * P[i]);                      //формула -c / (b + a * P_i-1)
		Q[i + 1] = (progONKA[i][3] - progONKA[i][0] * Q[i]) / (progONKA[i][1] + progONKA[i][0] * P[i]); //формула (d - a * Q_i-1) / (b + a * P_i-1)
		printf("%d\t%.4lf\t%.4lf\n\t", i + 1, P[i + 1], Q[i + 1]);                      //вывод значений
	}

	//вычисление значений
	cout << "\n\tЗначения переменных:\n";
	massiveY[n + 1] = Q[n + 1]; //последняя переменная равна последнему прогоночному коэффициенту Q
	for (int i = n; i >= 0; i--)
	{
		massiveY[i] = Q[i + 1] + P[i + 1] * massiveY[i + 1];  //х = Q + P * x_i+1
		printf("\ty(%.3lf)\t= %lf\n", massiveX[i], massiveY[i]);
	}

	system("pause>>void");
	return 0;
}

// K(x)y'' + L(x)y' + M(x)y = F(x)
double K(double x)
{
	return 1;
}
double L(double x)
{
	return 1/pow(x,2);
}
double M(double x)
{
	return 4;
}
double F(double x)
{
	return pow(x,2);
}