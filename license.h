#ifndef LICENSE_H
#define LICENSE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include "settings.h"

class License : public QObject
{
    Q_OBJECT
private:
    QNetworkRequest req;
    QNetworkReply   *reply;
    QString         string_NextCheck;
    QDateTime       dateTime_NextCheck;
    QDateTime       dateTime_CurrentTime;

    Settings*       settings;

    void checkRemoteFile();
public:
    explicit License(QObject *parent = nullptr);
    void checkLicense();
    void setSettings(Settings* s) {settings = s;}

signals:
    void sig_LicenseFailed();

public slots:
    void slotMetaDataChanged();
};

#endif // LICENSE_H
