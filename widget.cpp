#include "widget.h"
#include <GL/glu.h>

MyWidget::MyWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    resize(1024,720);
    paintTimer = new QTimer(this);
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(repaint()));    
}

void MyWidget::startflow(int t)
{
    paintTimer->start(t);
}

void MyWidget::set_angle_scale_incs(float angle_t, float scale_t)
{
    angleinc = angle_t;
    scale = scale_t;
}

void MyWidget::initTexture(uint index, QImage &texture1)
{
    texture1.convertTo(QImage::Format_RGBA8888);
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, GLsizei(texture1.width()), GLsizei(texture1.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());
}

void MyWidget::LoadGLTextures()
{
    glGenTextures(3, texture);

    QImage texture1;
    texture1.load(":/files/bricks.jpg");
    initTexture(0, texture1);

    texture1.load(":/files/illusion.jpg");
    initTexture(1, texture1);

    texture1.load(":/files/texture1.bmp");
    initTexture(2, texture1);
}

void MyWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_F) {
        ++texture_count%=3;
    }
    if (event->key()==Qt::Key_S) {
        if (paintTimer->isActive()) paintTimer->stop();
        else paintTimer->start();
    }
    if (event->key()==Qt::Key_M) {
        ++model_count%=2;
    }
    if (event->key()==Qt::Key_R) {
        angleinc += 0.1f;
      //  qDebug()<<angleinc;
    }
    if (event->key()==Qt::Key_T) {
        angleinc -= 0.1f;
      //  qDebug()<<angleinc;
    }
    if (event->key()==Qt::Key_Y) {
        scale += scaleinc;
      //  qDebug()<<scale;
    }
    if (event->key()==Qt::Key_U) {
        scale -= scaleinc;
       // qDebug()<<scale;
    }
}

void MyWidget::initLight()
{
    GLfloat light_ambient[] = { 0, 0, 0, 0.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 2.0 };
    GLfloat light_position[] = { 0.0, 2.0, 2.0, 1.0 };

    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}

void MyWidget::initializeGL()
{
    glEnable(GL_MULTISAMPLE);
    LoadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(1,1,1,1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    initLight();
    model[0] = objloader::Instance().load(":/files/monkey.obj");
    model[1] = objloader::Instance().load(":/files/sidor.obj");
    torus = objloader::Instance().load(":/files/torus.obj");
}

void MyWidget::resizeGL(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
    qreal aspectratio = qreal(nWidth) / qreal(nHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 90.0, aspectratio, 0.1, 100.0 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTranslatef(0,0,-2.5);
    glDeleteLists(torus,1);
    torus = objloader::Instance().draw(angle/100);
    glCallList(torus);
    glTranslatef(0,0,0.1f);
    glRotatef(angle,0.0f,1.0f,0.0f);
    glScalef(scale,scale,scale);
    glBindTexture(GL_TEXTURE_2D, texture[texture_count]);
    glCallList(model[model_count]);

    angle += angleinc;
}


