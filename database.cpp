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
    QSqlError err = dataBase->lastError();
    emit sig_SendStatusRequest(err, tableModel);

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
        break;
    case requestComedy:
        break;
    case requestHorrors:
        break;

    default:
        break;
    }
    emit sig_SendDataFromDB(tableView, requestType);

}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
