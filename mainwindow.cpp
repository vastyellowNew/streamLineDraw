#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "load.h"
#include "colourmap.h"
#include <stdlib.h>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->GLwidget, SIGNAL(rotate(float,float,float)), ui->widget, SLOT(rot(float,float,float)));
    QObject::connect(ui->widget, SIGNAL(resetRot(float,float)), ui->GLwidget, SLOT(resetRot(float,float)));
    QObject::connect(ui->widget, SIGNAL(resetZoom(float)), ui->GLwidget, SLOT(resetZoom(float)));
    QObject::connect(ui->LegGLWidget, SIGNAL(mapNum(int)), this, SLOT(mapNumber(int)));
    this->ui->HighL->hide();
    this->ui->LowL->hide();
    this->ui->MidL->hide();
    this->ui->LegL->hide();
    this->ui->ScalL->hide();
    this->ui->maxSlider->hide();
    this->ui->minSlider->hide();
    this->ui->LegGLWidget->hide();
    this->ui->hideLBox->setEnabled(false);
    this->ui->speedMBox->setEnabled(false);
    this->ui->helMBox->setEnabled(false);
    this->ui->helMagMBox->setEnabled(false);
    this->ui->rescaleBox->setEnabled(false);
    this ->ui->GLwidget->mTextureNo = 0;
    this ->ui->LegGLWidget->mTextureNo = 0;
    this->ui->GLwidget->mPCRibbons = 0;
    this->ui->progressSlider->setMaximum(0);
    this->ui->progressSlider->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_X_valueChanged(double arg1)
{
    if (arg1 >= mData->getXSize()-1)
    {
        this->ui->X->setValue(mData->getXSize() -2);
    }
    else
    {
        m_seed.setX(arg1);
        MainWindow::rake();
    }
}

void MainWindow::on_Y_valueChanged(double arg1)
{
    if (arg1 >= mData->getYSize()-1)
    {
        this->ui->Y->setValue(mData->getYSize() -2);
    }
    else
    {
        m_seed.setY(arg1);
        MainWindow::rake();
    }
}

void MainWindow::on_Z_valueChanged(double arg1)
{
    if (arg1 >= mData->getZSize()-1)
    {
        this->ui->Z->setValue(mData->getZSize() -2);
    }
    else
    {
        m_seed.setZ(arg1);
        MainWindow::rake();
    }
}

void MainWindow::on_loadData_released()
{
    load *loadData = new load(mData);
    loadData->setWindowTitle("Load Data");
    QObject::connect(loadData, SIGNAL(draw (bool)), this, SLOT(dataP()));
    loadData->setModal(true);
    loadData->exec();

    this->ui->GLwidget->mData = mData;
    this->ui->GLwidget->update();
}

void MainWindow::dataP()
{
    xSize = mData->getXSize();
    ySize = mData->getYSize();
    zSize = mData->getZSize();
    //Auto scale ribbon width and ribbon spacing
    int defaultFactor = (int)ceil((sqrt((xSize*xSize)+(ySize*ySize)+(zSize*zSize)))/60);
    ribWidth = 0.5*defaultFactor;
    this->ui->ribbonWBox->setValue(ribWidth);
    this->ui->GLwidget->mSphereRad = ribWidth;

    calc = false;
    speedF.clear();
    m_heliF.clear();
    m_heliMagF.clear();
    mheliMagFFast.clear();
    mSeedPoints.reset();
    mSeedPoints.setSize(xSize, ySize, zSize);
    //this->ui->GLwidget->mPCRibbons = 100;
    //this->ui->GLwidget->mNoOfRibbs = 100;
    //malloc to allocate memory size of speedField
    speedField=(float *) malloc(xSize*ySize*zSize*sizeof(float));

    for (int iz=0; iz < zSize; ++iz)
    {
        for (int iy=0; iy < ySize; ++iy)
        {
            for (int ix=0; ix < xSize; ++ix)
            {
                speedField[iz*xSize*ySize + iy*xSize +ix] = mData->getSpeed(ix,iy,iz);

                speedF.set(ix,iy,iz, mData->getSpeed(ix,iy,iz));
            }
        }
    }
    m_minScal = speedF.getLCS();
    m_maxScal = speedF.getUCS();
    m_modMaxScal = m_maxScal;
    m_modMinScal =m_minScal;
    this->ui->GLwidget->minScal = m_minScal;
    this->ui->GLwidget->maxScal = m_maxScal;
    this->ui->HighL->setText(QString::number(speedF.getUCS()));
    this->ui->LowL->setText(QString::number(speedF.getLCS()));
    this->ui->MidL->setText(QString::number(speedF.getLCS()+(speedF.getUCS() - speedF.getLCS())/2));
    this->ui->ScalL->setText("Speed");
    this->ui->HighL->show();
    this->ui->LowL->show();
    this->ui->MidL->show();
    this->ui->LegL->show();
    this->ui->ScalL->show();
    this->ui->LegGLWidget->show();
    this->ui->hideLBox->setEnabled(true);
    this->ui->speedMBox->setEnabled(true);
    this->ui->speedMBox->setChecked(true);
    this->ui->helMBox->setChecked(false);
    this->ui->helMBox->setEnabled(true);
    this->ui->helMagMBox->setChecked(false);
    this->ui->helMagMBox->setEnabled(true);
    this->ui->rescaleBox->setEnabled(true);
    MainWindow::rake();
    this->ui->GLwidget->m_dSw = true;
    this->ui->GLwidget->m_speedFGrad = speedFGrad;
    this->ui->GLwidget->update();
}

