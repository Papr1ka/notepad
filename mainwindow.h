#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <notebooksettings.h>
#include <dialogabout.h>

#include <QMainWindow>
#include <QFileDialog> //Библиотека с диалоговыми окнами для открытия файла
#include <QMessageBox>
#include <QCloseEvent> //Библиотека с обработчиком события сигнала завершения
#include <QFontDialog> //Библиотека с диалоговым окном для выбора шрифта
#include <QTextBrowser>
#include <windows.h>
#include <QInputDialog>
#include <dialogfind.h>
#include <QDateTime>
#include <QLabel>
#include <QDir>

#include <limits.h>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void recieveData(QString query, QTextDocument::FindFlags flags); //Получить данные для поиска выражения от диалога поиска

    void recieveOnDelete(); //получить сигнал о закрытии диалога поиска

private slots:

    void on_Menu_Open_triggered(); //Меню открыть

    void on_Menu_Save_As_triggered(); //Меню Сохранить как

    void closeEvent(QCloseEvent *event); //Обработчик события закрытия программы

    void on_Menu_Save_triggered(); //Меню сохранить

    void on_textEdit_textChanged(); //Текст редактора изменён

    void on_Menu_Create_triggered(); //Меню создать

    void on_Menu_Exit_triggered(); //Меню выйти

    void on_Menu_Fonts_triggered(); //Меню выбор шрифта

    void on_Menu_Set_Black_Theme_triggered(); //Меню установить тёмную тему

    void on_Menu_Set_White_Theme_triggered(); //Меню установить светлую тему

    void on_textEdit_cursorPositionChanged(); //Позиция курсора изменена

    void on_Menu_State_String_Visible_Change_triggered(); //Меню изменить видимость строки состояния

    void on_Menu_Create_New_Window_triggered(); //Меню создать новое окно

    void on_Menu_About_triggered(); //Меню о программе

    void on_Menu_Redo_triggered(); //Меню отменить

    void on_Menu_Cut_triggered(); //Меню вырезать

    void on_Menu_Copy_triggered(); //Меню копировать

    void on_Menu_Paste_triggered(); //Меню вставить

    void on_Menu_Remove_triggered(); //Меню удалить

    void on_Menu_Search_triggered(); //Меню найти

    void on_Menu_Search_Further_triggered(); //Меню найти далее

    void on_Menu_Search_Previously_triggered(); //Меню найти ранее

    void on_Menu_Go_To_triggered(); //Меню перейти на строку

    void on_Menu_Select_All_triggered(); //Меню выбрать всё

    void on_Menu_DateTime_triggered(); //Меню дата и время

    void on_textEdit_selectionChanged(); //Выделение изменено

    void on_Menu_scale_plus_triggered(); //Меню увеличить масштаб

    void on_Menu_scale_minus_triggered(); //Меню уменьшить масштаб

    void on_Menu_scale_set_default_triggered(); //Меню установить масштаб по умолчанию

private:
    Ui::MainWindow *ui;

    QString path; //путь до последнего файла
    QString fileName; //имя последнего файла

    QString dirPath = QDir::currentPath(); //Путь до текущей папки для определённости

    QString findQuery = ""; //Последний поисковый запрос

    QLabel *stateString = nullptr; //Строка состояния

    QString makeStateString(int row, int col, int selected); //составить строку состояния из значений
    QString makeStateString(int row, int col); //составить строку состояний из значений ьез выделения

    void scaleTo(int scale); //устанавливает масштаб в TextEdit в нужное значение

    void saveToTextFileUTF8(); //Сохранить текстовый файл с информацией из TextEdit

    bool saveDocument(); //Сохранить документ

    bool saveDocumentAs(); //Сохранить документ как

    bool saveDecorator(bool (MainWindow::*saveFunction) ()); //Обёртка для методов сохранения

    void loadTheme(NotebookSettings::Themes theme); //Применить тему для блокнота

    void setStateStringActive(bool flag); //Изменить отображение строки состояния

    void setEnabledMenuActions(int modifiedFlags); //Изменить активность кнопок меню в зависимости от текущего состояния документа (символов нет, текст выделен)

    int checkSave(); //Проверка, модифицирован ли файл, если да, то будет ли пользователь его сохранять, при проверке вызывает QMessageBox

    bool lastSelectedCharactersCountIsNull = true; //В момент последнего выделения было ли выделено 0 символов

    /*
    32 bool flags
    код : число десятичное : название флага
    00000001: 1 :документ изменен
    00000010: 2 :документ создан (по умолчанию да)
    00000100: 4 :документ служебно заблокирован (слот on text changed не отрабатывает)
    00001000: 8 :строка состояния служебно заблокирована (слот on cursor changed не отрабатывает), Строка состояния не отображается
    00010000: 16 :документ пуст
    00100000: 32 :выделение не содержит элементов
    */
    unsigned __int8 flags = 2 + 16 + 32;

    int scale = 100; //масштаб, проценты, умноженные на 100

    NotebookSettings *Settings; //Настройки блокнота

    DialogFind *dialog = nullptr; //Диалог для поиска

#define FLAGS_SIZE (UINT8_MAX)
};

#endif // MAINWINDOW_H
