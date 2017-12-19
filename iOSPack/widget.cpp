#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <stdio.h>
#include <QDir>





Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

// 打开项目
void Widget::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName();
    if(!fileName.isEmpty()){
        ui->lineEdit->setText(fileName);
        ui->textBrowser->setText("");
    }


}

// 打包
void Widget::on_pushButton_2_clicked()
{
    ui->textBrowser->setText("");
    // 用户输入的target和scheme
    QString target = ui->lineEdit_2->text();
    QString scheme = ui->lineEdit_3->text();

    // 项目的路径
    projectPath = ui->lineEdit->text();

    int i = fileName.lastIndexOf("/");
    QString projectNames = fileName.right(fileName.length() - i - 1);
    int j = projectNames.indexOf(".");
    // 项目的名称
    QString projectName = projectNames.left(j);



    if(projectPath.isEmpty()){
        QMessageBox::information(this,"提示","请选择项目");
        return;
    }


    // target截取
    if(target.isEmpty()){
        projectTarget = projectName;
    } else {
        projectTarget = target;
    }

    // scheme截取
    if(scheme.isEmpty()){
        projectscheme = projectName;
    } else {
        projectscheme = scheme;
    }

    QString filePath = fileName;
    int m = filePath.lastIndexOf("/");
    // 项目的根路径
    QString buildPath = filePath.left(filePath.length() - (filePath.length() - m));
    QString build = "xcodebuild -workspace " + fileName + " -scheme " + projectscheme +
            " -configuration Release archive -archivePath  " + buildPath + "/" + projectTarget + " -destination generic/platform=iOS";
    ui->textBrowser->append("正在打包......请稍等......这需要一段时间...\n");
    // 执行打包命令
    process.start(build);
    QObject::connect(&process,SIGNAL(finished(int)),this,SLOT(processFinished()));

}


void Widget::processFinished()
{
    QByteArray baData;
    baData = process.readAll();
    QString str = QString(baData);
    ui->textBrowser->append(str);

    if(str.contains("ARCHIVE SUCCEEDED"))
    {
        ui->textBrowser->append("===============打包成功......正在导出......===============");
        // 导出ipa包
        exportArchive();

    } else {
//        ui->textBrowser->append("===============打包失败......请查看日志......===============");
    }
    qDebug() << str;

}



void Widget::exportArchive()
{
    QString filePath = fileName;
    int m = filePath.lastIndexOf("/");
    // 项目的根路径
    QString buildPath = filePath.left(filePath.length() - (filePath.length() - m));
    QString exportCmd = "xcodebuild -exportArchive -archivePath " + buildPath + "/" + projectTarget + ".xcarchive " + " -exportPath " + "~/Desktop/" + "projectArchive" + " -exportOptionsPlist " + buildPath + "/" + "exportOptions.plist";
    QObject::connect(&exportProcess,SIGNAL(finished(int)),this,SLOT(exportFinished()));
    exportProcess.start(exportCmd);
}



void Widget::exportFinished()
{
    QByteArray baData;
    baData = exportProcess.readAll();
    QString str = QString(baData);
    qDebug() << str;
    ui->textBrowser->append(str);
    if(str.contains("EXPORT SUCCEEDED")){
        ui->textBrowser->append("===============......导出成功......===============");
    }else {
//        ui->textBrowser->append("===============......导出失败......===============");
    }

    if(isdelete){  // 要删除临时文件

        QString filePath = fileName;
        int m = filePath.lastIndexOf("/");
        // 项目的根路径
        QString buildPath = filePath.left(filePath.length() - (filePath.length() - m));
        DelDir(buildPath + "/" + projectTarget + ".xcarchive");
        if(!ui->lineEdit_4->text().isEmpty()){
            DelDir(ui->lineEdit_4->text());
        }
        QMessageBox::information(this,"","临时文件清理完成");






    }

}




//删除文件夹
bool Widget::DelDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList){ //遍历文件信息
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        }else{ // 递归删除
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}












// 使用说明
void Widget::on_pushButton_3_clicked()
{
    QMessageBox::information(this,"tishi","请把exportOptions.plist文件放到项目目录下,使用前请先用Xcode打出相应的包(打包一次即可),就会自动配置好描述文件,然后就可以一直使用本工具打包");

}

// 改变打包类型
void Widget::on_pushButton_5_clicked()
{
     QMessageBox::information(this,"tishi","修改exportOptions.plist中method的值:( app-store, ad-hoc, package, enterprise, development, developer-id),修改后请先用xcode打包一次");
}

// 注意事项
void Widget::on_pushButton_4_clicked()
{
    QMessageBox::information(this,"tishi","ipa会打包到桌面projectArchive文件夹中,target和schme不写,默认和项目名称一样");
}


// 选择了复选框
void Widget::on_checkBox_clicked()
{
    isdelete = ui->checkBox->checkState();
    qDebug() << ui->checkBox->checkState() << isdelete << endl;
}
