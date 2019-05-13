#include "MainWindow.h"
#include <QTcpServer>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QTcpSocket>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new(Ui::MainWindow))
    , m_tcpServer(new QTcpServer(this))
{
    ui->setupUi(this);


    //if listen got no paragram, serverSocket'll listen on all network interfaces
    if (!m_tcpServer->listen(QHostAddress(QHostAddress::LocalHost), 46666)) {
        QMessageBox::critical(this, tr("Fortune Server"),
            tr("Unable to start the server: %1.")
            .arg(m_tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;

    ipAddress = m_tcpServer->serverAddress().toString();
   
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    
    ui->statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
        "Run the Fortune Client example now.")
        .arg(ipAddress).arg(m_tcpServer->serverPort()));


    connect(m_tcpServer, &QTcpServer::newConnection, this, &MainWindow::receiveNewConnection);
}

void MainWindow::receiveNewConnection()
{
    static int connectTime = 0;
    ui->connection->setText(QString::number(++connectTime));
    if (m_tcpServer->hasPendingConnections()) {
        QTcpSocket *client = m_tcpServer->nextPendingConnection();
        connect(client, &QTcpSocket::readyRead, this, &MainWindow::readString);
    }
}

void MainWindow::readString()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    QByteArray recvString = client->readAll();
    ui->recvLabel->setText(recvString);
    client->write(recvString);

}
