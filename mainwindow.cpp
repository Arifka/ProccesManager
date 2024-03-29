#include <Windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "smarthandle.h"
#include <string>
#include <QString>
#include <QChar>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QStringList listPriority;
    modelPriority = new QStringListModel(this);
    listPriority << "Background" << "Lowest" << "Below normal" << "Normal" << "Above normal" << "Highest";
    modelPriority->setStringList(listPriority);
    ui->list_priority->setModel(modelPriority);

    modelProcess = new ListProcces(this);
    modelThreads = new ListProcces(this);

    ui->listProcess->setModel(modelProcess);

    QStringList emptyList;
    emptyModel = new QStringListModel(this);
    emptyModel->setStringList(emptyList);
    //ui->listThreads->setModel(modelThreads);
    //ui->listProcess->setEditTriggers(QAbstractItemView::AnyKeyPressed);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listProcess_clicked(const QModelIndex &)
{
    processNumber = ui->listProcess->currentIndex().row();
    qDebug() << processNumber;
    modelThreads->setStringList(modelProcess->ListThreads[processNumber]);
    ui->listThreads->setModel(modelThreads);
}

void MainWindow::on_pushButtonSetPriority_clicked()
{
    QString selectedPriority = ui->list_priority->itemText(ui->list_priority->currentIndex());
    if (ui->line_priority->text() != selectedPriority) {
        modelProcess->setPriorty(ui->listProcess->currentIndex().row(), ui->listThreads->currentIndex().row(), selectedPriority);
    }
}

void MainWindow::on_listThreads_clicked(const QModelIndex &)
{
    QString buf = modelProcess->getPriority(ui->listProcess->currentIndex().row(), ui->listThreads->currentIndex().row());
    ui->line_priority->setText(buf);
}

void MainWindow::on_pushButtonUpdateThreadList_clicked()
{
    modelProcess->timerUpdate();
    ui->listProcess->setModel(modelProcess);
    ui->listThreads->setModel(emptyModel);

}
