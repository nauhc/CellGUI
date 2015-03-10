#ifndef DOCKMENU_H
#define DOCKMENU_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QCheckBox>

class DockMenu : public QWidget
{
    Q_OBJECT
public:
    explicit DockMenu(QWidget *parent = 0);

signals:

public slots:

private:
    QGroupBox           *propGroup;
    QVBoxLayout         *propVLayout;
    QCheckBox           *prop0;
    QCheckBox           *prop1;
    QCheckBox           *prop2;
    QCheckBox           *prop3;
    QCheckBox           *prop4;
    QVector<QString>    checkedBoxes;

    void createProptyCheckbox();


private slots:
    void box_checked(int state);

};

#endif // DOCKMENU_H
