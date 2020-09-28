
#define WINVER       0x0500
#define _WIN32_WINNT 0x0500

#include <windows.h>


UINT nDelay;

WORD wKeyCode_LKEY;
BOOL bDown_LKEY;
UINT nIDTimer_LKEY;

INPUT inputDown_LKEY[1] = {
	{INPUT_KEYBOARD, NULL, 0}
};

INPUT inputUp_LKEY[1] = {
	{INPUT_KEYBOARD, NULL, KEYEVENTF_KEYUP}
};

INPUT inputEmulate_LKEY[3] = {
	{INPUT_KEYBOARD, NULL, KEYEVENTF_KEYUP},
	{INPUT_KEYBOARD, VK_NONCONVERT, 0},
	{INPUT_KEYBOARD, VK_NONCONVERT, KEYEVENTF_KEYUP}
};

WORD wKeyCode_RKEY;
BOOL bDown_RKEY;
UINT nIDTimer_RKEY;

INPUT inputDown_RKEY[1] = {
	{INPUT_KEYBOARD, NULL, 0}
};

INPUT inputUp_RKEY[1] = {
	{INPUT_KEYBOARD, NULL, KEYEVENTF_KEYUP}
};

INPUT inputEmulate_RKEY[3] = {
	{INPUT_KEYBOARD, NULL, KEYEVENTF_KEYUP},
	{INPUT_KEYBOARD, VK_CONVERT, 0},
	{INPUT_KEYBOARD, VK_CONVERT, KEYEVENTF_KEYUP}
};


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wp, LPARAM lp);
VOID CALLBACK TimerProc_LKEY(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime);
VOID CALLBACK TimerProc_RKEY(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime);


int WinMainCRTStartup(void)
{
	CreateMutex(NULL, FALSE, TEXT("ConvertAndNonConvertEmulator"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 0;
	}


	nDelay = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("Delay"), 200, TEXT(".\\ConvertAndNonConvertEmulator.ini"));

	wKeyCode_LKEY = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("NonConvertKey"), VK_LMENU, TEXT(".\\ConvertAndNonConvertEmulator.ini"));
	inputDown_LKEY[0].ki.wVk = wKeyCode_LKEY;
	inputUp_LKEY[0].ki.wVk = wKeyCode_LKEY;
	inputEmulate_LKEY[0].ki.wVk = wKeyCode_LKEY;

	wKeyCode_RKEY = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("ConvertKey"), VK_RMENU, TEXT(".\\ConvertAndNonConvertEmulator.ini"));
	inputDown_RKEY[0].ki.wVk = wKeyCode_RKEY;
	inputUp_RKEY[0].ki.wVk = wKeyCode_RKEY;
	inputEmulate_RKEY[0].ki.wVk = wKeyCode_RKEY;


	SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, NULL, 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

VOID CALLBACK TimerProc_LKEY(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, nIDTimer_LKEY);
	nIDTimer_LKEY = 0;

	SendInput(1, inputDown_LKEY, sizeof(INPUT));
}

VOID CALLBACK TimerProc_RKEY(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, nIDTimer_RKEY);
	nIDTimer_RKEY = 0;

	SendInput(1, inputDown_RKEY, sizeof(INPUT));
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode == HC_ACTION)
	{
		KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lp;
		if (!(kbs->flags & LLKHF_INJECTED))
		{
			if (wp == WM_KEYDOWN || wp == WM_SYSKEYDOWN)
			{
				if (kbs->vkCode == wKeyCode_LKEY)
				{
					if (bDown_LKEY == FALSE)
					{
						bDown_LKEY = TRUE;

						nIDTimer_LKEY = SetTimer(NULL, nIDTimer_LKEY, nDelay, (TIMERPROC)TimerProc_LKEY);
					}

					if (nIDTimer_LKEY != 0) return -1;
				}
				else if (nIDTimer_LKEY != 0)
				{
					KillTimer(NULL, nIDTimer_LKEY);
					nIDTimer_LKEY = 0;

					if (kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						bDown_LKEY = FALSE;

						SendInput(3, inputEmulate_LKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDown_LKEY, sizeof(INPUT));
					}
				}

				if (kbs->vkCode == wKeyCode_RKEY)
				{
					if (bDown_RKEY == FALSE)
					{
						bDown_RKEY = TRUE;

						nIDTimer_RKEY = SetTimer(NULL, nIDTimer_RKEY, nDelay, (TIMERPROC)TimerProc_RKEY);
					}

					if (nIDTimer_RKEY != 0) return -1;
				}
				else if (nIDTimer_RKEY != 0)
				{
					KillTimer(NULL, nIDTimer_RKEY);
					nIDTimer_RKEY = 0;

					if (kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						bDown_RKEY = FALSE;

						SendInput(3, inputEmulate_RKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDown_RKEY, sizeof(INPUT));
					}
				}
			}

			if (wp == WM_KEYUP)
			{
				if (kbs->vkCode == wKeyCode_LKEY)
				{
					bDown_LKEY = FALSE;

					if (nIDTimer_LKEY != 0)
					{
						KillTimer(NULL, nIDTimer_LKEY);
						nIDTimer_LKEY = 0;

						SendInput(3, inputEmulate_LKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputUp_LKEY, sizeof(INPUT));
					}

					return -1;
				}

				if (kbs->vkCode == wKeyCode_RKEY)
				{
					bDown_RKEY = FALSE;

					if (nIDTimer_RKEY != 0)
					{
						KillTimer(NULL, nIDTimer_RKEY);
						nIDTimer_RKEY = 0;

						SendInput(3, inputEmulate_RKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputUp_RKEY, sizeof(INPUT));
					}

					return -1;
				}
			}
		}
	}

	return CallNextHookEx(NULL, nCode, wp, lp);
}
