#include "notebooksettings.h"

NotebookSettings::NotebookSettings()
{
    QDir currentDir = QDir::current();
    QString absPath = currentDir.path() + "//" + FILE_NAME; //текущее расположение файла

    if (!currentDir.exists(FILE_NAME))
    {
        this->createSettingsFile(absPath);
    }
    else
    {
        settingsFile.setFileName(absPath);
    }
}

QString NotebookSettings::getSettingNameFromEnum(NotebookSettings::Settings setting) //Получить название опции из Enum'а
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

void NotebookSettings::setNotebookStateString(bool flag) //Установить значение видимости строки состояния
{
    this->setNotebookSetting(NotebookSettings::StateString, QString::number(flag));
}

bool NotebookSettings::getNotebookStateString() //Получить значение видимости строки состояния
{
    QString isStateStringActiveString = this->getNotebookSetting(NotebookSettings::StateString);
    bool isInt;
    int isStateStringActive = isStateStringActiveString.toInt(&isInt);
    if (isInt && isStateStringActive == 0)
    {
        return false; //Если не отображается и всё корректно
    }
    return true; //во всех иных случаях, даже если данные испорчены
}

NotebookSettings::Themes NotebookSettings::getNotebookTheme() //Получить значение темы блокнота
{
    QString themeString = this->getNotebookSetting(NotebookSettings::Theme);
    bool isInt;
    int theme = themeString.toInt(&isInt);
    if (themeString == "" or !isInt or theme > 1 or theme < 0) //Если белая и всё корректно
    {
        return NotebookSettings::White;
    }
    else //во всех иных случаях, даже если данные испорчены
    {
        NotebookSettings::Themes a;
        a = static_cast<NotebookSettings::Themes>(theme);
        return a;
    }
}

QFont NotebookSettings::getNotebookFont() //Получить значение шрифта для редактора
{
    QString fontString = this->getNotebookSetting(NotebookSettings::Font);
    QFont font;
    bool isFontConvertedCorrectly = font.fromString(fontString);
    if (fontString == "" or !isFontConvertedCorrectly) //Если значение шрифта отсутствует или получено некорректно
    {
        return QFont("Noto Sans", 18);
    }
    else //Если всё конвертировалось корректно
    {
        return font;
    }
}

void NotebookSettings::setNotebookFont(QFont font) //Установить (сохранить) значение шрифта для редактора
{
    this->setNotebookSetting(NotebookSettings::Font, font.toString());
}

void NotebookSettings::setNotebookTheme(NotebookSettings::Themes theme) //Установить (сохранить) значение темы блокнота
{
    this->setNotebookSetting(NotebookSettings::Theme, QString::number(static_cast<int>(theme)));
}

void NotebookSettings::setNotebookSetting(NotebookSettings::Settings setting, QString value) //Сохранить настройку в виде ключ:значение в файл
{
    if (not settingsFile.exists()) //Если файл существует
    {
        this->createSettingsFile(settingsFile.fileName()); //Иначе создать
    }

    QString settingName = this->getSettingNameFromEnum(setting); //Имя настройки

    QString cell = settingName + ":" + value; //настройка: значение, которые необходимо записать

    settingsFile.open(QIODevice::ReadWrite | QIODevice::Text);

    QByteArray allData = settingsFile.readAll(); //читаем всё
    QString allTextData(allData);

    QStringList dataList = allTextData.split("\n"); //разбиваем на массив строк по \n

    //ищем строку с настройкой по индексу

    int length = dataList.length(); //Сколько строк было в файле
    int param_index = -1; //Номер строки с нужной нам настройкой, -1 если не нашлось

    for (int i = 0; i < length; i++) //Поиск строки с нужной настройкой
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

    settingsFile.seek(0); //Перейти в начало файла
    settingsFile.write(newData); //И Вставить изменённый вариант
    settingsFile.resize(newData.size() + dataList.length() - 1); //изменяем размер файла

    settingsFile.close();
}


QString NotebookSettings::getNotebookSetting(NotebookSettings::Settings setting) //поиск настройки, если найдена, возвращает строку, если нет, то пустую строку
{
    if (not settingsFile.exists()) //Если файл существует
    {
        this->createSettingsFile(settingsFile.fileName()); //Иначе создать
    }

    QString settingName = this->getSettingNameFromEnum(setting); //Имя настройки

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray allData = settingsFile.readAll(); //читаем всё
    QString allTextData(allData);

    QStringList dataList = allTextData.split("\n"); //разбиваем на массив строк по \n

    //ищем строку с настройкой по индексу

    int length = dataList.length(); //Сколько строк было в файле
    int param_index = -1; //Номер строки с нужной нам настройкой, -1 если не нашлось

    for (int i = 0; i < length; i++) //Поиск строки с нужной настройкой
    {
        if (dataList[i].contains(settingName))
        {
            param_index = i;
            break;
        }
    }

    settingsFile.close();

    if (param_index != -1) //если нашли, то вернуть чистое (без /n значение)
    {
        QString result = dataList[param_index].section(":", -1);
        result.remove("\n");
        return result;
    }
    else //если не нашли, то вернуть пустую строку
    {
        return "";
    }
}


void NotebookSettings::createSettingsFile(QString absPath) //Создать файл настроек
{
    settingsFile.setFileName(absPath);
    settingsFile.open(QIODevice::WriteOnly);
    settingsFile.close();
}

