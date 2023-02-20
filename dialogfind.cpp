#include "dialogfind.h"
#include "ui_dialogfind.h"

DialogFind::DialogFind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFind)
{
    ui->setupUi(this);
    this->setWindowTitle("Найти");
}

DialogFind::DialogFind(QString query, QWidget *parent) :
    DialogFind(parent) //Конструктор, если уже есть что искать
{
    this->query = query;
    ui->lineEdit_query->setText(query);
}

DialogFind::~DialogFind()
{
    delete ui;
    emit onDelete(); //Сообщить об удалении, чтобы была возможность удалить указатель на этот класс
}

void DialogFind::on_pushButton_search_futrher_clicked() //Кнопка найти далее
{
    QString query = ui->lineEdit_query->text();
    emit sendData(query, this->flags); //Сообщить данные (запрос для поиска, флаги для поиска)
}


void DialogFind::on_checkBox_case_sensetive_stateChanged(int arg1) //Кнопка поиск с учётом регистра
{
    if (arg1)
    {
        this->flags = this->flags | QTextDocument::FindFlag::FindCaseSensitively;
    }
    else
    {
        this->flags = this->flags & 5; //mask = 101, FindCaseSensitively = 2
    }
}


void DialogFind::on_radioButton_down_clicked() //Кнопка поиск вниз
{
    this->flags = this->flags | QTextDocument::FindFlag::FindBackward;
}


void DialogFind::on_radioButton_up_clicked() //Кнопка поиск вверх
{
    this->flags = this->flags & 6; //mask = 110, FindBackward = 1
}


void DialogFind::on_cancel_clicked() //Кнопка отмена
{
    this->done(0);
}

