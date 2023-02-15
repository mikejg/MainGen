#include "license.h"

License::License(QObject *parent)
    : QObject{parent}
{

}

void License::checkLicense()
{
  qDebug() << Q_FUNC_INFO;
  string_NextCheck = settings->value("NextCheck","").toString();
  qDebug() << "NextCheck: " << string_NextCheck;
  if(string_NextCheck.isEmpty())
  {
      qDebug() << "if(string_NextCheck.isEmpty())";
      checkRemoteFile();
      return;
  }

  dateTime_CurrentTime = QDateTime::currentDateTime();
  dateTime_NextCheck = QDateTime::fromString(string_NextCheck);
  if(dateTime_CurrentTime > dateTime_NextCheck)
  {
      qDebug() << "if(dateTime_CurrentTime > dateTime_NextCheck)";
      checkRemoteFile();
  }

  return;
}

void License::checkRemoteFile()
{
   qDebug() << Q_FUNC_INFO;
   QNetworkAccessManager *nam = new QNetworkAccessManager(this);

   req = QNetworkRequest(QUrl("http://www.gareiss.org/goeltenbodt.txt"));
   reply = nam->get(req);
   connect(reply, SIGNAL(metaDataChanged()), this, SLOT(slotMetaDataChanged()));
}

void License::slotMetaDataChanged()
{
    int int_Status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << Q_FUNC_INFO;
    qDebug() << "Status: " << int_Status;
    if(int_Status == 200)
    {
       dateTime_NextCheck = QDateTime::currentDateTime().addDays(30);
       string_NextCheck = dateTime_NextCheck.toString();
       settings->setValue("NextCheck", string_NextCheck);
       return;
    }

    emit sig_LicenseFailed();
}
