// Project2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <Python.h>
#include "DropboxConnection.h"
#include <functional>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "aes256.h"
#include "sha256.h"
#include "md5.h"

using namespace std;

map<string, string> fileInfoDict;

string readFile(string filename) {

	ifstream in;
	string buff;
	char c;

	in.open(filename, ifstream::binary);

	while ((c = in.get()) != EOF) buff.push_back(c);

	in.close();

	return buff;
}

void writeFile(string filename, char* buff, int n) {

	ofstream outfile;

	outfile.open(filename, ofstream::binary);
	outfile.write(buff, n);

	outfile.close();
}
string getFileName(string key, string filename) {

	SHA256 sha256;

	string ecryptedName = sha256(key + filename);

	return ecryptedName;
}

void generateAesKey(string key, string filename, unsigned char* aes_key) {

	SHA256 sha256;
	string aes_key_space = sha256(key + filename);

	for (int i = 0; i < 32; i++)
		aes_key[i] = aes_key_space[2 * i];

}

void encryptFile(string key, string filename, string filePath) {

	unsigned char aes_key[32];
	string buff;
	unsigned char *encryptedBuff;
	aes256_context ctx;

	string encryptedName = getFileName(key, filename);
	generateAesKey(key, filename, aes_key);

	buff = readFile(filePath);

	int len = buff.size();
	int pad = 16 - (len % 16);

	encryptedBuff = (unsigned char *)malloc(len + pad);

	for (int i = 0; i < len; i++) {
		encryptedBuff[i] = buff[i];
	}

	for (int i = 0; i < pad; i++) {
		encryptedBuff[len + i] = pad;
	}

	aes256_init(&ctx, aes_key);

	unsigned char* b = encryptedBuff;
	for (int i = 0; i < len + pad; i = i + 16, b = b + 16)
		aes256_encrypt_ecb(&ctx, b);

	writeFile(encryptedName, (char*)encryptedBuff, len + pad);

	aes256_done(&ctx);

	//cout << encryptedName;
}


void decryptFile(string key, string filename, string saveTo) {

	unsigned char aes_key[32];
	string buff;
	unsigned char *decryptedBuff;
	aes256_context ctx;

	string encryptedName = getFileName(key, filename);
	generateAesKey(key, filename, aes_key);

	buff = readFile(encryptedName);

	int len = buff.size();
	decryptedBuff = (unsigned char *)malloc(len);

	for (int i = 0; i < len; i++) {
		decryptedBuff[i] = buff[i];
	}

	cout << "Total Length: " << len << endl;

	aes256_init(&ctx, aes_key);

	unsigned char *b = decryptedBuff;
	for (int i = 0; i < len; i = i + 16, b = b + 16)
		aes256_decrypt_ecb(&ctx, b);

	int pad = decryptedBuff[len - 1];
	cout << "Padding: " << pad << endl;
	len = len - pad;

	string destPath = saveTo + "/" + filename;
	writeFile(destPath, (char *)decryptedBuff, len);

	aes256_done(&ctx);
}



vector<string> split(const string &s, const string &seperator){
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()){
		int flag = 0;
		while (i != s.size() && flag == 0){
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]){
					++i;
					flag = 0;
					break;
				}
		}

		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0){
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]){
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j){
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}


void loadFileInfoDict() {
	ifstream fin("fileInfo.txt");
	string s;
	fileInfoDict.clear();
	if (!fin){
		return;
	}
	while (getline(fin, s)){
		vector<string> info = split(s, ":");
		fileInfoDict[info[0]] = info[1];
	}
}

void writeFileInfoDict() {
	ofstream fout("fileInfo.txt");
	map<string, string>::iterator it = fileInfoDict.begin();
	string buffToWrite = "";
	for (; it != fileInfoDict.end(); it++) {
		buffToWrite = buffToWrite + it->first + ":" + it->second + "\n";
	}
	fout.write(buffToWrite.c_str(), strlen(buffToWrite.c_str()));
}


// priginalFilePath: the path of file to upload
void uploadFile(string originalFilePath) {
	vector<string> splitedString = split(originalFilePath, "\\");
	string originalName = splitedString[splitedString.size() - 1];
	string encryptedName = getFileName("Password", originalName);
	encryptFile("Password", originalName, originalFilePath);

	string encrypteFileBuff = readFile(encryptedName);
	string fileHash = MD5(encrypteFileBuff).toStr();

	DropboxConnection::uploadFile(encryptedName.c_str());

	// save original file name and md5hash
	fileInfoDict[originalName] = fileHash;
	writeFileInfoDict();

	// delete local encrypted file
	remove(encryptedName.c_str());
}

// originalFileName: decrypted file name
// saveTo: the path to save file, without last "/"
void downloadFile(string originalFileName, string saveTo) {
	string encryptedName = getFileName("Password", originalFileName);
	string dropboxPath = "/" + encryptedName;
	DropboxConnection::downloadFile(dropboxPath.c_str(), "");
	decryptFile("Password", originalFileName, saveTo);

	// delete local encrypted file
	remove(encryptedName.c_str());
}

void listDropboxFiles() {
	map<string, string>::iterator it = fileInfoDict.begin();
	string buffToWrite = "";
	for (; it != fileInfoDict.end(); it++) {
		cout << it->first << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	loadFileInfoDict();
	// uploadFile("C:/Users/ZhangBoxuan/Desktop/test.txt");
	// downloadFile("test.txt", "C:/Users/ZhangBoxuan/Desktop");
	//listDropboxFiles();

	while (true) {
		cout << "***********************************************************" << endl
			<< "***         Applied Cryptography -- Project 2           ***" << endl
			<< "***********************************************************" << endl
			<< "***         Boxuan Zhang (N18295695)                    ***" << endl
			<< "***         Yao Jin                                     ***" << endl
			<< "***         Yuanduo Chen                                ***" << endl
			<< "***********************************************************" << endl
			<< "*** List files in cloud: list                           ***" << endl
			<< "*** Upload file: upload FILE_PATH                       ***" << endl
			<< "*** Download file: download FILE_NAME PATH_TO_SAVE_FILE ***" << endl
			<< "***********************************************************" << endl
			<< "Please enter: ";

		string inputString;
		getline(cin, inputString);

		vector<string> args = split(inputString, " ");
		if (args[0] == "list") {
			listDropboxFiles();
		}
		else if (args[0] == "upload") {
			uploadFile(args[1]);
		}
		else if (args[0] == "download") {
			downloadFile(args[1], args[2]);
		}
	}



	system("pause");

	return 0;
}

