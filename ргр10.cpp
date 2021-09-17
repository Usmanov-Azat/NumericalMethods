#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#define pi  3.14159265359
#define e   2.71828182846

using namespace std;

const int razmer = 256;       //максимальное кол-во значений "х" и "у"
const double epsilon = 10e-3;   //точность
const double h = 0.1;       //длина шага

// функция
double f(double x, double y);

int main()
{
	
	double ans = 0;             //значение интеграла
	double prevAns = 0;         //предыдущее значение интеграла
	double K[4] = { 0 };        //параметры
	setlocale(LC_ALL, "rus");   //русский язык
	int n = 4;                  //число всех значений "х" и "у"
	int count = 0;              //кол-во значений, которые нужно найти методом Адамса
	double massiveX[razmer] = { 0 };  //все х
	double massiveY[razmer] = { 0 };  //все у
	double summa = 0;             //сигма
	//ввод кол-ва значений
	printf("Введите кол-во значений, которые нужно найти методом Адамса: ");
	scanf("%d", &count);
	system("cls");

	/******************************************************************************************************************************************************************
	******************************************************************************************************************************************************************/

	printf("Метод Рунге-Кута для ОДУ первого порядка\n");

	//начальные условия
	massiveX[0] = 0;
	massiveY[0] = 0;

	//вывод заголовка таблицы
	printf("i\tx_i\ty_i\tK_1\tK_2\tK_3\tK_4\n");

	//метод Рунге-Кута
	for (size_t i = 0; i < 6; i++)
	{
		//вычисление параметров
		K[0] = f(massiveX[i], massiveY[i]);
		K[1] = f(massiveX[i] + h / 2, massiveY[i] + h / 2 * K[0]);
		K[2] = f(massiveX[i] + h / 2, massiveY[i] + h / 2 * K[1]);
		K[3] = f(massiveX[i] + h, massiveY[i] + h * K[2]);

		//вычисление следующего значения у
		massiveY[i + 1] = massiveY[i] + h / 6 * (K[0] + 2 * K[1] + 2 * K[2] + K[3]);

		//следующий х
		massiveX[i + 1] = massiveX[i] + h;

		//вывод таблицы значений
		printf("%d\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\n", i, massiveX[i], massiveY[i], K[0], K[1], K[2], K[3]);
	}

	/******************************************************************************************************************************************************************
	******************************************************************************************************************************************************************/

	printf("\n\nМетод Адамса для ОДУ первого порядка\n");

	for (size_t i = 5; i < 6 + count - 1; i++)
	{
		//вычисление следующего значения у
		massiveY[i + 1] = massiveY[i] + h / 24 * (55 * f(massiveX[i], massiveY[i]) - 59 * f(massiveX[i - 1], massiveY[i - 1]) + 37 * f(massiveX[i - 2], massiveY[i - 2]) - 9 * f(massiveX[i - 3], massiveY[i - 3]));
		massiveX[i + 1] = massiveX[i] + h;  //следующий х

		//вывод значений
		printf("y(%.2lf) = %lf\n", massiveX[i + 1], massiveY[i + 1]);
	}

	system("pause>>void");
	return 0;
}

// функция
double f(double x, double y)
{
	return 1 + 0.3 * sin(x) * y - y * y*y;
}