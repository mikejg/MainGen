#ifndef MFILE_H
#define MFILE_H

#include <QFile>
#include <QStringList>
#include <QTextStream>

class MFile : public QFile
{
    Q_OBJECT

private:
    QTextStream* textStream_Read;
    QStringList  stringList_Content;

public:
    explicit MFile(QObject *parent = nullptr);
    MFile(QString name);

    QStringList get_Content() {return stringList_Content;}
    bool        read_Content();

signals:
    void sig_Err(QString);
    void sig_Log(QString);
};

#endif // MFILE_H
