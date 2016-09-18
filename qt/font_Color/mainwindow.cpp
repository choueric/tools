#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDialog>
#include <QColorDialog>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_label = ui->ledLabel;
    showFullScreen();
    //m_label->setFixedWidth(((QWidget *)this)->width());

    m_label->setText("123456");
    //m_label->adjustSize();

    QString fontStr;
    m_config.readIni(GRP_LED, KEY_FONT, fontStr);
    m_font.fromString(fontStr);
    setFont(m_font);
    ui->fontLabel->setText(m_font.toString());

    QString colorStr;
    m_config.readIni(GRP_LED, KEY_FORECOLOR, colorStr);
    m_foreColor.setNamedColor(colorStr);
    setForeColor(m_foreColor);
    ui->colorLabel->setText(m_foreColor.name());

    m_config.readIni(GRP_LED, KEY_BACKCOLOR, colorStr);
    m_backColor.setNamedColor(colorStr);
    setBackColor(m_backColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFont(const QFont &font)
{
    m_label->setFont(font);
}

void MainWindow::setForeColor(const QColor &color)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    m_label->setPalette(pa);
}

void MainWindow::setBackColor(const QColor &color)
{
    QPalette pa;
    pa.setColor(QPalette::Background, color);
    setAutoFillBackground(true);
    setPalette(pa);
}

void MainWindow::on_fontBtn_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, m_font, this);
    if (ok) {
        m_font = font;
        m_config.writeIni(GRP_LED, KEY_FONT, font.toString());

        setFont(m_font);
        ui->fontLabel->setText(m_font.toString());
    }
}

void MainWindow::on_colorBtn_clicked()
{
    QColor color = QColorDialog::getColor(m_foreColor);

    if (color.isValid()) {
        m_foreColor = color;
        m_config.writeIni(GRP_LED, KEY_FORECOLOR, m_foreColor.name());

        setForeColor(m_foreColor);
        ui->colorLabel->setText(m_foreColor.name());
    }
}

void MainWindow::on_exitBtn_clicked()
{
    qApp->quit();
}

void MainWindow::on_backColorBtn_clicked()
{
    QColor color = QColorDialog::getColor(m_foreColor);

    if (color.isValid()) {
        m_backColor = color;
        m_config.writeIni(GRP_LED, KEY_BACKCOLOR, m_backColor.name());

        setBackColor(m_backColor);
    }
}
