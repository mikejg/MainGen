#ifndef TOUCHPROBE_H
#define TOUCHPROBE_H

#include <QWidget>

namespace Ui {
class TouchProbe;
}

class TouchProbe : public QWidget
{
    Q_OBJECT

private:
    Ui::TouchProbe *ui;

public:
    explicit TouchProbe(QWidget *parent = nullptr);
    ~TouchProbe();

public slots:
    void slot_NewPixmap(QPixmap);
};

#endif // TOUCHPROBE_H
