#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
Если вдруг необходимо, чтобы программа выдавала MessageBox'ы, если файл или шрифт не выбран, то разкоментировать строки с кодовым словом "ёпрст"
 */

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->stateString = new QLabel; //строку стостояния в статусбар
    stateString->setText("Стр 1 стлб 1");
    stateString->setFont(QFont("Segoe UI", 10));
    ui->statusBar->addPermanentWidget(stateString);

    this->setWindowTitle("Безымянный.txt - Блокнот");
    this->setWindowIcon(QIcon(":/ico/note.ico"));

    this->Settings = new NotebookSettings; //создаём экземпляр класса настроек

    ui->textEdit->setFont(this->Settings->getNotebookFont()); //устанавливаем сохранённые шрифт и тему

    if (Settings->getNotebookStateString() == false)
    {
        this->setStateStringActive(Settings->getNotebookStateString());
    }
    this->loadTheme(this->Settings->getNotebookTheme());
    this->setEnabledMenuActions(16 + 32); //По умолчанию кнопки меню не активны
}


MainWindow::~MainWindow()
{
    delete Settings;
    delete ui;
    delete stateString;
    if (this->dialog != nullptr)
    {
        delete this->dialog;
    }
}


void MainWindow::recieveData(QString query, QTextDocument::FindFlags flags) //Строка для поиска и Флаги для поиска из диалога
{
    this->findQuery = query;

    bool finded = ui->textEdit->find(query, flags);
    if (not finded)
    {
        QMessageBox::information(this->dialog, "Блокнот", QString("Не удаётся найти \"") + query + "\"");
    }
}

void MainWindow::recieveOnDelete() //слот обработки сигнала onDelete от диалога поиска выражения в TextEdit
{
    cout << "need to delete" << endl;
    this->dialog = nullptr;
}

void MainWindow::setEnabledMenuActions(int modifiedFlags) //Изменить Активность кнопок меню
{
    if (modifiedFlags & 16)
    {
        ui->Menu_Search->setEnabled(!ui->Menu_Search->isEnabled());
        ui->Menu_Search_Further->setEnabled(!ui->Menu_Search_Further->isEnabled());
        ui->Menu_Search_Previously->setEnabled(!ui->Menu_Search_Previously->isEnabled());

    }
    if (modifiedFlags & 32)
    {
        ui->Menu_Cut->setEnabled(!ui->Menu_Cut->isEnabled());
        ui->Menu_Copy->setEnabled(!ui->Menu_Copy->isEnabled());
        ui->Menu_Remove->setEnabled(!ui->Menu_Remove->isEnabled());
        ui->Menu_Search_By_Bing->setEnabled(!ui->Menu_Search_By_Bing->isEnabled());
    }
}

void MainWindow::loadTheme(NotebookSettings::Themes theme) //загрузка shylesheet в зависимости от темы
{
    QString styleSheet;
    switch (theme) {
        case NotebookSettings::Black:
            styleSheet = BLACK_THEME;
            ui->Menu_Set_White_Theme->setChecked(false);
            ui->Menu_Set_Black_Theme->setChecked(true);
            break;
        case NotebookSettings::White:
            styleSheet = WHITE_THEME;
            ui->Menu_Set_Black_Theme->setChecked(false);
            ui->Menu_Set_White_Theme->setChecked(true);
            break;
    }
    this->setStyleSheet(styleSheet);
    if (this->stateString != nullptr)
    {
        this->stateString->setStyleSheet(styleSheet);
    }
}

void MainWindow::saveToTextFileUTF8() //запись текста в UTF8 из TextEdit в файл по пути path
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::WriteOnly);

    file.write(ui->textEdit->toPlainText().toUtf8());
    file.close();
}

int MainWindow::checkSave() //проверка, модифицирован ли файл, если да, то будет ли пользователь его сохранять
{
    if (this->flags & 1)
    {
        return QMessageBox::question(this, "Блокнот", QString("Вы хотите сохранить изменения в файле") + '"' + path + '"' + " ?", QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel, QMessageBox::Save);
    }
    return QMessageBox::NoButton;
}


