#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setFont(const QFont &font);
    void setForeColor(const QColor &color);
    void setBackColor(const QColor &color);
private slots:
    void on_fontBtn_clicked();

    void on_colorBtn_clicked();

    void on_exitBtn_clicked();

    void on_backColorBtn_clicked();

private:
    Ui::MainWindow *ui;
    Config m_config;
    QLabel *m_label;
    QFont m_font;
    QColor m_foreColor;
    QColor m_backColor;

};

#endif // MAINWINDOW_H