void MainWindow::calculations()
{

    //malloc to allocate memory size of m_curl
    m_curl=(float **) malloc(xSize*ySize*zSize*sizeof(float *));
    for(int i=0;i<xSize*ySize*zSize;i++)
        m_curl[i]=(float *) malloc(3*sizeof(float));


    m_heliField.setSize(xSize,ySize,zSize);

    speedFGrad->setSize(xSize, ySize, zSize);

    for (int iz=0; iz < zSize; ++iz)
    {
        for (int iy=0; iy < ySize; ++iy)
        {
            for (int ix=0; ix < xSize; ++ix)
            {
                speedFGrad->setX(ix, iy, iz, MainWindow::grad(ix,iy,iz,0));
                speedFGrad->setY(ix, iy, iz, MainWindow::grad(ix,iy,iz,1));
                speedFGrad->setZ(ix, iy, iz, MainWindow::grad(ix,iy,iz,2));

                m_curl[iz*xSize*ySize + iy*xSize +ix][0] = mData->WYgrad(ix,iy,iz) - mData->VZgrad(ix,iy,iz);
                m_curl[iz*xSize*ySize + iy*xSize +ix][1] = mData->UZgrad(ix,iy,iz) - mData->WXgrad(ix,iy,iz);
                m_curl[iz*xSize*ySize + iy*xSize +ix][2] = mData->VXgrad(ix,iy,iz) - mData->UYgrad(ix,iy,iz);

                float helicity = ((m_curl[iz*xSize*ySize + iy*xSize +ix][0])*mData->getXVec(ix,iy,iz)
                        +(m_curl[iz*xSize*ySize + iy*xSize +ix][1])*mData->getYVec(ix,iy,iz)
                        +(m_curl[iz*xSize*ySize + iy*xSize +ix][2])*mData->getZVec(ix,iy,iz));

                m_heliField.setValue(ix, iy, iz, helicity);

                m_heliF.set(ix,iy,iz,helicity);
                m_heliMagF.set(ix, iy, iz, fabs(helicity));
            }
        }
    }
    this->ui->GLwidget->m_heliField = m_heliField;
    m_heliMagF.convert(&mheliMagFFast);
    calc = true;
    this->ui->GLwidget->update();
}

float MainWindow::grad(int x, int y, int z, int pos)
{
//    int xSize = mData->getXSize();
//    int ySize = mData->getYSize();
//    int zSize = mData->getZSize();
    float Grad;
    float a1;
    float a2;
    //if at start edge of vector field, take forward difference
    if (pos == 0)
    {
        if (x == 0)
        {
            a1 = mData->getSpeed(x,y,z);
            a2 = mData->getSpeed(x+1,y,z);
            Grad = a2-a1;
        }
        //if at end of vector field, take bakward difference
        else if (x == xSize -1)
        {
            float a1 = mData->getSpeed(x-1,y,z);
            float a2 = mData->getSpeed(x,y,z);
            Grad = a2-a1;
        }
        //middle of vector field, take central difference
        else
        {
            float a1 = mData->getSpeed(x-1,y,z);
            float a2 = mData->getSpeed(x+1,y,z);
            Grad = (a2-a1)/2;
        }
    }
    else if (pos == 1)
    {
        if (y == 0)
        {
            a1 = mData->getSpeed(x,y,z);
            a2 = mData->getSpeed(x,y+1,z);
            Grad = a2-a1;
        }
        //if at end of vector field, take bakward difference
        else if (y == ySize -1)
        {
            float a1 = mData->getSpeed(x,y-1,z);
            float a2 = mData->getSpeed(x,y,z);
            Grad = a2-a1;
        }
        //middle of vector field, take central difference
        else
        {
            float a1 = mData->getSpeed(x,y-1,z);
            float a2 = mData->getSpeed(x,y+1,z);
            Grad = (a2-a1)/2;
        }
    }
    else if (pos == 2)
    {
        if (z == 0)
        {
            a1 = mData->getSpeed(x,y,z);
            a2 = mData->getSpeed(x,y,z+1);
            Grad = a2-a1;
        }
        //if at end of vector field, take bakward difference
        else if (z == zSize -1)
        {
            float a1 = mData->getSpeed(x,y,z-1);
            float a2 = mData->getSpeed(x,y,z);
            Grad = a2-a1;
        }
        //middle of vector field, take central difference
        else
        {
            float a1 = mData->getSpeed(x,y,z-1);
            float a2 = mData->getSpeed(x,y,z+1);
            Grad = (a2-a1)/2;
        }
    }
    return Grad;
}