void MainWindow::closeEvent(QCloseEvent *event) //обработка сигнала на закрытие программы, защита от несохранённых изменений
{

    int result = this->checkSave(); //защита от несохранённых изменений
    bool saved = true;
    bool exit = false;

    switch (result)
    {
        case QMessageBox::Save:
            saved = this->saveDecorator(&MainWindow::saveDocument);
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        case QMessageBox::Ignore:
            event->accept();
            exit = true;
            break;
        case QMessageBox::NoButton:
            event->accept();
            exit = true;
            break;
    }
    if (not saved) //если пользователь не смог сохранить, не выходить
    {
        event->ignore();
    }
    if (exit) //если мы выходим
    {
        if (this->dialog != nullptr) //и диалог требуется удалить
        {
            this->dialog->done(0);
            delete this->dialog;
        }
    }
}

bool MainWindow::saveDecorator(bool (MainWindow::*saveFunction) ()) //декоратор для функций сохранения, вызов, сброс флага модифицированности, созданного файла, изменение названия окна true, если сохранено, false иначе
{

    bool result = (this->*saveFunction)();
    if (result)
    {
        this->flags = this->flags & (FLAGS_SIZE - 1);
        this->setWindowTitle(fileName + " - Блокнот");

        this->flags = this->flags & (FLAGS_SIZE - 2);
    }
    return result;
}

bool MainWindow::saveDocumentAs() //сохранить документ как true, если успешно
{
    QString pretenderToPath;
    if (path != "")
    {
        pretenderToPath = QFileDialog::getSaveFileName(this, tr("Сохранить как"), path, tr("Text files (*.txt)"));
    }
    else
    {
        pretenderToPath = QFileDialog::getSaveFileName(this, tr("Сохранить как"), dirPath, tr("Text files (*.txt)"));
    }


    if (pretenderToPath.isEmpty()) //Файл не выбран
    {
//        QMessageBox::information(this, "Ошибка", "Файл не выбран"); ёпрст
        return false;
    }
    else
    {
        this->path = pretenderToPath;
        this->saveToTextFileUTF8(); //сохранение в файл

        this->fileName = path.section('/', -1); //отделение имени файла
        return true;
    }
}

bool MainWindow::saveDocument() //авто выбор способа сохранения (по старому пути, если файл был установлен или по новому (нужно выбрать при помощи сохранить как), если путь некорректен или не установлен
{
    if (path != "")
    {
        bool exists = QFile::exists(path);
        if (exists) //по старому пути
        {
            this->saveToTextFileUTF8();
            return true;
        }
        else //по новому
        {
            QMessageBox::information(this, "Блокнот", "Ошибка, путь к файлу не сушествует");
            return this->saveDocumentAs();
        }
    }
    else //по новому
    {
        return this->saveDocumentAs();
    }
}

void MainWindow::on_Menu_Open_triggered() //Меню Открыть файл
{
    QString pretenderToPath; //путь до файла

    int check = this->checkSave();

    switch (check)
    {
        case QMessageBox::Save:
            this->saveDecorator(&MainWindow::saveDocument);
            break;
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Ignore:
            break;
        case QMessageBox::NoButton:
            break;
    }

    pretenderToPath = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "C:\\Users\\student.A-424\\Documents", tr("Text files (*.txt)"));

    if (pretenderToPath.isEmpty()) //Файл не выбран
    {
//        QMessageBox::information(this, "Ошибка", "Файл не выбран"); //не будем душить тем, что он итак понял ёпрст
        return;
    }
    else
    {
        //подготовка
        this->path = pretenderToPath; //запоминание пути до файла
        this->flags = this->flags & (FLAGS_SIZE - 2); //убрать флаг документ создан
        this->flags = this->flags & (FLAGS_SIZE - 1); //убрать флаг документ изменён
        this->flags = this->flags | 4; //заблокировать обработку события изменения текста в TextEdt


        ui->textEdit->clear(); //очистка поля редактора
        QFile file; //файл

        this->fileName = path.section('/', -1); //выделение имя файла

        this->setWindowTitle(fileName + " - Блокнот");

        //работа с файлом
        file.setFileName(path); //связываем с файлом на диске
        file.open(QIODevice::ReadOnly); //открываем файл на чтение

        long long fileSize; //Размер файла;
        fileSize = file.size(); //Получаем размер файла

        QByteArray ba; //массив для передачи данных
        ba.clear();
        ba = file.read(fileSize);

        ui->textEdit->append(QString(ba));
        file.close();

        this->flags = this->flags & (FLAGS_SIZE - 4); //разблокировать обработку события изменения текста в TextEdt
    }
}

