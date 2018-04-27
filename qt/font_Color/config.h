#ifndef CONFIG_H
#define CONFIG_H

#include <qstring.h>

const QString GRP_CAMERA = "camera";
const QString KEY_IP = "IP";
const QString KEY_USR = "user";
const QString KEY_PASSWD = "passwd";

const QString GRP_RFREADER = "rfreader";
const QString KEY_CAPTAL = "captal";
const QString KEY_DEBUG = "debug";

const QString GRP_LED = "led";
const QString KEY_FONT = "font";
const QString KEY_FORECOLOR = "forecolor";
const QString KEY_BACKCOLOR = "backcolor";

class Config {
public:
    Config();
    ~Config();
    void writeIni(QString group, QString key, QString value);
    void readIni(QString group, QString key, QString &value);

private:
    QString m_filename;

};

#endif // CONFIG_H