void MainWindow::rake()
{
    //normalise vector coordinates
    float rX = this->ui->rX->value();
    float rY = this->ui->rY->value();
    float rZ = this->ui->rZ->value();
    float mag = sqrt(rX*rX + rY*rY + rZ*rZ);
    if (mag !=0)
    {
        rX=(rX/mag);
        rY=(rY/mag);
        rZ=(rZ/mag);
    }


    int nSeeds;
    if (this->ui->rLenBox->value() >0) nSeeds = this->ui->sNoBox->value();
    else nSeeds = 1;

    float xIt = rX*this->ui->rLenBox->value()/nSeeds;
    float yIt = rY*this->ui->rLenBox->value()/nSeeds;
    float zIt = rZ*this->ui->rLenBox->value()/nSeeds;

    this->ui->GLwidget->m_ribbon.clear();
    this->ui->GLwidget->mRibSeed.clear();
    for (int i =0; i < nSeeds; ++i)
    {
        if (m_seed.x() + i*xIt < 0 || m_seed.x() + i*xIt > xSize - 2) break;
        if (m_seed.y() + i*yIt < 0 || m_seed.y() + i*yIt > ySize - 2) break;
        if (m_seed.z() + i*zIt < 0 || m_seed.z() + i*zIt > zSize - 2) break;
        MainWindow::matRibbon(m_seed.x() + i*xIt, m_seed.y() + i*yIt, m_seed.z() + i*zIt);
        this->ui->GLwidget->m_ribbon.append(m_ribbon);
        this->ui->GLwidget->mRibSeed.append(QVector3D(m_seed.x() + i*xIt, m_seed.y() + i*yIt, m_seed.z() + i*zIt));
    }
    this->ui->GLwidget->update();

}

