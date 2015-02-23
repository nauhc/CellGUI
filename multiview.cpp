#include "multiview.h"

MultiView::MultiView(QWidget *parent) : QWidget(parent){
    QGridLayout *layout = new QGridLayout(this);

    QLabel *label1 = new QLabel("111111");
    QLabel *label2 = new QLabel("222222");
    QLabel *label3 = new QLabel("333333");
    QLabel *label4 = new QLabel("444444");
    QLabel *label5 = new QLabel("555555");
    layout->addWidget(label1, 0, 0);
    layout->addWidget(label2, 0, 1);
    layout->addWidget(label3, 0, 2);
    layout->addWidget(label4, 1, 0);
    layout->addWidget(label5, 1, 1);
    layout->addWidget(label5, 1, 2);

}
