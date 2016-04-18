#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void drawGeometry(QOpenGLShaderProgram *program, QString name, bool mirror);
    void drawMirror(QOpenGLShaderProgram *program);
    void drawAxis(QOpenGLShaderProgram *program);

private:
    void initBaseGeometry();
    void initLeftGeometry();
    void initRightGeometry();
    void initMirrorGeometry();
    void initAxisGeometry();

    void drawNamedGeometry(QOpenGLShaderProgram *program, QOpenGLBuffer arrayBuf, QOpenGLBuffer indexBuf, int len);

    QOpenGLBuffer arrayBufBase;
    QOpenGLBuffer indexBufBase;

    QOpenGLBuffer arrayBufLeft;
    QOpenGLBuffer arrayBufMirrorLeft;
    QOpenGLBuffer indexBufLeft;

    QOpenGLBuffer arrayBufRight;
    QOpenGLBuffer arrayBufMirrorRight;
    QOpenGLBuffer indexBufRight;

    QOpenGLBuffer arrayBufMirror;
    QOpenGLBuffer indexBufMirror;

    QOpenGLBuffer arrayBufAxis;
};

#endif // GEOMETRYENGINE_H
