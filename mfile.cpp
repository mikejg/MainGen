#include "mfile.h"

MFile::MFile(QObject *parent)
    : QFile{parent}
{

}

MFile::MFile(QString name)
    : QFile{name}
{

}

bool MFile::read_Content()
{
    /* Versuche das File zu öffnen,
     * wenn das fehl schlägt gib eine Fehlermeldung raus
     * und bende die Funktion mit false */
    if(!this->open(QFile::ReadOnly))
    {
        emit sig_Err("Konnte " + fileName() + " nicht oeffenen");
        return false;
    }

    /* Erzeuge einen QStream auf das File
     * Setze die Codierung auf Latin1 */
    textStream_Read = new QTextStream(this);
    textStream_Read->setAutoDetectUnicode(false);
    textStream_Read->setEncoding(QStringConverter::Latin1);

    /* Lösche den Inhalt von stringList_Content
     * Lese Zeile für Zeile und schreibe jede Zeile in die QStringList stringList_Content
     * schliesse das File */
    stringList_Content.clear();
    while(!textStream_Read->atEnd())
    {
        stringList_Content.append(textStream_Read->readLine());
        //emit sig_Log(stringList_Content.last());
    }
    this->close();
    return true;
}

