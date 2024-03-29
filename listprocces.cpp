#include "listprocces.h"


ListProcces::ListProcces(QObject *parent) : QStringListModel(parent)
{
    SmartHandle processSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    SmartHandle threadSnap(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));
    if (!processSnap || !threadSnap) qDebug() << 1;;

    THREADENTRY32 te{ sizeof(THREADENTRY32) };
    if (Thread32First(threadSnap, &te) == FALSE) qDebug() << 2;
    do {
        threads.push_back(te);
    } while (Thread32Next(threadSnap, &te));

    PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
    if (Process32First(processSnap, &pe) == FALSE) qDebug() << 3;

    do {
        std::vector<THREADENTRY32> subThreads;
        for (const auto &thread : threads)
        {
            if (thread.th32OwnerProcessID == pe.th32ProcessID)
                subThreads.push_back(thread);
        }
        processInfos.push_back(ProcessInfo{ pe, subThreads });
    } while (Process32Next(processSnap, &pe));

    for (size_t i = 0;i < processInfos.size(); i++) {
        ProcessInfo processInfo = processInfos[i];
        QString buffProcess = convert(processInfo.pe.szExeFile);
        ListProcess << buffProcess;
        QStringList j;
        ListThreads.push_back(j);
        for (size_t k = 0;k < processInfo.threads.size(); k++) {
            THREADENTRY32 thread = processInfo.threads[k];
            std::string buf = std::to_string(thread.th32ThreadID);
            QString buffThreads = QString::fromStdString(buf);
            ListThreads[i] << buffThreads;
        }
    }

    this->setStringList(ListProcess);


//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &ListProcces::timerUpdate);
////        timer->start(1000 * 1);
}


QString ListProcces::convert(const WCHAR *wc)
{
    char* c = new char[wcslen(wc)];
    size_t len = wcstombs(c, wc, wcslen(wc));
    if (len > 0u) c[len] = '\0';
    return QString(c);
}

QString ListProcces::convertDW(const unsigned long int dw)
{
    std::string str = std::to_string(dw);
    return  QString::fromUtf8(str.data(), str.size());
}



QString ListProcces::getPriority(int processNumber, int threadNumber)
{
    HANDLE currProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processInfos[processNumber].pe.th32ProcessID);
    HANDLE currThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, processInfos[processNumber].threads[threadNumber].th32ThreadID);


    DWORD dwProcessClass = GetPriorityClass(currProcess);
    DWORD dwThreadPriority = GetThreadPriority(currThread);


    if (dwProcessClass & REALTIME_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 16: return QString("Background");
        case 22: return QString("Lowest");
        case 23: return QString("Below normal");
        case 24: return QString("Normal");
        case 25: return QString("Above normal");
        case 26: return QString("Highest");
        case 31: return QString("Time critical");
        default: return QString("Error");
        }
    }
    if (dwProcessClass & HIGH_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 1: return QString("Background");
        case 11: return QString("Lowest");
        case 12: return QString("Below normal");
        case 13: return QString("Normal");
        case 14: return QString("Above normal");
        case 15: return QString("Highest");
        default: return QString("Error");
        }
    }
    if (dwProcessClass & NORMAL_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 1: return QString("Background");
        case 6: return QString("Lowest");
        case 7: return QString("Below normal");
        case 8: return QString("Normal");
        case 9: return QString("Above normal");
        case 10: return QString("Highest");
        case 15: return QString("Time critical");
        default: return QString("Error");
        }
    }
    if (dwProcessClass & IDLE_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 1: return QString("Background");
        case 2: return QString("Lowest");
        case 3: return QString("Below normal");
        case 4: return QString("Normal");
        case 5: return QString("Above normal");
        case 6: return QString("Highest");
        case 15: return QString("Time critical");
        default: return QString("Error");
        }
    }
    if (dwProcessClass & BELOW_NORMAL_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 1: return QString("Background");
        case 4: return QString("Lowest");
        case 5: return QString("Below normal");
        case 6: return QString("Normal");
        case 7: return QString("Above normal");
        case 8: return QString("Highest");
        case 15: return QString("Time critical");
        default: return QString("Error");
        }
    }
    if (dwProcessClass & ABOVE_NORMAL_PRIORITY_CLASS) {
        switch (processInfos[processNumber].threads[threadNumber].tpBasePri) {
        case 1: return QString("Background");
        case 8: return QString("Lowest");
        case 9: return QString("Below normal");
        case 10: return QString("Normal");
        case 11: return QString("Above normal");
        case 12: return QString("Highest");
        case 15: return QString("Time critical");
        default: return QString("Error");
        }
    }
    else return QString("No access");
    CloseHandle(currProcess);
    CloseHandle(currThread);
