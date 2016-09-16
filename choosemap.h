#ifndef CHOOSEMAP_H
#define CHOOSEMAP_H

#include <QDialog>

namespace Ui {
class chooseMap;
}

class chooseMap : public QDialog
{
    Q_OBJECT

public:
    explicit chooseMap(QWidget *parent = 0);
    ~chooseMap();
signals:
    void mapNo(int choice);

private slots:
    /**
     * @brief on_buttonBox_accepted
     */
    void on_buttonBox_accepted();

    void on_radioButton_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_radioButton_4_clicked(bool checked);

    void on_radioButton_5_clicked(bool checked);

    void on_radioButton_6_clicked(bool checked);

private:
    Ui::chooseMap *ui;

    int mChoice = 0;
};

#endif // CHOOSEMAP_H