void MainWindow::matRibbon(float x, float y, float z)
{
    m_ribbon.clear();

    float seed[3] = {x, y, z};
    float norm[3] = {0.0f,0.0f,1.0f}; // Normal vector face
    int sw = 0; //Switch for first itteration of loop only
    bool forFin = false; //Switch for when forward integrator has finished
    bool firstBack = true;
    //Streamline
    for (int i = 0; i < this->ui->steps->value(); ++i)
    {
        //float* sVec = mData.getNVec(seed[0],seed[1],seed[2]);

        vecA pos;
        //forward integrator
        if (this->ui->comboBox->currentIndex() == 0)
        {
            //Early termination if line exceedes area
            if (seed[0] > xSize-1 || seed[0] < 0 ||
                    seed[1] > ySize-1 || seed[1] < 0 ||
                    seed[2] > zSize-1 || seed[2] < 0) break;
            pos = MainWindow::rk4(seed[0], seed[1], seed[2]);
            if (!pos.ok) break;
        }
        //backward integrator
        else if (this->ui->comboBox->currentIndex() == 1)
        {
            //Early termination if line exceedes area
            if (seed[0] > xSize-1 || seed[0] < 0 ||
                    seed[1] > ySize-1 || seed[1] < 0 ||
                    seed[2] > zSize-1 || seed[2] < 0) break;
            pos = MainWindow::rk4Back(seed[0], seed[1], seed[2]);
            if (!pos.ok) break;
        }
        //forward&backward integrator
        else
        {
            if (i < this->ui->steps->value()/2 && !forFin)
            {
                //Early termination if line exceedes area
                //if (backFin && forFin) break;
                if (seed[0] >= xSize-1 || seed[0] < 0 ||
                        seed[1] >= ySize-1 || seed[1] < 0 ||
                        seed[2] >= zSize-1 || seed[2] < 0) forFin = true;
                else pos = MainWindow::rk4(seed[0], seed[1], seed[2]);
                if (!pos.ok) forFin = true;

            }
            if (forFin || i >= this->ui->steps->value()/2 )
            {
                if (firstBack)
                {
                    seed[0] = x;
                    seed[1] = y;
                    seed[2] = z;
                    firstBack = false;
                    sw = 0; //reset rotation
                    norm[0] = 0.0;
                    norm[1] = 0.0;
                    norm[2] = 1.0;
                }
                //Early termination if line exceedes area
                if (seed[0] > xSize-1 || seed[0] < 0 ||
                        seed[1] > ySize-1 || seed[1] < 0 ||
                        seed[2] > zSize-1 || seed[2] < 0) break;
                //automatic stream ribbon stop if too close to another ribbon
                pos = MainWindow::rk4Back(seed[0], seed[1], seed[2]);
                if (!pos.ok) break;
            }
        }
        float inter[3] = {pos.x, pos.y, pos.z};
        float angle = pos.ang;

        /*
        norm[0] += (xVec*dt + 2*xVec2*dt + 2*xVec3*dt + xVec4)/6;
        norm[1] += (yVec*dt + 2*yVec2*dt + 2*yVec3*dt + yVec4)/6;
        norm[2] += (zVec*dt + 2*zVec2*dt + 2*zVec3*dt + zVec4)/6;
        */
        //float norm[3] = {0.0f,0.0f,1.0f};

        if (sw == 1) //Only run after first itteration. Turns the ribbon surface by angle
        {
            float normin = sqrt(inter[0]*inter[0] + inter[1]*inter[1] + inter[2]*inter[2]);
            if (normin ==0 || angle == 0) break; //Early termination
            float xTrn = inter[0]/normin;
            float yTrn = inter[1]/normin;
            float zTrn = inter[2]/normin;
            float cosa = cos(angle);
            float sina = sin(angle);
            float cosaM = 1 - cosa;

            /*rotation matrix = {cosa+xTrn^2*cosaM, xTrn*yTrn*cosaM-zTrn*sina, xTrn*zTrn*cosaM+yTrn*sina;
                                xTrn*yTrn*cosaM+zTrn*sina, cosa+yTrn^2*cosaM, yTrn*zTrn*cosaM-xTrn*sina;
                                xTrn*zTrn*cosaM-yTrn*sina, yTrn*zTrn*cosaM+xTrn*sina, cosa+zTrn^2*cosaM};
             Transposed!!
            */

            //Dot product norm matrix with rotation matrix
            float newX = norm[0]*(cosa+xTrn*xTrn*cosaM) +
                    norm[1]*(xTrn*yTrn*cosaM-zTrn*sina) +
                    norm[2]*(xTrn*zTrn*cosaM+yTrn*sina);
            float newY = norm[0]*(xTrn*yTrn*cosaM+zTrn*sina) +
                    norm[1]*(cosa+yTrn*yTrn*cosaM) +
                    norm[2]*(yTrn*zTrn*cosaM-xTrn*sina);
            float newZ = norm[0]*(xTrn*zTrn*cosaM-yTrn*sina) +
                    norm[1]*(yTrn*zTrn*cosaM+xTrn*sina) +
                    norm[2]*(cosa+zTrn*zTrn*cosaM);
            norm[0] = newX;
            norm[1] = newY;
            norm[2] = newZ;


        }
        //cross product norm and inter
        float norm2[3];
        norm2[0] = norm[2]*inter[1] - norm[1]*inter[2];
        norm2[1] = norm[0]*inter[2] - norm[2]*inter[0];
        norm2[2] = norm[1]*inter[0] - norm[0]*inter[1];

        if (sw == 0) //run first time to test if initial normal is ok
        {
            if (norm2[0] != 0 || norm2[1] != 0 || norm2[2] != 0 )
            {
                norm[0] = 1.0f;
                norm[2] = 0.0f;
                norm2[0] = norm[2]*inter[1] - norm[1]*inter[2];
                norm2[1] = norm[0]*inter[2] - norm[2]*inter[0];
                norm2[2] = norm[1]*inter[0] - norm[0]*inter[1];
            }
        }


        norm[0] = inter[2]*norm2[1] - inter[1]*norm2[2];
        norm[1] = inter[0]*norm2[2] - inter[2]*norm2[0];
        norm[2] = inter[1]*norm2[0] - inter[0]*norm2[1];

        float normb = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
        if (normb != 0)
        {
            norm[0] = norm[0]/normb;
            norm[1] = norm[1]/normb;
            norm[2] = norm[2]/normb;
        }

        if (!forFin) //Going forward add to end of list
        {
            m_ribbon.append(QVector3D((seed[0]+norm[0]*(ribWidth/2)), (seed[1]+norm[1]*(ribWidth/2)), (seed[2]+norm[2]*(ribWidth/2))));
            m_ribbon.append(QVector3D((seed[0]-norm[0]*(ribWidth/2)), (seed[1]-norm[1]*(ribWidth/2)), (seed[2]-norm[2]*(ribWidth/2))));
        }
        else //Going backward add to front of list
        {
            m_ribbon.prepend(QVector3D((seed[0]-norm[0]*(ribWidth/2)), (seed[1]-norm[1]*(ribWidth/2)), (seed[2]-norm[2]*(ribWidth/2))));
            m_ribbon.prepend(QVector3D((seed[0]+norm[0]*(ribWidth/2)), (seed[1]+norm[1]*(ribWidth/2)), (seed[2]+norm[2]*(ribWidth/2))));
        }
        this->ui->GLwidget->mNoOfRibbs = i*2;
        this->ui->progressSlider->setMaximum(i*2);
        //Move to next point
        seed[0] += inter[0];
        seed[1] += inter[1];
        seed[2] += inter[2];

        sw = 1;
    }

}

