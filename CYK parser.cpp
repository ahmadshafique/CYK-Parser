#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<algorithm>
using namespace std;

#define MAX 100

string gram[MAX][MAX];  //to store entered grammar
int np;					//no of productions

//concatenates unique non-terminals
string concat(string a, string b)
{
	string r = a;
	for (int i = 0; i < b.length(); i++)
		if (r.find(b[i]) > r.length())
			r += b[i];
	return r;
}

//creates every combination of variables from a and b . For eg: BA * AB = {BA, BB, AA, BB}
string gen_comb(string a, string b)
{
	string pri, re = "";
	for (int i = 0; i < a.length(); i++)
		for (int j = 0; j < b.length(); j++)
		{
			pri = "";
			pri = pri + a[i] + b[j];

			//searches if the generated productions p can be created by variables
			for (int j = 0; j < np; j++)
			{
				for (int k = 1; gram[j][k] != ""; k++)
				{
					if (gram[j][k] == pri)
					{
						re = concat(re, gram[j][0]);
					}
				}
			}
		}
	return re;
}

int main(int argc, char** argv)
{
	int i, pt, j, l, k;
	string a, str, r, pr, start = "S";
	char buff[100];
	np = 0;
	bool prod_found=false;
	
	//ifstream ifile("/Users/ahmad/source/repos/CYKparser/CYKparser/f1.txt");
	ifstream ifile(argv[1]);

	while (ifile.getline(buff, 99))
	{
		a = buff;
		if (a.find("PRODUCTIONS:") < a.length())
		{	
			prod_found=true;
			break;
		}
	}
	
	if(!prod_found)
	{
		cout<<"Unable to read production rules! Terminating the program\n";
		ifile.close();
		system("pause");
		return 1;
	}

	while (ifile.getline(buff, 99)) {
		a = buff;
		a.erase(remove_if(a.begin(), a.end(), isspace), a.end());
		a.erase(remove(a.begin(), a.end(), ';'), a.end());
		pt = a.find("->");

		//left side of production
		gram[np][0] = a.substr(0, pt);
		a = a.substr(pt + 2, a.length());
		//right side of production
		for (j = 0; a.length()!=0; j++)
		{
			int i = a.find("|");
			if (i > a.length())
			{
				gram[np][j + 1] = a;
				a = "";
			}
			else
			{
				gram[np][j + 1] = a.substr(0, i);
				a = a.substr(i + 1, a.length());
			}
		}

		np++;
	}
	ifile.close();

	string table[MAX][MAX], st;
	set<string> strings;

	//ifstream ifile1("/Users/ahmad/source/repos/CYKparser/CYKparser/f2.txt");
	ifstream ifile1(argv[2]);
	while (ifile1.getline(buff, 999)) {
		str = buff;
		strings.insert(str);
	}
	ifile1.close();

	//For each string read from strings file
	for (set<string>::iterator it = strings.begin(); it != strings.end(); ++it) {
		str = *it;

		//Assigns values to principal diagonal of matrix
		for (i = 0; i < str.length(); i++)
		{
			r = "";
			st = "";
			st += str[i];

			for (j = 0; j < np; j++)
			{
				for (k = 1; gram[j][k] != ""; k++)
				{
					if (gram[j][k] == st)
					{
						r = concat(r, gram[j][0]);
					}
				}
			}
			table[i][i] = r;
		}

		//Assigns values to upper half of the matrix
		for (k = 1; k < str.length(); k++)
		{
			for (j = k; j < str.length(); j++)
			{
				r = "";
				for (l = j - k; l < j; l++)
				{
					pr = gen_comb(table[j - k][l], table[l + 1][j]);
					r = concat(r, pr);
				}
				table[j - k][j] = r;
			}
		}

		/*
		//Prints the matrix
		for (i = 0; i < str.length(); i++)
		{
			k = 0;
			l = str.length() - i - 1;
			for (j=l; j<str.length(); j++)
			{
				cout << "     " << matrix[k++][j] << " ";
			}
			cout << endl;
		}
		*/

		//Checks if last element of first row contains a Start variable
		if (table[0][str.length() - 1].find(start) <= table[0][str.length() - 1].length())
			cout << "Part of language\n";
		else
			cout << "Not part of language\n";
	}

	system("pause");
	return 0;
}