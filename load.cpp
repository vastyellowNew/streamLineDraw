#include "load.h"
#include "ui_load.h"
#include <QFileDialog>
#include <QMessageBox>

load::load(vecField *pData, QWidget *parent) :
    QDialog(parent),
    mData(pData),
    ui(new Ui::load)
{
    ui->setupUi(this);
    this->ui->tornadoRadio->setHidden(true);
    this->ui->abcRadio->setHidden(true);
    this->ui->DABox->setVisible(false);
    this->ui->DAlabel->setVisible(false);
    this->ui->DMFBox->setVisible(false);
    this->ui->DMFlabel->setVisible(false);
    this->ui->aLabel->setVisible(false);
    this->ui->aSpinBox->setVisible(false);
    this->ui->bLabel->setVisible(false);
    this->ui->bSpinBox->setVisible(false);
    this->ui->cLabel->setVisible(false);
    this->ui->cSpinBox->setVisible(false);
    //QObject::connect(this->ui->okButton, SIGNAL(clicked()), load, SLOT(accept()));
    //QObject::connect(this->ui->cancelButton, SIGNAL(clicked()), load, SLOT(reject()));
}

load::~load()
{
    delete ui;
}

void load::on_loadRadioButton_clicked()
{
    this->ui->generateRadioButton->setChecked(false);
    this->ui->tornadoRadio->setHidden(true);
    this->ui->abcRadio->setHidden(true);
    this->ui->pathField->setVisible(true);
    this->ui->browseButt->setVisible(true);
    this->ui->modChBox->setVisible(true);
    this->ui->aLabel->setVisible(false);
    this->ui->aSpinBox->setVisible(false);
    this->ui->bLabel->setVisible(false);
    this->ui->bSpinBox->setVisible(false);
    this->ui->cLabel->setVisible(false);
    this->ui->cSpinBox->setVisible(false);
    load::on_modChBox_clicked();
}


void load::on_generateRadioButton_clicked()
{
    this->ui->loadRadioButton->setChecked(false);
    this->ui->tornadoRadio->setVisible(true);
    this->ui->abcRadio->setVisible(true);
    this->ui->pathField->setVisible(false);
    this->ui->browseButt->setVisible(false);
    this->ui->DABox->setVisible(false);
    this->ui->DAlabel->setVisible(false);
    this->ui->DMFBox->setVisible(false);
    this->ui->DMFlabel->setVisible(false);
    this->ui->modChBox->setVisible(false);
    if (this->ui->abcRadio->isChecked())
    {
        this->ui->aLabel->setVisible(true);
        this->ui->aSpinBox->setVisible(true);
        this->ui->bLabel->setVisible(true);
        this->ui->bSpinBox->setVisible(true);
        this->ui->cLabel->setVisible(true);
        this->ui->cSpinBox->setVisible(true);
    }
}

void load::on_modChBox_clicked()
{
    if (this->ui->modChBox->isChecked())
    {
        this->ui->DABox->setVisible(true);
        this->ui->DAlabel->setVisible(true);
        this->ui->DMFBox->setVisible(true);
        this->ui->DMFlabel->setVisible(true);
    }
    else
    {
        this->ui->DABox->setVisible(false);
        this->ui->DAlabel->setVisible(false);
        this->ui->DMFBox->setVisible(false);
        this->ui->DMFlabel->setVisible(false);
    }
}

void load::on_tornadoRadio_clicked()
{
    this->ui->abcRadio->setChecked(false);
    this->ui->aLabel->setVisible(false);
    this->ui->aSpinBox->setVisible(false);
    this->ui->bLabel->setVisible(false);
    this->ui->bSpinBox->setVisible(false);
    this->ui->cLabel->setVisible(false);
    this->ui->cSpinBox->setVisible(false);
}

void load::on_abcRadio_clicked()
{
    this->ui->tornadoRadio->setChecked(false);
    this->ui->aLabel->setVisible(true);
    this->ui->aSpinBox->setVisible(true);
    this->ui->bLabel->setVisible(true);
    this->ui->bSpinBox->setVisible(true);
    this->ui->cLabel->setVisible(true);
    this->ui->cSpinBox->setVisible(true);
}

void load::on_browseButt_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"), "/home","RAW File (*.raw);;All files (*.*)");
    this->ui->pathField->setText(filename);
}


void load::on_okButton_released()
{
    if (this->ui->xBox->value() == 0 )
    {
        QMessageBox::warning(this,tr("Error"),tr("X value cannot be zero"));
        return;
    }
    else if (this->ui->yBox->value() == 0 )
    {
        QMessageBox::warning(this,tr("Error"),tr("Y value cannot be zero"));
        return;
    }
    else if (this->ui->zBox->value() == 0 )
    {
        QMessageBox::warning(this,tr("Error"),tr("Z value cannot be zero"));
        return;
    }
    else if (this->ui->loadRadioButton->isChecked())
    {
        if (this->ui->pathField->text() == "")
        {
            QMessageBox::warning(this,tr("Error"),tr("Please enter file name/path"));
            return;
        }
        mData->setSize(this->ui->xBox->value(),
                   this->ui->yBox->value(),
                   this->ui->zBox->value());

        bool readOK = readFile(this->ui->xBox->value(),
                 this->ui->yBox->value(),
                 this->ui->zBox->value(),
                 mData);
        if (readOK)
        {
            emit draw(true);
            close();
        }
    }
    else
    {
        mData->setSize(this->ui->xBox->value(),
                   this->ui->yBox->value(),
                   this->ui->zBox->value());

        if (this->ui->generateRadioButton->isChecked())
        {
            if (this->ui->tornadoRadio->isChecked())
            {
                genTornado(this->ui->xBox->value(),
                           this->ui->yBox->value(),
                           this->ui->zBox->value(),
                           10, mData);
                emit draw(true);
                close();

            }
            else if (this->ui->abcRadio->isChecked())
            {
                genAbc(this->ui->xBox->value(),
                       this->ui->yBox->value(),
                       this->ui->zBox->value(),
                       mData);

                emit draw(true);
                close();
            }
        }

    }
}

