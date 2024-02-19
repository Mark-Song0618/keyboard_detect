/*
 *  Author:     Mark Song
 *  Date:       2023.10.1
 *
 *  This is the abstract base class.
 *  It draws the layout of panel, and show keyboard hit.
 *  The derived class should override the specific layout.
 */


#ifndef PANELBASE_H
#define PANELBASE_H


#include <QWidget>
#include <vector>
#include <set>
#include <thread>

#include "pal.h"


class PanelBase : public QWidget
{
    Q_OBJECT
    friend class CommonPanel;
public:
    explicit PanelBase(QWidget *parent = nullptr);

    ~PanelBase();

    //todo: setRecordCnt, setTransparency,setwindow, setFPS...
    void            setRecordCnt(unsigned);

    void            setTransparency(unsigned);

    void            setCurrWindow(unsigned wid);
    std::string     getCurrWindow();

    void            setDetectFPS(unsigned fps);
    unsigned        getDetectFPS();

signals:
    void            updatePanelSig(std::vector<unsigned>);

protected:
    virtual void    _DrawLayout() {};
    virtual void    _DrawInfo() {};
    virtual void    _DrawRecord() {};

    virtual void    _updateInfo() {};
    virtual void    _updateRecord(std::vector<unsigned>&) {};

private:
    // draw panel
    void    _InitPanel();

    void    _InitKeyImages();

    void    _updatePanel(std::vector<unsigned>);

    std::string _tchar2string(TCHAR*);

    // this thread detect pressed key, and update records
    // then emit signal to update Panel
    void    _KBHitDetectFunc();
private:
    typedef struct Record_t {
        std::vector<unsigned> _keys;
        unsigned _time;
        Record_t() = default;
        Record_t(std::vector<unsigned> vec, unsigned time) : _keys(vec), _time(time){};
    }Record;
    bool                        _exit;

    // keys' Name of scanCode
    std::string                 _keyNames[KEY_NUM];
    std::set<unsigned>          _keyImages;

    // key history
    std::vector<Record>         _records;
    unsigned                    _recordCnt = 5;

    // user setting
    unsigned                    _fps;
    unsigned                    _maxRecord;
    unsigned                    _transparency;
    unsigned long               _currWin = MAXLONG;

    std::thread                 _kbDetectThread;
};

#endif // PANELBASE_H
