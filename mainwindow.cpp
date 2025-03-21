#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qjsondocument.h>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

QMap<QString, int> idMap;

QStringList loadCityData(const QString &filePath) {
    QStringList cityList;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return cityList;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        qWarning() << "Invalid JSON format.";
        return cityList;
    }

    QJsonArray jsonArray = doc.array();
    for (const QJsonValue &value : jsonArray) {
        if (!value.isObject()) continue;
        QJsonObject obj = value.toObject();

        int id = obj["id"].toInt();
        QString city = obj["city"].toString();
        QString district = obj["district"].toString();
        QString province = obj["province"].toString();
        QString stationStreet = obj["station_street"].toString();

        QString displayText = QString("%1, %2, %3, %4").arg(city, district, province, stationStreet);
        cityList.append(displayText);
        idMap[displayText] = id;
    }

    return cityList;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // API
    apiClient = new ApiClient(this);
    connect(apiClient, &ApiClient::statusUpdated, this, &MainWindow::updateStatus);
    connect(apiClient, &ApiClient::dataRecived, this, &MainWindow::dataRecived);

    // Search input
    QStringList cityList = loadCityData(":/citySearchData.json");
    QCompleter *completer = new QCompleter(cityList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->cityInput->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete apiClient;
}

void MainWindow::on_citySearch_clicked()
{
    QString city = ui->cityInput->text();
    int id = idMap[city];

    if(id == 0){
        ui->resultBrowser->setText("Try selecting full name!");
        return;
    }

    apiClient->makeRequest(QUrl(QString("https://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/%1").arg(id)));
}

void MainWindow::updateStatus(const QString &status){
    ui->resultBrowser->setText(status);
}

void MainWindow::dataRecived(const QJsonObject &data){
    QJsonDocument document(data);
    QByteArray byteArray = document.toJson();
    ui->resultBrowser->setText(QString(byteArray));
}
