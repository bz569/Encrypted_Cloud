// Project2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Python.h>
#include "DropboxConnection.h"



int _tmain(int argc, _TCHAR* argv[])
{
	//DropboxConnection::uploadFile("C:/Users/ZhangBoxuan/Desktop/test1.txt");
	//DropboxConnection::downloadFile("/test.txt", "C:/Users/ZhangBoxuan/Desktop/");
	DropboxConnection::listFiles();

	system("pause");

	return 0;
}

