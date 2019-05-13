#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QScopedPointer>
#include <QAbstractSocket>
#include <QDataStream>

class QTcpSocket;
//class QIODevice;
//class QDataStream;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
    void on_connect_clicked();
    void readFortune();
    void dealError(QAbstractSocket::SocketError);
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_socket;
    QScopedPointer<QDataStream> m_stream;
};
