
#ifndef PAL_H
#define PAL_H

#include <vector>
#include <Windows.h>
#include <mutex>
#pragma comment  (lib, "User32.lib")

/*
 *   Author:    Mark Song
 *   Date:      2023.10.1
 *
 *   This class is an interface to physical layer.
 *   It querys keyboard related events from system api, and provide it to app layer.
 *
 */

#define KEY_NUM 256

class PAL
{
public:
    PAL();
    static void init();    // set hook;
    static void finalize();  //unset hook;

    static std::vector<unsigned> getPressedKeys();

    typedef struct WinInfo_t {
        WinInfo_t(unsigned long wid, const char* name) : _id(wid), _name(name) {};
        std::string _name;
        unsigned long _id;
    }WinInfo;
    static std::vector<WinInfo> getWindows();
private:
    // hook function
    static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam); // keyboard hook
private:
    // pressed key filled by hook functions
    typedef struct KeyData_t {
        bool pressed;
        unsigned scanCode;
    }KeyData;

    static std::vector<KeyData_t> _keyDatas;
    static HHOOK                 _hook;
    static std::mutex            _mux;
};

#endif // PAL_H
