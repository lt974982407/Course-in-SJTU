#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "reg.h"
//#include "sig.h"
//#pragma comment(lib, "User32.lib")

int getmenu(LPCWSTR dir, item* items  )
{
   WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   //TCHAR szDir[MAX_PATH];
   //size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;

   // If the directory is not specified as a command-line argument,
   // print usage.


   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.


   //if (length_of_arg > (MAX_PATH - 3))
   //{
     // _tprintf(TEXT("\nDirectory path is too long.\n"));
      //return (-1);
   //}

   //_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   //StringCchCopy(szDir, MAX_PATH, argv[1]);
   //StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(dir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind)
   {
      //DisplayErrorBox(TEXT("FindFirstFile"));
      return dwError;
   }

   // List all the files in the directory with some info about them.
   int index = 0;
   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         //_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
      }
      else
      {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
         //_tprintf(TEXT("  %s   \n"), ffd.cFileName);
         QString str = QString::fromWCharArray(ffd.cFileName);

         items[index].name = str;
         QString str2 = "C:\\Users\\LITIE974982407\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\"+str;
         //std::cout<<str2.toStdString().data()<<std::endl;
         wchar_t path[1024] = {0};
         str2.toWCharArray(path);
         QString pub;
         QString time;
         GetSignaturePublisher(path,pub,time);
         items[index].flag = VerifyEmbeddedSignature(path);
         QString qdes;
         FileDescription(path,qdes);
         items[index].description = qdes;
         items[index].pub = pub;
         items[index].time = time;
         items[index].ImagePath = str2;
         index++;
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);

   dwError = GetLastError();
   //if (dwError != ERROR_NO_MORE_FILES)
   //{
      //DisplayErrorBox(TEXT("FindFirstFile"));
   //}

   FindClose(hFind);
   return index;
}

#endif // MENU_H
