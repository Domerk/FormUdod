#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ------------------------- Всякая красота ----------------------------

    this->setWindowTitle(tr("Запись в объединения"));

    // --------------------------- Main ToolBar ----------------------------

    // Иконки: http://www.flaticon.com/packs/web-application-ui/4

    ui->mainToolBar->addAction(QIcon(":/icons/Icons/save.png"), tr("Сохранить"), this, SLOT(saveInfo()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/clear.png"), tr("Очистить форму"), this, SLOT(clearForm()));
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/help.png"), tr("Помощь"), this, SLOT(help()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/info.png"), tr("О программе"), this, SLOT(programInfo()));

    ui->toolBar->addAction(QIcon(":/icons/Icons/db.png"), tr("Настройки соединения"), this, SLOT(changeConfig()));

    names = new QRegularExpression("^[А-ЯЁ]{1}[а-яё]*(-[А-ЯЁ]{1}[а-яё]*)?$");
    words = new QRegularExpression();

    dialog = new ConnectionDialog();
    //connect (dialog, SIGNAL(connectReconfig()), this, SLOT(configIsChange()));
    connect (dialog, SIGNAL(connectReconfig()), this, SLOT(connectDB()));

    // Получение инфы обобъединениях

    QFile file (":/data/Other/association.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        if(stream.status() == QTextStream::Ok)
        {
            QStringList qsl;
            while (!stream.atEnd())
            {
                QString str = stream.readLine().simplified().replace(QRegularExpression("-{2,}"), "-");
                if (!str.isEmpty() && str.at(0) != '#')
                    qsl.append(str);
            }

            qsl.sort();
            qsl.prepend("- Не выбрано -");
            ui->ass1->addItems(qsl);
            ui->ass2->addItems(qsl);
            ui->ass3->addItems(qsl);
        }
        file.close();
    }


    // ----------------------------- DataBase ------------------------------

    // Временная строчка - указан путь к базе.
    // D:/Domerk/GUAP/Diplom/kcttTempDB.sqlite
    if (!connectDB())
    {
        // просим указать параметры соединения
        changeConfig();
    }

}

MainWindow::~MainWindow()
{
    if (myDB.isOpen())
        myDB.close();

    delete names;
    delete words;
    delete dialog;
    delete ui;
}

// ============================================================
// ============== Установка соединения с базой ================
// ============================================================

