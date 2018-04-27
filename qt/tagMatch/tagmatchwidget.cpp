#include "tagmatchwidget.h"
#include "ui_tagmatchwidget.h"
#include <qdebug.h>
#include <QMessageBox>

tagMatchWidget::tagMatchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tagMatchWidget)
{
    ui->setupUi(this);
    m_addModel = new QStringListModel(this);
   // m_addModel->setStringList(m_addTags);
    ui->addListView->setModel(m_addModel);

    m_matchModel = new QStringListModel(this);
    //m_matchModel->setStringList(m_matchTags);
    ui->matchListView->setModel(m_matchModel);
}

tagMatchWidget::~tagMatchWidget()
{
    delete ui;
}

void tagMatchWidget::on_addLineEdit_returnPressed()
{
    QString tag = ui->addLineEdit->text();
    doAddTag(tag);
}

void tagMatchWidget::on_addBtn_clicked()
{
    QString tag = ui->addLineEdit->text();
    doAddTag(tag);
}

void tagMatchWidget::on_matchBtn_clicked()
{
    QString tag = ui->matchLineEdit->text();
    doMatchTag(tag);
}

void tagMatchWidget::on_matchLineEdit_returnPressed()
{
    QString tag = ui->matchLineEdit->text();
    doMatchTag(tag);
}

void tagMatchWidget::doAddTag(QString tag)
{
    QStringList list = m_addModel->stringList();
    int i = list.indexOf(tag);
    if (i != -1) {
        QMessageBox msg;
        msg.setText("Tag is already in list");
        msg.exec();
        return;
    }
    m_addModel->insertRow(m_addModel->rowCount());
    QModelIndex index = m_addModel->index(m_addModel->rowCount()-1);
    m_addModel->setData(index, tag);
    ui->addLineEdit->clear();
}

void tagMatchWidget::doMatchTag(QString tag)
{
    QStringList addList = m_addModel->stringList();
    int i = addList.indexOf(tag);
    ui->matchLineEdit->clear();
    if (i == -1) {
        QMessageBox msg;
        msg.setText("Tag is not in list");
        msg.exec();
        return;
    }
    m_addModel->removeRow(i);

    m_matchModel->insertRow(m_matchModel->rowCount());
    QModelIndex index = m_matchModel->index(m_matchModel->rowCount()-1);
    m_matchModel->setData(index, tag);
}
