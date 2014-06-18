#include "mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myController(new Controller())
{
//    myController = new Controller();
//    QObject::connect(myController, SIGNAL(processedImage(QImage)),
//                     this, SLOT(updateVideoplayerUI(QImage)));

    ui->setupUi(this);
    connect(myController, SIGNAL(processedImage(QImage, QImage)), this, SLOT(updateVideoplayerUI(QImage, QImage)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)), myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)), myController, SLOT(setBlkSize(int)));

    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(3); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

}

MainWindow::~MainWindow(){
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
        ui->horizontalSlider->setValue(myController->getCurrentFrame());
        ui->frameLabel->setText(QString::number(int(myController->getCurrentFrame()))
                                +" / "+ QString::number(myController->getNumberOfFrames()));

//        int x = 75;
//        int y = 110;
//        int width = 150;
//        int height = 100;

//        QRectF rectangle(x, y, width, height);
//        QPainter painter(ui->orgVideo);
//        painter.drawRect(rectangle);

    }

}

void MainWindow::on_playVideoButton_clicked()
{
    if(myController->videoIsStopped()){
        myController->playVideo();
        ui->playVideoButton->setText("Pause");
        ui->stopVideoButton->setEnabled(false);
    }else{
        myController->stopVideo();
        ui->playVideoButton->setText("Play");
        ui->stopVideoButton->setEnabled(true);
    }
}

void MainWindow::on_stopVideoButton_clicked()
{
    if(!myController->videoIsStopped())
        myController->stopVideo();
    myController->releaseVideo();

    QPixmap pixmap1(1,1); // Works
    pixmap1 = pixmap1.scaled(ui->roiVideo->width(), ui->roiVideo->height());
    pixmap1.fill(QColor(0, 0, 0));
    ui->roiVideo->setPixmap(pixmap1);
    QPixmap pixmap2(1,1); // Works
    pixmap2 = pixmap2.scaled(ui->orgVideo->width(), ui->orgVideo->height());
    pixmap2.fill(QColor(0, 0, 0));
    ui->orgVideo->setPixmap(pixmap2);

    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);
    ui->horizontalSlider->setValue(0);
}

void MainWindow::on_loadVideoButton_clicked()
{
    if(!myController->videoIsNull()){
        delete myController;
        myController = new Controller();

    }

//    QString filename = QFileDialog::getOpenFileName(this,
//                                                    tr("Open Video"), "../../../video/",
//                                                    tr("Video Files (*.avi *.mpg *.mp4"));
    QString filename = "/Users/chuanwang/Sourcecode/CellGUI/video/movie.mp4";
//    QString filename = "/Users/chuanwang/Sourcecode/CellGUI/video/movie.avi";

    if (!filename.isEmpty()){
            if (!myController->loadVideo(filename.toStdString())){
                QMessageBox msgBox;
                msgBox.setText("The selected video could not be opened!");
                msgBox.exec();
            }
            else{
                ui->loadVideoButton->setEnabled(false);
                ui->playVideoButton->setEnabled(true);
                //ui->horizontalSlider->setEnabled(true);
                ui->horizontalSlider->setMaximum(myController->getNumberOfFrames());
                ui->frameLabel->setText("0 / " + QString::number(myController->getNumberOfFrames()));
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



