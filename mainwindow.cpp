#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ui_mainwindow.h"

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

void MainWindow::updateVideoplayerUI(QImage img, QImage ROIimg){
    if(!img.isNull()){
        //original video display
        ui->orgVideo->setAlignment(Qt::AlignCenter);
        ui->orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                    ui->orgVideo->size(),
                    Qt::KeepAspectRatio,
                    Qt::FastTransformation));
        //roi video display
        ui->roiVideo->setAlignment(Qt::AlignCenter);
        ui->roiVideo->setPixmap(
                    QPixmap::fromImage(ROIimg).scaled(
                    ui->roiVideo->size(),
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
    connect(myController, SIGNAL(processedImage(QImage, QImage)), this, SLOT(updateVideoplayerUI(QImage, QImage)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)), myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)), myController, SLOT(setBlkSize(int)));
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(7); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

//    QString filename = QFileDialog::getOpenFileName(this,
//                                                    tr("Open Video"), "../../../video/",
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

