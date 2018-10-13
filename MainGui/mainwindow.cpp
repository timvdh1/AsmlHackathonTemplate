#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpManager = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked()
{
    QString ipAddress = ui->editIP->text();
  /*  QUrl url("http://10.134.40.1");
    //QUrl url("http://10.131.221.101");

    QUrlQuery params;
    params.addQueryItem("command_type", QString(1));
    params.addQueryItem("shape_type", QString(ui->comboShape->currentIndex()));

    url.setQuery(params.query());

    QNetworkRequest request(url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    //request.setRawHeader("X-Parse-Application-Id")
     QNetworkReply *reply = mpManager->get(request);


     connect(mpManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished()) );
*/


     QString shapeStr = QString::number(ui->comboShape->currentIndex());
     QUrl urlPost("http://"+ipAddress+"/?json=a&shape_type="+shapeStr);
     QNetworkRequest requestPost(urlPost);
     QByteArray jsonDocument("json={\"jsonrpc\": \"2.0\", \"command_type\": \"1\", \"shape_type\": \"2\"");
     requestPost.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

     //QNetworkReply *replyPost = mpManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
     QNetworkReply *replyPost = mpManager->post(requestPost,   jsonDocument);
}

void MainWindow::slotReadyRead()
{

}
void MainWindow::requestFinished()
{

}
