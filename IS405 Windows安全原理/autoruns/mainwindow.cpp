#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include "reg.h"
#include "menu.h"
//#include "description.h"
#include "task.h"

void InitCol (QTableWidget *table);
void InitLogon (QTableWidget *table);
void InitService (QTableWidget *table);
void InitDriver (QTableWidget *table);
void InitTask (QTableWidget *table);
void InitDlls (QTableWidget *table);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->resize(2000,1000);

    InitLogon(ui->logonTable);
    InitService(ui->ServiceTable);
    InitDriver(ui->DriversTable);
    InitTask(ui->TaskTable);
    InitDlls (ui->DllTable);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void InitCol (QTableWidget *table){
     QStringList strs = {"Item", "Description", "Publisher","ImagePath","Time","Verified"};
     table->resize(1800,1000);
     table->setColumnCount(6);
     table->setHorizontalHeaderLabels(strs);
     table->setShowGrid(false);
     table-> verticalHeader()->setVisible(false);
     table->setColumnWidth(0, 100);
     table->setColumnWidth(1, 170);
     table->setColumnWidth(2, 130);
     table->setColumnWidth(3, 800);
     table->setColumnWidth(4, 100);
     table->setColumnWidth(5, 170);

}

void InitLogon (QTableWidget *table){
    InitCol(table);
    item * items;
    HKEY rootkey = HKEY_LOCAL_MACHINE;
    LPCSTR keydata = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    items = new item [1024];
    int num = getValue(rootkey,keydata,items);
    table->setRowCount(1000);
    int offset = 0;

    for (int i = 0; i < num; i++){
        if(items[i].name == QString::fromWCharArray(L"")){
            offset ++;
            continue;
        }
        table->setItem(i-offset,0,new QTableWidgetItem (items[i].name));
        table->setItem(i-offset,1,new QTableWidgetItem (items[i].description));
        table->setItem(i-offset,2,new QTableWidgetItem (items[i].pub));
        table->setItem(i-offset,3,new QTableWidgetItem (items[i].ImagePath));
        table->setItem(i-offset,4,new QTableWidgetItem (items[i].time));
        if (items[i].flag){
             table->setItem(i-offset,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i-offset,5,new QTableWidgetItem ("Not verified"));
        }
    }
    delete [] items;

    HKEY rootkey2 = HKEY_LOCAL_MACHINE;
    LPCSTR keydata2 =  "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
    items = new item [1024];
    int num2 = getValue(rootkey2,keydata2,items);
    for (int i = num; i < num+num2; i++){
        if(items[i-num].name == QString::fromWCharArray(L"")){
            offset ++;
            continue;
        }
        table->setItem(i-offset,0,new QTableWidgetItem (items[i-num].name));
        table->setItem(i-offset,1,new QTableWidgetItem (items[i-num].description));
        table->setItem(i-offset,2,new QTableWidgetItem (items[i-num].pub));
        table->setItem(i-offset,3,new QTableWidgetItem (items[i-num].ImagePath));
        table->setItem(i-offset,4,new QTableWidgetItem (items[i-num].time));
        if (items[i-num].flag){
             table->setItem(i-offset,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i-offset,5,new QTableWidgetItem ("Not verified"));
        }
    }
    delete [] items;

    HKEY rootkey3 = HKEY_CURRENT_USER;
    LPCSTR keydata3 = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    items = new item [1024];
    int num3 = getValue(rootkey3,keydata3,items);
    for (int i = num+num2; i < num+num2+num3; i++){
        if(items[i-num-num2].name == QString::fromWCharArray(L"")){
            offset ++;
            continue;
        }
        table->setItem(i-offset,0,new QTableWidgetItem (items[i-num-num2].name));
        table->setItem(i-offset,1,new QTableWidgetItem (items[i-num-num2].description));
        table->setItem(i-offset,2,new QTableWidgetItem (items[i-num-num2].pub));
        table->setItem(i-offset,3,new QTableWidgetItem (items[i-num-num2].ImagePath));
        table->setItem(i-offset,4,new QTableWidgetItem (items[i-num-num2].time));
        table->setItem(i-offset,5,new QTableWidgetItem (items[i-num-num2].flag));
        if (items[i-num-num2].flag){
             table->setItem(i-offset,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i-offset,5,new QTableWidgetItem ("Not verified"));
        }
    }
    delete[] items;

    items = new item [20];
    int num4 = getmenu(TEXT("C:\\Users\\LITIE974982407\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\*"),items);
    for(int i = num+num2+ num3; i< num+num2+num3+num4; i++){
        table->setItem(i-offset,0,new QTableWidgetItem (items[i-num-num2-num3].name));
        table->setItem(i-offset,1,new QTableWidgetItem (items[i-num-num2-num3].description));
        table->setItem(i-offset,2,new QTableWidgetItem (items[i-num-num2-num3].pub));
        table->setItem(i-offset,3,new QTableWidgetItem (items[i-num-num2-num3].ImagePath));
        table->setItem(i-offset,4,new QTableWidgetItem (items[i-num-num2-num3].time));
        table->setItem(i-offset,5,new QTableWidgetItem (items[i-num-num2-num3].flag));
        if (items[i-num-num2-num3].flag){
             table->setItem(i-offset,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i-offset,5,new QTableWidgetItem ("Not verified"));
        }
    }
    delete[] items;
}

