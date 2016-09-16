#include "glwidget.h"
#include <QOpenGLWidget>
#include <GL/glut.h>
#include <stdio.h>
#include <QMouseEvent>
#include <tuple>


GLuint textureList;

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{


}

//Colour texture from http://www.paraview.org/ParaView/index.php/Default_Color_Map
void LoadAllTextures(int choice)
{
    ColourMap map;
    map.setMap(choice);

    glNewList(textureList = glGenLists(1), GL_COMPILE);
    glEnable(GL_TEXTURE_1D);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, map.getSize(), 0, GL_RGB, GL_FLOAT,map.getMap());
    glEndList();
}

void GLWidget::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);



    glEnable(GL_COLOR_MATERIAL);
    GLfloat material_specular[] = {0.9f, 0.8f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 30);

}

void GLWidget::paintGL()
{
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
    GLWidget::setLight();
    LoadAllTextures(mTextureNo);

    if (m_dSw)
    {
        xSize = mData->getXSize();
        ySize = mData->getYSize();
        zSize = mData->getZSize();



        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


        float edge = sqrt((xSize/2)*(xSize/2)+(ySize/2)*(ySize/2)+(zSize/2)*(zSize/2));
        GLdouble diam = (edge+1)*m_zoomFactor;
        GLdouble zNear = 0.1;
        GLdouble zFar = zSize+2*diam;//zNear + diam;
        GLdouble left = xSize/2 - diam;
        GLdouble right = xSize/2 + diam;
        GLdouble bottom = ySize/2 - diam;
        GLdouble top = ySize/2 + diam;

        GLdouble aspect = (GLdouble) this->widthMM() / this->heightMM();

        if ( aspect < 1.0 ) { // window taller than wide
           bottom /= aspect;
           top /= aspect;
        } else {
           left *= aspect;
           right *= aspect;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(left, right, bottom, top, zNear, zFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt( 0+camDelta[0],0+camDelta[1],zSize+diam, 0+lookAtDelta[0],0+lookAtDelta[1],0, 0,1,0 );

        //rotation
        glTranslatef(xSize/2,ySize/2,zSize/2);
        glRotatef(xRot/16.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot/16.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(zRot/16.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-xSize/2,-ySize/2,-zSize/2);

        GLWidget::drawFrame();

        glEnable(GL_TEXTURE_1D);
        glCallList(textureList);
        double sFactor = 1/(maxScal-minScal);
        float colF;

//        GLWidget::twoRibbon();
        int noOfRibbons = (int)(m_ribbon[0].length()*mPCRibbons/mNoOfRibbs);

        for (int i = 0; i <m_ribbon.length() ; ++i)
        {
                if (mRibSeed.length() > 0 && mDrawSeed)
                {
                    glDisable(GL_TEXTURE_1D);
                    glColor3f(0,0,0);
                    glTranslatef(mRibSeed[i].x(), mRibSeed[i].y() , mRibSeed[i].z());
                    glutSolidSphere(mSphereRad,8,8);
                    glTranslatef(-mRibSeed[i].x(), -mRibSeed[i].y() , -mRibSeed[i].z());
                    glColor3f(1,1,1);
                    glEnable(GL_TEXTURE_1D);
                }
                float xLoc;
                float yLoc;
                float zLoc;
                glBegin(GL_LINE_STRIP);
                for (int j = 0; j <noOfRibbons; j+=2)
                {
                    xLoc = m_ribbon[i][j].x() + ((m_ribbon[i][j+1].x() - m_ribbon[i][j].x())/2);
                    yLoc = m_ribbon[i][j].y() + ((m_ribbon[i][j+1].y() - m_ribbon[i][j].y())/2);
                    zLoc = m_ribbon[i][j].z() + ((m_ribbon[i][j+1].z() - m_ribbon[i][j].z())/2);
                    //GLWidget::drawBox(floor(xLoc), floor(yLoc), floor(zLoc) );
                    if (m_colourHeli ==1) colF = ((m_heliField.getValue(xLoc, yLoc, zLoc)-minScal)*sFactor);
                    else if (m_colourHeli ==2) colF = ((fabs(m_heliField.getValue(xLoc, yLoc, zLoc))-minScal)*sFactor);
                    else colF = ((mData->getSpeed(xLoc,yLoc,zLoc)-minScal)*sFactor);
                    if (colF > 1 ) colF = 1;
                    if (colF < 0 ) colF = 0;
                    glTexCoord1f(colF);
                    glVertex3f(xLoc, yLoc, zLoc);
                }
                glEnd();
                if (noOfRibbons +1 < m_ribbon[0].length())
                {/*
                    float xLoc = m_ribbon[0][noOfRibbons].x() + ((m_ribbon[0][noOfRibbons+1].x() - m_ribbon[0][noOfRibbons].x())/2);
                    float yLoc = m_ribbon[0][noOfRibbons].y() + ((m_ribbon[0][noOfRibbons+1].y() - m_ribbon[0][noOfRibbons].y())/2);
                    float zLoc = m_ribbon[0][noOfRibbons].z() + ((m_ribbon[0][noOfRibbons+1].z() - m_ribbon[0][noOfRibbons].z())/2);*/
                    GLWidget::drawBox(floor(xLoc), floor(yLoc), floor(zLoc) );
                }

         }

    }

}

void GLWidget::drawFrame()
{
    glDisable(GL_TEXTURE_1D);

    glColor3f(0,0,0);
    glLineWidth(2);
    //glutSolidSphere(1,20,20);

    // Draw single continious line as outline:
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f );
    glVertex3f(xSize, 0.0f, 0.0f);
    glVertex3f(xSize, ySize, 0.0f);
    glVertex3f(0.0f, ySize, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, zSize);
    glVertex3f(xSize, 0.0f, zSize);
    glVertex3f(xSize, ySize, zSize);
    glVertex3f(0.0f, ySize, zSize);
    glVertex3f(0.0f, 0.0f, zSize);
    glEnd(); // GL_LINES

    //Add extraedges to complete outline
    glBegin(GL_LINES);
    glVertex3f(xSize, 0.0f, 0.0f);
    glVertex3f(xSize, 0.0f, zSize);
    glVertex3f(xSize, ySize, 0.0f);
    glVertex3f(xSize, ySize, zSize);
    glVertex3f(0.0f, ySize, 0.0f);
    glVertex3f(0.0f, ySize, zSize);
    glEnd();

    glColor3f(1,1,1);
    glLineWidth(2);
}

void GLWidget::drawBox(int x, int y, int z)
{
    glDisable(GL_TEXTURE_1D);

    glColor3f(0,0,0);
    glLineWidth(2);
    //glutSolidSphere(1,20,20);

    // Draw single continious line as outline:
    glBegin(GL_LINE_STRIP);
    glVertex3f(x, y, z );
    glVertex3f(x+1, y, z);
    glVertex3f(x+1, y+1, z);
    glVertex3f(x, y+1, z);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z+1);
    glVertex3f(x+1, y, z+1);
    glVertex3f(x+1, y+1, z+1);
    glVertex3f(x, y+1, z+1);
    glVertex3f(x, y, z+1);
    glEnd(); // GL_LINES

    //Add extraedges to complete outline
    glBegin(GL_LINES);
    glVertex3f(x+1, y, z);
    glVertex3f(x+1, y, z+1);
    glVertex3f(x+1, y+1, z);
    glVertex3f(x+1, y+1, z+1);
    glVertex3f(x, y+1, z);
    glVertex3f(x, y+1, z+1);
    glEnd();

    glColor3f(1,1,1);
    glLineWidth(2);
}

//void GLWidget::twoRibbon()
//{
//    m_ribbon.clear();

//    float seed[3] = {m_seed.x() , m_seed.y(), m_seed.z()};

//    vecA pos = GLWidget::rk4(seed[0], seed[1], seed[2]);
//    if (!pos.ok) return;
//    float inter[3] = {pos.x, pos.y, pos.z};
//    float norm[3] = {0.0f,0.0f,1.0f}; // Normal vector face

//    //cross product norm and inter
//    float norm2[3];
//    norm2[0] = norm[2]*inter[1] - norm[1]*inter[2];
//    norm2[1] = norm[0]*inter[2] - norm[2]*inter[0];
//    norm2[2] = norm[1]*inter[0] - norm[0]*inter[1];

//    if (norm2[0] != 0 || norm2[1] != 0 || norm2[2] != 0 )
//    {
//        norm[0] = 1.0f;
//        norm[2] = 0.0f;
//        norm2[0] = norm[2]*inter[1] - norm[1]*inter[2];
//        norm2[1] = norm[0]*inter[2] - norm[2]*inter[0];
//        norm2[2] = norm[1]*inter[0] - norm[0]*inter[1];
//    }

//    norm[0] = inter[2]*norm2[1] - inter[1]*norm2[2];
//    norm[1] = inter[0]*norm2[2] - inter[2]*norm2[0];
//    norm[2] = inter[1]*norm2[0] - inter[0]*norm2[1];

//    float normb = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
//    if (normb != 0)
//    {
//        norm[0] = norm[0]/normb;
//        norm[1] = norm[1]/normb;
//        norm[2] = norm[2]/normb;
//    }
//    float ribSeed[3] = {(seed[0]+norm[0]*ribWidth), (seed[1]+norm[1]*ribWidth), (seed[2]+norm[2]*ribWidth)};

//    //Early termination if ribbon exceedes area
//    if (ribSeed[0] > xSize-1 || ribSeed[0] < 0 ||
//            ribSeed[1] > ySize-1 || ribSeed[1] < 0 ||
//            ribSeed[2] > zSize-1 || ribSeed[2] < 0) return;

//    //Streamline
//    for (int i = 0; i < steps; ++i)
//    {
//        //add seed point
//        m_ribbon.append(QVector3D(seed[0], seed[1], seed[2]));
//        //add ribbon edge
//        m_ribbon.append(QVector3D(ribSeed[0], ribSeed[1], ribSeed[2]));

//        //next streamline position
//        pos = GLWidget::rk4(seed[0], seed[1], seed[2]);
//        if (!pos.ok) break;

//        //Move to next point
//        seed[0] += pos.x;
//        seed[1] += pos.y;
//        seed[2] += pos.z;

//        //Early termination if line exceedes area
//        if (seed[0] > xSize-1 || seed[0] < 0 ||
//                seed[1] > ySize-1 || seed[1] < 0 ||
//                seed[2] > zSize-1 || seed[2] < 0) break;

//        vecA ribVec = GLWidget::rk4(ribSeed[0], ribSeed[1], ribSeed[2]);
//        if (!ribVec.ok) break;

//        //Move to next point of ribbon edge
//        ribSeed[0] += ribVec.x;
//        ribSeed[1] += ribVec.y;
//        ribSeed[2] += ribVec.z;

//        //find vector between new streamline point and new ribbon point
//        float xdiff = ribSeed[0]-seed[0];
//        float ydiff = ribSeed[1]-seed[1];
//        float zdiff = ribSeed[2]-seed[2];
//        //normalize vector
//        float diffMag = sqrt(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
//        xdiff = xdiff/diffMag;
//        ydiff = ydiff/diffMag;
//        zdiff = zdiff/diffMag;
//        //multiply by ribbon width to get new point
//        ribSeed[0] = seed[0] + xdiff*ribWidth;
//        ribSeed[1] = seed[1] + ydiff*ribWidth;
//        ribSeed[2] = seed[2] + zdiff*ribWidth;

//        //Early termination if ribbon exceedes area
//        if (ribSeed[0] > xSize-1 || ribSeed[0] < 0 ||
//                ribSeed[1] > ySize-1 || ribSeed[1] < 0 ||
//                ribSeed[2] > zSize-1 || ribSeed[2] < 0) break;

//    }

//}

//vecA GLWidget::rk45(float x, float y, float z)
//{
//    float xVec = mData->getXVec(x,y,z);
//    float yVec = mData->getYVec(x,y,z);
//    float zVec = mData->getZVec(x,y,z);

//    //normalise
//    float mag = sqrt(xVec*xVec + yVec*yVec + zVec*zVec);
//    yVec = yVec/mag;
//    yVec = yVec/mag;
//    zVec = zVec/mag;

//    /
//    //Euler
//    seed[0] += xVec*dt;
//    seed[1] += yVec*dt;
//    seed[2] += zVec*dt;

//    //Rung-Kutta RK2
//    float inter[3];
//    inter[0] = seed[0] +xVec*dt/2;
//    inter[1] = seed[1] +yVec*dt/2;
//    inter[2] = seed[2] +zVec*dt/2;

//    xVec = mData.getXVec(inter[0],inter[1],inter[2]);
//    yVec = mData.getYVec(inter[0],inter[1],inter[2]);
//    zVec = mData.getZVec(inter[0],inter[1],inter[2]);

//    seed[0] += xVec*dt;
//    seed[1] += yVec*dt;
//    seed[2] += zVec*dt;

//    //Rung-Kutta-Fehlberg RK45
//    float inter[3];

//    inter[0] = seed[0] +xVec*dt/4;
//    inter[1] = seed[1] +yVec*dt/4;
//    inter[2] = seed[2] +zVec*dt/4;
//    float angV = mData.angV(seed[0],seed[1],seed[2]);

//    float xVec2 = mData.getXVec(inter[0],inter[1],inter[2]);
//    float yVec2 = mData.getYVec(inter[0],inter[1],inter[2]);
//    float zVec2 = mData.getZVec(inter[0],inter[1],inter[2]);
//    float angV2 = mData.angV(inter[0],inter[1],inter[2]);

//    inter[0] = seed[0] +xVec2*dt/2;
//    inter[1] = seed[1] +yVec2*dt/2;
//    inter[2] = seed[2] +zVec2*dt/2;

//    float xVec3 = mData.getXVec(inter[0],inter[1],inter[2]);
//    float yVec3 = mData.getYVec(inter[0],inter[1],inter[2]);
//    float zVec3 = mData.getZVec(inter[0],inter[1],inter[2]);
//    float angV3 = mData.angV(inter[0],inter[1],inter[2]);

//    inter[0] = seed[0] +xVec3*dt;
//    inter[1] = seed[1] +yVec3*dt;
//    inter[2] = seed[2] +zVec3*dt;

//    float xVec4 = mData.getXVec(inter[0],inter[1],inter[2]);
//    float yVec4 = mData.getYVec(inter[0],inter[1],inter[2]);
//    float zVec4 = mData.getZVec(inter[0],inter[1],inter[2]);
//    float angV4 = mData.angV(inter[0],inter[1],inter[2]);

//    inter[0] = (xVec*dt + 2*xVec2*dt + 2*xVec3*dt + xVec4)/6;
//    inter[1] = (yVec*dt + 2*yVec2*dt + 2*yVec3*dt + yVec4)/6;
//    inter[2] = (zVec*dt + 2*zVec2*dt + 2*zVec3*dt + zVec4)/6;

//    //Calculated angle rotated using RK4?
//    float angle = (angV*dt + 2*angV2*dt + 2*angV3*dt + angV4)/6;

//    //Calculated angle rotated using RK4?
//    inter[3] = (angV*dt + 2*angV2*dt + 2*angV3*dt + angV4)/6;
//    vecA toReturn;
//    toReturn.x = inter[0];
//    toReturn.y = inter[1];
//    toReturn.z = inter[2];
//    toReturn.zng = inter[3];
//    return toReturn;
//}
//

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, (float)w/h, 0.01, 100.0);

    update();
}

