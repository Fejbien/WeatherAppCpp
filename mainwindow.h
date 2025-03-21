#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./apiClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ApiClient *apiClient;

private slots:
    void on_citySearch_clicked();

private:
    Ui::MainWindow *ui;
    void updateStatus(const QString &status);
    void dataRecived(const QJsonObject &data);
};
#endif // MAINWINDOW_H
