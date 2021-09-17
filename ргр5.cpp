#include <iostream>         //ввод\ввывод
#include <windows.h>        //работа с windows
#include <fstream>
#include "ConsoleWindow.h"  //указатель на консольное окно
#include <string>

using namespace std;

#define KEY_DOWN(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)  //макрос для определения нажатой кнопки

const int abc = 256;   //максимальное кол-во значений "х" и "у"
HWND hwnd = GetConcolWindow();
HDC hdc = GetWindowDC(hwnd);

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);

double min_elem(double* mas, int n);

double max_elem(double* mas, int n);

/*******************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/

int main()
{
	
	double xmas[abc] = { 0 };   //все х
	double ymas[abc] = { 0 };   //все у
	string buf;
	setlocale(LC_ALL, "rus");   //русский язык
	ifstream fin("text.txt");   //файл для чтения значений
	int n = 0;               //число всех значений "х" и "у"
	double summa = 1;             //произведение множителей знаменателя
	double block = 0;           //значение всего члена
	double then = 0;             //ответ
	double oldAns = 0;          //предыдущее значение ответа
	double pointer = 0.1662;      //заданная точка
	printf("Введите значение х: ");
	cin >> pointer;   //ввод х
	system("cls");  //очистка консоли

	//счет кол-ва элементов
	while (fin >> xmas[0])
		n++;
	n /= 2;

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	printf("x |");
	for (size_t i = 0; i < n; i++)
	{
		fin >> xmas[i]; //считывание х
		printf("%.3lf\t|", xmas[i]);
	}

	printf("\ny |");
	for (size_t i = 0; i < n; i++)
	{
		fin >> ymas[i]; //считывание у
		printf("%.3lf\t|", ymas[i]);
	}

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//метод интеполяции Лагранжа
	printf("\n\nМетод интеполяции Лагранжа\n");
	printf("L_d(x) =   ", n - 1);
	for (size_t i = 0; i < n; i++)
	{
		printf("(%.3lf)", ymas[i]); //вывод значения у
		block = ymas[i];            //инициализация первого множителя значением у
		for (size_t j = 0; j < n; j++)
		{
			//на каждой итерации пропускаем значение х, номер которого равен данной итерации (при расчете 1 члена пропускаем вычисления с х_1)
			if (i != j)
			{
				printf("*(x - %.3lf)", xmas[j]);    //вывод остальных значений х
				summa *= (xmas[i] - xmas[j]);         //произведение множителей знаменателя
				block *= (pointer - xmas[j]);         //произведение множителей числителя
			}
		}
		printf(" / %e\n\t", summa);  //вывод знаменателя

		//если вычисляем последний член , то + не выводим
		if (i != n - 1) printf(" + ");

		block /= summa;   //вычисление значения всего члена
		then += block;   //суммируем ответ
		summa = 1;        //возвращаем к исходному значению
	}

	//вывод ответа
	printf("\nL_%d(%lf) = %lf\n", n - 1, pointer, then);
	oldAns = then;

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//метод интерполяции Ньютона
	printf("\nМетод интеполяции Ньютона");
	int pos = n - 1;    //кол-во вычислений разделенных разностей
	then = ymas[0];          //присваиваем ответу первое значение у
	printf("\nN_%d(x) =(%.2lf)", n - 1, ymas[0]);
	for (size_t i = 1; i < n; i++)
	{
		//расчет разделенных разностей
		for (size_t j = 0; j < pos; j++)
			ymas[j] = (ymas[j + 1] - ymas[j]) / (xmas[j + i] - xmas[j]);

		printf(" + (%.2lf)", ymas[0]);  //вывод значения у
		block = ymas[0];                //инициализация множителя первым значением у
		for (size_t j = n; j > n - i; j--)
		{
			printf("*(x - %.2lf)", xmas[n - j]);   //вывод множителей
			block *= (pointer - xmas[n - j]);        //вычисление значения всего члена
		}

		pos--;          //уменьшаем кол-во вычислений разделенных разностей
		then += block;   //вычисляем ответ
	}

	//вывод ответа
	printf("\n\nN_%d(%lf) = %lf\n", n - 1, pointer, then);
	printf("Погрешность составила: %.3lf\n\n", abs(then - oldAns));
	system("pause");

	/*******************************************************************************************************************************************************************
	*******************************************************************************************************************************************************************/

	//возвращение в начало файла
	fin.clear();
	fin.seekg(0, ios::beg);

	for (size_t i = 0; i < n; i++)
		fin >> xmas[i]; //считывание х

	for (size_t i = 0; i < n; i++)
		fin >> ymas[i]; //считывание у

	system("cls");
	printf("Нажмите 'T', чтобы продолжить\n");
	int width = 900;
	int height = 450;
	int x = 30;
	int y = 60;
	int zeroPosY = y + height * 0.9;
	int stepX = width / (n + 1);
	int stepY = (zeroPosY - y) / (n);
	HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	HPEN Pen2 = CreatePen(PS_SOLID, 1, RGB(220, 20, 0));
	HPEN Pen3 = CreatePen(PS_SOLID, 1, RGB(45, 255, 48));
	HBRUSH brush1 = CreateSolidBrush(RGB(219, 0, 0));
	HBRUSH brush2 = CreateSolidBrush(RGB(45, 255, 48));
	SetBkColor(hdc, RGB(12, 12, 12));

	while (!KEY_DOWN('T'))
	{
		SetTextColor(hdc, RGB(160, 160, 160));
		SelectObject(hdc, Pen1);
		SelectObject(hdc, brush1);

		//границы
		Line(hdc, x, y - 10, x + width, y - 10);
		Line(hdc, x, y - 10, x, y + height);
		Line(hdc, x + width, y - 10, x + width, y + height);
		Line(hdc, x, y + height, x + width, y + height);

		//ось Х
		Line(hdc, x, y + height * 0.9, x + width, y + height * 0.9);

		int posX = x + stepX;
		int posY = zeroPosY;

		//деления на оси Х
		for (size_t i = 0; i < n; i++)
		{
			Line(hdc, posX, posY + 5, posX, posY - 5);
			buf = to_string(xmas[i]);
			buf = buf.substr(0, buf.find(',') + 5);
			TextOutA(hdc, posX - buf.length() * 3, posY + 10, buf.c_str(), buf.length());
			posX += stepX;
		}
		posX = x;
		posY -= stepY;
		double mx = 0;
		for (size_t i = 0; i < n - 1; i++)
			mx += abs(ymas[i + 1] - ymas[i]);
		mx /= n;
		double min = min_elem(ymas, n);
		double max = max_elem(ymas, n);

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
		int prevY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[0] - min) / (max - min);
		for (size_t i = 2; i < n + 1; i++)
		{
			posX = x + stepX * i;
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[i - 1] - min) / (max - min);
			Line(hdc, posX - 1, posY + 1, posX - stepX + 1, prevY);
			prevY = posY;
		}

		//точки графика
		int radius = 2;
		for (size_t i = 1; i < n + 1; i++)
		{
			SelectObject(hdc, Pen2);
			posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (ymas[i - 1] - min) / (max - min);
			Ellipse(hdc, (x + stepX * i) - radius, posY - radius, (x + stepX * i) + radius, posY + radius);
		}

		//заданное значение
		SelectObject(hdc, Pen3);
		SelectObject(hdc, brush2);
		posX = x;
		posY = zeroPosY;
		for (size_t i = 0; i < n; i++)
		{
			if (xmas[i] > pointer)
			{
				posX += (pointer - xmas[i - 1]) * stepX / (xmas[1] - xmas[0]);
				posY = zeroPosY - stepY - (zeroPosY - 2 * stepY - y) * (then - min) / (max - min);
				break;
			}
			posX = x + stepX * (i + 1);
		}

		SetTextColor(hdc, RGB(160, 220, 160));
		buf = to_string(then);
		buf = buf.substr(0, buf.find(',') + 5);
		TextOutA(hdc, x + 60, posY - 10, buf.c_str(), buf.length());

		buf = to_string(pointer);
		buf = buf.substr(0, buf.find(',') + 5);
		TextOutA(hdc, posX, zeroPosY - 25, buf.c_str(), buf.length());

		Ellipse(hdc, posX - radius, posY - radius, posX + radius, posY + radius);
		Line(hdc, posX, zeroPosY - 5, posX, zeroPosY + 5);
		Line(hdc, x - 5, posY, x + 5, posY);
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

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

double min_elem(double* mas, int n)
{
	double min = mas[0];
	for (size_t i = 1; i < n; i++)
		if (mas[i] < min)
			min = mas[i];
	return min;
}

double max_elem(double* mas, int n)
{
	double max = mas[0];
	for (size_t i = 1; i < n; i++)
		if (mas[i] > max)
			max = mas[i];
	return max;
}