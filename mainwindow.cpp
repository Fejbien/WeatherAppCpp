#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qjsondocument.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    apiClient = new ApiClient(this);
    connect(apiClient, &ApiClient::statusUpdated, this, &MainWindow::updateStatus);
    connect(apiClient, &ApiClient::dataRecived, this, &MainWindow::dataRecived);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete apiClient;
}

void MainWindow::on_citySearch_clicked()
{
    QString city = ui->cityInput->toPlainText();

    apiClient->makeRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/data/getData/3497"));
}

void MainWindow::updateStatus(const QString &status){
    ui->resultBrowser->setText(status);
}

void MainWindow::dataRecived(const QJsonObject &data){
    QJsonDocument document(data);
    QByteArray byteArray = document.toJson();
    ui->resultBrowser->setText(QString(byteArray));
}
