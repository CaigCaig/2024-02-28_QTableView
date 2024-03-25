#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();

    queryModel = new QSqlQueryModel();

    msg = new QMessageBox();
}

DataBase::~DataBase()
{
    delete dataBase;
    delete queryModel;
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
    emit sig_SendStatusRequest(err);
}


void DataBase::ReadAnswerFromDB(int requestType)
{
    /*
     * Используем оператор switch для разделения запросов
    */

    switch (requestType) {
    //Для наших запросов вид таблицы не поменяетя. Поэтому будет единый обработчик.
    case requestAllFilms:
        tableModel->clear();
        tableModel->setTable("film");
        tableModel->select();
        tableModel->removeColumns(0, 1);
        tableModel->removeColumns(2, 11);
        tableModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Жанр"));
        emit sig_SendDataFromDB_TM(tableModel);
        break;
    case requestComedy:
        //queryModel->clear();
        //queryModel->setTable("film");
        //queryModel->select();
        //queryModel->removeColumns(0, 1);
        //queryModel->removeColumns(2, 11);
        queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDB_QM(queryModel);
        break;
    case requestHorrors:
        queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDB_QM(queryModel);
        break;

    default:
        break;
    }
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