void MainWindow::on_Menu_Save_As_triggered() //Меню Сохранить как
{
    this->saveDecorator(&MainWindow::saveDocumentAs);
}

void MainWindow::on_Menu_Save_triggered() //Меню Сохранить
{
    this->saveDecorator(&MainWindow::saveDocument);
}

void MainWindow::on_textEdit_textChanged() //Событие изменения содержимого TextEdit
{
    if (!(flags & 4)) //если файл не заблокирован
    {
        if (ui->textEdit->document()->characterCount() == 1) //если документ пуст
        {
            if (!(flags & 16))
            {
                this->setEnabledMenuActions(16);
            }
            this->flags = flags | 16;
        }
        else
        {
            if (flags & 16)
            {
                this->setEnabledMenuActions(16);
            }
            this->flags = flags & (FLAGS_SIZE - 16);
        }
        if (!(flags & 1)) //и не модифицирован
        {
            if (flags & 2) //Если файл создан и не имеет собственного названия
            {
               this->setWindowTitle("*Безымянный.txt - Блокнот");
            }
            else //Если файл имеет собственное название
            {
                this->setWindowTitle("*" + this->fileName + " - Блокнот");
            }
            this->flags = flags | 1; //Зафиксировать то, что файл изменён
        }
        else
        {
            if ((flags & 16) and (flags & 2)) //Если файл создан, не имеет собственного названия и пуст
            {
                this->setWindowTitle("Безымянный.txt - Блокнот");
                this->flags = (flags | 16) & (FLAGS_SIZE - 1);
            }
        }
    }
}


void MainWindow::on_Menu_Create_triggered() //Меню Создать
{
    int result = this->checkSave(); //Защита от несохранённых изменений

    switch (result)
    {
        case QMessageBox::NoButton:
            break;
        case QMessageBox::Save:
            this->saveDecorator(&MainWindow::saveDocument);
        case QMessageBox::Ignore:
            break;
        case QMessageBox::Cancel:
            return;
    }

    this->flags = this->flags | 4; //блокировка обработки событий изменения текста в TextEdt

    //сброс флагов, пути, имени файла, содержимого TextEdit, названия окна
    this->flags = this->flags | 2;
    this->flags = this->flags & (FLAGS_SIZE - 1);

    this->fileName.clear();
    this->path.clear();
    ui->textEdit->clear();
    this->setWindowTitle("Безымянный.txt - Блокнот");


    this->flags = this->flags & (FLAGS_SIZE - 4); //разблокировка обработки событий изменения текста в TextEdt
}


void MainWindow::on_Menu_Exit_triggered() //Меню Выход
{
    QWidget::close();
}


void MainWindow::on_Menu_Fonts_triggered() //Меню Выбор Шрифта
{
    bool choosed;
    QFont font = QFontDialog::getFont(&choosed, this->Settings->getNotebookFont(), this); //Получение шрифта от пользователя, по умолчанию выбран последний сохранённый из файла настроек
    if (choosed)
    {
        ui->textEdit->setFont(font);
        this->Settings->setNotebookFont(font);
    }
    else
    {
//        QMessageBox::information(this, "Блокнот", "Ошибка, шрифт не выбран"); ёпрст
    }
}

void MainWindow::on_Menu_Set_Black_Theme_triggered() //Меню Тёмная тема
{
    this->loadTheme(NotebookSettings::Black);
    this->Settings->setNotebookTheme(NotebookSettings::Black);
}


void MainWindow::on_Menu_Set_White_Theme_triggered() //Меню Светлая тема
{
    this->loadTheme(NotebookSettings::White);
    this->Settings->setNotebookTheme(NotebookSettings::White);
}


