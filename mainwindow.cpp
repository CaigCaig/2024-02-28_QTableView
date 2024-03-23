#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    tableView = new QTableView();

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);
     connect(dataBase, &DataBase::sig_SendDataFromDB_TM, this, &MainWindow::ScreenDataFromDB_TM);
     connect(dataBase, &DataBase::sig_SendDataFromDB_QM, this, &MainWindow::ScreenDataFromDB_QM);

    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);

}

MainWindow::~MainWindow()
{
    delete tableView;
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");


       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{

    ///Тут должен быть код ДЗ

    auto i = ui->cb_category->currentIndex();
    switch (i)
    {
    case 0:
        request = request_all;
        break;
    case 1:
        request = request_comedy;
        break;
    case 2:
        request = request_horror;
        break;
    }

    auto req = [&]{dataBase->RequestToDB(request);};
    //QtConcurrent::run(req);
    dataBase->RequestToDB(request);
    //ui->tv_result->setModel();

}

/*!
 * \brief Слот отображает значение в QTableView
 * \param view
 * \param typeRequest
 */

void MainWindow::ScreenDataFromDB_TM(QSqlTableModel *tableModel)
{
    tableView->setModel(tableModel);
    tableView->model()->submit();
    ui->tv_result = tableView;
}
void MainWindow::ScreenDataFromDB_QM(QSqlQueryModel *queryModel)
{
    tableView->setModel(queryModel);
    tableView->model()->submit();
    ui->tv_result = tableView;
}


void MainWindow::ScreenDataFromDB(QTableView *view, int typeRequest)
{

    ///Тут должен быть код ДЗ
    if (view->model() != nullptr)
    {
        view->model()->submit();
        switch (typeRequest)
        {
        case requestAllFilms:
            //QList<QString> hdrs;
            //for(int i = 0, j = 0; i < view->model()->columnCount(); i++, j++){
            //    view->model()->setHeaderData(i, Qt::Horizontal, headers[j])
                //hdrs << view->model()horizontalHeaderItem(i)->text();
            //}
            ui->tv_result = view;
            break;
        case requestHorrors:
            break;
        case requestComedy:


            /*
            ui->tb_result->setRowCount(widget->rowCount( ));
            ui->tb_result->setColumnCount(widget->columnCount( ));
            QStringList hdrs;
            for(int i = 0; i < widget->columnCount(); ++i){
                hdrs << widget->horizontalHeaderItem(i)->text();
            }
            ui->tb_result->setHorizontalHeaderLabels(hdrs);

            for(int i = 0; i<widget->rowCount(); ++i){
                for(int j = 0; j<widget->columnCount(); ++j){
                    ui->tb_result->setItem(i,j, widget->item(i,j)->clone());
                }
            }

            ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            */



            break;
        default:
            break;
        }
    }
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */

/*
void MainWindow::ScreenDataFromDB(const QTableWidget *widget, int typeRequest)
{

    ///Тут должен быть код ДЗ
    switch (typeRequest) {

    case requestAllFilms:
    case requestHorrors:
    case requestComedy:{

        ui->tb_result->setRowCount(widget->rowCount( ));
        ui->tb_result->setColumnCount(widget->columnCount( ));
        QStringList hdrs;
        for(int i = 0; i < widget->columnCount(); ++i){
            hdrs << widget->horizontalHeaderItem(i)->text();
        }
        ui->tb_result->setHorizontalHeaderLabels(hdrs);

        for(int i = 0; i<widget->rowCount(); ++i){
            for(int j = 0; j<widget->columnCount(); ++j){
                ui->tb_result->setItem(i,j, widget->item(i,j)->clone());
            }
        }

        ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        break;

    }
    default:
        break;
    }
}
*/

/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
//void MainWindow::ReceiveStatusRequestToDB(QSqlError err, QSqlTableModel *tableModel)
{
    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else
    {

        auto i = ui->cb_category->currentIndex();
        switch (i)
        {
        case 0:
            request_type = requestAllFilms;
            break;
        case 1:
            request_type = requestComedy;
            break;
        case 2:
            request_type = requestHorrors;
            break;
        }
        dataBase->ReadAnswerFromDB(request_type);

        /*
        tableModel->clear();
        tableModel->setTable("film");
        tableModel->select();
        tableModel->removeColumns(0, 1);
        tableModel->removeColumns(2, 11);
        tableModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Жанр"));
        ui->tv_result->setModel(tableModel);
        */

        /*
        queryModel->clear();
        queryModel->setTable("film");
        queryModel->select();
        queryModel->removeColumns(0, 1);
        queryModel->removeColumns(2, 11);
        queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        queryModel->removeColumns(2, 1);
        queryModel->setHeaderData(2, Qt::Horizontal, tr("Жанр"));
        ui->tv_result->setModel(queryModel);
        */

        //tableView.setModel(tableModel);

    }
}

/*
логику работы с tableModel нужно перенести в DataBase! И там в зависимости от выбранного запроса или запрашивать и передавать в главное окно указатель на tableModel или делать тоже самое но с queryModel.
*/


void MainWindow::on_pb_clear_clicked()
{
    //ui->tv_result->;
}

