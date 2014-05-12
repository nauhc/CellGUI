#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myController = new Controller();
//    QObject::connect(myController, SIGNAL(processedImage(QImage)),
//                     this, SLOT(updateVideoplayerUI(QImage)));

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    if(myController->isNull())
        delete myController;
    delete ui;
}

void MainWindow::updateVideoplayerUI(QImage img){
    if(!img.isNull()){
        ui->orgVideo->setAlignment(Qt::AlignCenter);
        ui->orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                    ui->orgVideo->size(),
                    Qt::KeepAspectRatio,
                    Qt::FastTransformation));

    }
}

void MainWindow::on_playVideoButton_clicked()
{
    if(myController->isStopped()){
        myController->play();
    }else{
        myController->Stop();
    }
}



void MainWindow::on_pauseVideoButton_clicked()
{
    if(!myController->isStopped())
        myController->Stop();
}

void MainWindow::on_loadVideoButton_clicked()
{
    if(!myController->isNull()){
        delete myController;
        myController = new Controller();
    }

    connect(myController, SIGNAL(processedImage(QImage)), this, SLOT(updateVideoplayerUI(QImage)));

//    QString filename = QFileDialog::getOpenFileName(this,
//                                                    tr("Open Video"), "../../../../video/",
//                                                    tr("Video Files (*.avi *.mpg *.mp4"));
    QString filename = "/Users/chuanwang/Sourcecode/CellGUI/video/movie.avi";


    if (!filename.isEmpty()){
            if (!myController->loadVideo(filename.toStdString())){
                QMessageBox msgBox;
                msgBox.setText("The selected video could not be opened!");
                msgBox.exec();
            }
            else{
                ui->playVideoButton->setEnabled(true);
                ui->pauseVideoButton->setEnabled(true);
                ui->orgVideo->setAlignment(Qt::AlignCenter);
                //ui->orgVideo->setPixmap(QPixmap::fromImage(myController->getFrame(1)).scaled(
                //                                            ui->orgVideo->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }

}
