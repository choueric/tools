#include "config.h"
#include <qfileinfo.h>

#include <qsettings.h>

Config::Config()
{
    m_filename = "./config.ini";
    QFileInfo info(m_filename);

    if (info.exists() == false) {
        writeIni(GRP_CAMERA, KEY_IP, "192.168.0.64");
        writeIni(GRP_CAMERA, KEY_USR, "admin");
        writeIni(GRP_CAMERA, KEY_PASSWD, "aA20165612");

		writeIni(GRP_RFREADER, KEY_CAPTAL, "true");
		writeIni(GRP_RFREADER, KEY_DEBUG, "false");

        writeIni(GRP_LED, KEY_FONT, "12");
        writeIni(GRP_LED, KEY_FORECOLOR, "red");
        writeIni(GRP_LED, KEY_BACKCOLOR, "black");
    }
}

Config::~Config()
{

}

void Config::writeIni(QString group, QString key, QString value)
{
    QSettings accountSet(m_filename, QSettings::IniFormat);
    accountSet.setIniCodec("UTF-8");
    accountSet.beginGroup(group);
    accountSet.setValue(key, value);
    accountSet.endGroup();
}

void Config::readIni(QString group, QString key, QString &value)
{
    QSettings accountSet(m_filename, QSettings::IniFormat);
    accountSet.setIniCodec("UTF-8");
    accountSet.beginGroup(group);
    value = accountSet.value(key).toString();
    accountSet.endGroup();
}