vecA MainWindow::rk4(float x, float y, float z)
{
    float xVec = mData->getXVec(x,y,z);
    float yVec = mData->getYVec(x,y,z);
    float zVec = mData->getZVec(x,y,z);
    vecA toReturn;
    toReturn.ok = false;

    //normalise
    float mag = sqrt(xVec*xVec + yVec*yVec + zVec*zVec);
    xVec = xVec/mag;
    yVec = yVec/mag;
    zVec = zVec/mag;

    /*
    //Euler
    seed[0] += xVec*dt;
    seed[1] += yVec*dt;
    seed[2] += zVec*dt;

    //Rung-Kutta RK2
    float inter[3];
    inter[0] = seed[0] +xVec*dt/2;
    inter[1] = seed[1] +yVec*dt/2;
    inter[2] = seed[2] +zVec*dt/2;

    xVec = mData.getXVec(inter[0],inter[1],inter[2]);
    yVec = mData.getYVec(inter[0],inter[1],inter[2]);
    zVec = mData.getZVec(inter[0],inter[1],inter[2]);

    seed[0] += xVec*dt;
    seed[1] += yVec*dt;
    seed[2] += zVec*dt;
*/
    //Rung-Kutta RK4
    float inter[4];

    inter[0] = x +xVec*dt/2;
    inter[1] = y +yVec*dt/2;
    inter[2] = z +zVec*dt/2;
    float angV = mData->angV(x,y,z);

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec2 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec2 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec2 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV2 = mData->angV(inter[0],inter[1],inter[2]);
    mag = sqrt(xVec2*xVec2 + yVec2*yVec2 + zVec2*zVec2);
    xVec2 = xVec2/mag;
    yVec2 = yVec2/mag;
    zVec2 = zVec2/mag;

    inter[0] = x +xVec2*dt/2;
    inter[1] = y +yVec2*dt/2;
    inter[2] = z +zVec2*dt/2;

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec3 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec3 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec3 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV3 = mData->angV(inter[0],inter[1],inter[2]);

    mag = sqrt(xVec3*xVec3 + yVec3*yVec3 + zVec3*zVec3);
    xVec3 = xVec3/mag;
    yVec3 = yVec3/mag;
    zVec3 = zVec3/mag;

    inter[0] = x +xVec3*dt;
    inter[1] = y +yVec3*dt;
    inter[2] = z +zVec3*dt;

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec4 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec4 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec4 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV4 = mData->angV(inter[0],inter[1],inter[2]);

    mag = sqrt(xVec4*xVec4 + yVec4*yVec4 + zVec4*zVec4);
    xVec4 = xVec4/mag;
    yVec4 = yVec4/mag;
    zVec4 = zVec4/mag;

    inter[0] = (xVec*dt + 2*xVec2*dt + 2*xVec3*dt + xVec4*dt)/6;
    inter[1] = (yVec*dt + 2*yVec2*dt + 2*yVec3*dt + yVec4*dt)/6;
    inter[2] = (zVec*dt + 2*zVec2*dt + 2*zVec3*dt + zVec4*dt)/6;

    //normalise
    mag = sqrt(inter[0]*inter[0] + inter[1]*inter[1] + inter[2]*inter[2]);
    inter[0] = inter[0]/mag;
    inter[1] = inter[1]/mag;
    inter[2] = inter[2]/mag;

    //Calculated angle rotated using RK4?
    inter[3] = (angV*dt + 2*angV2*dt + 2*angV3*dt + angV4*dt)/6;

    toReturn.x = inter[0];
    toReturn.y = inter[1];
    toReturn.z = inter[2];
    toReturn.ang = inter[3];
    toReturn.ok = true;
    return toReturn;
}

