
#define WINVER       0x0500
#define _WIN32_WINNT 0x0500

#include <windows.h>


UINT LongPressDelay;
BOOLEAN isFixHastyTyping;

WORD KeyCode_LKEY;
BOOLEAN isDown_LKEY = FALSE;
UINT_PTR TimerID_LKEY = 0;

INPUT inputDown_LKEY[1] = {
	{INPUT_KEYBOARD, NULL, 0}
};

INPUT inputEmulate_LKEY[2] = {
	{INPUT_KEYBOARD, VK_NONCONVERT, 0},
	{INPUT_KEYBOARD, VK_NONCONVERT, KEYEVENTF_KEYUP}
};

WORD KeyCode_RKEY;
BOOLEAN isDown_RKEY = FALSE;
UINT_PTR TimerID_RKEY = 0;

INPUT inputDown_RKEY[1] = {
	{INPUT_KEYBOARD, NULL, 0}
};

INPUT inputEmulate_RKEY[2] = {
	{INPUT_KEYBOARD, VK_CONVERT, 0},
	{INPUT_KEYBOARD, VK_CONVERT, KEYEVENTF_KEYUP}
};


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wp, LPARAM lp);
VOID CALLBACK TimerProc_LKEY(HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD dwTime);
VOID CALLBACK TimerProc_RKEY(HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD dwTime);


int WinMainCRTStartup(void)
{
	CreateMutex(NULL, FALSE, TEXT("ConvertAndNonConvertEmulator"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 0;
	}


	LongPressDelay = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("LongPressDelay"), 200, TEXT(".\\ConvertAndNonConvertEmulator.ini"));

	isFixHastyTyping = (GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("FixHastyTyping"), 0, TEXT(".\\ConvertAndNonConvertEmulator.ini")) == 1);

	KeyCode_LKEY = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("NonConvertKey"), VK_LMENU, TEXT(".\\ConvertAndNonConvertEmulator.ini"));
	inputDown_LKEY[0].ki.wVk = KeyCode_LKEY;

	KeyCode_RKEY = GetPrivateProfileInt(TEXT("ConvertAndNonConvertEmulator"), TEXT("ConvertKey"), VK_RMENU, TEXT(".\\ConvertAndNonConvertEmulator.ini"));
	inputDown_RKEY[0].ki.wVk = KeyCode_RKEY;


	SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, NULL, 0);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

VOID CALLBACK TimerProc_LKEY(HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, TimerID_LKEY);
	TimerID_LKEY = 0;

	SendInput(1, inputDown_LKEY, sizeof(INPUT));
}

VOID CALLBACK TimerProc_RKEY(HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, TimerID_RKEY);
	TimerID_RKEY = 0;

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
				if (kbs->vkCode == KeyCode_LKEY)
				{
					if (isDown_LKEY == FALSE)
					{
						isDown_LKEY = TRUE;

						TimerID_LKEY = SetTimer(NULL, TimerID_LKEY, LongPressDelay, (TIMERPROC)TimerProc_LKEY);
					}

					if (TimerID_LKEY != 0) return -1;
				}
				else if (TimerID_LKEY != 0)
				{
					KillTimer(NULL, TimerID_LKEY);
					TimerID_LKEY = 0;

					if (isFixHastyTyping == TRUE && kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						isDown_LKEY = FALSE;

						SendInput(2, inputEmulate_LKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDown_LKEY, sizeof(INPUT));
					}
				}

				if (kbs->vkCode == KeyCode_RKEY)
				{
					if (isDown_RKEY == FALSE)
					{
						isDown_RKEY = TRUE;

						TimerID_RKEY = SetTimer(NULL, TimerID_RKEY, LongPressDelay, (TIMERPROC)TimerProc_RKEY);
					}

					if (TimerID_RKEY != 0) return -1;
				}
				else if (TimerID_RKEY != 0)
				{
					KillTimer(NULL, TimerID_RKEY);
					TimerID_RKEY = 0;

					if (isFixHastyTyping == TRUE && kbs->vkCode >= 0x41 && kbs->vkCode <= 0x5A)
					{
						isDown_RKEY = FALSE;

						SendInput(2, inputEmulate_RKEY, sizeof(INPUT));
					}
					else
					{
						SendInput(1, inputDown_RKEY, sizeof(INPUT));
					}
				}
			}

			if (wp == WM_KEYUP)
			{
				if (kbs->vkCode == KeyCode_LKEY)
				{
					isDown_LKEY = FALSE;

					if (TimerID_LKEY != 0)
					{
						KillTimer(NULL, TimerID_LKEY);
						TimerID_LKEY = 0;

						SendInput(2, inputEmulate_LKEY, sizeof(INPUT));

						return -1;
					}
				}

				if (kbs->vkCode == KeyCode_RKEY)
				{
					isDown_RKEY = FALSE;

					if (TimerID_RKEY != 0)
					{
						KillTimer(NULL, TimerID_RKEY);
						TimerID_RKEY = 0;

						SendInput(2, inputEmulate_RKEY, sizeof(INPUT));

						return -1;
					}
				}
			}
		}
	}

	return CallNextHookEx(NULL, nCode, wp, lp);
}
