# coding=utf-8
"""
Applied Cryptography Project 2
Python Module for Dropbox

Description:

Upload file: EncryptedCloudConnection.py upload FILE_PATH
List file in Dropbox: EncryptedCloudConnection.py list
Download file EncryptedCloudConnection.py download FILE_PATH

Group Member；
Boxuan Zhang
Yao Jin
Yuanduo Chen
"""

import dropbox
import json
import sys

# def __init__(self):
#     self.app_key = 's4j59x5ex076ffu'
#     self.app_secret = 'f6weklkcv7tuyc2'
#     self.access_token = "wH4LxtX-P7AAAAAAAAAABd_Io3zHVGYFN-XbmGToR02TbJ_29ziDyUUSVWjmW92j"

def uploadFile(path):
    access_token = "wH4LxtX-P7AAAAAAAAAABd_Io3zHVGYFN-XbmGToR02TbJ_29ziDyUUSVWjmW92j"
    client = dropbox.client.DropboxClient(access_token)

    fileName = path.split('/')[-1]

    f = open(path)
    response = client.put_file('/'+fileName, f)

def listFiles():
    access_token = "wH4LxtX-P7AAAAAAAAAABd_Io3zHVGYFN-XbmGToR02TbJ_29ziDyUUSVWjmW92j"
    client = dropbox.client.DropboxClient(access_token)

    folder_metadata = client.metadata('/')
    fileList = []
    for content in folder_metadata[u'contents']:
        filePath = content[u'path'].encode("utf-8")
        fileList.append(filePath)
    for file in fileList:
        print file

def downloadFile(path, saveTo):
    access_token = "wH4LxtX-P7AAAAAAAAAABd_Io3zHVGYFN-XbmGToR02TbJ_29ziDyUUSVWjmW92j"
    client = dropbox.client.DropboxClient(access_token)

    f, metadata = client.get_file_and_metadata(path)
    fileName = path.split("/")[-1]
    out = open(saveTo + fileName, 'wb')
    out.write(f.read())
    out.close()

# if __name__ == "__main__":
#     connection = EncryptedCloudConnection()
#     function = sys.argv[1]
#     if function == 'upload':
#         connection.uploadFile(sys.argv[2])
#     elif function == 'list':
#         list = connection.listFiles()
#         for filePath in list:
#             print filePath
#     elif function == 'download':
#         connection.downloadFile(sys.argv[2])