void load::genTornado(int xs, int ys, int zs, int time, vecField *sallyv)
{
/*
*  Gen_Sally creates a vector field of dimension [xs,ys,zs,3] from
*  a proceedural function. By passing in different time arguements,
*  a slightly different and rotating field is created.
*
*  The magnitude of the vector field is highest at some funnel shape
*  and values range from 0.0 to around 0.4 (I think).
*/

    float x, y, z;
    int ix, iy, iz;
    float r, xc, yc, scale, temp, z0;
    float r2 = 8;
    float SMALL = 0.00000000001;
    float xdelta = 1.0 / (xs - 1.0);
    float ydelta = 1.0 / (ys - 1.0);
    float zdelta = 1.0 / (zs - 1.0);

    for (iz = 0; iz < zs; iz++)
    {
        z = iz * zdelta;                        // map z to 0->1
        xc = 0.5 + 0.1*(sin(0.04*time + 10.0*z));   // For each z-slice, determine the spiral circle.
        yc = 0.5 + 0.1*(cos(0.03*time + 3.0*z));    //    (xc,yc) determine the center of the circle.
        r = 0.1 + 0.4 * z*z + 0.1 * z * (sin(8.0*z)); //  The radius also changes at each z-slice.
        r2 = 0.2 + 0.1*z;                           //    r is the center radius, r2 is for damping
        for (iy = 0; iy < ys; iy++)
        {
            y = iy * ydelta;
            for (ix = 0; ix < xs; ix++)
            {
                x = ix * xdelta;
                temp = sqrt((y - yc)*(y - yc) + (x - xc)*(x - xc));
                scale = (r - temp);
                /*
                *  I do not like this next line. It produces a discontinuity
                *  in the magnitude. Fix it later.
                *
                */
                if (scale > r2)
                    scale = 0.8 - scale;
                else
                    scale = 1.0;
                z0 = 0.1 * (0.1 - temp*z);
                if (z0 < 0.0)  z0 = 0.0;
                temp = sqrt(temp*temp + z0*z0);
                scale = (r + r2 - temp) * scale / (temp + SMALL);
                scale = scale / (1 + z);
                sallyv->setX(ix, iy, iz, scale * (y - yc) + 0.1*(x - xc));
                sallyv->setY(ix, iy, iz, scale * -(x - xc) + 0.1*(y - yc));
                sallyv->setZ(ix, iy, iz, scale * z0);
            }
        }
    }
}

void load::genAbc(int xs, int ys, int zs, vecField *abcv)
{

    int ix, iy, iz;
    for (iz = 0; iz < zs; iz++)
    {
        for (iy = 0; iy < ys; iy++)
        {
            for (ix = 0; ix < xs; ix++)
            {
                abcv->setX(ix, iy, iz, mA*sin(iz) + mC*cos(iy));
                abcv->setY(ix, iy, iz, mB*sin(ix) + mA*cos(iz));
                abcv->setZ(ix, iy, iz, mC*sin(iy) + mB*cos(ix));
            }
        }
    }
}

bool load::readFile(int xs, int ys, int zs, vecField *vecf)
{

    //Open binary file for reading and read each character
    std::ifstream infile;
    QString qPath = this->ui->pathField->text();
    QByteArray ba = qPath.toLatin1();
    const char *path = ba.data();

    infile.open(path, std::ios::in | std::ios::binary);
    if (infile.is_open())
    {
        float addFact = 0.0f;
        float multFact = 1.0f;
        if (this->ui->modChBox->isChecked())
        {
            addFact = this->ui->DABox->value();
            multFact = this->ui->DMFBox->value();
        }
        unsigned char c = 0;
        float f = 0;

        //Loop through data X changes first/fastest.
        for (int iz = 0; iz < zs; iz++)
        {
            for (int iy = 0; iy < ys; iy++)
            {
                for (int ix = 0; ix < xs; ix++)
                {
                    //Vec vec;
                    //Read x then y then z and store in vec (vector)
                    //Data needs converting to float from char and adjusted
                    infile.read((char *)&c, 1);
                    f = (float)c;
                    vecf->setX(ix,iy,iz, ((f / 255) + addFact)*multFact);

                    infile.read((char *)&c, 1);
                    f = (float)c;
                    vecf->setY(ix,iy,iz, ((f / 255) + addFact)*multFact);

                    infile.read((char *)&c, 1);
                    f = (float)c;
                    vecf->setZ(ix,iy,iz, ((f / 255) + addFact)*multFact);


                    //speed[ix][iy][iz] = (sqrt(powf(vec.x,2)+powf(vec.y,2)+powf(vec.z,2)))*255;

                    // Find max speed in slice for colour scaling
                    //if (iz == ZLoc)
                    //{
                        //if (speed[ix][iy][iz] >maxSpeed) maxSpeed = speed[ix][iy][iz];

                    //}

                }
            }
        }
        //Close the file when finished
        infile.close();
        return true;
    }
    else
    {
        QMessageBox::warning(this,tr("Error"),tr("File name/path error"));
        return false;
    }
}



void load::on_aSpinBox_valueChanged(double arg1)
{
    mA = arg1;
}

void load::on_bSpinBox_valueChanged(double arg1)
{
    mB = arg1;
}

void load::on_cSpinBox_valueChanged(double arg1)
{
    mC = arg1;
}
