#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class QTcpServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindow *ui;

    QTcpServer *m_socket;
};
