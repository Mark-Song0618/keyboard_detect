
#include "panelbase.h"

PanelBase::PanelBase(QWidget *parent)
  : QWidget{parent},
    _exit(false),
    _fps(20),
    _maxRecord(5),
    _transparency(50),
    _currWin(0)
{
    TCHAR keyName[256] = {0};
    for(unsigned i = 0; i < KEY_NUM; ++i) {
        if (GetKeyNameText(i<<16, keyName, 256)) {
            _keyNames[i] = _tchar2string(keyName);
        }
    }

    //_InitKeyImages();

    _InitPanel();

    connect(this, &PanelBase::updatePanelSig, this, &PanelBase::_updatePanel);

    PAL::init();

    _kbDetectThread = std::thread(&PanelBase::_KBHitDetectFunc, this);
}

PanelBase::~PanelBase() {
    PAL::finalize();

    _exit = true;
    if (_kbDetectThread.joinable()) {
        _kbDetectThread.join();
    }
}

void
PanelBase::_InitPanel()
{
    _DrawLayout();
    _DrawInfo();
    _DrawRecord();
}

void
PanelBase::_InitKeyImages()
{
    for (unsigned i = 1; i < 70; ++i) {
        _keyImages.insert(i);
    }
    _keyImages.insert(79);
    _keyImages.insert(8);
    _keyImages.insert(83);
    _keyImages.insert(87);
    _keyImages.insert(88);
    _keyImages.insert(9);
    _keyImages.insert(91);
}

#include <iostream>
void
PanelBase::_KBHitDetectFunc()
{
    // calculate interval by fps;
    unsigned frameTime = 1000 / _fps;
    while(!_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameTime));
        std::vector<unsigned> keys = PAL::getPressedKeys();
        if (keys.empty()) {
            continue;
        }
        emit updatePanelSig(keys);
    }
}

void
PanelBase::_updatePanel(std::vector<unsigned> keys)
{

    _updateInfo();
    _updateRecord(keys);
}

std::string
PanelBase::_tchar2string(TCHAR* tcharArray) {
#ifdef UNICODE
    int length = WideCharToMultiByte(CP_UTF8, 0, tcharArray, -1, NULL, 0, NULL, NULL);
    char* charArray = new char[length];
    WideCharToMultiByte(CP_UTF8, 0, tcharArray, -1, charArray, length, NULL, NULL);
#else
    int length = lstrlen(tcharArray) + 1;
    char* charArray = new char[length];
    strcpy_s(charArray, length, tcharArray);
#endif
    std::string result(charArray);
    delete[] charArray;
    return result;
}
