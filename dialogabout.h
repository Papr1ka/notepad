#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <iostream>

using namespace std;

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = nullptr);
    ~DialogAbout();

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
