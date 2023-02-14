#include "license.h"

License::License(QObject *parent)
    : QObject{parent}
{

}

void License::checkLicense(QSettings* settings)
{
  qDebug() << Q_FUNC_INFO;
  string_NextCheck = settings->value("NextCheck","").toString();
  if(string_NextCheck.isEmpty())
  {
      qDebug() << "if(string_NextCheck.isEmpty())";
      checkRemoteFile();
      return;
  }
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
    qDebug() << Q_FUNC_INFO;
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

}
