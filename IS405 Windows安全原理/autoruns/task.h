#ifndef TASK_H
#define TASK_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <QString>
#include <taskschd.h>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

struct taskDetail{
    QString name;
    QString imagePath;
};

void getTasks(ITaskFolder* rootFolder, HRESULT hr,taskDetail* tasks, int& tol);

void gettask (taskDetail *tasks, int & tol){
    //  Initialize COM.
        CoUninitialize();
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if( FAILED(hr) )
        {
            //printf("\nCoInitializeEx failed: %x", hr );
            //std::cout<<hr<<std::endl;
            return;
        }

        //  Set general COM security levels.
        hr = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            0,
            NULL);

        if( FAILED(hr) )
        {
           // printf("\nCoInitializeSecurity failed: %x", hr );
            CoUninitialize();
            return ;
        }

        //  ------------------------------------------------------
        //  Create an instance of the Task Service.
        ITaskService *pService = NULL;
        hr = CoCreateInstance( CLSID_TaskScheduler,
                               NULL,
                               CLSCTX_INPROC_SERVER,
                               IID_ITaskService,
                               (void**)&pService );
        if (FAILED(hr))
        {
              //printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
              CoUninitialize();
              return ;
        }

        //  Connect to the task service.
        hr = pService->Connect(_variant_t(), _variant_t(),
            _variant_t(), _variant_t());
        if( FAILED(hr) )
        {
            //printf("ITaskService::Connect failed: %x", hr );
            pService->Release();
            CoUninitialize();
            return ;
        }

        ITaskFolder* pRootFolder = NULL;
        hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
        getTasks(pRootFolder,hr,tasks,tol);

        pRootFolder->Release();

        pService->Release();
        CoUninitialize();

}

void getTasks(ITaskFolder* rootFolder, HRESULT hr,taskDetail* tasks, int& tol){
    ITaskFolderCollection* pFolders = NULL;
    hr = rootFolder->GetFolders(0, &pFolders);
    LONG numFolders = 0;
    pFolders->get_Count(&numFolders);
    if (FAILED(hr))
        {
        return;
        }
    //std::cout<<"hhh"<<std::endl;

    IRegisteredTaskCollection* pTaskCollection = NULL;
    hr = rootFolder->GetTasks( NULL, &pTaskCollection );
    //std::cout<<"numTasks"<<std::endl;
    if (FAILED(hr))
        {
            return ;
        }
    LONG numTasks = 0;
    pTaskCollection->get_Count(&numTasks);
    //std::cout<<"numTasks"<<numTasks<<std::endl;
    if (numTasks == 0)
        {
            pTaskCollection->Release();
            return ;
        }
    //TASK_STATE taskState;
    for(LONG i=0; i < numTasks; i++)
        {
            IRegisteredTask* pRegisteredTask = NULL;
            hr = pTaskCollection->get_Item( _variant_t(i+1), &pRegisteredTask );

            if( SUCCEEDED(hr) )
            {
                BSTR taskName = NULL;
                hr = pRegisteredTask->get_Name(&taskName);
                if( SUCCEEDED(hr) )
                {
                    //printf("\nTask Name: %S", taskName);
                    tasks[tol].name = QString::fromWCharArray(taskName);
                    //std::cout<<tasks[tol].name.toStdString()<<std::endl;
                    SysFreeString(taskName);

                    ITaskDefinition* taskDefination = NULL;
                    hr = pRegisteredTask->get_Definition(&taskDefination);
                    IActionCollection* taskActions = NULL;
                    hr = taskDefination->get_Actions(&taskActions);
                    taskDefination->Release();
                    IAction* action = NULL;
                    hr = taskActions->get_Item(1, &action);
                    taskActions->Release();

                    IExecAction* execAction = NULL;
                    hr = action->QueryInterface(IID_IExecAction, (void**)&execAction);
                    action->Release();
                    BSTR imagePath = NULL;
                    hr = execAction->get_Path(&imagePath);
                    if (SUCCEEDED(hr)){
                       tasks[tol].imagePath = QString::fromWCharArray(imagePath);
                       //std::cout<<tasks[tol].imagePath.toStdString()<<std::endl;
                    }
                    execAction->Release();
                }
                tol++;
                pRegisteredTask->Release();
            }
        }

        pTaskCollection->Release();
        //CoUninitialize();



        //LONG numFolders = 0;
        pFolders->get_Count(&numFolders);
        //std::cout<<"numFolders"<<numFolders<<std::endl;
        if (numFolders != 0) {
                for (LONG i = 0; i < numFolders; i++)
                {
                    //std::cout<<"111"<<std::endl;
                    ITaskFolder* pRootFolder = NULL;
                    hr = pFolders->get_Item(_variant_t(i + 1), &pRootFolder);
                    if (SUCCEEDED(hr))
                    {
                        BSTR name = NULL;
                        hr = pRootFolder->get_Name(&name);
                        if (FAILED(hr)) // fail to get folder name
                        {
                            return ;
                        }
                        SysFreeString(name);
        //                list[0].length++;
                       getTasks(pRootFolder, hr, tasks,tol);
                    }
                }
               // pFolders->Release();
            }
}

#endif // TASK_H
