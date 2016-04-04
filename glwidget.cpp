
#include "glwidget.h"
#include <QMouseEvent>

#include <math.h>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    angularSpeed(0)
{
    angularSpeed = 0.1;
    startAngleX = 15.0;
    startAngleY = -30.0;
    startAngleZ = -7.0;

    needRotateBase = true;
    needRotateLeft = true;
    needRotateRight = true;
    needRotateMirror = true;
    rotationBaseDir = 1;
    rotationLeftDir = 1;
    rotationRightDir = 1;
    rotationMirrorDir = 1;

    collision = false;
}

GLWidget::~GLWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete geometries;

    doneCurrent();
}

void GLWidget::timerEvent(QTimerEvent *)
{

    detectCollision();
    if(collision) {
        return;
    }

    if(needRotateBase) {
        rotationBase+=angularSpeed * rotationBaseDir;
        if(rotationBase > 360.0) {
            rotationBase -= 360.0;
        } else if(rotationBase < -360.0) {
            rotationBase += 360.0;
        }
    }
    if(needRotateMirror) {
        rotationMirror+=angularSpeed * rotationMirrorDir;
        if(rotationMirror > 360.0) {
            rotationMirror -= 360.0;
        } else if(rotationMirror < -360.0) {
            rotationMirror += 360.0;
        }
    }
    if(needRotateLeft) {
        rotationLeft+=angularSpeed * rotationLeftDir;
        if(rotationLeft > 360.0) {
            rotationLeft-= 360.0;
        } else if(rotationLeft < -360.0) {
            rotationLeft += 360.0;
        }
    }
    if(needRotateRight) {
        rotationRight+=angularSpeed * rotationRightDir;
        if(rotationRight > 360.0) {
            rotationRight-= 360.0;
        } else if(rotationRight < -360.0) {
            rotationRight += 360.0;
        }
    }
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1, 1, 1, 1);

    initShaders();
    //prepareTexture();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);


    geometries = new GeometryEngine;

    glGenTextures(1, &texture);
    glGenFramebuffers(1, &frameBuffer);

    glGenRenderbuffers(1, &depthBuffer);


    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void GLWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();

    if (!program2.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader2.glsl"))
        close();
    if (!program2.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader2.glsl"))
        close();
    if (!program2.link())
        close();
    if (!program2.bind())
        close();
}

void GLWidget::prepareTexture()
{
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 600, 600, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 600, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


    glClearColor(0.87, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program.programId());


    // Calculate model view transformation
    QMatrix4x4 matrix;
    float offsetY = rotationMirror / 3000.0;
    matrix.translate(0.0, offsetY, -0.15);

    //matrix.rotate(-(startAngleX + 90) + rotationMirror, QVector3D(1,0,0));
    matrix.rotate(-(90) + rotationMirror, QVector3D(1,0,0));
    //matrix.rotate(startAngleZ, QVector3D(0,0,1));
    matrix.rotate(-rotationBase, QVector3D(0,1,0));
    //matrix.rotate(startAngleZ, QVector3D(0,0,1));
    matrix.scale(0.015);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    //geometries->drawAxis(&program);
    // Draw cube geometry

    matrix.rotate(rotationLeft, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "left");
    matrix.rotate(-rotationLeft, QVector3D(0,0,1));
    matrix.rotate(rotationRight, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "right");
    matrix.rotate(-rotationRight, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "base");


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GLWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar =47.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void GLWidget::paintGL()
{
    prepareTexture();



    // Clear color and depth buffer
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(program.programId());


    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.5, 1.0, -13.0);
    matrix.rotate(startAngleY, QVector3D(0,1,0));
    matrix.rotate(startAngleX, QVector3D(1,0,0));
    matrix.rotate(startAngleZ, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawAxis(&program);

    matrix.rotate(rotationBase, QVector3D(0,1,0));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "base");
    matrix.rotate(rotationLeft, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "left");
    matrix.rotate(-rotationLeft, QVector3D(0,0,1));
    matrix.rotate(rotationRight, QVector3D(0,0,1));
    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawGeometry(&program, "right");


    QMatrix4x4 matrix2;
    matrix2.translate(0.0, -1.5, -16.0);
    matrix2.rotate(startAngleY, QVector3D(0,1,0));
    matrix2.rotate(startAngleX, QVector3D(1,0,0));
    matrix2.rotate(startAngleZ, QVector3D(0,0,1));

    matrix2.rotate(rotationMirror, QVector3D(1,0,0));

    glUseProgram(program2.programId());
    program2.setUniformValue("mvp_matrix", projection * matrix2);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //geometries->drawGeometry(&program2, "right");
    geometries->drawMirror(&program2);

}

void GLWidget::detectCollision() {
    collision = false;
    qreal rotationBaseTmp = rotationBase + angularSpeed * rotationBaseDir;
    if(!needRotateBase) {
        rotationBaseTmp = rotationBase;
    }
    qreal rotationMirrorTmp = rotationMirror + angularSpeed * rotationMirrorDir;
    if(!needRotateMirror) {
        rotationMirrorTmp = rotationMirror;
    }
    qreal rotationLeftTmp = rotationLeft + angularSpeed * rotationLeftDir;
    if(!needRotateLeft) {
        rotationLeftTmp = rotationLeft;
    }
    qreal rotationRightTmp = rotationRight + angularSpeed * rotationRightDir;
    if(!needRotateRight) {
        rotationRightTmp = rotationRight;
    }

    if(abs(rotationMirrorTmp) > 42) {
        collision = true;
    }


}



// Кнопка вращения всей фигуры по часовой стрелке
void GLWidget::push_button1() {
     rotationBaseDir = 1;
}

// Кнопка вращения всей фигуры против часовой стрелки
void GLWidget::push_button2() {
    rotationBaseDir = -1;
}

// Кнопка остановки фигуры
void GLWidget::push_button3() {
    needRotateBase = !needRotateBase;
}

// Кнопка вращения правого крыла вверх
void GLWidget::push_button4() {
    rotationRightDir = 1;
}

// Кнопка вращения правого крыла вниз
void GLWidget::push_button5() {
    rotationRightDir = -1;
}

// Кнопка остановки правого крыл
void GLWidget::push_button6() {
    needRotateRight = !needRotateRight;
}

// Кнопка вращения левого крыла вверх
void GLWidget::push_button7() {
    rotationLeftDir = -1;
}

// Кнопка вращения левого крыла вверх
void GLWidget::push_button8() {
    rotationLeftDir = 1;
}

// Кнопка остановки вращения левого крыла
void GLWidget::push_button9() {
    needRotateLeft = !needRotateLeft;
}

// Кнопка вращения зеркала по часовой стрелке
void GLWidget::push_button10() {
    rotationMirrorDir = 1;
}

// Кнопка вращения зеркала против часовой стрелки
void GLWidget::push_button11() {
    rotationMirrorDir = -1;
}

// Кнопка остановки вращения зеркала
void GLWidget::push_button12() {
    needRotateMirror = !needRotateMirror;
}