void MainWindow::on_textEdit_cursorPositionChanged() //Событие изменения положения курсора в TextEdit
{
    if (!(this->flags & 8)) //если событие разблокировано
    {
        QString state; //Строка состояния, куда будут записаны данные
        QTextCursor cursor = ui->textEdit->textCursor(); //получение копии курсора TextEdit
        int col = cursor.columnNumber() + 1; //номер колонки
        int str = cursor.blockNumber() + 1; //номер строки
        int selected = abs(cursor.selectionEnd() - cursor.selectionStart()); //Количество выделенных символов
        if (selected != 0)
        {
            state = "Стр " + QString::number(str) + " стлб " + QString::number(col) + "   " + QString::number(selected) + " выделено";;
        }
        else
        {
            state = "Стр " + QString::number(str) + " стлб " + QString::number(col);
        }
        this->stateString->setText(state);
    }
}

void MainWindow::on_textEdit_selectionChanged() //Изменение текущего выделения
{
    if (!(this->flags & 8)) //если событие разблокировано
    {
        QString state; //Строка состояния, куда будут записаны данные
        QTextCursor cursor = ui->textEdit->textCursor(); //получение копии курсора TextEdit
        int col = cursor.columnNumber() + 1; //номер колонки
        int str = cursor.blockNumber() + 1; //номер строки
        int selected = abs(cursor.selectionEnd() - cursor.selectionStart()); //Количество выделенных символов

        if (selected == 0) //если не выделено не одного элемента
        {
            if (!lastSelectedCharactersCountIsNull)
            {
                state = "Стр " + QString::number(str) + " стлб " + QString::number(col); //меняем строку состояния
                this->lastSelectedCharactersCountIsNull = true;
                this->stateString->setText(state);
            }
            if (!(flags & 32)) //если изменилось состояние выделения (было выделено > 0 символов), а теперь 0
            {
                this->setEnabledMenuActions(32); //поменять активность кнопок меню
            }
            this->flags = flags | 32;
        }
        else //если выделены элементы
        {
            this->lastSelectedCharactersCountIsNull = false;
            state = "Стр " + QString::number(str) + " стлб " + QString::number(col) + "   " + QString::number(selected) + " выделено"; //меняем строку состояния
            this->stateString->setText(state);
            this->stateString->repaint();

            if (flags & 32) //если изменилось состояние выделения (было выделено > 0 символов), а теперь 0
            {
                this->setEnabledMenuActions(32); //поменять активность кнопок меню
            }
            this->flags = flags & (FLAGS_SIZE - 32);
        }
    }
}

void MainWindow::on_Menu_State_String_Visible_Change_triggered() //Меню Строка Состояния
{
    this->setStateStringActive(this->flags & 8);
}

void MainWindow::setStateStringActive(bool flag) //Изменить Отображение Строки Состояния
{
    if (flag) //показать строку состояния
    {
        this->flags = this->flags & (FLAGS_SIZE - 8); //разблокировать событие обработки изменения положения курсора
        ui->statusBar->show();
    }
    else //спрятать строку состояния
    {
        this->flags = this->flags | 8; //заблокировать событие
        ui->statusBar->hide();
    }
    this->Settings->setNotebookStateString(flag); //Изменить наличие галочки на QAction Menu_State_String
    ui->Menu_State_String_Visible_Change->setChecked(flag);
}

void MainWindow::on_Menu_Create_New_Window_triggered() //Меню Создать Окно
{
    MainWindow *window = new MainWindow;
    window->setAttribute(Qt::WA_DeleteOnClose); //Чтобы при закрытии отдельного окна освобождалась память, а не при закрытии головного окна
    window->show();
}


void MainWindow::on_Menu_About_triggered() //Меню О Программе
{
    DialogAbout *dialog = new DialogAbout;
    dialog->setModal(true);
    dialog->exec();
}

void MainWindow::on_Menu_Redo_triggered() //Меню Отменить
{
    ui->textEdit->undo();
}


void MainWindow::on_Menu_Cut_triggered() //Меню Вырезать
{
    ui->textEdit->cut();
}


void MainWindow::on_Menu_Copy_triggered() //Меню Копировать
{
    ui->textEdit->copy();
}


void MainWindow::on_Menu_Paste_triggered() //Меню Вставить
{
    ui->textEdit->paste(); //сделать проверку на символы Utf-8, картинка вставилась хреново
}


void MainWindow::on_Menu_Remove_triggered() //Меню Удалить
{
    QTextCursor test = ui->textEdit->textCursor();
    test.removeSelectedText();
}

