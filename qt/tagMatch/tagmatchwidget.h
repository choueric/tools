#ifndef TAGMATCHWIDGET_H
#define TAGMATCHWIDGET_H

#include <QWidget>
#include <qstringlistmodel.h>

namespace Ui {
class tagMatchWidget;
}

class tagMatchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tagMatchWidget(QWidget *parent = 0);
    ~tagMatchWidget();

private slots:
    void on_addLineEdit_returnPressed();

    void on_addBtn_clicked();

    void on_matchBtn_clicked();

    void on_matchLineEdit_returnPressed();

private:
    Ui::tagMatchWidget *ui;
    QStringListModel *m_addModel;
    QStringListModel *m_matchModel;

private:
    void doAddTag(QString tag);
    void doMatchTag(QString tag);
};

#endif // TAGMATCHWIDGET_H
