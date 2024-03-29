#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Windows.h>
#include <QStringListModel>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <smarthandle.h>
#include <listprocces.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listProcess_clicked(const QModelIndex &index);

    void on_pushButtonSetPriority_clicked();

    void on_listThreads_clicked(const QModelIndex &index);

    void on_pushButtonUpdateThreadList_clicked();

private:
    Ui::MainWindow *ui;
    ListProcces *modelProcess;
    ListProcces *modelThreads;
    QStringListModel *modelPriority;
    QStringListModel* emptyModel;
    int processNumber;
};
#endif // MAINWINDOW_H
