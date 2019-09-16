#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool loadFile(const char* fileName, string& result)
{
	try 
	{
		string line;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{
			while (myfile.good())
			{
				getline(myfile, line);
				result += line + "\n ";
			}
			myfile.close();
			result += "\0";
		}
		else 
		{
			cout << "Falha na leitura do shader" << endl;
			throw new exception;
		}

		return true;
	}
	catch (exception e)
	{
		cerr << "[ERRO]" << e.what() << ":" << fileName << endl;
	}

	return false;
}