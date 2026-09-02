// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CClipBuffer.h"


class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
private:
	
	CListViewCtrl m_HistoryListLeft, m_HistoryListRight;
	int m_nHotKeyId = 0;  // ID хоткея
	bool isAddToListRight = false;

public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)  
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDC_BUTTON_UPDATE, OnUpdate)
		COMMAND_ID_HANDLER(IDC_BUTTON_TO_RIGHT, OnItemLeftToRight)
		COMMAND_ID_HANDLER(IDC_BUTTON_ERASE, OnEraseRight)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	  // Обработчик горячей клавиши
	LRESULT OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (wParam == m_nHotKeyId)
		{
			ToggleWindowVisibility();  // Показать/скрыть окно
		}
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();
		isAddToListRight = false;
		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		InitListControl(m_HistoryListLeft, IDC_LIST1);
		InitListControl(m_HistoryListRight, IDC_LIST2);
		LoadListLeft();
		LoadRightListFromFile();
		RegisterHotKey();
		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates

		UnregisterHotKey();
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		BOOL bShow = (BOOL)wParam;

		// Если окно скрывается
		if (!bShow)
		{
			SaveRightListToFile();
		}

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LoadListLeft();
		return 0;
	}

	LRESULT OnEraseRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_HistoryListRight.DeleteAllItems();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 

		// Получаем выбранный текст из правого списка
		int nSelected = m_HistoryListRight.GetNextItem(-1, LVNI_SELECTED);
		if (nSelected == -1)
		{
			MessageBox(L"Пожалуйста, выберите элемент для вставки", L"Информация", MB_OK);
			return 0;
		}

		wchar_t buffer[256] = { 0 };
		m_HistoryListRight.GetItemText(nSelected, 0, buffer, 256);
		std::wstring strText = buffer;

		// Вставляем текст в активное окно
		InsertTextToForegroundWindow(strText);

		// Не закрываем диалог, просто сворачиваем его или скрываем
		ShowWindow(SW_HIDE);

	//	CloseDialog(wID);
		return 0;
	}

	LRESULT OnItemLeftToRight(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		int nSelected = m_HistoryListLeft.GetNextItem(-1, LVNI_SELECTED);

		if (nSelected != -1)
		{
			wchar_t buffer[256] = { 0 };
			m_HistoryListLeft.GetItemText(nSelected, 0, buffer, 256);
			std::wstring strText = buffer;

			// Поиск элемента в правом списке
			LVFINDINFO findInfo = { 0 };
			findInfo.flags = LVFI_STRING;  // поиск по строке
			findInfo.psz = strText.c_str();

			int nFoundIndex = m_HistoryListRight.FindItem(&findInfo, -1);
			if (nFoundIndex == -1)
				m_HistoryListRight.InsertItem(0, strText.c_str()); // 0 - в начало
			isAddToListRight = true;
			// Или так, чтобы сохранить порядок:
			// int nNewIndex = m_HistoryListRight.GetItemCount();
			// m_HistoryListRight.InsertItem(nNewIndex, strText);
		}
		else
		{
			MessageBox(L"Пожалуйста, выберите элемент для копирования", L"Информация", MB_OK);
		}

		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	private:

		void SendUnicodeText(const std::wstring& text)
		{
			if (text.empty()) return;

			// Для каждого символа создаем два события: нажатие и отпускание
			std::vector<INPUT> inputs;
			inputs.reserve(text.length() * 2);

			for (wchar_t ch : text)
			{
				// 1. Нажатие клавиши (KeyDown)
				INPUT inputDown = { 0 };
				inputDown.type = INPUT_KEYBOARD;
				inputDown.ki.wVk = 0;                     // 0 для Unicode-ввода
				inputDown.ki.wScan = ch;                  // Сам символ
				inputDown.ki.dwFlags = KEYEVENTF_UNICODE; // Флаг Unicode-ввода
				inputs.push_back(inputDown);

				// 2. Отпускание клавиши (KeyUp)
				INPUT inputUp = inputDown;
				inputUp.ki.dwFlags |= KEYEVENTF_KEYUP;
				inputs.push_back(inputUp);
			}

			// Отправляем всю цепочку символов в системную очередь ввода
			::SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
		}

		void InsertTextToForegroundWindow(const std::wstring& text)
		{
			// 1. Сначала полностью ПРЯЧЕМ наше собственное окно.
			// Это автоматически заставит Windows вернуть фокус на предыдущее честное активное окно пользователя.

			::ShowWindow(m_hWnd, SW_MINIMIZE); // Или SW_HIDE

			// Даем Windows 100-150 мс, чтобы она переключила фокус на редактор
			::Sleep(150);

			// 2. Теперь GetForegroundWindow() гарантированно вернет окно редактора, 
			// так как наше окно уже скрыто и не может быть активным.
			HWND hTargetWnd = ::GetForegroundWindow();

			if (hTargetWnd == NULL)
			{
				::MessageBox(NULL, L"Не найдено активное окно", L"Ошибка", MB_OK);
				return;
			}

			// 3. Отправляем текст (фокус уже там, SetForegroundWindow не нужен)
			SendUnicodeText(text);
		}

	
		
		void LoadListLeft()
		{
			m_HistoryListLeft.DeleteAllItems();
			CClipBuffer &buffer = CClipBuffer::GetInstance();
			std::vector<std::wstring> items = buffer.GetBuffer();
			
			for (const auto& item : items) {
				// Добавляем в ListView
				m_HistoryListLeft.InsertItem(0, item.c_str());
			}

		}

		void InitListControl(CListViewCtrl &m_HistoryList, int idc_rc)
		{
			m_HistoryList.Attach(GetDlgItem(idc_rc));

			// Устанавливаем стиль отображения (таблица)
			m_HistoryList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
			// Добавляем колонки
			LVCOLUMN lvc = { 0 };
			lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

			lvc.pszText = _T("Буфер обмена");
			lvc.cx = 200;
			lvc.iSubItem = 0;
			m_HistoryList.InsertColumn(0, &lvc);

			lvc.pszText = _T("Тип");
			lvc.cx = 80;
			lvc.iSubItem = 1;
			m_HistoryList.InsertColumn(1, &lvc);
		}

		void ToggleWindowVisibility()
		{
			if (IsWindowVisible())
			{
				ShowWindow(SW_HIDE);
			}
			else
			{
				ShowWindow(SW_RESTORE);  // Восстанавливаем, если было свернуто
				ShowWindow(SW_SHOW);     // Показываем

				// Делаем активным
				::SetForegroundWindow(m_hWnd);
				SetFocus();
			}
		}

		void RegisterHotKey()
		{
			// Регистрируем хоткей: Ctrl + Shift + V
			m_nHotKeyId = 1;  // Уникальный ID

			BOOL bResult = ::RegisterHotKey(
				m_hWnd,                  // Окно, которое будет получать сообщение
				m_nHotKeyId,             // ID хоткея
				MOD_CONTROL | MOD_SHIFT, // Модификаторы: Ctrl+Shift
				'V'                      // Клавиша V
			);

			if (!bResult)
			{
				// Если не зарегистрировалось (может быть занято), пробуем другую комбинацию
				bResult = ::RegisterHotKey(m_hWnd, m_nHotKeyId, MOD_CONTROL, 'F');
			}
		}

		void UnregisterHotKey()
		{
			if (m_nHotKeyId != 0)
			{
				::UnregisterHotKey(m_hWnd, m_nHotKeyId);
			}
		}

		// Сохранение правого списка в бинарный файл
		void SaveRightListToFile()
		{
			if (!isAddToListRight) return;			
			// Получаем путь к файлу в папке приложения
			TCHAR szPath[MAX_PATH];
			GetModuleFileName(NULL, szPath, MAX_PATH);
			PathRemoveFileSpec(szPath);
			PathAppend(szPath, _T("clipboard_history.dat"));

			// Открываем файл для записи
			HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE)
				return;

			// Получаем количество элементов в правом списке
			int nCount = m_HistoryListRight.GetItemCount();

			// Сохраняем количество элементов
			DWORD dwWritten = 0;
			WriteFile(hFile, &nCount, sizeof(int), &dwWritten, NULL);

			for (int i = 0; i < nCount; i++)
			{
				// Получаем текст из первой колонки
				wchar_t buffer[1024] = { 0 };
				m_HistoryListRight.GetItemText(i, 0, buffer, 1024);

				// Получаем длину текста
				int nLen = wcslen(buffer);

				// Сохраняем длину
				WriteFile(hFile, &nLen, sizeof(int), &dwWritten, NULL);

				// Сохраняем текст (включая завершающий ноль)
				if (nLen > 0)
				{
					WriteFile(hFile, buffer, nLen * sizeof(wchar_t), &dwWritten, NULL);
				}

				// Сохраняем данные из второй колонки (Тип)
				wchar_t typeBuffer[256] = { 0 };
				m_HistoryListRight.GetItemText(i, 1, typeBuffer, 256);
				int nTypeLen = wcslen(typeBuffer);
				WriteFile(hFile, &nTypeLen, sizeof(int), &dwWritten, NULL);
				if (nTypeLen > 0)
				{
					WriteFile(hFile, typeBuffer, nTypeLen * sizeof(wchar_t), &dwWritten, NULL);
				}
			}

			CloseHandle(hFile);
			isAddToListRight = false;
		}

		// Загрузка правого списка из бинарного файла
		void LoadRightListFromFile()
		{
			// Получаем путь к файлу
			TCHAR szPath[MAX_PATH];
			GetModuleFileName(NULL, szPath, MAX_PATH);
			PathRemoveFileSpec(szPath);
			PathAppend(szPath, _T("clipboard_history.dat"));

			// Открываем файл для чтения
			HANDLE hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE)
				return;

			// Очищаем правый список
			m_HistoryListRight.DeleteAllItems();

			// Читаем количество элементов
			int nCount = 0;
			DWORD dwRead = 0;
			ReadFile(hFile, &nCount, sizeof(int), &dwRead, NULL);

			// Читаем каждый элемент
			for (int i = 0; i < nCount; i++)
			{
				// Читаем длину текста
				int nLen = 0;
				ReadFile(hFile, &nLen, sizeof(int), &dwRead, NULL);

				// Читаем текст
				std::wstring strText;
				if (nLen > 0)
				{
					strText.resize(nLen);
					ReadFile(hFile, &strText[0], nLen * sizeof(wchar_t), &dwRead, NULL);
				}

				// Читаем тип
				int nTypeLen = 0;
				ReadFile(hFile, &nTypeLen, sizeof(int), &dwRead, NULL);

				std::wstring strType;
				if (nTypeLen > 0)
				{
					strType.resize(nTypeLen);
					ReadFile(hFile, &strType[0], nTypeLen * sizeof(wchar_t), &dwRead, NULL);
				}

				// Добавляем элемент в правый список
				int nIndex = m_HistoryListRight.InsertItem(i, strText.c_str());
				if (nIndex != -1 && !strType.empty())
				{
					m_HistoryListRight.SetItemText(nIndex, 1, strType.c_str());
				}
			}

			CloseHandle(hFile);
		}
};
