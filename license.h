#ifndef LICENSE_H
#define LICENSE_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class License : public QObject
{
    Q_OBJECT
private:
    QNetworkRequest req;
    QNetworkReply *reply;
    QString string_NextCheck;

    void checkRemoteFile();
public:
    explicit License(QObject *parent = nullptr);
    void checkLicense(QSettings* s);
signals:
public slots:
    void slotMetaDataChanged();
};

#endif // LICENSE_H
