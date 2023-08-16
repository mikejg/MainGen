#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QDebug>

class Tool : public QObject
{
    Q_OBJECT

private:


    QString number;
    QString description;
    QString gesammtLaenge;
    QString ausspannLaenge;
    QString freistellLaenge;
    QString halterName;
    QString halterComment;



    int counter;

public:
    enum State {In, Out, Disassembled};
    State   state;
    explicit Tool(QObject *parent = nullptr);
    void set_Number (QString n)         {number = n;}
    QString  get_Number()               {return number;}
    void set_Description (QString d)    {description = d;}
    QString  get_Description()          {return description;}
    void set_counter(int c)             {counter = c;}
    int get_counter()                   {return counter;}
    void inc_counter()                  {counter++;}
    void set_ToolGL(QString tgl)        {gesammtLaenge = tgl;}
    QString get_ToolGL()                {return gesammtLaenge;}
    void set_ToolAL(QString agl)        {ausspannLaenge = agl;}
    QString get_ToolAL()                {return ausspannLaenge;}
    void set_ToolFL(QString tfl)        {freistellLaenge = tfl;}
    QString get_ToolFL()                {return freistellLaenge;}
    void set_HalterName (QString n)     {halterName = n;}
    QString  get_HalterName()           {return halterName;}
    void set_HalterComment (QString n)  {halterComment = n;}
    QString  get_HalterComment()        {return halterComment;}
    void set_State(State s)             {state = s;}
    State get_State()                   {return state;}
    QString get_StateString();
    void clear();

signals:

};

#endif // TOOL_H
