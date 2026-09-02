// WTL10 Application Wizard5.cpp : main source file for WTL10 Application Wizard5.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

// Подключаем библиотеку WinRT
#pragma comment(lib, "WindowsApp.lib")
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <iostream>

using namespace winrt;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation::Collections;

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//HRESULT hRes = ::CoInitialize(NULL);
    HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

    // 2. Инициализация WinRT с обработкой ошибок
    bool winRTInitialized = false;

    try
    {
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        winRTInitialized = true;
    }
    catch (const winrt::hresult_error& e)
    {
        // Код ошибки 0x80010106 = RPC_E_CHANGED_MODE
        if (e.code() == 0x80010106)
        {
            // Пробуем другой режим
            try
            {
                winrt::init_apartment(winrt::apartment_type::single_threaded);
                winRTInitialized = true;
            }
            catch (...)
            {
                // Игнорируем - WinRT недоступен
                winRTInitialized = false;
            }
        }
        else
        {
            // Другая ошибка - WinRT недоступен
            winRTInitialized = false;
        }
    }
    catch (...)
    {
        // WinRT недоступен (Windows 7 или старее)
        winRTInitialized = false;
    }

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();

    if (winRTInitialized)
    {
        try
        {
            winrt::uninit_apartment();
        }
        catch (...)
        {
            // Игнорируем ошибки при завершении
        }
    }

	::CoUninitialize();

	return nRet;
}
