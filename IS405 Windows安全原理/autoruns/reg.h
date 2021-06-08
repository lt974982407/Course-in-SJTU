#ifndef REG_H
#define REG_H

#include <windows.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <QString>
#include <cstdlib>
#include "sig.h"
//#include "checkSig.h"
#include "description.h"

struct item{
    DWORD type;
    BOOL flag;
    QString name;
    QString pub;
    QString time;
    QString description;
    QString ImagePath;
};

void correction (QString & str){
    if (str.contains("\""))
           str = str.replace("\"", "");
    if (str.contains("\\??\\"))
            str = str.replace("\\??\\", "");
    if (str.contains("\\SystemRoot", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("\\SystemRoot", 0, Qt::CaseInsensitive), 11, "C:\\Windows");
    if (str.startsWith("system32", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("system32", 0, Qt::CaseInsensitive), 8, "C:\\Windows\\System32");
    if (str.startsWith("System32", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("System32", 0, Qt::CaseInsensitive), 8, "C:\\Windows\\System32");
    if (str.contains("%systemroot%", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("%systemroot%", 0, Qt::CaseInsensitive), 13, "C:\\Windows\\");
    if (str.contains("@%windir%", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("@%windir%", 0, Qt::CaseInsensitive), 9, "C:\\Windows");
    if (str.contains("%windir%", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("%windir%", 0, Qt::CaseInsensitive), 8, "C:\\Windows");
    if (str.contains("%SystemRoot%", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("%SystemRoot%", 0, Qt::CaseInsensitive), 13, "C:\\Windows\\");
    if (str.contains("%Systemroot%", Qt::CaseInsensitive))
            str = str.replace(str.indexOf("%Systemroot%", 0, Qt::CaseInsensitive), 13, "C:\\Windows\\");
    if(str.contains(".exe"))
    {
        str = str.section(".exe",0,0);
        str = str+".exe";
    }

    if(str.contains(".sys"))
    {
        str = str.section(".sys",0,0);
        str = str+".sys";
    }

}

DWORD getType(HKEY rootkey, LPCSTR subkey){
    HKEY hkey;
    int flag = 0;
    if (ERROR_SUCCESS == RegOpenKeyExA(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if(ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            for (DWORD index = 0; index < dwKeyValueCnt; ++index){
                LPTSTR  lpszKeyValueName = new TCHAR [dwKeyValueNameMaxLen*2+ 1];//[dwKeyValueNameMaxLen*2 + 1];
                memset(lpszKeyValueName, 0, sizeof(TCHAR)*(dwKeyValueNameMaxLen + 1));
                DWORD  dwNameCnt = dwKeyValueNameMaxLen + 1;
                DWORD  dwKeyValueDataLen = dwKeyValueDataMaxLen + 1;
                LPBYTE lpbKeyValueData = (LPBYTE) malloc(dwKeyValueDataMaxLen);//new BYTE[dwKeyValueDataMaxLen*2 + 1];
                DWORD  dwKeyValueType;
                RegEnumValue(hkey,index,lpszKeyValueName,&dwNameCnt,NULL,&dwKeyValueType,lpbKeyValueData,&dwKeyValueDataLen);
                int size = WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, NULL, 0, NULL, NULL);
                char* chRtn  =new char[size*sizeof(char)];
                WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, chRtn, size, NULL, NULL);
                std::string str(chRtn);
                delete[] chRtn;
                if(str == "Type"){
                    flag = 1;
                }
                delete [] lpszKeyValueName;
                free (lpbKeyValueData);
            }

        }
        if (flag){
            DWORD type;
            DWORD dwtype = REG_DWORD;
            DWORD dwvalue;
            RegQueryValueEx(hkey, _T("Type"), 0, &dwtype, (LPBYTE)&type, &dwvalue);
            return type;
        }
        else {
            return 0;
        }
    }
    return 0;
}

QString getDescription(HKEY rootkey, LPCSTR subkey){
    HKEY hkey;
    if (ERROR_SUCCESS == RegOpenKeyExA(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if(ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            for (DWORD index = 0; index < dwKeyValueCnt; ++index){
                LPTSTR  lpszKeyValueName = new TCHAR [dwKeyValueNameMaxLen*2+ 1];//[dwKeyValueNameMaxLen*2 + 1];
                memset(lpszKeyValueName, 0, sizeof(TCHAR)*(dwKeyValueNameMaxLen + 1));
                DWORD  dwNameCnt = dwKeyValueNameMaxLen + 1;
                DWORD  dwKeyValueDataLen = dwKeyValueDataMaxLen + 1;
                LPBYTE lpbKeyValueData = (LPBYTE) malloc(dwKeyValueDataMaxLen*2);//new BYTE[dwKeyValueDataMaxLen*2 + 1];
                DWORD  dwKeyValueType;
                RegEnumValue(hkey,index,lpszKeyValueName,&dwNameCnt,NULL,&dwKeyValueType,lpbKeyValueData,&dwKeyValueDataLen);
                int size = WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, NULL, 0, NULL, NULL);
                char* chRtn  =new char[size*sizeof(char)];
                WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, chRtn, size, NULL, NULL);
                std::string str(chRtn);
                delete[] chRtn;
                if(str == "Description"){
                    wchar_t* ch =(wchar_t *)lpbKeyValueData;
                    std::wstring res = std::wstring(ch);
                    QString qtstring = QString::fromWCharArray(res.c_str());
                    delete [] lpszKeyValueName;
                    free (lpbKeyValueData);
                    return qtstring;
                }
                delete [] lpszKeyValueName;
                free (lpbKeyValueData);
            }

        }
    }
    return "";
}

TCHAR* getImagePath(HKEY rootkey, LPCSTR subkey){
    HKEY hkey;
    TCHAR lpbKeyValueData[1024];
    if (ERROR_SUCCESS == RegOpenKeyExA(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if(ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            for (DWORD index = 0; index < dwKeyValueCnt; ++index){
                LPTSTR  lpszKeyValueName = new TCHAR [dwKeyValueNameMaxLen*2+ 1];
                memset(lpszKeyValueName, 0, sizeof(TCHAR)*(dwKeyValueNameMaxLen + 1));
                DWORD  dwNameCnt = dwKeyValueNameMaxLen + 1;
                DWORD  dwKeyValueDataLen = dwKeyValueDataMaxLen + 1;
                DWORD  dwKeyValueType;
                //DWORD type;
                RegEnumValue(hkey,index,lpszKeyValueName,&dwNameCnt,NULL,&dwKeyValueType,LPBYTE(lpbKeyValueData),&dwKeyValueDataLen);
                int size = WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, NULL, 0, NULL, NULL);
                char* chRtn  =new char[size*sizeof(char)];
                WideCharToMultiByte(CP_ACP, 0, lpszKeyValueName, -1, chRtn, size, NULL, NULL);
                std::string str(chRtn);
                delete[] chRtn;
                if(str == "ImagePath"){
                    wchar_t str2[1024] = {0};
                    QString path1 = QString::fromWCharArray(lpbKeyValueData);
                    correction(path1);
                     path1.toWCharArray(str2);
                    return str2;//lpbKeyValueData;
                }
                delete [] lpszKeyValueName;
                //free (lpbKeyValueData);
            }

        }
    }
    return lpbKeyValueData;
}




int getpath(HKEY rootkey,LPCWSTR subkey,item* items){
    HKEY hkey;
    int indexx = 0;
    if (ERROR_SUCCESS == RegOpenKeyEx(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if (ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            for (DWORD index = 0; index < dwSubKeyCnt; ++index){
                DWORD dwSubKeySize = dwSubKeyNameMaxLen + 1;
                LPTSTR  lpszSubName = new TCHAR[dwSubKeyNameMaxLen + 1];
                memset(lpszSubName, 0, sizeof(TCHAR)*(dwSubKeyNameMaxLen + 1));
                RegEnumKeyEx(hkey,index,lpszSubName,&dwSubKeySize,NULL, NULL, NULL, NULL);
                char* name = new char[1024];
                int size = WideCharToMultiByte(CP_ACP, 0, lpszSubName, -1, NULL, 0, NULL, NULL);
                char* chRtn  =new char[size*sizeof(char)];
                WideCharToMultiByte(CP_ACP, 0, lpszSubName, -1, chRtn, size, NULL, NULL);
                std::string str(chRtn);
                delete[] chRtn;
                std::string path = "System\\CurrentControlSet\\Services\\" + std::string(str);
                LPCSTR res = path.c_str();
                DWORD ttype = getType(HKEY_LOCAL_MACHINE,res);
                if(ttype == 0)
                    continue;
                items[indexx].name = QString::fromWCharArray( lpszSubName);
                wchar_t str2[1024] = {0};
                QString::fromWCharArray(getImagePath(HKEY_LOCAL_MACHINE,res)).toWCharArray(str2);
                items[indexx].flag = VerifyEmbeddedSignature(str2);
                items[indexx].type = ttype;
                QString qdes = getDescription(HKEY_LOCAL_MACHINE,res);
                if(qdes.startsWith("@%", Qt::CaseInsensitive) || qdes.startsWith("C:", Qt::CaseInsensitive)|| qdes.startsWith("@C:", Qt::CaseInsensitive)){
                    wchar_t temp[1024] = {0};
                    qdes.toWCharArray(temp);
                    QString strr;
                    DllDescription(temp,strr);
                    if (strr == QString::fromWCharArray(L"") || strr == qdes){
                        items[indexx].description = "";
                    }
                    else{
                        items[indexx].description = strr;
                    }

                }
                else{
                    items[indexx].description = qdes;
                }
                if(qdes == ""){
                    QString des;
                    FileDescription(str2,des);
                    items[indexx].description = des;
                }
                items[indexx].ImagePath = QString::fromWCharArray(getImagePath(HKEY_LOCAL_MACHINE,res));
                QString pub;
                QString time;
                GetSignaturePublisher(getImagePath(HKEY_LOCAL_MACHINE,res),pub,time);
                items[indexx].pub = pub;
                items[indexx].time = time;
                indexx++;
                delete [] name;
                delete[] lpszSubName;
            }
        }
    }
    return indexx;
}

int getValue(HKEY rootkey,LPCSTR subkey,item* items){
    HKEY hkey;
    int tolnum = 0;
    if (ERROR_SUCCESS == RegOpenKeyExA(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if (ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            tolnum = dwKeyValueCnt;
            for (DWORD index = 0; index < dwKeyValueCnt; ++index){
                LPTSTR  lpszKeyValueName = new TCHAR [dwKeyValueNameMaxLen*2+ 1];//[dwKeyValueNameMaxLen*2 + 1];
                memset(lpszKeyValueName, 0, sizeof(TCHAR)*(dwKeyValueNameMaxLen + 1));
                DWORD  dwNameCnt = dwKeyValueNameMaxLen + 1;
                DWORD  dwKeyValueDataLen = dwKeyValueDataMaxLen + 1;
                TCHAR lpbKeyValueData[1024];
                DWORD  dwKeyValueType;
                RegEnumValue(hkey,index,lpszKeyValueName,&dwNameCnt,NULL,&dwKeyValueType,LPBYTE(lpbKeyValueData),&dwKeyValueDataLen);
                wchar_t str2[1024] = {0};
                QString path1 = QString::fromWCharArray(lpbKeyValueData);
                correction(path1);
                path1.toWCharArray(str2);
                items[index].name = QString::fromWCharArray(lpszKeyValueName);
                QString des;
                FileDescription(str2,des);
                items[index].description = des;
                items[index].ImagePath = path1;
                QString pub;
                QString time;
                GetSignaturePublisher(str2,pub,time);
                items[index].pub = pub;
                items[index].time = time;
                items[index].flag = VerifyEmbeddedSignature(str2);
                delete[] lpszKeyValueName;
            }
        }

    }
    return tolnum;
}

int getDlls (HKEY rootkey,LPCSTR subkey,item* items){
    HKEY hkey;
    int tolnum = 0;
    if (ERROR_SUCCESS == RegOpenKeyExA(rootkey, subkey, 0, KEY_READ, &hkey)){
        DWORD dwSubKeyCnt;          // 子键的数量
        DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueCnt;        // 键值项的数量
        DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)
        DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)
        if (ERROR_SUCCESS == RegQueryInfoKey(hkey,NULL,NULL,0,&dwSubKeyCnt,&dwSubKeyNameMaxLen,NULL,&dwKeyValueCnt,&dwKeyValueNameMaxLen,&dwKeyValueDataMaxLen,NULL,NULL)){
            tolnum = dwKeyValueCnt;
            for (DWORD index = 0; index < dwKeyValueCnt; ++index){
                LPTSTR  lpszKeyValueName = new TCHAR [dwKeyValueNameMaxLen*2+ 1];//[dwKeyValueNameMaxLen*2 + 1];
                memset(lpszKeyValueName, 0, sizeof(TCHAR)*(dwKeyValueNameMaxLen + 1));
                DWORD  dwNameCnt = dwKeyValueNameMaxLen + 1;
                DWORD  dwKeyValueDataLen = dwKeyValueDataMaxLen + 1;
                TCHAR lpbKeyValueData[1024];
                DWORD  dwKeyValueType;
                RegEnumValue(hkey,index,lpszKeyValueName,&dwNameCnt,NULL,&dwKeyValueType,LPBYTE(lpbKeyValueData),&dwKeyValueDataLen);
                wchar_t str2[1024] = {0};
                QString path1 = QString::fromWCharArray(lpbKeyValueData);
                path1 = "C:\\windows\\syswow64\\"+path1;
                path1.toWCharArray(str2);
                items[index].name = QString::fromWCharArray(lpszKeyValueName);
                QString des;
                FileDescription(str2,des);
                items[index].description = des;
                items[index].ImagePath = path1;
                QString pub;
                QString time;
                GetSignaturePublisher(str2,pub,time);
                items[index].pub = pub;
                items[index].time = time;
                items[index].flag = VerifyEmbeddedSignature(str2);
                delete[] lpszKeyValueName;
            }
        }

    }
    return tolnum;
}

#endif // REG_H
