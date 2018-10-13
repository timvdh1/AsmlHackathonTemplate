#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSend_clicked();
    void slotReadyRead();
    void requestFinished();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *mpManager;

};

#endif // MAINWINDOW_H
