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
    DialogFind(QString query, QWidget *parent = nullptr); //Конструктор, с инициализацией данных для поиска
    ~DialogFind();

signals:
    void sendData(QString query, QTextDocument::FindFlags flags = QTextDocument::FindFlags()); //отправить данные для поиска
    void onDelete(); //сообщить об удалении класса (DiaflogFind)

private slots:
    void on_pushButton_search_futrher_clicked(); //Найти далее

    void on_checkBox_case_sensetive_stateChanged(int arg1); //Установить флаг учёт регистра

    void on_radioButton_down_clicked(); //Установить флаг найти вниз

    void on_radioButton_up_clicked(); //Найти вверх

    void on_cancel_clicked(); //Отмена (закрытие диалога)

private:
    Ui::DialogFind *ui;
    QString query;
    QTextDocument::FindFlags flags = QTextDocument::FindFlags(); //Флаги для поиска
};

#endif // DIALOGFIND_H
