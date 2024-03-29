#ifndef LISTPROCCES_H
#define LISTPROCCES_H
#include <Windows.h>
#include <TlHelp32.h>
#include <QStringListModel>
#include <smarthandle.h>
#include <QMessageBox>
#include <QStringList>
#include <string>
#include <sstream>
#include <QTimer>

class ListProcces : public QStringListModel
{
public:

    explicit ListProcces(QObject *parent = 0);
    QString convert (const WCHAR* wc);
    QString convertDW (const unsigned long int dw);
    std::vector<QStringList> ListThreads;
    QString getPriority(int processNumber, int threadNumber);
    void setPriorty(int processNumber, int threadNumber, QString priority);
    void timerUpdate();
//    void clearData(QStringList SL);

private:
    std::vector<ProcessInfo> processInfos;
    std::vector<THREADENTRY32> threads;
    int PIsize;
    int Tsize;
    QStringList ListProcess;
    QTimer* timer;
};

#endif // LISTPROCCES_H
