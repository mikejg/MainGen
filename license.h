#ifndef LICENSE_H
#define LICENSE_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

class License : public QObject
{
    Q_OBJECT
private:
    QNetworkRequest req;
    QNetworkReply *reply;
    QString string_NextCheck;
    QSettings* settings;
    QDateTime dateTime_NextCheck;
    QDateTime dateTime_CurrentTime;

    void checkRemoteFile();
public:
    explicit License(QObject *parent = nullptr);
    void checkLicense();
    void setSettings(QSettings* s) {settings = s;}

signals:
    void sig_LicenseFailed();

public slots:
    void slotMetaDataChanged();
};

#endif // LICENSE_H
