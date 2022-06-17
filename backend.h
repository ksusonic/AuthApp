#pragma once

#include <QObject>
#include <QThread>
#include <QRandomGenerator>
#include <qqml.h>


class CodeGen : public QObject {
    Q_OBJECT

public slots:
     void doWork() {emit workFinished(Generate());}
signals:
    void workFinished(const QString& newCode);
private:
    const QString Data = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

     QString Generate(qint32 size = 5) {
        QString res;
        for (qint32 i = 0; i < size; ++i) {
            res += Data[QRandomGenerator::global()->bounded(Data.size())];
        }
        qDebug() << "Generated code: " << res;
        return res;
    }
};


class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString Code READ Code WRITE setCode NOTIFY codeChanged)
    Q_PROPERTY(qint16 Time READ Time WRITE setTime NOTIFY timeChanged)
    QML_ELEMENT

public:
    BackEnd(QObject *parent = nullptr)
        : QObject(parent)
        , code_("NULL")
        , time_(10)
        , worker(new CodeGen())
        , workerThread(new QThread())
    {
        worker->moveToThread(workerThread);
        QObject::connect(this, &BackEnd::requestCode, worker, &CodeGen::doWork);
        QObject::connect(worker, &CodeGen::workFinished, this, &BackEnd::setCode);
        workerThread->start();
        worker->doWork();
    }
     QString Code() {return code_;}
     qint16 Time() {return time_;}

public slots:
     void regenerateCode() {
         emit requestCode();
     }
     void setCode(const QString &newCode) {
        code_ = newCode;
        emit codeChanged();
    }
     void setTime(const qint16 &newTime) {
        time_ = newTime;
        emit timeChanged();
    }

     bool checkCode(const QString& inputCode) const {return inputCode == code_;}

signals:
    void requestCode();
    void codeChanged();
    void timeChanged();

private:
    QString code_;
    qint16 time_;
    CodeGen *worker;
    QThread* workerThread;
};
