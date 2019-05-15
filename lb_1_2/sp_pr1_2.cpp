#include <Windows.h>
#include "sp_pr1_2.h"
#include <tchar.h>


#define IDC_BTN_SAVE 1000
#define IDC_BTN_ADD 1010
#define IDC_EDIT1 1020
#define IDC_LISTBOX 1030


//ПРОТОТИП ОКОННОЙ ПРОЦЕДУРЫ
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);

//ОБЪЯВЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
LPCTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszAplicationTitle = TEXT("Главное окно приложения. Програмист Гутырчик Олег.");
LPTSTR g_lpszDestroyMessage = TEXT("Разрушается окно. В связи с этим поступило сообщение WM_DESTROY, из обработчика которого и выполнен данный вывод.");
LPCTSTR g_lpszLBClickMessage = TEXT("Handling of message WM_LBUTTONDOWN, which is send on left mouse button click");

//СТАРТОВАЯ ФУНКЦИЯ
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	HBRUSH brush = CreateSolidBrush(RGB(127, 127, 0));

	//Код регистрации класса окна
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hbrBackground = brush;
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//Создание главного окна
	hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
		0,
		0,
		600,
		450,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow); //Отображение окна на экране
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) //WinMain читает сообщения из очереди сообщений потока. Выбранные сообщения передаются в соотв. оконную процедуру.
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	//Create mass of lastnames
	LPCSTR lastname[15] = { "Пупкин","Иванов","Петров","Сидоров","Гоги" };


//Переменные дескрипторов дочерних окон
	HDC hDC;
	int wmId, wmEvent;
	static HWND hButtonSave;
	static HWND hButtonAdd;
	static HWND hButtonExit;
	static HWND hEdit;
	static HWND hListBox;
	RECT rectangle;
	switch (msg)
	{

//цикл сообщений
	case WM_DESTROY:// Завершение работы приложения
	{
		MessageBox(NULL, g_lpszDestroyMessage,
			TEXT("Окно уничтожается!"), MB_OK | MB_ICONERROR);
		PostQuitMessage(5); // Посылка WM_QUIT приложению
		break;
	}

	case WM_LBUTTONDOWN:
	{
		TCHAR _buff[1024];
		hDC = GetDC(hWnd);
		//TextOut(hDC, 50, 200, g_lpszLBClickMessage, 300);
		//TextOut(hDC, LOWORD(lParam), HIWORD(lParam), g_lpszLBClickMessage, 600);
		//ReleaseDC(hWnd, hDC);
		
		int xPos, yPos;
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		rectangle.left = xPos;
		rectangle.top = yPos;
		rectangle.right = xPos + 200;
		rectangle.bottom = yPos + 100;
		_stprintf_s(_buff, g_lpszLBClickMessage);
		DrawText(hDC, _buff, lstrlen(_buff), &rectangle, DT_CENTER | DT_WORDBREAK);
		ReleaseDC(hWnd, hDC);
		break;
	}
	/*case WM_PAINT:
	{
	LPCTSTR lpszString2 = TEXT("Обработка сообщения WM_PAINT. Это соообщение окно получает после того, как оно было закрыто другим окном и затем открыто.");
	PAINTSTRUCT paint;
	hDC = BeginPaint(hWnd, &paint);
	TextOut(hDC, 20, 100, lpszString2, 300);
	EndPaint(hWnd, &paint);
	break;
	}*/
	case WM_CREATE:
	{
		LPCTSTR lpszString3 = TEXT("Выполняется обработка WM_CREATE");
		LPCTSTR lpszTitleCreate = TEXT("WM_CREATE");
		MessageBox(NULL, lpszString3, lpszTitleCreate, MB_OK);
		if (!(hEdit = CreateWindowEx(0L, L"Edit", L"Введите фамилию и нажмите кнопку «Войти»",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			20, 50, 320, 80, hWnd, (HMENU)(IDC_EDIT1),
			NULL, NULL))) return (-1);
		if (!(hListBox = CreateWindowEx(0L, L"ListBox", L"Список",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			370, 50, 160, 180, hWnd, (HMENU)(IDC_LISTBOX),
			NULL, NULL))) return (-1);

		//Add lastnames to hListBox
		for (int i = 0; i < 5; i++)
		{
			SendMessageA(hListBox, LB_ADDSTRING, 1, (LPARAM)lastname[i]);
		}

		if (!(hButtonSave = CreateWindowEx(0L, L"Button", L"В буфер",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			20, 240, 80, 24, hWnd, (HMENU)(IDC_BTN_SAVE),
			NULL, NULL))) return (-1);
		if (!(hButtonAdd = CreateWindowEx(0L, L"Button", L"В список",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			120, 240, 80, 24, hWnd, (HMENU)(IDC_BTN_ADD),
			NULL, NULL))) return (-1);
		if (!(hButtonExit = CreateWindowEx(0L, L"Button", L"Выход",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			220, 240, 80, 24, hWnd, (HMENU)(IDCANCEL),
			NULL, NULL))) return (-1);
		return 0;
	}
	case WM_COMMAND:
	{
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		static TCHAR pszTextBuff[500];
		switch (wmId)
		{
		case IDCANCEL:
		{
			DestroyWindow(hWnd);
			break;
		}
		case IDC_BTN_SAVE:
		{
			int cch;
			cch = SendMessage(hEdit, WM_GETTEXT, 500, (LPARAM)pszTextBuff);
			if (cch == 0) MessageBox(hWnd, L"Введите текст", L"Читаем Edit", MB_OK);
			else
			{
				TCHAR Buff1[500] = { 0 };
				SYSTEMTIME st;
				GetSystemTime(&st);
				wsprintf(Buff1, L"Время : %d ч %d мин %d сек\n", st.wHour, st.wMinute, st.wSecond);
				lstrcat(Buff1, __TEXT("Текст в памяти: "));
				lstrcat(Buff1, pszTextBuff);
				MessageBox(hWnd, Buff1, L"Содержимое буфера", MB_OK);

				int n = 0; //True/false flag for lastname match
				for (int i = 0; i < 5; i++)
				{
					TCHAR Buff2[100] = { 0 }; //Lastname from hListBox field will be stored here
					SendMessage(hEdit, WM_GETTEXT, 500, (LPARAM)pszTextBuff); //Get lastname from hEdit field
					SendMessage(hListBox, LB_GETTEXT, (WPARAM)i,(LPARAM)Buff2); //Get lastname from hListBox field
					if (_tcscmp(pszTextBuff, Buff2) == 0) //Compare lastnames and set flag=1 if match
					{ 
						n = 1; 
						break;
					}
				}
				if (n == 0)
				{
					SendMessageA(hEdit, WM_SETTEXT, 0, (LPARAM)"Вы не зарегистрированы"); //Send message to hEdit field
					DestroyWindow(hWnd);
					break;
					//MessageBox(hWnd, L"Вы не зарегистрированы", L"Message", MB_OK);
				}
				else 
				{
					SendMessageA(hEdit, WM_SETTEXT, 0, (LPARAM)"Продолжайте работу");
					//MessageBox(hWnd, L"Продолжайте работу", L"Message", MB_OK);
				}
			}
			break;
		}
		case IDC_BTN_ADD:
		{
			int ind;
			ind = SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)pszTextBuff);
			if (ind == LB_ERR) MessageBox(hWnd, L"Ошибка в списке", L"", MB_OK);
			break;
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
	}
};

