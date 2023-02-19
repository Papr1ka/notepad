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
    void recieveData(QString query, QTextDocument::FindFlags flags);

    void recieveOnDelete();

private slots:

    void on_Menu_Open_triggered();

    void on_Menu_Save_As_triggered();

    void closeEvent(QCloseEvent *event); //обработчик события закрытия программы

    void on_Menu_Save_triggered();

    void on_textEdit_textChanged();

    void on_Menu_Create_triggered();

    void on_Menu_Exit_triggered();

    void on_Menu_Fonts_triggered();

    void on_Menu_Set_Black_Theme_triggered();

    void on_Menu_Set_White_Theme_triggered();

    void on_textEdit_cursorPositionChanged();

    void on_Menu_State_String_Visible_Change_triggered();

    void on_Menu_Create_New_Window_triggered();

    void on_Menu_About_triggered();

    void on_Menu_Redo_triggered();

    void on_Menu_Cut_triggered();

    void on_Menu_Copy_triggered();

    void on_Menu_Paste_triggered();

    void on_Menu_Remove_triggered();

    void on_Menu_Search_By_Bing_triggered();

    void on_Menu_Search_triggered();

    void on_Menu_Search_Further_triggered();

    void on_Menu_Search_Previously_triggered();

    void on_Menu_Go_To_triggered();

    void on_Menu_Select_All_triggered();

    void on_Menu_DateTime_triggered();

    void on_textEdit_selectionChanged();

private:
    Ui::MainWindow *ui;

    QString path;
    QString fileName;

    QString dirPath = QDir::currentPath();

    QString findQuery = "";

    void saveToTextFileUTF8();

    bool saveDocument();

    bool saveDocumentAs();

    bool saveDecorator(bool (MainWindow::*saveFunction) ());

    void loadTheme(NotebookSettings::Themes theme);

    void setStateStringActive(bool flag);

    void setEnabledMenuActions(int modifiedFlags);

    int checkSave();

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

    NotebookSettings *Settings;

    DialogFind *dialog = nullptr;

#define FLAGS_SIZE (UINT8_MAX)
};

#endif // MAINWINDOW_H