void GLWidget::dSw(bool sw1)
{
    m_dSw = sw1;
}

void GLWidget::resetRot(float xORot, float yORot)
{
    xRot = xORot;
    yRot = yORot;
    zRot = 0;
    update();
}

void GLWidget::resetZoom(float zoomFactor)
{
    m_zoomFactor = zoomFactor;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = (event->x() - lastPos.x());
    float dy = (event->y() - lastPos.y());

    if (event->buttons() &Qt::LeftButton)
    {
        camDelta[0] -= dx/100.0;
        camDelta[1] += dy/100.0;
        lookAtDelta[0] -= dx/100.0;
        lookAtDelta[1] += dy/100.0;

    }

    else if (event->buttons() &Qt::RightButton)
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            zRot += 8*dx;
        }
        else
        {
            xRot += 8*dy;
            yRot += 8*dx;
        }
        emit rotate(xRot, yRot, zRot);
    }

    lastPos = event->pos();

    update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    //camDelta[2] -= (event->delta() / 8.0 / 15);
    //lookAtDelta[2] -= (event->delta() / 8.0 / 15);

    m_zoomFactor -= (event->delta() / 8.0 / 150);
    if (m_zoomFactor < 0) m_zoomFactor = 0.001;
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton )//|| event->button == Qt::RightButton)
    {
        lastPos = event->pos();
    }
    else if (event->button() == Qt::RightButton)
    {
        lastPos = event->pos();
    }
}

