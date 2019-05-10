#include "MainWindow.h"
#include <QTcpServer>
#include <QNetworkInterface>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new(Ui::MainWindow))
{
    ui->setupUi(this);

    m_tcpServer = new QTcpServer(this);

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
    //QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    //// use the first non-localhost IPv4 address
    //for (int i = 0; i < ipAddressesList.size(); ++i) {
    //    if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
    //        ipAddressesList.at(i).toIPv4Address()) {
    //        ipAddress = ipAddressesList.at(i).toString();
    //        break;
    //    }
    //}
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
    qDebug() << "new connection!";
}
