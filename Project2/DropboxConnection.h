#pragma once
using namespace std;
#include <string>

// upload, download, list file for a certain Dropbox account
// Dropbox account: acproject2015spring@gmail.com
// Dropbox password:  acproject2015spring
class DropboxConnection
{
public:
	DropboxConnection();
	~DropboxConnection();

	// upload file to Dropbox
	// path: the path of file to upload
	static void uploadFile(char* path);

	// download file from Dropbox
	// path: the Dropbox path of file to download
	// saveTo: the folder to save the downloaded file
	static void downloadFile(char* path, char* saveTo);

	// list all files in Dropbox
	static void listFiles();
};