void GLWidget::drawArrow(float x2, float y2, float z2, float x1, float y1, float z1)
{

        // Co-ordinates of the line are
        // start: xyz_1
        // end: xyz_2

    glPushMatrix();
    glPushAttrib( GL_POLYGON_BIT ); // includes GL_CULL_FACE
    glDisable(GL_CULL_FACE); // draw from all sides

    // Calculate vector along direction of line
    float v[3];
    v[0] = x2-x1;
    v[1] = y2-y1;
    v[2] = z2-z1;

    // Line thickness
    glLineWidth(2);

    // Draw single line:
    glBegin(GL_LINES);
    //glTexCoord1f(colF);
    //glVertex3f(x1, y1, z1); // Starting point for line (arrow head end)
    glVertex3f( x1+0.05*v[0], y1+0.05*v[1], z1+0.05*v[2] ); // Make the line a bit shorter,
    //glTexCoord1f(colF);
    glVertex3f(x2, y2, z2);        // so the arrowhead is not blunted by the line thickness
    glEnd(); // GL_LINES

        float norm_of_v = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );

        // Size of cone in arrow:
        float coneFractionAxially = 0.1; // radius at thickest part 0.025
        float coneFractionRadially = 0.5; // length of arrow 0.12

        float coneHgt = coneFractionAxially * norm_of_v;
        float coneRadius = coneFractionRadially * norm_of_v;


        // Set location of arrowhead to be at the startpoint of the line
        float vConeLocation[3];
        vConeLocation[0] = x1;
        vConeLocation[1] = y1;
        vConeLocation[2] = z1;

        // Initialize transformation matrix
        float mat44[16] =
            {1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1};


        // Normalize v to get Unit Vector norm_startVec
        float norm_startVec[3];
        norm_startVec[0] = v[0]/norm_of_v;
        norm_startVec[1] = v[1]/norm_of_v;
        norm_startVec[2] = v[2]/norm_of_v;

        // Normalize zaxis to get Unit Vector norm_endVec
        //float zaxis[3] = { 0.0, 0.0, 1.0 };
        float norm_endVec[3] = { 0.0, 0.0, 1.0 };
        //Unit( zaxis, norm_endVec );
        // If vectors are identical, set transformation matrix to identity
        if ( ((norm_startVec[0] - norm_endVec[0]) > 1e-14) && ((norm_startVec[1] - norm_endVec[1]) > 1e-14) && ((norm_startVec[2] - norm_endVec[2]) > 1e-14) )
        {
            mat44[0] = 1.0;
            mat44[5] = 1.0;
            mat44[10] = 1.0;
            mat44[15] = 1.0;
        }
        // otherwise create the matrix
        else
        {

            // Vector cross-product, result = axb
            float axb[3] = { norm_startVec[1] * norm_endVec[2] - norm_startVec[2] * norm_endVec[1],
                norm_startVec[2] * norm_endVec[0] - norm_startVec[0] * norm_endVec[2],
                norm_startVec[0] * norm_endVec[1] - norm_startVec[1] * norm_endVec[0] };
            //cross(norm_startVec, norm_endVec, axb);

            // Normalize axb to get Unit Vector norm_axb
        float mag1 = sqrt(axb[0]*axb[0] + axb[1]*axb[1] + axb[2]*axb[2]);
            float norm_axb[3] = { axb[0]/mag1, axb[1]/mag1, axb[2]/mag1 };
            //float norm_axb[3];
            //Unit( axb, norm_axb );

            // Build the rotation matrix
            float ac = acos(norm_startVec[0]*norm_endVec[0]+norm_startVec[3]*norm_endVec[3]+norm_startVec[2]*norm_endVec[2] );

            float s = sin( ac );
            float c = cos( ac );
            float t = 1 - c;

            float x = norm_axb[0];
            float y = norm_axb[1];
            float z = norm_axb[2];

            // Fill top-left 3x3
            mat44[0] = t*x*x + c;
            mat44[1] = t*x*y - s*z;
            mat44[2] = t*x*z + s*y;

            mat44[4] = t*x*y + s*z;
            mat44[5] = t*y*y + c;
            mat44[6] = t*y*z - s*x;

            mat44[8] = t*x*z - s*y;
            mat44[9] = t*y*z + s*x;
            mat44[10] = t*z*z + c;

            mat44[15] = 1.0;
        }

        // Translate and rotate arrowhead to correct position
        glTranslatef( vConeLocation[0], vConeLocation[1], vConeLocation[2] );
        glMultMatrixf( mat44 );

        GLUquadric* cone_obj = gluNewQuadric();
        gluCylinder(cone_obj, 0, coneHgt, coneRadius, 8, 1);

        glPopAttrib(); // GL_CULL_FACE
        glPopMatrix();
}

