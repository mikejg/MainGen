#ifndef HOLDER_H
#define HOLDER_H

#include <QObject>

class Holder : public QObject
{
    Q_OBJECT

private:
    QString string_Name;
    QString string_Comment;
    int counter;

public:
    explicit Holder(QObject *parent = nullptr);
    void set_Name (QString n)           {string_Name = n;};
    QString  get_Name()                 {return string_Name;};
    void set_Comment (QString d)        {string_Comment = d;};
    QString  get_Comment()              {return string_Comment;}
    void set_counter(int c)             {counter = c;};
    int get_counter()                   {return counter;};
    void inc_counter()                  {counter++;};

    void clear();
signals:

};

#endif // HOLDER_H
