#include <iostream>
#include <fstream>

using namespace std;

const int razmer = 256;           //максимальное кол-во значений "х" и "у"
const double h = 10e-5;         //длина шага

//метод интерполяции лагранжа
double Lagrange(double ymas[razmer], double xmas[razmer], double n, double point);

int main()
{
	
	double massiveY[razmer] = { 0 };  //все у
	double y_d[6];              //вычисленные первые производные 6 способами
	double y_dd[3];             //вычисленные вторые производные 3 способами
	double test = 0;
	setlocale(LC_ALL, "rus");
	ifstream fin("text.txt");   //файл для чтения значений
	double n = 0;               //число всех значений "х" и "у"
	double point = 1.3832;
	double massiveX[razmer] = { 0 };  //все х
	printf("Введите х: ");
	cin >> point;
	system("cls");

	//счет кол-ва элементов
	while (fin >> massiveX[0])
		n++;

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	printf("x |");
	for (size_t i = 0; i < n / 2; i++)
	{
		fin >> massiveX[i]; //считывание х
		printf("%.3lf\t|", massiveX[i]);
	}

	printf("\ny |");
	for (size_t i = 0; i < n / 2; i++)
	{
		fin >> massiveY[i]; //считывание у
		printf("%.3lf\t|", massiveY[i]);
	}

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	printf("\n");
	//printf("\n\nМетод интеполяции Лагранжа\n");

	//первые производные
	y_d[0] = (Lagrange(massiveY, massiveX, n, point + h) - Lagrange(massiveY, massiveX, n, point)) / h;
	y_d[1] = (Lagrange(massiveY, massiveX, n, point) - Lagrange(massiveY, massiveX, n, point - h)) / h;
	y_d[2] = (Lagrange(massiveY, massiveX, n, point + h) - Lagrange(massiveY, massiveX, n, point - h)) / 2 / h;
	y_d[3] = (-3 * Lagrange(massiveY, massiveX, n, point) + 4 * Lagrange(massiveY, massiveX, n, point + h) - Lagrange(massiveY, massiveX, n, point + 2 * h)) / 2 / h;
	y_d[4] = (Lagrange(massiveY, massiveX, n, point - 2 * h) - 4 * Lagrange(massiveY, massiveX, n, point - h) + 3 * Lagrange(massiveY, massiveX, n, point)) / 2 / h;
	y_d[5] = (-2 * Lagrange(massiveY, massiveX, n, point - h) - 3 * Lagrange(massiveY, massiveX, n, point) + 6 * Lagrange(massiveY, massiveX, n, point + h) - Lagrange(massiveY, massiveX, n, point + 2 * h)) / 6 / h;

	//вторые производные
	y_dd[0] = (Lagrange(massiveY, massiveX, n, point + h) - 2 * Lagrange(massiveY, massiveX, n, point) + Lagrange(massiveY, massiveX, n, point - h)) / h / h;
	y_dd[1] = (-Lagrange(massiveY, massiveX, n, point - 3 * h) + 4 * Lagrange(massiveY, massiveX, n, point - 2 * h) - 5 * Lagrange(massiveY, massiveX, n, point - h) + 2 * Lagrange(massiveY, massiveX, n, point)) / h / h;
	y_dd[2] = (-2 * Lagrange(massiveY, massiveX, n, point) + 5 * Lagrange(massiveY, massiveX, n, point + h) - 4 * Lagrange(massiveY, massiveX, n, point + 2 * h) + Lagrange(massiveY, massiveX, n, point + 3 * h)) / h / h;

	//вывод производных
	printf("\nПервые производные:\n");
	for (size_t i = 0; i < 2; i++)
		printf("y_d_%d(%lf) = %lf + O(h)\n", i + 1, point, y_d[i]);
	for (size_t i = 2; i < 5; i++)
		printf("y_d_%d(%lf) = %lf + O(h^2)\n", i + 1, point, y_d[i]);
	printf("y_d_%d(%lf) = %lf + O(h^3)\n", 6, point, y_d[5]);

	printf("\nВторые производные:\n");
	for (size_t i = 0; i < 3; i++)
		printf("y_dd_%d(%lf) = %lf + O(h^2)\n", i + 1, point, y_dd[i]);

	//закрытие файла
	fin.close();
	system("pause>>void");
	return 0;
}

//метод интерполяции лагранжа
double Lagrange(double ymas[razmer], double xmas[razmer], double n, double point)
{
	double block;
	double sum = 1;
	double ans = 0;
	for (size_t i = 0; i < n / 2; i++)
	{
		block = ymas[i];            //инициализация первого множителя значением у
		for (size_t j = 0; j < n / 2; j++)
		{
			//на каждой итерации пропускаем значение х, номер которого равен данной итерации (при расчете 1 члена пропускаем вычисления с х_1)
			if (i != j)
			{
				sum *= (xmas[i] - xmas[j]);         //произведение множителей знаменателя
				block *= (point - xmas[j]);         //произведение множителей числителя
			}
		}

		block /= sum;   //вычисление значения всего члена
		ans += block;   //суммируем ответ
		sum = 1;        //возвращаем к исходному значению
	}
	return ans;
}