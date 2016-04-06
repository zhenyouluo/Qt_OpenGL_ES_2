#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QWidget>

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
    void push_button1();
    void push_button2();
    void push_button3();
    void push_button4();
    void push_button5();
    void push_button6();
    void push_button7();
    void push_button8();
    void push_button9();
    void push_button10();
    void push_button11();
    void push_button12();

protected:
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();
    void prepareTexture();
    void detectCollision();

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram program2;
    GeometryEngine *geometries;
    GLuint texture;
    GLuint frameBuffer;
    GLuint depthBuffer;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    qreal startAngleX;
    qreal startAngleY;
    qreal startAngleZ;
    qreal rotationBase;
    qreal rotationLeft;
    qreal rotationRight;
    qreal rotationMirror;
    bool needRotateBase;
    bool needRotateLeft;
    bool needRotateRight;
    bool needRotateMirror;
    int rotationBaseDir;
    int rotationLeftDir;
    int rotationRightDir;
    int rotationMirrorDir;

    bool collisionBase;
    bool collisionLeft;
    bool collisionRight;
};

#endif // GLWIDGET_H
