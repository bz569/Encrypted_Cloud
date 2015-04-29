#include "stdafx.h"
#include "DropboxConnection.h"
#include <iostream>
#include <Python.h>


DropboxConnection::DropboxConnection()
{
}


DropboxConnection::~DropboxConnection()
{
}

void DropboxConnection::uploadFile(char* path){
	Py_Initialize();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./Project2/Project2')");
	pModule = PyImport_ImportModule("EncryptedCloudConnetion");
	PyErr_Print();
	pFunc = PyObject_GetAttrString(pModule, "uploadFile");
	PyObject *pyParam = PyTuple_New(1);
	PyObject *pyValue = PyString_FromString(path);
	PyTuple_SetItem(pyParam, 0, pyValue);
	PyEval_CallObject(pFunc, pyParam);

	Py_Finalize();
}

void DropboxConnection::downloadFile(char* path, char* saveTo){
	Py_Initialize();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./Project2/Project2')");
	pModule = PyImport_ImportModule("EncryptedCloudConnetion");
	PyErr_Print();
	pFunc = PyObject_GetAttrString(pModule, "downloadFile");
	PyObject *pyParam = PyTuple_New(2);
	PyObject *pyValue1 = PyString_FromString(path);
	PyTuple_SetItem(pyParam, 0, pyValue1);
	PyObject *pyValue2 = PyString_FromString(saveTo);
	PyTuple_SetItem(pyParam, 1, pyValue2);
	PyEval_CallObject(pFunc, pyParam);

	Py_Finalize();
}

void DropboxConnection::listFiles() {
	Py_Initialize();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./Project2/Project2')");
	pModule = PyImport_ImportModule("EncryptedCloudConnetion");
	PyErr_Print();
	pFunc = PyObject_GetAttrString(pModule, "listFiles");
	PyEval_CallObject(pFunc, NULL);

	Py_Finalize();
}