//    qDebug() << convertDW(GetPriorityClass(currProcess));
//    QString a = convertDW(GetPriorityClass(currProcess)) + "   " + convertDW(GetThreadPriority(currThread));
//    return a;
//    QString b = QString::number(processInfos[processNumber].threads[threadNumber].tpBasePri) + "  " + QString::number(processInfos[processNumber].pe.pcPriClassBase);
    //    return b;
}

void ListProcces::setPriorty(int processNumber, int threadNumber, QString priority)
{
    HANDLE currThread = OpenThread(THREAD_SET_INFORMATION, FALSE, processInfos[processNumber].threads[threadNumber].th32ThreadID);
    qDebug() << 0;
    if (priority == "Background") {
        SetThreadPriority(currThread, THREAD_PRIORITY_IDLE);
        qDebug() << 3;
    }
    if (priority == "Lowest") SetThreadPriority(currThread, THREAD_PRIORITY_LOWEST);
    if (priority == "Below normal") SetThreadPriority(currThread, THREAD_PRIORITY_BELOW_NORMAL);
    if (priority == "Normal") SetThreadPriority(currThread, THREAD_PRIORITY_NORMAL);
    if (priority == "Above normal") SetThreadPriority(currThread, THREAD_PRIORITY_ABOVE_NORMAL);
    if (priority == "Highest") SetThreadPriority(currThread, THREAD_PRIORITY_HIGHEST);
    CloseHandle(currThread);
}

void ListProcces::timerUpdate()
{
    processInfos.resize(0);
    threads.resize(0);
    ListThreads.resize(0);
    ListProcess.clear();

    SmartHandle processSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    SmartHandle threadSnap(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));
    if (!processSnap || !threadSnap) qDebug() << 1;;

    THREADENTRY32 te{ sizeof(THREADENTRY32) };
    if (Thread32First(threadSnap, &te) == FALSE) qDebug() << 2;
    do {
        threads.push_back(te);
    } while (Thread32Next(threadSnap, &te));

    PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
    if (Process32First(processSnap, &pe) == FALSE) qDebug() << 3;

    do {
        std::vector<THREADENTRY32> subThreads;
        for (const auto &thread : threads)
        {
            if (thread.th32OwnerProcessID == pe.th32ProcessID)
                subThreads.push_back(thread);
        }
        processInfos.push_back(ProcessInfo{ pe, subThreads });
    } while (Process32Next(processSnap, &pe));

    for (size_t i = 0;i < processInfos.size(); i++) {
        ProcessInfo processInfo = processInfos[i];
        QString buffProcess = convert(processInfo.pe.szExeFile);
        ListProcess << buffProcess;
        QStringList j;
        ListThreads.push_back(j);
        for (size_t k = 0;k < processInfo.threads.size(); k++) {
            THREADENTRY32 thread = processInfo.threads[k];
            std::string buf = std::to_string(thread.th32ThreadID);
            QString buffThreads = QString::fromStdString(buf);
            ListThreads[i] << buffThreads;
        }
    }

    this->setStringList(ListProcess);
}

