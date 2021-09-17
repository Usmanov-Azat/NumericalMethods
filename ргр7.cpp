#include <iostream>         //ввод\ввывод
#include <windows.h>        //работа с windows
#include <fstream>
#include "ConsoleWindow.h"  //указатель на консольное окно
#include <string>

#define KEY_DOWN(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)  //макрос для определения нажатой кнопки

using namespace std;

const int length = 256;           //максимальное кол-во значений "х" и "у"

HWND hwnd = GetConcolWindow();
HDC hdc = GetWindowDC(hwnd);

//вычисление квадратичной невязки
double squareResidual(double ymas[length], double xmas[length], double coeff[3], int n);

//вычисление линейной невязки
double linearResidual(double ymas[length], double xmas[length], double coeff[3], int n);

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);

double minimum(double* mas, int n);

double maximum(double* mas, int n);

/*******************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/

int main()
{
	setlocale(LC_ALL, "rus");           //русский язык
	ifstream fin("text.txt");           //файл для чтения значений
	int n = 0;                          //число всех значений "х" и "у"
	double maxsiveX[length] = { 0 };        //все х
	double ymas[length] = { 0 };        //все у
	double coeff[3];                    //коэффициенты для матрицы
	double matrix[3][4] = { 0 };        //матрица для квадратичной аппроксимации
	double linearMatrix[2][3] = { 0 };  //матрица для линейной аппроксимации
	double k;                           //коэффициент для преобразования матрицы
	double qudrGraf[length] = { 0 };
	double linearGraf[length] = { 0 };

	//счет кол-ва элементов
	while (fin >> maxsiveX[0])
		n++;
	n /= 2;

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	printf("x\t|");
	for (size_t i = 0; i < n; i++)
	{
		fin >> maxsiveX[i]; //считывание х
		printf("%.4lf\t|", maxsiveX[i]);
	}

	printf("\ny\t|");
	for (size_t i = 0; i < n; i++)
	{
		fin >> ymas[i]; //считывание у
		printf("%.4lf\t|", ymas[i]);
	}

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	printf("\n\nМетод квадратичной аппроксимации\n");

	//1 элемент
	for (size_t i = 0; i < n; i++)
		matrix[0][0] += maxsiveX[i] * maxsiveX[i] * maxsiveX[i] * maxsiveX[i];
	//2 и 4 элемент
	for (size_t i = 0; i < n; i++)
		matrix[0][1] = matrix[1][0] += maxsiveX[i] * maxsiveX[i] * maxsiveX[i];
	//обратная диагональ
	for (size_t i = 0; i < n; i++)
		matrix[0][2] = matrix[1][1] = matrix[2][0] += maxsiveX[i] * maxsiveX[i];
	//6 и 8 элемент
	for (size_t i = 0; i < n; i++)
		matrix[1][2] = matrix[2][1] += maxsiveX[i];
	//последний элемент
	matrix[2][2] += n;

	//элементы правой части (сверху вниз)
	for (size_t i = 0; i < n; i++)
		matrix[0][3] += maxsiveX[i] * maxsiveX[i] * ymas[i];
	for (size_t i = 0; i < n; i++)
		matrix[1][3] += maxsiveX[i] * ymas[i];
	for (size_t i = 0; i < n; i++)
		matrix[2][3] += ymas[i];

	//запоминаем полученные коэффициенты в матрицу линейной аппроксимации
	linearMatrix[0][0] = matrix[0][2];
	linearMatrix[0][1] = linearMatrix[1][0] = matrix[1][2];
	linearMatrix[1][1] = n;

	linearMatrix[0][2] = matrix[1][3];
	linearMatrix[1][2] = matrix[2][3];

	//решение методом Гаусса
	for (size_t i = 0; i < 2; i++)
	{
		k = matrix[i + 1][0] / matrix[0][0];        //вычисления коэффициента для преобразования матрицы
		for (size_t j = 0; j < 4; j++)
		{
			matrix[i + 1][j] -= matrix[0][j] * k;   //избавление от коэффициента при х во 2 и 3 строках
		}
	}

	k = matrix[2][1] / matrix[1][1];        //вычисления коэффициента
	for (size_t j = 1; j < 4; j++)
		matrix[2][j] -= matrix[1][j] * k;   //окончание приведения к треугольному виду

	//вычисление коэффициентов
	coeff[0] = matrix[2][3] / matrix[2][2];
	coeff[1] = (matrix[1][3] - matrix[1][2] * coeff[0]) / matrix[1][1];
	coeff[2] = (matrix[0][3] - matrix[0][2] * coeff[0] - matrix[0][1] * coeff[1]) / matrix[0][0];

	for (size_t i = 0; i < n; i++)
		qudrGraf[i] = coeff[2] * maxsiveX[i] * maxsiveX[i] + coeff[1] * maxsiveX[i] + coeff[0];

	//вывод многочлена и невязки
	printf("P(x) = (%lf)x^2 + (%lf)x + (%lf)\n", coeff[2], coeff[1], coeff[0]);

	printf("\ny\t|");
	for (size_t i = 0; i < n; i++)
		printf("%.4lf\t|", qudrGraf[i]);
	printf("\n");

	printf("Квадртатичная невязка = %lf\n", squareResidual(ymas, maxsiveX, coeff, n));

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	printf("\nМетод линейной аппроксимации\n");

	//решение методом линейной аппроксимации
	k = linearMatrix[1][0] / linearMatrix[0][0];        //вычисления коэффициента для преобразования матрицы
	for (size_t j = 0; j < 3; j++)
		linearMatrix[1][j] -= linearMatrix[0][j] * k;   //избавление от коэффициента при х во 2 строке

	//вычисление коэффициентов
	coeff[0] = linearMatrix[1][2] / linearMatrix[1][1];
	coeff[1] = (linearMatrix[0][2] - linearMatrix[0][1] * coeff[0]) / linearMatrix[0][0];

	for (size_t i = 0; i < n; i++)
		linearGraf[i] = coeff[1] * maxsiveX[i] + coeff[0];

	//вывод многочлена и невязки
	printf("P(x) = (%lf)x + (%lf)\n", coeff[1], coeff[0]);

	printf("\ny\t|");
	for (size_t i = 0; i < n; i++)
		printf("%.4lf\t|", linearGraf[i]);
	printf("\n");

	printf("Линейная невязка = %lf\n\n", linearResidual(ymas, maxsiveX, coeff, n));

	system("pause");

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	for (size_t i = 0; i < n; i++)
		fin >> maxsiveX[i]; //считывание х

	for (size_t i = 0; i < n; i++)
		fin >> ymas[i]; //считывание у

	system("cls");
	printf("Белый - исходные данные, красный - квадратичная аппроксимация, зеленый - линейная. Нажмите 'T', чтобы продолжить\n");
	string buf;
	int width = 900;
	int height = 450;
	int x = 30;
	int y = 60;
	int zeroPosY = y + height * 0.9;
	int stepX = width / (n);
	int stepY = (zeroPosY - y) / (n);
	HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
	HPEN Pen2 = CreatePen(PS_SOLID, 1, RGB(220, 20, 0));
	HPEN Pen3 = CreatePen(PS_SOLID, 1, RGB(45, 255, 48));
	HBRUSH brush1 = CreateSolidBrush(RGB(220, 20, 0));
	HBRUSH brush2 = CreateSolidBrush(RGB(45, 255, 48));
	SetTextColor(hdc, RGB(160, 160, 160));
	SetBkColor(hdc, RGB(12, 12, 12));

	while (!KEY_DOWN('T'))
	{
		SetTextColor(hdc, RGB(160, 160, 160));
		SelectObject(hdc, Pen1);
		SelectObject(hdc, brush1);

		//границы
		Line(hdc, x, y - 10, x + width + 20, y - 10);
		Line(hdc, x, y - 10, x, y + height);
		Line(hdc, x + width + 20, y - 10, x + width + 20, y + height);
		Line(hdc, x, y + height, x + width + 20, y + height);

		//ось Х
		Line(hdc, x, y + height * 0.9, x + width + 20, y + height * 0.9);

		int posX = x + stepX;
		int posY = zeroPosY;

		//деления на оси Х
		for (size_t i = 0; i < n; i++)
		{
			Line(hdc, posX, posY + 5, posX, posY - 5);
			buf = to_string(maxsiveX[0] + (maxsiveX[n - 1] - maxsiveX[0]) / (n - 2) * i);
			buf = buf.substr(0, buf.find(',') + 5);
			TextOutA(hdc, posX - buf.length() * 3, posY + 10, buf.c_str(), buf.length());
			posX += stepX;
		}
		posX = x;
		posY -= stepY;

		double min = minimum(ymas, n);
		double max = maximum(ymas, n);

		//деления на оси У
		for (size_t i = 0; i < n; i++)
		{
			buf = to_string(min + (max - min) / (n - 2) * i);
			buf = buf.substr(0, buf.find(',') + 5);
			TextOutA(hdc, x + 10, posY - 10, buf.c_str(), buf.length());
			Line(hdc, posX + 5, posY, posX - 5, posY);
			posY -= stepY;
		}

		//отрезки графика
		int prevY[] =
		{
			zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[0] - min) / (max - min),
			zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (qudrGraf[0] - min) / (max - min),
			zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (linearGraf[0] - min) / (max - min)
		};
		int prevX = x + stepX;
		for (size_t i = 2; i < n + 1; i++)
		{
			posX = x + stepX + (width - 2 * stepX) * (maxsiveX[i - 1] - maxsiveX[0]) / (maxsiveX[n - 1] - maxsiveX[0]);

			//исходные данные
			SelectObject(hdc, Pen1);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[i - 1] - min) / (max - min);
			Line(hdc, posX - 1, posY, prevX, prevY[0]);
			prevY[0] = posY;

			//метод квадратичной аппроксимации
			SelectObject(hdc, Pen2);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (qudrGraf[i - 1] - min) / (max - min);
			Line(hdc, posX - 1, posY, prevX, prevY[1]);
			prevY[1] = posY;

			//метод линейной аппроксимации
			SelectObject(hdc, Pen3);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (linearGraf[i - 1] - min) / (max - min);
			Line(hdc, posX - 1, posY, prevX, prevY[2]);
			prevY[2] = posY;

			prevX = posX;
		}

		//точки графика
		SelectObject(hdc, Pen2);
		int radius = 2;
		prevX = x + stepX;
		for (size_t i = 1; i < n + 1; i++)
		{
			posX = x + stepX + (width - 2 * stepX) * (maxsiveX[i - 1] - maxsiveX[0]) / (maxsiveX[n - 1] - maxsiveX[0]);

			//исходные данные
			SelectObject(hdc, Pen2);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[i - 1] - min) / (max - min);
			Ellipse(hdc, posX - radius, posY - radius, posX + radius, posY + radius);

			//метод квадратичной аппроксимации
			SelectObject(hdc, Pen3);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (qudrGraf[i - 1] - min) / (max - min);
			Ellipse(hdc, posX - radius, posY - radius, posX + radius, posY + radius);

			//метод линейной аппроксимации
			SelectObject(hdc, Pen1);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (linearGraf[i - 1] - min) / (max - min);
			Ellipse(hdc, posX - radius, posY - radius, posX + radius, posY + radius);

			prevX = posX;
		}
	}

	DeleteObject(brush1);
	DeleteObject(brush2);
	DeleteObject(Pen1);
	DeleteObject(Pen2);
	DeleteObject(Pen3);

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//закрытие файла
	fin.close();
	return 0;
}

/*******************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/

//вычисление квадратичной невязки
double squareResidual(double ymas[length], double xmas[length], double coeff[3], int n)
{
	double ans = 0;
	for (size_t i = 0; i < n; i++)
		ans += (coeff[2] * xmas[i] * xmas[i] + coeff[1] * xmas[i] + coeff[0] - ymas[i]) * (coeff[2] * xmas[i] * xmas[i] + coeff[1] * xmas[i] + coeff[0] - ymas[i]);
	return ans;
}

//вычисление линейной невязки
double linearResidual(double ymas[length], double xmas[length], double coeff[3], int n)
{
	double ans = 0;
	for (size_t i = 0; i < n; i++)
		ans += (coeff[1] * xmas[i] + coeff[0] - ymas[i]) * (coeff[1] * xmas[i] + coeff[0] - ymas[i]);
	return ans;
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

double minimum(double* mas, int n)
{
	double min = mas[0];
	for (size_t i = 1; i < n; i++)
		if (mas[i] < min)
			min = mas[i];
	return min;
}

double maximum(double* mas, int n)
{
	double max = mas[0];
	for (size_t i = 1; i < n; i++)
		if (mas[i] > max)
			max = mas[i];
	return max;
}