void InitService (QTableWidget *table){
    InitCol(table);
    item * items;
    HKEY rootkey = HKEY_LOCAL_MACHINE;
    LPCWSTR keydata = TEXT ("System\\CurrentControlSet\\Services");
    items = new item [1024];
    int num = getpath(rootkey,keydata,items);
    int sernum = 0;
    for (int i = 0; i < num; i++){
        if(items[i].type == 16 ||items[i].type == 32 || items[i].type == 256){
            sernum++;
        }
    }
    table->setRowCount(sernum + 1);
    int ii = 0;
    for (int i = 0; i < num; i++){
        if(!(items[i].type == 16 ||items[i].type == 32 || items[i].type == 256)){
            continue;
        }
        table->setItem(ii,0,new QTableWidgetItem (items[i].name));
        table->setItem(ii,1,new QTableWidgetItem (items[i].description));
        table->setItem(ii,2,new QTableWidgetItem (items[i].pub));
        table->setItem(ii,3,new QTableWidgetItem (items[i].ImagePath));
        table->setItem(ii,4,new QTableWidgetItem (items[i].time));
        if(items[i].flag == true){
            table->setItem(ii,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(ii,5,new QTableWidgetItem ("Not verified"));
        }
        ii++;
    }
    delete[] items;

}

void InitDriver (QTableWidget *table){
    InitCol(table);
    item * items;
    HKEY rootkey = HKEY_LOCAL_MACHINE;
    LPCWSTR keydata = TEXT ("System\\CurrentControlSet\\Services");
    items = new item [1024];
    int num = getpath(rootkey,keydata,items);
    int sernum = 0;
    for (int i = 0; i < num; i++){
        if(items[i].type == 1 ||items[i].type == 2 || items[i].type == 8){
            sernum++;
        }
    }
    table->setRowCount(sernum + 1);
    int ii = 0;
    for (int i = 0; i < num; i++){
        if(!(items[i].type == 1 ||items[i].type == 2 || items[i].type == 8)){
            continue;
        }
        table->setItem(ii,0,new QTableWidgetItem (items[i].name));
        table->setItem(ii,1,new QTableWidgetItem (items[i].description));
        table->setItem(ii,2,new QTableWidgetItem (items[i].pub));
        table->setItem(ii,3,new QTableWidgetItem (items[i].ImagePath));
        table->setItem(ii,4,new QTableWidgetItem (items[i].time));
        if(items[i].flag == true){
            table->setItem(ii,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(ii,5,new QTableWidgetItem ("Not verified"));
        }
        ii++;
    }
    delete[] items;

}

void InitTask (QTableWidget *table){
    InitCol(table);
    int num = 0;
    taskDetail* tasks = new taskDetail[1024];
    gettask(tasks,num);
    table->setRowCount(num + 1);
    for (int i = 0;i < num; i++){
        table->setItem(i,0,new QTableWidgetItem (tasks[i].name));
        QString impath = tasks[i].imagePath;
        correction(impath);
        table->setItem(i,3,new QTableWidgetItem (impath));
        wchar_t str2[1024] = {0};
        impath.toWCharArray(str2);
        QString des;
        FileDescription(str2,des);
        table->setItem(i,1,new QTableWidgetItem (des));
        QString pub,time;
        GetSignaturePublisher(str2,pub,time);
        table->setItem(i,2,new QTableWidgetItem (pub));
        table->setItem(i,4,new QTableWidgetItem (time));
        if (VerifyEmbeddedSignature(str2)){
            table->setItem(i,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i,5,new QTableWidgetItem ("Not verified"));
        }

    }
    delete[] tasks;
}

void InitDlls (QTableWidget *table){
    InitCol(table);
    HKEY rootkey = HKEY_LOCAL_MACHINE;
    LPCSTR keydata = "System\\CurrentControlSet\\Control\\Session Manager\\KnownDlls";
    item* items;
    items = new item [1024];
    int num = 0;
    num = getDlls(rootkey,keydata,items);
    table->setRowCount(num + 1);
    for (int i = 0; i < num; i++){
        table->setItem(i,0,new QTableWidgetItem (items[i].name));
        table->setItem(i,1,new QTableWidgetItem (items[i].description));
        table->setItem(i,2,new QTableWidgetItem (items[i].pub));
        table->setItem(i,3,new QTableWidgetItem (items[i].ImagePath));
        table->setItem(i,4,new QTableWidgetItem (items[i].time));
        if(items[i].flag == true){
            table->setItem(i,5,new QTableWidgetItem ("Verified"));
        }
        else{
            table->setItem(i,5,new QTableWidgetItem ("Not verified"));
        }
    }


    delete[] items;
}
