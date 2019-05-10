#include "MainWindow.h"
#include <QTcpServer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new(Ui::MainWindow))
{
    ui->setupUi(this);

    m_socket = new QTcpServer(this);
}