bool MainWindow::connectDB()
{
    if (myDB.isOpen())
    {
        myDB.close();
    }
    else
    {
        myDB = QSqlDatabase::addDatabase("QSQLITE");    // Указываем СУБД
    }

    QSettings settings ("Kctt", "KcttTempDB");
    myDB.setHostName(settings.value("hostname", "localhost").toString());
    myDB.setDatabaseName(settings.value("dbname", "kcttTempDB").toString());
    myDB.setUserName(settings.value("username").toString());
    myDB.setPassword(settings.value("password").toString());

    if (myDB.open() && !myDB.isOpenError() && myDB.isValid())                            // Открываем соединение
    {
        ui->lblStatus->setText(tr("Соединение установлено")); // Выводим сообщение
        return true;                 // Возвращаем true
    }
    else
    {
        ui->lblStatus->setText(tr("Ошибка соединения: соединение не установлено"));
    }

    return false;
}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::saveInfo()
{
    if (myDB.isOpen())
    {
        // Проверяем заполнение обязательных полей

        QString surname = ui->surname->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString name = ui->name->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString docNum = ui->docNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString schoolNum = ui->schoolNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString phone = ui->phone->text().simplified().replace(QRegularExpression("-{2,}"), "-");

        QString docType = ui->docType->currentText();
        QString gender = ui->gender->currentText();

        if (surname == "" || name == "" || docNum == "" || schoolNum == "" || phone == "" || docType == "" || gender == "" || (ui->ass1->currentIndex() == 0 && ui->ass2->currentIndex() == 0 && ui->ass3->currentIndex() == 0))
        {
            // Если одно или несколько обязательных полей не заполнены
            // Сообщаем об этом пользователю
            QMessageBox messageBox(QMessageBox::Information,
                                   tr("Сохранение"),
                                   tr("Ошибка сохранения: Не заполнено одно или несколько обязательных полей!"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
            messageBox.exec();
            // Выходим
            return;
        }

        QString patrname = ui->patrname->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString schoolArea = ui->schoolArea->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString classNum = ui->classNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString mail = ui->mail->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString parents = ui->parents->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString address = ui->address->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");

        QString birthday;

        if (ui->bday->currentIndex() != 0 && ui->bmon->currentIndex() != 0)
            birthday = ui->bday->currentText() + "." + ui->bmon->currentText() + "." + QString::number(ui->byear->value());

        bool isName = true;
        bool isSurname = true;
        bool isPatrName = true;

        QRegularExpressionMatch match = names->match(name);
        isName = match.hasMatch();

        match = names->match(surname);
        isSurname = match.hasMatch();

        if (!patrname.isEmpty())
        {
            match = names->match(patrname);
            isPatrName = match.hasMatch();
        }

        if (!isName || !isSurname || !isPatrName)
        {
            QMessageBox messageBox(QMessageBox::Information,
                                   tr("Сохранение"),
                                   tr("Ошибка сохранения: Одно или несколько полей заполнены неверно!"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
            messageBox.exec();
            return;
        }


        // Создаём окно, запрашивающее подтверждение действия
        QMessageBox messageBox(QMessageBox::Question,
                    tr("Сохранение"),
                    tr("Сохранить введённые данные?"),
                    QMessageBox::Yes | QMessageBox::No,
                    this);

        messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
        messageBox.setButtonText(QMessageBox::No, tr("Нет"));

        // Если действие подтверждено
        if (messageBox.exec() == QMessageBox::Yes)
        {
            QString strQuery = "INSERT INTO Учащийся (";
            strQuery.append("'Фамилия', 'Имя', 'Отчество', 'Тип документа', 'Номер документа', 'Пол', 'Дата рождения', ");
            strQuery.append("'Район школы', 'Школа', 'Класс', 'Родители', 'Домашний адрес', 'Телефон', 'e-mail') VALUES ('");
            strQuery.append(surname + "', '" + name  + "', '" + patrname  + "', '" + docType  + "', '" + docNum  + "', '" + gender  + "', '" + birthday  + "', '");
            strQuery.append(schoolArea  + "', '" + schoolNum  + "', '" + classNum  + "', '" + parents  + "', '" + address  + "', '" + phone  + "', '" + mail  + "');");

            QSqlQuery query;
            query.exec(strQuery);
            strQuery.clear();

            QStringList qsl;
            getDataAss(&qsl, ui->ass1);
            getDataAss(&qsl, ui->ass2);
            getDataAss(&qsl, ui->ass3);
            for (QString & newCours : qsl)
            {
                strQuery.append("INSERT INTO Запись ('Тип документа', 'Номер документа', 'Объединение') VALUES ('");
                strQuery.append(docType  + "', '" + docNum  + "', '" + newCours + "');");
                query.exec(strQuery);
                strQuery.clear();
            }

            cleaner();
        }

    }
    else
    {
        // Выводим сообщение с инфой
        // Вероятно, имеет смысл иметь возможность вызвать отсюда окна с настройками соединения


        ui->lblStatus->setText(tr("Ошибка сохранения: отсутсвует соединение с базой"));
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Сохранение"),
                               tr("Сохранение невозможно: отсутсвует соединение с базой."),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
        messageBox.exec();
    }

}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::clearForm()
{
    // Создаём окно, запрашивающее подтверждение действия
    QMessageBox messageBox(QMessageBox::Question,
                tr("Очистка формы"),
                tr("Вы действительно хотите очистить форму? <br /> Все данные будут потеряны!"),
                QMessageBox::Yes | QMessageBox::No,
                this);

    messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
    messageBox.setButtonText(QMessageBox::No, tr("Нет"));

    // Если действие подтверждено
    if (messageBox.exec() == QMessageBox::Yes)
    {
        cleaner();
    }

}

void MainWindow::getDataAss(QStringList* qsl, QComboBox* comboBox)
{
    if (comboBox->currentIndex() != 0)
        qsl->append(comboBox->currentText());
}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::cleaner()
{
    ui->surname->clear();
    ui->name->clear();
    ui->patrname->clear();
    ui->docNum->clear();
    ui->schoolArea->clear();
    ui->schoolNum->clear();
    ui->classNum->clear();
    ui->phone->clear();
    ui->mail->clear();

    ui->parents->clear();
    ui->address->clear();

    ui->docType->setCurrentIndex(0);
    ui->gender->setCurrentIndex(0);
    ui->bday->setCurrentIndex(0);
    ui->bmon->setCurrentIndex(0);

    ui->byear->clear();

    ui->ass1->setCurrentIndex(0);
    ui->ass2->setCurrentIndex(0);
    ui->ass3->setCurrentIndex(0);
}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::help()
{

}

void MainWindow::programInfo()
{

}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::changeConfig()
{
    dialog->exec();
}

