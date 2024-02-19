
#include "commonpanel.h"
#include <iostream>

bool
dragWin::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == _owner) {
        switch (ev->type()) {
        case QEvent::MouseMove:
            if (_dragging) {
                _end = dynamic_cast<QMouseEvent*>(ev)->globalPosition();
            }
            break;
        case QEvent::MouseButtonPress:
            _dragging = true;
            _start = dynamic_cast<QMouseEvent*>(ev)->globalPosition();
            break;
        case QEvent::MouseButtonRelease:
            if (_dragging) {
                _dragging = false;
                if (QPointF(_start - _end).manhattanLength() > _thresh_hold) {
                    QPointF orig = _owner->pos();
                    QPointF dest = orig + _end - _start;
                    _owner->move(dest.x(), dest.y());
                }
            }
            break;
        default:
            break;
        }
    }
    return false;
}

CommonPanel::CommonPanel()
    :_drag(this)
{
    setWindowFlag(Qt::FramelessWindowHint);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255,182,193));
    setPalette(pal);
    setWindowOpacity(0.8);
    _DrawLayout();
    _DrawInfo();
    _DrawRecord();
    installEventFilter(&_drag);

    connect(_pb_menu[1], &QPushButton::clicked,
            [=](){
                exit(0);
            });
    connect(_cb_info, &QComboBox::activated,
            [](int idx) {
                // todo:set current window
            });

    int totalH = _gbox_info->height() +
                 _gbox_records->height() +
                 _gb_tips->height() +
                 _frame_menu->height();
    resize(300, totalH);
}

void
CommonPanel::_DrawLayout()
{
    QVBoxLayout *panelLayout =  new QVBoxLayout(this);

    // init info
    _gbox_info = new QGroupBox("Basic Info", this);
    _gbox_info->setFixedHeight(80);
    panelLayout->addWidget(_gbox_info);

    QGridLayout *infoLayout = new QGridLayout(_gbox_info);
    _lb_info0 = new QLabel("FPS:", this);
    _lb_info1 = new QLabel("Current Win:", this);
    _le_info = new QLineEdit(this);
    _cb_info = new QComboBox(this);
    _cb_info->setEditable(false);
    infoLayout->addWidget(_lb_info0, 0, 0);
    infoLayout->addWidget(_le_info,  0, 1);
    infoLayout->addWidget(_lb_info1, 1, 0);
    infoLayout->addWidget(_cb_info,  1, 1);

    // init record
    _gbox_records = new QGroupBox("Hit Records", this);
    _gbox_records->setFixedHeight(160);
    panelLayout->addWidget(_gbox_records);

    QVBoxLayout *recordLayout = new QVBoxLayout(_gbox_records);
    recordLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _lb_recs.resize(_recordCnt);
    _pb_recs.resize(_recordCnt);
    for (auto &pbVec : _pb_recs) {
        pbVec.resize(_max_key_cb_size + 1);
    }
    for (int i = 0; i < _recordCnt; ++i) {
        QHBoxLayout* layout = new QHBoxLayout();
        layout->setAlignment(Qt::AlignLeft);
        recordLayout->addLayout(layout);
        _lb_recs[i] = new QLabel(this);
        _lb_recs[i]->setFixedWidth(40);
        layout->addWidget(_lb_recs[i]);
        for (int j = 0; j <=_max_key_cb_size; ++j) {
            _pb_recs[i][j] = new QPushButton(this);
            _pb_recs[i][j]->setFixedHeight(20);
            layout->addWidget(_pb_recs[i][j]);
        }
    }

    _gb_tips = new QGroupBox("tips", this);
    panelLayout->addWidget(_gb_tips);
    _gb_tips->setFixedHeight(300);
    QHBoxLayout* layout = new QHBoxLayout(_gb_tips);
    _txt_tips = new QTextEdit(this);
    layout->addWidget(_txt_tips);


    _frame_menu = new QFrame(this);
    QHBoxLayout *menu = new QHBoxLayout(_frame_menu);
    panelLayout->addWidget(_frame_menu);

    _pb_menu.push_back(new QPushButton("Setting", this));
    _pb_menu.push_back(new QPushButton("Quit", this));
    menu->addWidget(_pb_menu[0]);
    menu->addWidget(_pb_menu[1]);
}

void
CommonPanel::_DrawInfo()
{
    _le_info->setText(QString().setNum(_fps));
    // todo : update window
}

void
CommonPanel::_DrawRecord()
{
    // clear old data
    for (auto & pb_rec : _pb_recs) {
        for (auto &pb : pb_rec) {
            pb->setText("");
            pb->setVisible(false);
        }
    }
    for (auto &lb : _lb_recs) {
        lb->setVisible(false);
    }

    // set context
    for (int i = 0; i < _records.size(); ++i) {
        auto record = _records[i];
        std::string str = record._time < 100 ?
                          std::to_string(record._time) :
                          ">99 ";
        str += "->";
        _lb_recs[i]->setText(str.c_str());
        _lb_recs[i]->setVisible(true);
        for (int j = 0; j < record._keys.size(); ++j) {
            if (j < _max_key_cb_size) {
                if (_keyImages.find(record._keys[j]) != _keyImages.end()) {
                    QString path(":/keys/");
                    path += std::to_string(record._keys[j]).c_str();
                    path += ".jpg";
                    _pb_recs[i][j]->setText("");
                    _pb_recs[i][j]->setIcon(QIcon(path));
                    _pb_recs[i][j]->setIconSize(QSize(30, 30));
                    _pb_recs[i][j]->setFlat(true);
                    _pb_recs[i][j]->setStyleSheet("border: 0px");
                } else {
                    _pb_recs[i][j]->setText({_keyNames[record._keys[j]].c_str()});
                }
                _pb_recs[i][j]->setFixedWidth(_pb_recs[i][j]->text().size() * 10 + 20);
                _pb_recs[i][j]->setVisible(true);
            } else {
                _pb_recs[i][j]->setText("...");
                _pb_recs[i][j]->setFixedSize(30, 30);
                _pb_recs[i][j]->setVisible(true);
                break;
            }
        }
    }
}

void
CommonPanel::_updateInfo()
{
    //1. update info
    //2. drawInfo
    _DrawInfo();
}

void
CommonPanel::_updateRecord(std::vector<unsigned>& pressedKeys)
{
    //1. updateRecord
    for (auto key: pressedKeys) {
        std::cout << "scanCode: " << key << ", KeyName: " << _keyNames[key] << std::endl;
    }

    if (!_records.empty() && pressedKeys == _records.back()._keys) {
        ++(_records.back()._time);
    } else {
        _records.push_back(Record(pressedKeys, 1));
        while(_records.size() > _recordCnt) {
            _records.erase(_records.begin());
        }
    }

    // 2. drawRecord
    _DrawRecord();
}