void GLWidget::setLight()
{
    mlight_diffuse[0] = mDiffuse;
    mlight_diffuse[1] = mDiffuse;
    mlight_diffuse[2] = mDiffuse;
    mlight_diffuse[3] = 1.0f;
    mlight_specular[0] = mSpecular;
    mlight_specular[1] = mSpecular;
    mlight_specular[2] = mSpecular;
    mlight_specular[3] = 1.0f;
    mlight_ambient[0] = mAmbient;
    mlight_ambient[1] = mAmbient;
    mlight_ambient[2] = mAmbient+0.1f;
    mlight_ambient[3] = 1.0f;
    mlight0_position[0] = mL0X;
    mlight0_position[1] = mL0Y;
    mlight0_position[2] = mL0Z;
    mlight1_position[0] = mL1X;
    mlight1_position[1] = mL1Y;
    mlight1_position[2] = mL1Z;
    mlight2_position[0] = mL2X;
    mlight2_position[1] = mL2Y;
    mlight2_position[2] = mL2Z;

    glLightfv(GL_LIGHT0, GL_AMBIENT, mlight_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mlight_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, mlight0_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mlight_specular);

    glLightfv(GL_LIGHT1, GL_AMBIENT, mlight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, mlight_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, mlight1_position);
    glLightfv(GL_LIGHT1, GL_SPECULAR, mlight_specular);

    glLightfv(GL_LIGHT2, GL_AMBIENT, mlight_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, mlight_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, mlight2_position);
    glLightfv(GL_LIGHT2, GL_SPECULAR, mlight_specular);

    if (mL0) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
    if (mL1) glEnable(GL_LIGHT1);
    else glDisable(GL_LIGHT1);
    if (mL2) glEnable(GL_LIGHT2);
    else glDisable(GL_LIGHT2);
}