vecA MainWindow::rk4Back(float x, float y, float z)
{
    float xVec = mData->getXVec(x,y,z);
    float yVec = mData->getYVec(x,y,z);
    float zVec = mData->getZVec(x,y,z);
    vecA toReturn;
    toReturn.ok = false;

    //normalise
    float mag = sqrt(xVec*xVec + yVec*yVec + zVec*zVec);
    xVec = -xVec/mag;
    yVec = -yVec/mag;
    zVec = -zVec/mag;

    /*
    //Euler
    seed[0] += xVec*dt;
    seed[1] += yVec*dt;
    seed[2] += zVec*dt;

    //Rung-Kutta RK2
    float inter[3];
    inter[0] = seed[0] +xVec*dt/2;
    inter[1] = seed[1] +yVec*dt/2;
    inter[2] = seed[2] +zVec*dt/2;

    xVec = mData.getXVec(inter[0],inter[1],inter[2]);
    yVec = mData.getYVec(inter[0],inter[1],inter[2]);
    zVec = mData.getZVec(inter[0],inter[1],inter[2]);

    seed[0] += xVec*dt;
    seed[1] += yVec*dt;
    seed[2] += zVec*dt;
*/
    //Rung-Kutta RK4
    float inter[4];

    inter[0] = x +xVec*dt/2;
    inter[1] = y +yVec*dt/2;
    inter[2] = z +zVec*dt/2;
    float angV = mData->angV(x,y,z);

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec2 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec2 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec2 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV2 = mData->angV(inter[0],inter[1],inter[2]);
    //normalise
    mag = sqrt(xVec2*xVec2 + yVec2*yVec2 + zVec2*zVec2);
    xVec2 = -xVec2/mag;
    yVec2 = -yVec2/mag;
    zVec2 = -zVec2/mag;

    inter[0] = x +xVec2*dt/2;
    inter[1] = y +yVec2*dt/2;
    inter[2] = z +zVec2*dt/2;

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec3 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec3 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec3 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV3 = mData->angV(inter[0],inter[1],inter[2]);
    //normalise
    mag = sqrt(xVec3*xVec3 + yVec3*yVec3 + zVec3*zVec3);
    xVec3 = -xVec3/mag;
    yVec3 = -yVec3/mag;
    zVec3 = -zVec3/mag;

    inter[0] = x +xVec3*dt;
    inter[1] = y +yVec3*dt;
    inter[2] = z +zVec3*dt;

    if (inter[0] < 0 || inter[0] >=xSize-1) return toReturn;
    if (inter[1] < 0 || inter[1] >=ySize-1) return toReturn;
    if (inter[2] < 0 || inter[2] >=zSize-1) return toReturn;

    float xVec4 = mData->getXVec(inter[0],inter[1],inter[2]);
    float yVec4 = mData->getYVec(inter[0],inter[1],inter[2]);
    float zVec4 = mData->getZVec(inter[0],inter[1],inter[2]);
    float angV4 = mData->angV(inter[0],inter[1],inter[2]);
    //normalise
    mag = sqrt(xVec4*xVec4 + yVec4*yVec4 + zVec4*zVec4);
    xVec4 = -xVec4/mag;
    yVec4 = -yVec4/mag;
    zVec4 = -zVec4/mag;

    inter[0] = (xVec*dt + 2*xVec2*dt + 2*xVec3*dt + xVec4*dt)/6;
    inter[1] = (yVec*dt + 2*yVec2*dt + 2*yVec3*dt + yVec4*dt)/6;
    inter[2] = (zVec*dt + 2*zVec2*dt + 2*zVec3*dt + zVec4*dt)/6;

    //normalise
    mag = sqrt(inter[0]*inter[0] + inter[1]*inter[1] + inter[2]*inter[2]);
    inter[0] = inter[0]/mag;
    inter[1] = inter[1]/mag;
    inter[2] = inter[2]/mag;

    //Calculated angle rotated using RK4?
    inter[3] = -(angV*dt + 2*angV2*dt + 2*angV3*dt + angV4*dt)/6;

    toReturn.x = inter[0];
    toReturn.y = inter[1];
    toReturn.z = inter[2];
    toReturn.ang = inter[3];
    toReturn.ok = true;
    return toReturn;
}

void MainWindow::on_rLenBox_valueChanged(QString arg1)
{
    Q_UNUSED(arg1)
    MainWindow::rake();
}

void MainWindow::on_sNoBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    MainWindow::rake();
}

void MainWindow::on_rX_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    MainWindow::rake();
}

void MainWindow::on_rY_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    MainWindow::rake();
}

void MainWindow::on_rZ_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    MainWindow::rake();
}

void MainWindow::on_speedMBox_clicked(bool checked)
{
    if (checked)
    {
        this->ui->helMBox->setChecked(false);
        this->ui->helMagMBox->setChecked(false);
        this->ui->ScalL->setText("Speed");
        this->ui->GLwidget->m_colourHeli = 0;
        m_minScal = speedF.getLCS();
        m_maxScal = speedF.getUCS();
        this->ui->GLwidget->minScal = m_minScal;
        this->ui->GLwidget->maxScal = m_maxScal;
        this->ui->HighL->setText(QString::number(m_maxScal));
        this->ui->LowL->setText(QString::number(m_minScal));
        this->ui->MidL->setText(QString::number(m_minScal+(m_maxScal - m_minScal)/2));
        this->ui->rescaleBox->setEnabled(true);
        if (this->ui->rescaleBox->isChecked())
        {
            this->ui->maxSlider->show();
            this->ui->maxSlider->setValue(100);
            this->ui->minSlider->show();
            this->ui->minSlider->setValue(1);
        }
        if (!this->ui->hideLBox->isChecked())
        {
            this->ui->HighL->show();
            this->ui->LowL->show();
            this->ui->MidL->show();
            this->ui->LegL->show();
            this->ui->ScalL->show();
            this->ui->LegGLWidget->show();
         }
    }
    else
    {
        this->ui->GLwidget->minScal = - std::numeric_limits<double>::infinity();
        this->ui->GLwidget->maxScal = - std::numeric_limits<double>::infinity();;
        this->ui->HighL->hide();
        this->ui->LowL->hide();
        this->ui->MidL->hide();
        this->ui->LegL->hide();
        this->ui->ScalL->hide();
        this->ui->LegGLWidget->hide();
        this->ui->maxSlider->hide();
        this->ui->minSlider->hide();
        this->ui->rescaleBox->setEnabled(false);
        this->ui->GLwidget->m_colourHeli = 0;
    }
    this->ui->GLwidget->update();
}

