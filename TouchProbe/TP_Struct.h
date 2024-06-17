#ifndef TP_STRUCT_H
#define TP_STRUCT_H
#include <QString>

struct Struct_Ausrichten
{
    Struct_Ausrichten()
    {
        string_Alpha = "0.000";
        string_DFA   = "10.000";
    }

    QString string_Pos;
    QString string_Frame;
    QString string_Messrichtung;
    QString string_Messachse;
    QString string_Positionierachse;
    QString string_Alpha;
    QString string_L2;
    QString string_TSA;
    QString string_DFA;
    QString string_Antasten;

};
#endif // TP_STRUCT_H
