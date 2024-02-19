
#include "pal.h"
#include <assert.h>

std::vector<PAL::KeyData> PAL::_keyDatas;
std::mutex PAL::_mux;

HHOOK PAL::_hook;
void
PAL::init()
{
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, PAL::KeyboardHookProc, NULL, 0);
    assert(_hook);
}

void
PAL::finalize()
{
    UnhookWindowsHookEx(_hook);
}

#include <iostream>
LRESULT CALLBACK
PAL::KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* pKbdStruct = (KBDLLHOOKSTRUCT*)lParam;
    KeyData kd;
    if (wParam == WM_KEYDOWN) {
        kd.pressed = true;
    } else if (wParam == WM_KEYUP) {
        kd.pressed = false;
    }
    kd.scanCode = pKbdStruct->scanCode;
    _mux.lock();
    _keyDatas.push_back(kd);
    _mux.unlock();

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

std::vector<unsigned>
PAL::getPressedKeys()
{
    std::vector<unsigned> result;
    _mux.lock();
    for (auto kd: _keyDatas) {
        if (kd.pressed) {
            result.emplace_back(kd.scanCode);
        }
    }
    _keyDatas.clear();
    _mux.unlock();
    return result;
}

std::vector<PAL::WinInfo>
PAL::getWindows()
{
    std::vector<WinInfo> result;

    HWND hwnd = GetTopWindow(nullptr);
    while (hwnd != NULL) {
        char winName[256];
        if (IsWindowVisible(hwnd)) {
            GetWindowTextA(hwnd, winName, 256);
            result.emplace_back(reinterpret_cast<unsigned long>(hwnd), winName);
        }
        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
    }
    return result;
}
