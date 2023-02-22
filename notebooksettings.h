#ifndef NOTEBOOKSETTINGS_H
#define NOTEBOOKSETTINGS_H

#include <QDir>
#include <iostream>
#include <QFont>

#define FILE_NAME "settings.note"

using namespace std;

class NotebookSettings
{
public:
    enum Settings
    {
        Font,
        Theme,
        StateString
    };

    enum Themes
    {
        White,
        Black,
    };
#define BLACK_THEME "QPushButton{\
    background-color: rgb(45, 45, 45);\
    color: rgb(240, 240, 240);\
    }\
    \
    QTextEdit{\
    background-color: rgb(45, 45, 45);\
    color: rgb(240, 240, 240);\
    selection-background-color: rgb(201, 73, 180);\
    }\
    QMainWindow{\
        background-color: rgb(45, 45, 45);\
        color: rgb(240, 240, 240);\
    }\
    QStatusBar{\
        background-color: rgb(45, 45, 45);\
        color: rgb(240, 240, 240);\
    }\
    QLabel{\
        color: rgb(240, 240, 240);\
    }\
    QMessageBox{\
        background-color: rgb(45, 45, 45)\
    }"

#define WHITE_THEME "QPushButton{\
    background-color: rgb(240, 240, 240);\
    color: rgb(45, 45, 45);\
    }\
    \
    QTextEdit{\
    background-color: rgb(240, 240, 240);\
    color: rgb(45, 45, 45);\
    selection-background-color: rgb(201, 73, 180);\
    }\
    QMainWindow{\
        background-color: rgb(240, 240, 240);\
        color: rgb(45, 45, 45);\
    }\
    QStatusBar{\
        background-color: rgb(240, 240, 240);\
        color: rgb(45, 45, 45);\
    }\
    QLabel{\
        color: rgb(45, 45, 45);\
    }\
    QMessageBox{\
        background-color: rgb(240, 240, 240)\
    }"

    NotebookSettings();

    //сеттеры
    void setNotebookFont(QFont font);
    void setNotebookTheme(NotebookSettings::Themes theme);
    void setNotebookStateString(bool flag);

    //геттеры
    QFont getNotebookFont();
    NotebookSettings::Themes getNotebookTheme();
    bool getNotebookStateString();

private:

    void createSettingsFile(QString absPath);

    void setNotebookSetting(NotebookSettings::Settings setting, QString value);

    QString getSettingNameFromEnum(NotebookSettings::Settings setting);

    QString getNotebookSetting(NotebookSettings::Settings setting);

    QFile settingsFile;
};

#endif // NOTEBOOKSETTINGS_H