void MainWindow::on_helMBox_clicked(bool checked)
{
    if (checked)
    {
        this->ui->speedMBox->setChecked(false);
        this->ui->helMagMBox->setChecked(false);
        if (!calc)
        {
            MainWindow::calculations();
            calc = true;
        }
        this->ui->ScalL->setText("Helicity");
        this->ui->GLwidget->m_colourHeli = 1;
        m_minScal = m_heliF.getLCS();
        m_maxScal = m_heliF.getUCS();
        this->ui->GLwidget->minScal = m_minScal;
        this->ui->GLwidget->maxScal = m_maxScal;
        this->ui->HighL->setText(QString::number(m_maxScal));
        this->ui->LowL->setText(QString::number(m_minScal));
        this->ui->MidL->setText(QString::number(m_minScal+(m_maxScal - m_minScal)/2));
        if (!this->ui->hideLBox->isChecked())
        {
            this->ui->HighL->show();
            this->ui->LowL->show();
            this->ui->MidL->show();
            this->ui->LegL->show();
            this->ui->ScalL->show();
            this->ui->LegGLWidget->show();
        }
        this->ui->rescaleBox->setEnabled(true);
        if (this->ui->rescaleBox->isChecked())
        {
            this->ui->maxSlider->show();
            this->ui->maxSlider->setValue(100);
            this->ui->minSlider->show();
            this->ui->minSlider->setValue(1);
        }
    }
    else
    {
        this->ui->GLwidget->minScal = - std::numeric_limits<double>::infinity();
        this->ui->GLwidget->maxScal = - std::numeric_limits<double>::infinity();;
        this->ui->HighL->hide();
        this->ui->LowL->hide();
        this->ui->MidL->hide();
        this->ui->LegL->hide();
        this->ui->ScalL->hide();
        this->ui->LegGLWidget->hide();
        this->ui->maxSlider->hide();
        this->ui->minSlider->hide();
        this->ui->rescaleBox->setEnabled(false);
        this->ui->GLwidget->m_colourHeli = 0;
    }
    this->ui->GLwidget->update();
}

void MainWindow::on_hideLBox_clicked(bool checked)
{
    if (checked)
    {
        this->ui->HighL->hide();
        this->ui->LowL->hide();
        this->ui->MidL->hide();
        this->ui->LegL->hide();
        this->ui->ScalL->hide();
        this->ui->LegGLWidget->hide();
    }
    else if (!checked && (this->ui->speedMBox->isChecked() || this->ui->helMBox->isChecked() || this->ui->helMagMBox->isChecked()))
    {
        this->ui->HighL->show();
        this->ui->LowL->show();
        this->ui->MidL->show();
        this->ui->LegL->show();
        this->ui->ScalL->show();        
        this->ui->LegGLWidget->show();
        if (this->ui->rescaleBox->isChecked())
        {
            this->ui->maxSlider->show();
            this->ui->minSlider->show();
        }
    }
    this->ui->GLwidget->update();
}

void MainWindow::on_helMagMBox_clicked(bool checked)
{
    if (checked)
    {
        this->ui->speedMBox->setChecked(false);
        this->ui->helMBox->setChecked(false);
        if (!calc)
        {
            MainWindow::calculations();
            calc = true;
        }
        this->ui->ScalL->setText("Helicity Mag");
        this->ui->GLwidget->m_colourHeli = 2;
        m_minScal = m_heliMagF.getLCS();
        m_maxScal = m_heliMagF.getUCS();
        this->ui->GLwidget->minScal = m_minScal;
        this->ui->GLwidget->maxScal = m_maxScal;
        this->ui->HighL->setText(QString::number(m_maxScal));
        this->ui->LowL->setText(QString::number(m_minScal));
        this->ui->MidL->setText(QString::number(m_minScal+(m_maxScal - m_minScal)/2));
        if (!this->ui->hideLBox->isChecked())
        {
            this->ui->HighL->show();
            this->ui->LowL->show();
            this->ui->MidL->show();
            this->ui->LegL->show();
            this->ui->ScalL->show();
            this->ui->LegGLWidget->show();
        }
        this->ui->rescaleBox->setEnabled(true);
        if (this->ui->rescaleBox->isChecked())
        {
            this->ui->maxSlider->show();
            this->ui->maxSlider->setValue(100);
            this->ui->minSlider->show();
            this->ui->minSlider->setValue(1);
        }
    }
    else
    {
        this->ui->GLwidget->minScal = - std::numeric_limits<double>::infinity();
        this->ui->GLwidget->maxScal = - std::numeric_limits<double>::infinity();;
        this->ui->HighL->hide();
        this->ui->LowL->hide();
        this->ui->MidL->hide();
        this->ui->LegL->hide();
        this->ui->ScalL->hide();
        this->ui->LegGLWidget->hide();
        this->ui->maxSlider->hide();
        this->ui->minSlider->hide();
        this->ui->rescaleBox->setEnabled(false);
        this->ui->GLwidget->m_colourHeli = 0;
    }
    this->ui->GLwidget->update();
}

