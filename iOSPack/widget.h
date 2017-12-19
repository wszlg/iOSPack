#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString fileName;
    QProcess process;
    QProcess exportProcess;

    // 项目的路径
    QString projectPath;
    QString projectTarget;
    QString projectscheme;
    bool isdelete;
    bool DelDir(const QString &path);




private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void processFinished();
    void exportFinished();

    void exportArchive();


    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_checkBox_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
