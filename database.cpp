#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();

    //simpleQuery = new QSqlQuery();

    tableWidget = new QTableWidget();

    //tableModel = new QSqlTableModel();
    tableView = new QTableView();
    //tableView->setModel(tableModel);

    queryModel = new QSqlQueryModel();
    headers = new QStringList();

    msg = new QMessageBox();
}

DataBase::~DataBase()
{
    delete dataBase;
    //delete simpleQuery;
    delete queryModel;
    delete tableView;
    delete headers;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
    tableModel = new QSqlTableModel(this, *dataBase);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{

    ///Тут должен быть код ДЗ
    queryModel->setQuery(request, *dataBase);
    tableModel->setTable("film");
    tableModel->select();
    //tableModel->setHeaderData(0, Qt::Horizontal, tr("Название"));
    //tableModel->setHeaderData(1, Qt::Horizontal, tr("Год выпуска"));
    //tableModel->setHeaderData(2, Qt::Horizontal, tr("Жанр"));
    tableView->setModel(tableModel);
    for (int i = 3; i < tableModel->columnCount(); i++)
    {
        tableView->hideColumn(i);
    }
    tableView->show();

    //tableModel->select();
    //tableView->setModel(tableModel);
    /*
    for (uint32_t i = 0, j = 0; i < queryModel->columnCount(); i++, j++)
    {
        queryModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
    */
    QSqlError err = dataBase->lastError();
    qDebug() << err.text();
    //msg->setIcon(QMessageBox::Critical);
    //msg->setText(err.text());
    //msg->show();
    //msg->exec();
    emit sig_SendStatusRequest(err);

    /*
    if (simpleQuery->exec(request) == false)
    {
        err = simpleQuery->lastError();
    }
    */
    //emit sig_SendStatusRequest(err);

}


void DataBase::ReadAnswerFromDB(int requestType)
{
    /*
     * Используем оператор switch для разделения запросов
    */
    QStringList hdrs;
    //QString str;
    QVariantList vlist;
    uint32_t conterRows = 0;

    switch (requestType) {
    //Для наших запросов вид таблицы не поменяетя. Поэтому будет единый обработчик.
    case requestAllFilms:
        //tableModel-
        //tableModel->setHeaderData(0, Qt::Horizontal, "Название");
        //tableModel->setHeaderData(1, Qt::Horizontal, "Год выпуска");
        //tableModel->setHeaderData(2, Qt::Horizontal, "Жанр");

        //queryModel->


        /*
        tableWidget->setColumnCount(3);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Год выпуска" << "Жанр";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);
            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }
        */
        break;
    case requestComedy:
        /*
        tableWidget->setColumnCount(2);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Описание";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);
            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }
        */
        break;
    case requestHorrors:
    {
        /*
         * Объект tableWidget представляет из себя таблицу, которую можно отобразить
         * в ПИ. Зfдадим ее параметры и заголовки. Мы получаем из БД название фильма, год выпуска и жанр
        */

        /* QSqlQuery
         * После запроса к БД ответ помещается в объект QSqlQuery.
         * Его необходимо считвать построчно. Для чтения ответа используется метод
         * next, он работает следующим образом: при вызове метода указатель перемещается
         * на следующую запись, в ответе. После получения ответа он указывает, если можно так
         * выразится на -1 запись. Т.е. при первом вызове метода next указатель переместится
         * на первую запись, а сам метод вернет true. В случае если указатель переместится
         * за диапазон записей метод вернет false. Также он вернет false в случае отсутствия
         * записей.
         * Каждая запись является стройкой прочитанной из БД. Для перемещения по столбцам
         * используется метод QSqlQuery value(индекс столбца).
        */
        /* QTableWidget
         * Необходимо сформировать таблицу, которую потом передадим в MainWindow для отображения
         * Для каждой записи прочитанной при помощи метода next будем добавлять строку в tableWidget
         * и заполнять значениями из ответа. Каждая ячейка таблицы является объектом класса
         * QTableWidgetItem. Т.е. для заполнения каждой ячеки мы должны создать экземпляр этого метода.
         * Значение ячейки в таблицк устанавливается при помощи метода setItem.
        */
        /*
         * После заполнения всей таблицы мы ее передаем в MainWindow при помощи сигнала.
        */

        /*
        tableWidget->setColumnCount(2);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Описание";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);

            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }
        */

        break;
    }

    default:
        break;
    }

    /*
    tableView->setModel(tableModel);
    tableModel->select();
    tableView->show();
*/

    emit sig_SendDataFromDB(tableView, requestType);

}

/*
void DataBase::ReadAnswerFromDB(int requestType)
{
    QStringList hdrs;
    QString str;
    uint32_t conterRows = 0;

    switch (requestType) {
    //Для наших запросов вид таблицы не поменяетя. Поэтому будет единый обработчик.
    case requestAllFilms:
        tableWidget->setColumnCount(3);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Год выпуска" << "Жанр";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);
            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }
        break;
    case requestComedy:
        tableView->setColumn
        tableWidget->setColumnCount(2);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Описание";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);
            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }
        break;
    case requestHorrors:
    {
        tableWidget->setColumnCount(2);
        tableWidget->setRowCount(0);
        hdrs << "Название" << "Описание";
        tableWidget->setHorizontalHeaderLabels(hdrs);
        while(simpleQuery->next()){
            tableWidget->insertRow(conterRows);

            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(str);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }

        //emit sig_SendDataFromDB(tableWidget, requestType);

        break;
    }

    default:
        break;
    }

    emit sig_SendDataFromDB(tableWidget, requestType);

}
*/

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
