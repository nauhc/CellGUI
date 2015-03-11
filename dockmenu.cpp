#include "dockmenu.h"

DockMenu::DockMenu(QWidget *parent) :
    QWidget(parent)
{
    createProptyCheckbox();
}

void DockMenu::createProptyCheckbox()
{
    propGroup = new QGroupBox();
    prop0 = new QCheckBox("Area");
    prop1 = new QCheckBox("Perimeter");
    prop2 = new QCheckBox("Blebs Number & Size");
    prop3 = new QCheckBox("Centroid Trajectory");
    prop4 = new QCheckBox("Shape");

    prop0->setChecked(true);
    prop1->setChecked(false);
    prop2->setChecked(true);
    prop3->setChecked(true);
    prop4->setChecked(true);

    propVLayout = new QVBoxLayout();
    propVLayout->addWidget(prop0);
    propVLayout->addWidget(prop1);
    propVLayout->addWidget(prop2);
    propVLayout->addWidget(prop3);
    propVLayout->addWidget(prop4);

    propGroup->setLayout(propVLayout);
//    propGroup->setFixedHeight(100);

    mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(propGroup);
    mainVLayout->addStretch();

    setLayout(mainVLayout);

    connect(prop0, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop1, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop2, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop3, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop4, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));

}

void DockMenu::box_checked(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;

    if (state == Qt::Checked) {
        if(checkedBoxes.size() == 4){
            checkBox->setCheckState(Qt::Unchecked);
            return;
        } else if (!checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
            checkedBoxes.push_back(checkBox->objectName().remove(0, 9));
        }

    } else if (state == Qt::Unchecked) {
        if (checkedBoxes.size() == 0) {
            return; // should not happen
        } else if (checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
            checkedBoxes.remove(checkedBoxes.indexOf(checkBox->objectName().remove(0, 9)));
        }
    }
}