void MainWindow::on_Menu_Search_By_Bing_triggered() //Поиск При Помощи Bing
{
    QTextCursor test = ui->textEdit->textCursor();
    QString query;
    query = "https://www.bing.com/search?q=" + test.selectedText(); //Запрос
    int length = query.length();
    char charList[query.length()];

    for (int i = 0; i < length; i++)
    {
        charList[i] = static_cast<char>(query.at(i).toLatin1()); //Преобразование запроса в Char list
    }

    ShellExecuteA(0, "open", charList, NULL, NULL, SW_SHOWDEFAULT); //Открыть браузер
}


void MainWindow::on_Menu_Search_triggered() //Меню Найти
{
    QTextCursor test = ui->textEdit->textCursor();
    QString selectedText = test.selectedText();

    if (dialog != nullptr) //Если уже открыт диалог, то закрыть, станет nullptr
    {
        cout << "need to close dialog" << dialog << endl;
        dialog->done(0);
        delete dialog;
    }

    if (selectedText != "") //если пользователь уже выделил то, что хочет найти
    {
        this->dialog = new DialogFind(selectedText);
    }
    else //если пользователь пока не ввёл то, что хочет найти
    {
        this->dialog = new DialogFind();
    }

    connect(dialog, SIGNAL(sendData(QString,QTextDocument::FindFlags)), this, SLOT(recieveData(QString,QTextDocument::FindFlags))); //привязка обработчиков событий
    connect(dialog, SIGNAL(onDelete()), this, SLOT(recieveOnDelete())); //чтобы затирался указатель dialog в MainWondow после удаления диалога
    dialog->setAttribute(Qt::WA_DeleteOnClose); //очистить память после закрытия
    dialog->setModal(false); //не модальное
    dialog->show();
}


void MainWindow::on_Menu_Search_Further_triggered() //Меню Найти Далее
{
    if (this->findQuery != "") //Если уже был некий непустой запрос и есть что искать
    {
        bool finded = ui->textEdit->find(findQuery); //Найти далее
        if (not finded) //Если не нашли
        {
            QMessageBox::information(this, "Блокнот", QString("Не удаётся найти \"") + findQuery + "\"");
        }
    }
    else
    {
        this->on_Menu_Search_triggered(); //Если запроса не было и мы не знаем что искать, то запускаем Найти
    }
}


void MainWindow::on_Menu_Search_Previously_triggered() //Меню Найти Ранее
{
    if (this->findQuery != "") //Если уже был некий непустой запрос и есть что искать
    {
        bool finded = ui->textEdit->find(findQuery, QTextDocument::FindFlag::FindBackward); //Найти ранее
        if (not finded) //Если не нашли
        {
            QMessageBox::information(this, "Блокнот", QString("Не удаётся найти \"") + findQuery + "\"");
        }
    }
    else
    {
        this->on_Menu_Search_triggered(); //Если запроса не было и мы не знаем что искать, то запускаем Найти
    }
}


void MainWindow::on_Menu_Go_To_triggered() //Меню Перейти
{
    QInputDialog dialog;
    bool isInt;
    int value = dialog.getInt(this, "Переход на строку", "Номер строки:", 1, INT_MIN, INT_MAX, 1, &isInt);
    if (isInt) //Если ввели число
    {
        int lineCount = ui->textEdit->document()->lineCount(); //Число строк
        if ((value <= lineCount) and (value > 0)) //Если ввели строку в диапазоне строк документа
        {
            QTextCursor cursor;
            cursor = ui->textEdit->textCursor(); //Копия текущего курсора

            int diff = cursor.blockNumber() + 1 - value; //сколько нужно идти строк от строки курсора до нужной

            if (diff >= 0) //если идти назад
            {
                cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, diff);
            }
            else //если идти вперёд
            {
                cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, -diff);
            }

            ui->textEdit->setTextCursor(cursor); //Установка курсора
        }
        else
        {
            QMessageBox::information(this, "Блокнот - переход на строку", "Номер строки не соответствует не одному номеру строки в редакторе");
        }
    }
}


void MainWindow::on_Menu_Select_All_triggered() //Меню Выделить Всё
{
    ui->textEdit->selectAll();
}


void MainWindow::on_Menu_DateTime_triggered() //Меню Дата
{
    ui->textEdit->insertPlainText(QDateTime::currentDateTime().toString("HH:mm dd.MM.yyyy"));
}
