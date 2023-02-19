#include "notebooksettings.h"

NotebookSettings::NotebookSettings()
{
    QDir currentDir = QDir::current();
    QString absPath = currentDir.path() + "//" + FILE_NAME;

    if (!currentDir.exists(FILE_NAME))
    {
        this->createSettingsFile(absPath);
    }
    else
    {
        settingsFile.setFileName(absPath);
    }
}

QString NotebookSettings::getSettingNameFromEnum(NotebookSettings::Settings setting)
{
    QString result = "";
    switch (setting) //из enum в строку
    {
        case NotebookSettings::Font:
            result = "font";
            break;
        case NotebookSettings::Theme:
            result = "theme";
            break;
        case NotebookSettings::StateString:
            result = "state_string";
            break;
    }
    return result;
}

void NotebookSettings::setNotebookStateString(bool flag)
{
    this->setNotebookSetting(NotebookSettings::StateString, QString::number(flag));
}

bool NotebookSettings::getNotebookStateString()
{
    QString isStateStringActiveString = this->getNotebookSetting(NotebookSettings::StateString);
    bool isInt;
    int isStateStringActive = isStateStringActiveString.toInt(&isInt);
    if (isInt && isStateStringActive == 0)
    {
        return false;
    }
    return true;
}

NotebookSettings::Themes NotebookSettings::getNotebookTheme()
{
    QString themeString = this->getNotebookSetting(NotebookSettings::Theme);
    bool isInt;
    int theme = themeString.toInt(&isInt);
    if (themeString == "" or !isInt or theme > 1 or theme < 0)
    {
        return NotebookSettings::White;
    }
    else
    {
        NotebookSettings::Themes a;
        a = static_cast<NotebookSettings::Themes>(theme);
        return a;
    }
}

QFont NotebookSettings::getNotebookFont()
{
    QString fontString = this->getNotebookSetting(NotebookSettings::Font);
    QFont font;
    font.fromString(fontString);
    if (fontString == "")
    {
        return QFont("Noto Sans", 18);
    }
    else
    {
        return font;
    }
}

void NotebookSettings::setNotebookFont(QFont font)
{
    this->setNotebookSetting(NotebookSettings::Font, font.toString());
}

void NotebookSettings::setNotebookTheme(NotebookSettings::Themes theme)
{
    this->setNotebookSetting(NotebookSettings::Theme, QString::number(static_cast<int>(theme)));
}

void NotebookSettings::setNotebookSetting(NotebookSettings::Settings setting, QString value)
{
    if (not settingsFile.exists())
    {
        this->createSettingsFile(settingsFile.fileName());
    }

    QString settingName = this->getSettingNameFromEnum(setting);

    QString cell = settingName + ":" + value; //настройка: значение, которые необходимо записать

    settingsFile.open(QIODevice::ReadWrite | QIODevice::Text);

    QByteArray allData = settingsFile.readAll(); //читаем всё
    QString allTextData(allData);

    QStringList dataList = allTextData.split("\n"); //разбиваем на массив строк по \n

    //ищем строку с настройкой по индексу

    int length = dataList.length();
    int param_index = -1;

    for (int i = 0; i < length; i++)
    {
        if (dataList[i].contains(settingName))
        {
            param_index = i;
            break;
        }
    }

    if (param_index != -1) //если нашли, то заменяем
    {
        dataList.removeAt(param_index);
        dataList.insert(param_index, cell);
    }
    else //если не нашли, то создаём в начале
    {
        dataList.insert(0, cell);
    }


    //перезапись
    QString newStringData = dataList.join("\n");
    QByteArray newData = newStringData.toUtf8();

    settingsFile.seek(0);
    settingsFile.write(newData);
    settingsFile.resize(newData.size() + dataList.length() - 1); //изменяем размер файла

    settingsFile.close();
}


QString NotebookSettings::getNotebookSetting(NotebookSettings::Settings setting) //поиск настройки, если найдена, возвращает строку, если нет, то пустую строку
{
    if (not settingsFile.exists())
    {
        this->createSettingsFile(settingsFile.fileName());
    }

    QString settingName = this->getSettingNameFromEnum(setting);

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray allData = settingsFile.readAll(); //читаем всё
    QString allTextData(allData);

    QStringList dataList = allTextData.split("\n"); //разбиваем на массив строк по \n

    //ищем строку с настройкой по индексу

    int length = dataList.length();
    int param_index = -1;

    for (int i = 0; i < length; i++)
    {
        if (dataList[i].contains(settingName))
        {
            param_index = i;
            break;
        }
    }

    settingsFile.close();

    if (param_index != -1) //если нашли, то заменяем
    {
        QString result = dataList[param_index].section(":", -1);
        result.remove("\n");
        return result;
    }
    else //если не нашли, то создаём в начале
    {
        return "";
    }
}


void NotebookSettings::createSettingsFile(QString absPath)
{
    settingsFile.setFileName(absPath);
    settingsFile.open(QIODevice::WriteOnly);
    settingsFile.close();
}

