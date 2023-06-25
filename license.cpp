#include "license.h"

License::License(QObject *parent)
    : QObject{parent}
{

}

void License::checkLicense()
{
  /* Hole aus den Settings das Datum für den nächsten Check*/
  string_NextCheck = settings->get_NextCheck();
  qDebug() << "Next Check: " << string_NextCheck;

  /* Wenn es keinen Eintrag in den Settings gibt führe den Check aus */
  if(string_NextCheck.isEmpty())
  {
      checkRemoteFile();
      return;
  }

  /* erstelle dateTime_CurrentTime mit dem aktuellen Datum */
  dateTime_CurrentTime = QDateTime::currentDateTime();
  /* erstelle dateTime_NextCheck aus string_NextCheck */
  dateTime_NextCheck = QDateTime::fromString(string_NextCheck);

  /* Wenn das aktuelle Datum nach dem dateTime_NextCheck liegt
   * führe den Check aus. */
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
   /* Es wird versucht die Datei 'goeltenbodt.txt' von www.gareiss.org
    * runterzuladen. Danach siehe slotMetaDataChanged() */
   QNetworkAccessManager *nam = new QNetworkAccessManager(this);

   req = QNetworkRequest(QUrl("http://www.gareiss.org/goeltenbodt.txt"));
   reply = nam->get(req);
   connect(reply, SIGNAL(metaDataChanged()), this, SLOT(slotMetaDataChanged()));
}

void License::slotMetaDataChanged()
{
    /* lade int_Status mit dem Status des Download Versuchs,
     * zum Beispiel 200 erfolgreich
     *              404 File not Found
     *
     * Wenn der Download erfolgreich war wird das Datum für den
     * nächsten Check um 90 Tage erhöht
     * ist der Download fehlgeschlagen wird das Signal sig_LicenseFailed()
     * gesendet und das Programm beendet */
    int int_Status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << Q_FUNC_INFO;
    qDebug() << "Status: " << int_Status;
    if(int_Status == 200)
    {
       dateTime_NextCheck = QDateTime::currentDateTime().addDays(90);
       string_NextCheck = dateTime_NextCheck.toString();
       settings->set_NextCheck(string_NextCheck);
       return;
    }

    emit sig_LicenseFailed();
}
