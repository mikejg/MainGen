#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QTextEdit>

#include "settings.h"
#include "toollist.h"
#include "parser_programm.h"

class Project : public QObject
{
    Q_OBJECT
private:


    Settings* settings;

    QString string_ProgrammDir;
    QString string_ProjectFullName;
    QString string_ProjectName;
    QString string_ProjectStatus;
    QString string_ProjectClamping;
    QString string_ProjectZeroPoint;
    QString string_MainProgramm;
    QString string_RohteilKontrolle;
    QString string_Material;

    QString string_RTx, string_RTy, string_RTz;
    QString string_BTx, string_BTy, string_BTz;
    QString string_ZRT;

    QString string_XPlus_Min, string_XPlus_Max, string_XMinus_Min, string_XMinus_Max;
    QString string_YPlus_Min, string_YPlus_Max, string_YMinus_Min, string_YMinus_Max;
    QString string_ZPlus_Min, string_ZPlus_Max;

    bool bool_AufmassMax;
    bool bool_Numbering;

    QStringList stringList_Prg;
    QStringList stringList_Content_MainProgramm;

    DBManager*       dbManager;

    Parser_Programm* parser_Programm;
    int              int_RepetitiveManufacturing;
    ToolList*        toolList;
    ToolList*        toolList_IN;

public:
    explicit Project(QObject *parent = nullptr);

    QString get_RohteilX() {return string_RTx;}
    QString get_RohteilY() {return string_RTy;}
    QString get_RohteilZ() {return string_RTz;}
    QString get_ZRohteil() {return string_ZRT;}

    QString get_BauteilX() {return string_BTx;}
    QString get_BauteilY() {return string_BTy;}
    QString get_BauteilZ() {return string_BTz;}

    QString get_XPlus_Min()  {return string_XPlus_Min;}
    QString get_XPlus_Max()  {return string_XPlus_Max;}
    QString get_XMinus_Min() {return string_XMinus_Min;}
    QString get_XMinus_Max() {return string_XMinus_Max;}

    QString get_YPlus_Min()  {return string_YPlus_Min;}
    QString get_YPlus_Max()  {return string_YPlus_Max;}
    QString get_YMinus_Min() {return string_YMinus_Min;}
    QString get_YMinus_Max() {return string_YMinus_Max;}

    QString get_ZPlus_Min()  {return string_ZPlus_Min;}
    QString get_ZPlus_Max()  {return string_ZPlus_Max;}

    bool        get_AufmassMax()                 {return bool_AufmassMax;}
    QStringList get_Content_MainProgramm()       {return stringList_Content_MainProgramm;}
    QString     get_MainProgramm()               {return string_MainProgramm;}
    QString     get_Material()                   {return string_Material;}
    bool        get_Numbering()                  {return bool_Numbering;}
    QString     get_ProgrammDir()                {return string_ProgrammDir;}
    QStringList get_Programms()                  {return stringList_Prg;}
    QString     get_ProjectClamping()            {return string_ProjectClamping;}
    QString     get_ProjectFullName()            {return string_ProjectFullName;}
    QString     get_ProjectName()                {return string_ProjectName;}
    QString     get_ProjectStatus()              {return string_ProjectStatus;}
    QString     get_ProjectZeroPoint()           {return string_ProjectZeroPoint;}
    int         get_RepetitiveManufacturing()    {return int_RepetitiveManufacturing;}
    QString     get_RohteilKontrolle()           {return string_RohteilKontrolle;}

    ToolList* get_ToolList()       {return toolList;}
    ToolList* get_ToolList_IN()    {return toolList_IN;}

    void set_RohteilX(QString d) {string_RTx = d;}
    void set_RohteilY(QString d) {string_RTy = d;}
    void set_RohteilZ(QString d) {string_RTz = d;}
    void set_ZRohteil(QString d) {string_ZRT = d;}

    void set_BauteilX(QString d) {string_BTx = d;}
    void set_BauteilY(QString d) {string_BTy = d;}
    void set_BauteilZ(QString d) {string_BTz = d;}

    void set_XPlus_Min(QString str)  {string_XPlus_Min = str;}
    void set_XPlus_Max(QString str)  {string_XPlus_Max = str;}
    void set_XMinus_Min(QString str) {string_XMinus_Min = str;}
    void set_XMinus_Max(QString str) {string_XMinus_Max = str;}

    void set_YPlus_Min(QString str)  {string_YPlus_Min = str;}
    void set_YPlus_Max(QString str)  {string_YPlus_Max = str;}
    void set_YMinus_Min(QString str) {string_YMinus_Min = str;}
    void set_YMinus_Max(QString str) {string_YMinus_Max = str;}

    void set_ZPlus_Min(QString str)  {string_ZPlus_Min = str;}
    void set_ZPlus_Max(QString str)  {string_ZPlus_Max = str;}

    void set_Content_MainProgramm(QTextEdit*);
    void set_MainProgramm(QString str) {string_MainProgramm = str;}
    void set_Material(QString str)         {string_Material = str;}
    void set_ProjectClamping(QString str)  {string_ProjectClamping = str;}
    void set_ProjectFullName();
    void set_ProjectName(QString str)      {string_ProjectName = str;}
    void set_ProjectStatus(QString str)    {string_ProjectStatus = str;}
    void set_ProjectZeroPoint(QString str) {string_ProjectZeroPoint = str;}
    void set_RepetitiveManufacturing();
    void set_RohteilKontrolle(QString str)  {string_RohteilKontrolle = str;}

    void set_ToolList_IN(ToolList* tl) {toolList_IN = tl;}
    void frame_Error(QStringList);

    bool loadProjectData();
    void logProjectData();
    bool loadProjectToolList();

    void save(QTextEdit*);
    void set_DBManager(DBManager* dbm);
    void set_Settings(Settings* s);
    bool sort_Programms();
    void clear();
    QMap<QString, QString> get_Data();
    void setData();

signals:
    void sig_Err(QString);
    void sig_Log(QString);

};

#endif // PROJECT_H
