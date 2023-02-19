#ifndef DIALOGFIND_H
#define DIALOGFIND_H

#include <QDialog>
#include <QTextDocument>

#include <iostream>

using namespace std;

namespace Ui {
class DialogFind;
}

class DialogFind : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFind(QWidget *parent = nullptr);
    DialogFind(QString query, QWidget *parent = nullptr);
    ~DialogFind();

signals:
    void sendData(QString query, QTextDocument::FindFlags flags = QTextDocument::FindFlags());
    void onDelete();

private slots:
    void on_pushButton_search_futrher_clicked();

    void on_checkBox_case_sensetive_stateChanged(int arg1);

    void on_radioButton_down_clicked();

    void on_radioButton_up_clicked();

    void on_cancel_clicked();

private:
    Ui::DialogFind *ui;
    QString query;
    QTextDocument::FindFlags flags = QTextDocument::FindFlags();
};

#endif // DIALOGFIND_H