void MainWindow::on_maxSlider_valueChanged(int value)
{
    if (value < this->ui->minSlider->value()) this->ui->minSlider->setValue(value-1);
    m_modMaxScal  = m_minScal+((m_maxScal-m_minScal)/100)*value;
    this->ui->GLwidget->maxScal = m_modMaxScal;
    this->ui->HighL->setText(QString::number(m_modMaxScal));
    this->ui->MidL->setText(QString::number(m_modMinScal+(m_modMaxScal - m_modMinScal)/2));
    this->ui->GLwidget->update();
}

void MainWindow::on_minSlider_valueChanged(int value)
{
    if (value > this->ui->maxSlider->value()) this->ui->maxSlider->setValue(value+1);
    m_modMinScal  = m_minScal+((m_maxScal-m_minScal)/100)*value;
    this->ui->GLwidget->minScal = m_modMinScal;
    this->ui->LowL->setText(QString::number(m_modMinScal));
    this->ui->MidL->setText(QString::number(m_modMinScal+(m_modMaxScal - m_modMinScal)/2));
    this->ui->GLwidget->update();
}

void MainWindow::on_rescaleBox_clicked(bool checked)
{
    if (checked)
    {
        this->ui->maxSlider->show();
        this->ui->minSlider->show();
    }
    else
    {
        this->ui->maxSlider->hide();
        this->ui->minSlider->hide();
        this->ui->maxSlider->setValue(100);
        this->ui->minSlider->setValue(1);
        this->ui->GLwidget->minScal = m_minScal;
        this->ui->GLwidget->maxScal = m_maxScal;
        this->ui->HighL->setText(QString::number(m_maxScal));
        this->ui->LowL->setText(QString::number(m_minScal));
        this->ui->MidL->setText(QString::number(m_minScal+(m_maxScal - m_minScal)/2));
    }
    this->ui->GLwidget->update();
}

//void MainWindow::on_autoBox_clicked(bool checked)
//{
//        if (!calc) MainWindow::calculations();

//        mSeedPoints.reset();
//        this->ui->GLwidget->m_ribbon.clear();
//        this->ui->GLwidget->mRibSeed.clear();
//        m_ribbon.clear();

//            int noOfRibbons = 0;
//            for (int i =0; i < mheliMagFFast.size(); ++i)
//            {
//                QVector3D heliPos = mheliMagFFast[i];
//                if (mSeedPoints.isClear(heliPos.x(), heliPos.y(), heliPos.z()))
//                {
//                    MainWindow::matRibbon(heliPos.x(), heliPos.y(), heliPos.z());
//                    if (m_ribbon.size() < mRibbonMinLength)
//                    {
//                        mSeedPoints.clearTemp();
//                        m_ribbon.clear();
//                    }
//                    else
//                    {
//                        this->ui->GLwidget->m_ribbon.append(m_ribbon);
//                        this->ui->GLwidget->mRibSeed.append(QVector3D(heliPos.x(), heliPos.y(), heliPos.z()));
//                        ++noOfRibbons;
//                        mSeedPoints.newRibbon();
//                        m_ribbon.clear();
//                    }
//                }
//            }
//            this->ui->GLwidget->mNoOfRibbs = noOfRibbons;
//            this->ui->totalRibbons->setMaximum(noOfRibbons);
//            this->ui->totalRibbons->setValue(noOfRibbons);
//            m_ribbon.clear();

//    this->ui->GLwidget->update();
//}

void MainWindow::mapNumber(int choice)
{
    this ->ui->GLwidget->mTextureNo = choice;
    this->ui->GLwidget->update();
    this ->ui->LegGLWidget->mTextureNo = choice;
    this->ui->LegGLWidget->update();
}

void MainWindow::on_pushButton_2_clicked()
{
    ribWidth = this->ui->ribbonWBox->value();
    this->ui->GLwidget->mSphereRad = ribWidth;
    dt = this->ui->dt->value();
    MainWindow::rake();
}

void MainWindow::on_showSeedsBox_clicked(bool checked)
{
    this->ui->GLwidget->mDrawSeed = checked;
    this->ui->GLwidget->update();
}

void MainWindow::on_progressSlider_valueChanged(int value)
{
    this->ui->GLwidget->mPCRibbons = value;
    this->ui->GLwidget->update();
}

void MainWindow::on_forwardButt_clicked()
{
    this->ui->progressSlider->setValue(this->ui->progressSlider->value()+1);
}

void MainWindow::on_backButt_clicked()
{
    this->ui->progressSlider->setValue(this->ui->progressSlider->value()-1);
}
