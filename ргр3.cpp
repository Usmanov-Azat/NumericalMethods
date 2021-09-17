#include <iostream> //ввод/ввывод
#include <fstream>  //файловые потоки
#include <cmath>    //математическая библиотека
#define M_PI 3.1415926535898

using namespace std;    //простарнство имен

const int N = 3;        //размерность матрицы

//функция перемножения матриц
void MartUmnoj(double(&mass_1)[N][N], double(&mass_2)[N][N]);

//поиск максимального элемента массива над диагональю
void maximum(double(&mass)[N][N], int& k, int& m);

//транспонирование матрицы
void tranponirovat(double mass_1[N][N], double mass_2[N][N]);

//основная программа
int main()
{
	setlocale(LC_ALL, "rus");   //русский язык

	ifstream file("input.txt"); //открытие файла для чтения матрицы

	
	double epsilon = 1e-7;          //точность
	int k, m;                   //координаты максимального элемента
	int counter = 1;              //счетчик итераций
	double matrix[N][N];          //матрица
	double rotMatr[N][N];       //матрица поворота
	double transRotMatr[N][N];  //обратная матрица к матрице поворота
	double V[N][N];             //массив собственных векторов
	double fi;                  //угол поворота
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//запись и вывод матрицы из файла
	printf("\tИсходная матрица:\n\t");
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			file >> matrix[i][j];
			cout << matrix[i][j] << "\t";
		}
		cout << "\n\t";
	}

	//делаем матрицу собственных векторов единичной для умножения на матрицу вращения
	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++)
		{
			if (i == j)
			{
				V[i][i] = 1;
				continue;
			}
			V[i][j] = 0;
		}

	//цикл поиска
	while (true)
	{
		maximum(matrix, k, m);            //поиск координат максимального по модулю элемента матрицы

		if (abs(matrix[k][m]) < epsilon)  //выход по достижению необходимой точности
			break;

		//если a_kk равен a_mm то принимаем угол поворота фи за pi/4 или -pi/4
		if (matrix[k][k] == matrix[m][m])
			fi = (matrix[k][m] < 0) ? -M_PI / 4 : M_PI / 4;
		else    //иначе ищем значение угла по формуле
			fi = atan(2 * matrix[k][m] / (matrix[k][k] - matrix[m][m])) / 2;

		//делаем матрицу поворота единичной
		for (size_t i = 0; i < N; i++)
			for (size_t j = 0; j < N; j++)
			{
				if (i == j)
				{
					rotMatr[i][i] = 1;
					continue;
				}
				rotMatr[i][j] = 0;
			}

		//в матрице поворота изменяем элементы стоящие на пересечении строк и столбцов с номерами k и m
		rotMatr[k][k] = cos(fi);
		rotMatr[k][m] = -sin(fi);
		rotMatr[m][k] = sin(fi);
		rotMatr[m][m] = cos(fi);

		//находим обратную матрицу к матрице поворота (транспонируем)
		tranponirovat(rotMatr, transRotMatr);

		MartUmnoj(transRotMatr, matrix);           //H^-1 * A
		for (size_t i = 0; i < N; i++)
			for (size_t j = 0; j < N; j++)
				matrix[i][j] = transRotMatr[i][j];//приравниваем элементы матрицы А к элементам матрицы H^-1 для дальнейшего умножения на H
		MartUmnoj(matrix, rotMatr);                //A * H

		//каждую итерацию выводим полученную матрицу
		printf("\n\tA%d:\n\t", counter++);
		for (size_t i = 0; i < N; i++)
		{
			for (size_t j = 0; j < N; j++)
				printf("%.4lf\t", matrix[i][j]);
			cout << "\n\t";
		}

		//перемножаем матрицы поворота для получения массива собственных векторов
		MartUmnoj(V, rotMatr);
	}

	//вывод конечной матрицы
	cout << "\n\tВывод полученной матрицы:\n\t";
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
			printf("%.4lf\t", matrix[i][j]);
		cout << "\n\t";
	}

	//вывод собственных векторов и собственных значений
	cout << "\n\tВывод собственных векторов и собственных значений:\n\t";
	for (size_t i = 0; i < N; i++)
	{
		printf("lymbda%d = %lf\n\tvector%d =\n\t", i + 1, matrix[i][i], i + 1);
		for (size_t j = 0; j < N; j++)
			printf("%lf\n\t", V[j][i] / V[i][i]);
		cout << "\n\t";
	}

	system("pause");
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//функция перемножения матриц
void MartUmnoj(double(&mass_1)[N][N], double(&mass_2)[N][N])
{
	double temp[N][N] = { 0 };  //временная матрица

	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++)
			for (size_t k = 0; k < N; k++)
				temp[i][j] += mass_1[i][k] * mass_2[k][j];  //находим произведение матриц

	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++)
			mass_1[i][j] = temp[i][j];  //приравниваем элементы 1 матрицы к элементам матрицы произведения
}

//поиск максимального элемента массива над диагональю
void maximum(double(&mass)[N][N], int& k, int& m)
{
	double temp = mass[0][1];   //максимальный элемент
	k = 0; m = 1;

	for (size_t i = 0; i < N; i++)
		for (size_t j = i + 1; j < N; j++)
			if (abs(temp) < abs(mass[i][j]))
			{
				temp = mass[i][j];
				k = i;
				m = j;
			}
}

//транспонирование матрицы
void tranponirovat(double mass_1[N][N], double mass_2[N][N])
{
	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++)
			mass_2[j][i] = mass_1[i][j];
}