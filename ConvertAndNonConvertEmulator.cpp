
#define WINVER       0x0500
#define _WIN32_WINNT 0x0500

#include <windows.h>


// 長押しと認識されるまでの時間（ミリ秒）
constexpr UINT DELAY = 150;



BOOL bDownLMENU;
UINT nIDTimerLMENU;

INPUT inputDownLMENU[1] = {
	{INPUT_KEYBOARD, VK_LMENU, 0}
};

INPUT inputUpLMENU[1] = {
	{INPUT_KEYBOARD, VK_LMENU, KEYEVENTF_KEYUP}
};

INPUT inputEmulateLMENU[3] = {
	{INPUT_KEYBOARD, VK_LMENU, KEYEVENTF_KEYUP},
	{INPUT_KEYBOARD, VK_NONCONVERT, 0},
	{INPUT_KEYBOARD, VK_NONCONVERT, KEYEVENTF_KEYUP}
};

BOOL bDownRMENU;
UINT nIDTimerRMENU;

INPUT inputDownRMENU[1] = {
	{INPUT_KEYBOARD, VK_RMENU, 0}
};

INPUT inputUpRMENU[1] = {
	{INPUT_KEYBOARD, VK_RMENU, KEYEVENTF_KEYUP}
};

INPUT inputEmulateRMENU[3] = {
	{INPUT_KEYBOARD, VK_RMENU, KEYEVENTF_KEYUP},
	{INPUT_KEYBOARD, VK_CONVERT, 0},
	{INPUT_KEYBOARD, VK_CONVERT, KEYEVENTF_KEYUP}
};


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wp, LPARAM lp);
VOID CALLBACK TimerProcLMENU(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime);
VOID CALLBACK TimerProcRMENU(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime);


int WinMainCRTStartup(void)
{
	CreateMutex(NULL, FALSE, TEXT("ConvertAndNonConvertEmulator"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 0;
	}


	SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, NULL, 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

VOID CALLBACK TimerProcLMENU(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, nIDTimerLMENU);
	nIDTimerLMENU = 0;

	SendInput(1, inputDownLMENU, sizeof(INPUT));
}

VOID CALLBACK TimerProcRMENU(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, nIDTimerRMENU);
	nIDTimerRMENU = 0;

	SendInput(1, inputDownRMENU, sizeof(INPUT));
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode == HC_ACTION)
	{
		KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lp;
		if (!(kbs->flags & LLKHF_INJECTED))
		{
			if (wp == WM_SYSKEYDOWN)
			{
				if (kbs->vkCode == VK_LMENU)
				{
					if (bDownLMENU == FALSE)
					{
						bDownLMENU = TRUE;

						nIDTimerLMENU = SetTimer(NULL, nIDTimerLMENU, DELAY, (TIMERPROC)TimerProcLMENU);
					}

					if (nIDTimerLMENU != 0) return -1;
				}
				else if (nIDTimerLMENU != 0)
				{
					KillTimer(NULL, nIDTimerLMENU);
					nIDTimerLMENU = 0;

					if (kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						bDownLMENU = FALSE;

						SendInput(3, inputEmulateLMENU, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDownLMENU, sizeof(INPUT));
					}
				}

				if (kbs->vkCode == VK_RMENU)
				{
					if (bDownRMENU == FALSE)
					{
						bDownRMENU = TRUE;

						nIDTimerRMENU = SetTimer(NULL, nIDTimerRMENU, DELAY, (TIMERPROC)TimerProcRMENU);
					}

					if (nIDTimerRMENU != 0) return -1;
				}
				else if (nIDTimerRMENU != 0)
				{
					KillTimer(NULL, nIDTimerRMENU);
					nIDTimerRMENU = 0;

					if (kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						bDownRMENU = FALSE;

						SendInput(3, inputEmulateRMENU, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDownRMENU, sizeof(INPUT));
					}
				}
			}

			if (wp == WM_KEYUP)
			{
				if (kbs->vkCode == VK_LMENU)
				{
					bDownLMENU = FALSE;

					if (nIDTimerLMENU != 0)
					{
						KillTimer(NULL, nIDTimerLMENU);
						nIDTimerLMENU = 0;

						SendInput(3, inputEmulateLMENU, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputUpLMENU, sizeof(INPUT));
					}

					return -1;
				}

				if (kbs->vkCode == VK_RMENU)
				{
					bDownRMENU = FALSE;

					if (nIDTimerRMENU != 0)
					{
						KillTimer(NULL, nIDTimerRMENU);
						nIDTimerRMENU = 0;

						SendInput(3, inputEmulateRMENU, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputUpRMENU, sizeof(INPUT));
					}

					return -1;
				}
			}
		}
	}

	return CallNextHookEx(NULL, nCode, wp, lp);
}
