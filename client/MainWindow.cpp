#include "MainWindow.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QIODevice>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(new QTcpSocket(this))
    , m_stream(new QDataStream)

{
    ui->setupUi(this);

    
    m_socket->bind();
    m_stream->setDevice(m_socket);

    connect(m_socket, &QIODevice::readyRead, this, &MainWindow::readFortune);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::dealError);
}

void MainWindow::on_connect_clicked()
{
   QString hostName = ui->IP->text();
   quint16 port = ui->PORT->text().toInt();

   m_socket->abort();
   m_socket->connectToHost(hostName, port);

   if (m_socket->waitForConnected(2000)) {
       QMessageBox::information(this, "warning", "connect sucess!");
   }
   else {
       QMessageBox::information(this, "warning", "connect failed!");
   }
}

void MainWindow::readFortune()
{
    m_stream->startTransaction();

    QString nextFortune;
    *m_stream >> nextFortune;

    if (!m_stream->commitTransaction())
        return;

    ui->Text->setText(nextFortune);
}

void MainWindow::dealError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
            tr("The host was not found. Please check the "
                "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
            tr("The connection was refused by the peer. "
                "Make sure the fortune server is running, "
                "and check that the host name and port "
                "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
            tr("The following error occurred: %1.")
            .arg(m_socket->errorString()));
    }
}

void MainWindow::on_sendButton_clicked()
{
    QString text = ui->sendEdit->text();
    m_socket->write(text.toLatin1());
 //   m_stream->startTransaction();

    *m_stream << text;

 //   if (!m_stream->commitTransaction())
 //       return;
}
