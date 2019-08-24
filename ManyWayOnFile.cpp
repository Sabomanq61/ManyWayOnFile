// ManyWayOnFile.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include<fstream>
#include <ctime>
#include "Header.h"
#include "Header1.h"
using namespace std;

void Rand_File(string,int);
void Print_File(string);
void Create_name(string *, int);
int Open_Read(fstream *f,string *, int);
void Open_Write(fstream *f, string *, int);
void Close_File(fstream *f, int);
void Splitting(string, string* ,fstream *,int);
bool Sort(string *, string*, fstream*, fstream*, int n,int flag);
int Find_Min(int *, int *, int);
bool Check_File_Order(string,int);

int main(int argc, char** argv)
{
	srand(time(0));
	int n(1);
	cout << "enter number in start file n = ";
	cin >> n;
	string name("start");
	Rand_File(name, n);    
	Print_File(name+".txt");
	
	int n_file;
	cout << "enter n_file = ";
	cin >> n_file;
	string *name_files_f,*name_files_g;

	name_files_f = new string[n_file];
	name_files_g = new string[n_file];
	string s1("f"), s2("g"), s3(".txt");
	
	for (int i = 0; i < n_file; i++)
	{
		name_files_f[i] = s1 + to_string(i) + s3;
		name_files_g[i] = s2 + to_string(i) + s3;
	}
	
	fstream *f, *g;
	f = new fstream[n_file];
	g = new fstream[n_file];
	Splitting(name, name_files_f, f,n_file);
	
	bool flag = false;
	int i = 0;
	
	while (!flag)
	{
		if (i % 2 == 0)
			flag = Sort(name_files_f, name_files_g, f, g, n_file,0);
		else
			flag = Sort(name_files_g, name_files_f, g, f, n_file,1);
		i++;
	}
	bool sort = false;
	if (i % 2 == 0)
		sort = Check_File_Order(name_files_f[0], n);
	else
		sort = Check_File_Order(name_files_g[0], n);
	if (sort)
		cout << "Sort was successful" << endl;
	else
		cout << "Error" << endl;
	system("pause");
	delete[]f;
	delete[]g;
	delete[]name_files_f;
	delete[]name_files_g;
	return 0;
}

void Rand_File(string name,int number)
{
	fstream start(name+".txt", ios::out);
	
	for (int i = 0; i < number; i++)
	{
		start << rand() % 1000 << " ";

	}
	start.close();
}

void Print_File(string name)
{
	ifstream f(name);
	int x = 0;
	while (f>>x)
	{
		cout << x << " ";
	}
	cout << endl;
	f.close();
}

void Create_name(string *name_files, int n_file)
{
	string s1("f"), s2("g"), s3(".txt");
	for (int i = 0; i < n_file; i++)
		name_files[i] = s1 + s3;
	for (int i = n_file; i <2*n_file; i++)
		name_files[i] = s2 + s3;
}

int Open_Read(fstream*f,string *name, int n_file)
{
	int cWf = n_file;
	for (int i = 0; i < n_file; i++)
	{
		f[i].open(name[i], ios::in);
		if (f[i].gcount() == 0)
			cWf--;
	}
	return cWf;
}

void Open_Write(fstream*f, string *name, int n_file)
{
	for (int i = 0; i < n_file; i++)
		f[i].open(name[i], ios::out);
}

void Close_File(fstream *f, int n_file)
{
	for (int i = 0; i < n_file; i++)
		f[i].close();
}

void Splitting(string name,string* file_names,fstream *f,int n_file)
{
	ifstream start(name+".txt");
	if (!start)
		cout << "Mistake open" << endl;
	int x, y;
	int i = 0;
	start >> x;

	Open_Write(f, file_names, n_file);
	f[i] << x<<" ";
	while (start >> y)
	{
		if (x < y)
			f[i] << y<<" ";
		else
		{
			if (i == (n_file - 1))
				i = 0;
			else
				i++;
			f[i] << y << " ";
		}
		x = y;
	}

	Close_File(f, n_file);
	start.close();
	for (int i = 0; i < n_file; i++)
	{
		cout << "f[" << i << "] :";
		Print_File(file_names[i]);
		cout << endl;
	}
}

bool Sort(string *name_f, string *name_g, fstream *f, fstream *g, int n,int flag)
{
	Open_Read(f, name_f, n);
	Open_Write(g, name_g, n);
	bool sign_end_sort = true;
	int number_file =0;
	int number_sect =0;
	int *work_flag = new int[n] {};
	int *sect_flag = new int[n] {};
	int *a = new int[n] {};

	for (int i = 0; i < n; i++)
	{
		int temp;
		if (f[i] >> temp)
		{
			a[i] = temp;
			work_flag[i] = 1;
			sect_flag[i] = 1;
			number_file++;
			number_sect++;
		}
	}
	
	
	int wrt = 0;
	while (number_file > 0)
	{
		while (number_sect > 0)
		{
			int min = Find_Min(a, sect_flag, n);
			g[wrt] << a[min] << " ";
			int temp;
			if (f[min] >> temp)
			{
				if (a[min] > temp)
				{
					number_sect--;
					sect_flag[min] = 0;
					sign_end_sort = false;
				}
				a[min] = temp;
			}
			else
			{
				number_sect--;
				number_file--;
				work_flag[min] = 0;
				sect_flag[min] = 0;
			}
		}
		wrt++;
		if (wrt == n)
			wrt = 0;
		for (int i2 = 0; i2 < n; i2++)
		{
			if (work_flag[i2] == 1)
				sect_flag[i2] = 1;
		}
		number_sect = number_file;
	}
	Close_File(f, n);
	Close_File(g, n);
	delete[]a;
	delete[]work_flag;
	delete[]sect_flag;
	
	char s;
	if (flag == 0)
		s = 'g';
	else
		s = 'f';
	for (int i = 0; i < n; i++)
	{
		cout <<s<< "[" << i << "] :";
		Print_File(name_g[i]);
		cout << endl;
	}
	return sign_end_sort;
}

int Find_Min(int *a, int *flag, int n)
{
	int i_min = 0;
	for (int i = 0; i < n; i++)
	{
		if (flag[i] == 1)
		{
			i_min = i;
			break;
		}
	}
	for (int i = i_min+1; i < n; i++)
	{
		if (a[i] < a[i_min] && (flag[i] == 1))
			i_min = i;
	}
	return i_min;
}

bool Check_File_Order(string t, int n)
{
	ifstream file(t);
	if (file.is_open())
	{
		int number(2), a, b;
		file >> a >> b;
		while (a <= b)
		{
			a = b;
			if (!(file >> b))
				break;
			number++;
		}
		if (number == n)
			return true;
		else
			return false;
	}
	return false;
}
