
#ifndef COMMONPANEL_H
#define COMMONPANEL_H

#include "panelbase.h"
#include <QPalette>
#include <QLayout>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QPoint>
#include <QMouseEvent>
#include <QFrame>
#include <QTextEdit>

class dragWin : public QWidget {
public:
    dragWin(QWidget* parent): _owner(parent){ parent->installEventFilter(this); }
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
private:
    bool     _dragging;

    QWidget* _owner;
    QPointF  _start;
    QPointF  _end;
    qreal    _thresh_hold = 50;
};

class CommonPanel final : public PanelBase
{
public:
    CommonPanel();
protected:
    virtual void    _DrawLayout() override;
    virtual void    _DrawInfo()   override;
    virtual void    _DrawRecord() override;

    virtual void    _updateInfo() override;
    virtual void    _updateRecord(std::vector<unsigned>&) override;
private:
    typedef std::vector<QPushButton*> buttonVec;
    typedef std::vector<buttonVec>    buttonVecVec;
    QGroupBox*              _gbox_info;
    QGroupBox*              _gbox_records;
    QLabel*                 _lb_info0;
    QLabel*                 _lb_info1;
    std::vector<QLabel*>    _lb_recs;
    QLineEdit*              _le_info;
    QComboBox*              _cb_info;
    buttonVecVec            _pb_recs;
    QGroupBox*              _gb_tips;
    QTextEdit*              _txt_tips;
    QFrame*                 _frame_menu;
    buttonVec               _pb_menu;
    unsigned                _max_key_cb_size = 3;

    dragWin                 _drag;
};

#endif // COMMONPANEL_H
