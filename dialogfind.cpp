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
    DialogFind(parent)
{
    this->query = query;
    ui->lineEdit_query->setText(query);
}

DialogFind::~DialogFind()
{
    delete ui;
    emit onDelete();
}

void DialogFind::on_pushButton_search_futrher_clicked()
{
    QString query = ui->lineEdit_query->text();
    cout << query.toStdString() << endl;
    emit sendData(query, this->flags);
}


void DialogFind::on_checkBox_case_sensetive_stateChanged(int arg1)
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


void DialogFind::on_radioButton_down_clicked()
{
    this->flags = this->flags | QTextDocument::FindFlag::FindBackward;
}


void DialogFind::on_radioButton_up_clicked()
{
    this->flags = this->flags & 6; //mask = 110, FindBackward = 1
}


void DialogFind::on_cancel_clicked()
{
    this->done(0);
}

