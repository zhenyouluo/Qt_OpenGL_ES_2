#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector4D texCoord;
};

GeometryEngine::GeometryEngine()
    : indexBufBase(QOpenGLBuffer::IndexBuffer),
      indexBufLeft(QOpenGLBuffer::IndexBuffer),
      indexBufRight(QOpenGLBuffer::IndexBuffer),
      indexBufMirror(QOpenGLBuffer::IndexBuffer)

{
     initializeOpenGLFunctions();
     arrayBufBase.create();
     indexBufBase.create();
     initBaseGeometry();

     arrayBufLeft.create();
     arrayBufMirrorLeft.create();
     indexBufLeft.create();
     arrayBufRight.create();
     arrayBufMirrorRight.create();
     indexBufRight.create();
     initLeftGeometry();
     initRightGeometry();

     arrayBufMirror.create();
     indexBufMirror.create();
     initMirrorGeometry();

     arrayBufAxis.create();
     initAxisGeometry();
 }

 GeometryEngine::~GeometryEngine()
 {
     arrayBufBase.destroy();
     indexBufBase.destroy();

     arrayBufLeft.destroy();
     arrayBufMirrorLeft.destroy();
     indexBufLeft.destroy();

     arrayBufRight.destroy();
     arrayBufMirrorRight.create();
     indexBufRight.destroy();

     arrayBufMirror.destroy();
     indexBufMirror.destroy();

     arrayBufAxis.destroy();
 }

 void GeometryEngine::initBaseGeometry()
  {
      float col = 1.0f;
      VertexData vertices[] = {
          // Передняя грань
          {QVector3D(-1.0f, -0.5f,  1.0f), QVector4D(1.0f, 1.0f, col, 1.0f)},  // v0
          {QVector3D( 1.0f, -0.5f,  1.0f), QVector4D(1.0f, 1.0f, col, 1.0f)}, // v1
          {QVector3D(0.0f,  0.5f,  1.0f), QVector4D(1.0f, 1.0f, col, 1.0f)},  // v2
          {QVector3D( 0.0f,  0.5f,  1.0f), QVector4D(1.0f, 1.0f, col, 1.0f)}, // v2

          // Правая грань
          {QVector3D( 1.0f, -0.5f,  1.0f), QVector4D(0.0f, 0.2f, col-0.1f, 1.0f)}, // v1
          {QVector3D( 1.0f, -0.5f, -1.0f), QVector4D(0.0f, 0.2f, col-0.1f, 1.0f)}, // v3
          {QVector3D( 0.0f,  0.5f,  1.0f), QVector4D(0.0f, 0.2f, col-0.1f, 1.0f)},  // v2
          {QVector3D( 0.0f,  0.5f, -1.0f), QVector4D(0.0f, 0.2f, col-0.1f, 1.0f)}, // v5

          // Задняя грань
          {QVector3D( 1.0f, -0.5f, -1.0f), QVector4D(0.8f, 0.8f, 0.39f, 1.0f)}, // v3
          {QVector3D(-1.0f, -0.5f, -1.0f), QVector4D(0.8f, 0.8f, 0.39f, 1.0f)},  // v4
          {QVector3D( 0.0f,  0.5f, -1.0f), QVector4D(0.8f, 0.8f, 0.39f, 1.0f)}, // v5
          {QVector3D(0.0f,  0.5f, -1.0f), QVector4D(0.8f, 0.8f, 0.39f, 1.0f)},  // v5

          // Vertex data for face 3
          {QVector3D(-1.0f, -0.5f, -1.0f), QVector4D(1.0f, 1.0f, 0.0f, 1.0f)}, // v3
          {QVector3D(-1.0f, -0.5f,  1.0f), QVector4D(1.0f, 1.0f, 0.0f, 1.0f)},  // v0
          {QVector3D(0.0f,  0.5f, -1.0f), QVector4D(1.0f, 1.0f, 0.0f, 1.0f)}, // v5
          {QVector3D(0.0f,  0.5f,  1.0f), QVector4D(1.0f, 1.0f, 0.0f, 1.0f)},  // v2

          // Vertex data for face 4
          {QVector3D(-1.0f, -0.5f, -1.0f), QVector4D(0.0f, 0.3f, col-0.4f, 1.0f)}, // v4
          {QVector3D( 1.0f, -0.5f, -1.0f), QVector4D(0.0f, 0.3f, col-0.4f, 1.0f)}, // v3
          {QVector3D(-1.0f, -0.5f,  1.0f), QVector4D(0.0f, 0.3f, col-0.4f, 1.0f)}, // v0
          {QVector3D( 1.0f, -0.5f,  1.0f), QVector4D(0.0f, 0.3f, col-0.4f, 1.0f)}, // v1
      };
      // Indices for drawing cube faces using triangle strips.
      // Triangle strips can be connected by duplicating indices
      // between the strips. If connecting strips have opposite
      // vertex order then last index of the first strip and first
      // index of the second strip needs to be duplicated. If
      // connecting strips have same vertex order then only last
      // index of the first strip needs to be duplicated.
      GLushort indices[] = {
           0,  1,  2,  3,  3,     // Face 0 - triangle strip
           4,  4,  5,  6,  7,  7, // Face 1 - triangle strip
           8,  8,  9, 10, 11, 11, // Face 2 - triangle strip
          12, 12, 13, 14, 15, 15, // Face 3 - triangle strip
          16, 16, 17, 18, 19, 19 // Face 4 - triangle strip
      };

      arrayBufBase.bind();
      arrayBufBase.allocate(vertices, 24 * sizeof(VertexData));

      indexBufBase.bind();
      indexBufBase.allocate(indices, 29 * sizeof(GLushort));
  }

 void GeometryEngine::initRightGeometry()
 {
     // For cube we would need only 8 vertices but we have to
     // duplicate vertex for each face because texture coordinate
     // is different.
     //
     VertexData vertices[] = {
         // Нижняя грань
         {QVector3D(0.0f, 0.0f, -1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f,  1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)},  // v1
         {QVector3D(-1.0f,  1.0f, -1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)}, // v5
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)},  // v0

         // Передняя грань
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f,  1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)}, // v1
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)},  // v2
         {QVector3D( 1.0f,  2.5f,  1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)}, // v3

         // Задняя грань
         {QVector3D(0.0f, 0.0f,  -1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)}, // v4
         {QVector3D(-1.0f,  1.0f,  -1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)},  // v5
         {QVector3D( 1.0f,  2.5f,  -1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)}, // v6
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)},  // v7

         // Верхняя грань
         {QVector3D( 1.0f,  2.5f,  1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)}, // v3
         {QVector3D( 1.0f,  2.5f,  -1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)},  // v6
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)}, // v2
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)},  // v7

         // Правая грань
         {QVector3D(-1.0f,  1.0f,  -1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v5
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v0
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v7
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v2

         // Левая грань
         {QVector3D(1.0f,  2.5f,  -1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v6
         {QVector3D(1.0f,  2.5f,  1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v3
         {QVector3D(0.0f, 0.0f,  -1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f, 1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}  // v1

     };

     VertexData verticesMirror[] = {
         // Нижняя грань
         {QVector3D(0.0f, 0.0f, -1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f,  1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)},  // v1
         {QVector3D(-1.0f,  1.0f, -1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)}, // v5
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(1.0f, 0.0f, 1.0f, 1.0f)},  // v0

         // Передняя грань
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f,  1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)}, // v1
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)},  // v2
         {QVector3D( 1.0f,  2.5f,  1.01f), QVector4D(0.0f, 1.0f, 1.0f, 1.0f)}, // v3

         // Задняя грань
         {QVector3D(0.0f, 0.0f,  -1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)}, // v4
         {QVector3D(-1.0f,  1.0f,  -1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)},  // v5
         {QVector3D( 1.0f,  2.5f,  -1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)}, // v6
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.9f, 1.0f, 0.0f, 1.0f)},  // v7

         // Верхняя грань
         {QVector3D( 1.0f,  2.5f,  1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)}, // v3
         {QVector3D( 1.0f,  2.5f,  -1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)},  // v6
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)}, // v2
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)},  // v7

         // Правая грань
         {QVector3D(-1.0f,  1.0f,  -1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v5
         {QVector3D(-1.0f,  1.0f,  1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v0
         {QVector3D( 1.0f, 4.0f, -1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v7
         {QVector3D( 1.0f, 4.0f, 1.01f), QVector4D(0.5f, 0.5f, 0.5f, 1.0f)}, // v2

         // Левая грань
         {QVector3D(1.0f,  2.5f,  -1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v6
         {QVector3D(1.0f,  2.5f,  1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v3
         {QVector3D(0.0f, 0.0f,  -1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f, 1.01f), QVector4D(1.0f, 0.84f, 0.0f, 1.0f)}  // v1

     };




     // Indices for drawing cube faces using triangle strips.
     // Triangle strips can be connected by duplicating indices
     // between the strips. If connecting strips have opposite
     // vertex order then last index of the first strip and first
     // index of the second strip needs to be duplicated. If
     // connecting strips have same vertex order then only last
     // index of the first strip needs to be duplicated.
     GLushort indices[] = {
          0,  1,  2,  3,  3,     // Face 0 - triangle strip
          4,  4,  5,  6,  7,  7 , // Face 1 - triangle strip
          8,  8,  9, 10, 11, 11, // Face 2 - triangle strip
         12, 12, 13, 14, 15, 15, // Face 3 - triangle strip
         16, 16, 17, 18, 19, 19 , // Face 4 - triangle strip
         20, 20, 21, 22, 23      // Face 5 - triangle strip

     };

     // Transfer vertex data to VBO 0
     arrayBufLeft.bind();
     arrayBufLeft.allocate(vertices, 24 * sizeof(VertexData));

     arrayBufMirrorLeft.bind();
     arrayBufMirrorLeft.allocate(verticesMirror, 24 * sizeof(VertexData));

     // Transfer index data to VBO 1
     indexBufLeft.bind();
     indexBufLeft.allocate(indices, 35 * sizeof(GLushort));
 }

 void GeometryEngine::initLeftGeometry()
 {
     // For cube we would need only 8 vertices but we have to
     // duplicate vertex for each face because texture coordinate
     // is different.
     VertexData vertices[] = {
         // Нижняя грань
         {QVector3D(0.0f, 0.0f,  1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f, -1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)},  // v5
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)}, // v1
         {QVector3D(1.0f,  1.0f, -1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)},  // v4

         // Передняя грань
         {QVector3D(0.0f, 0.0f,  1.02f), QVector4D(1.0f, 0.4, 0.7f, 1.0f)}, // v0
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(1.0f, 0.4f, 0.7f, 1.0f)}, // v1
         {QVector3D( -1.0f,  2.5f,  1.02f), QVector4D(1.0f, 0.4f, 0.7f, 1.0f)},  // v2
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(1.0f, 0.4f, 0.7f, 1.0f)}, // v3

         // Задняя грань
         {QVector3D(1.0f,  1.0f,  -1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f,  -1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)},  // v5
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)}, // v6
         {QVector3D( -1.0f,  2.5f,  -1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)},  // v7

         // Верхняя грань
         {QVector3D( -1.0f,  2.5f,  -1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)}, // v7
         {QVector3D( -1.0f,  2.5f,  1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)},  // v2
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)}, // v6
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)},  // v3

         // Правая грань
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v1
         {QVector3D(1.0f,  1.0f,  -1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v4
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v3
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v6

         // Левая грань
         {QVector3D(-1.0f,  2.5f,  1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v2
         {QVector3D(-1.0f,  2.5f,  -1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v7
         {QVector3D(0.0f, 0.0f, 1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f,  -1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}  // v5

     };

     VertexData verticesMirror[] = {
         // Нижняя грань
         {QVector3D(0.0f, 0.0f,  1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f, -1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)},  // v5
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)}, // v1
         {QVector3D(1.0f,  1.0f, -1.02f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)},  // v4

         // Передняя грань
         {QVector3D(0.0f, 0.0f,  1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)}, // v0
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)}, // v1
         {QVector3D( -1.0f,  2.5f,  1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)},  // v2
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(0.62f, 0.12f, 0.94f, 1.0f)}, // v3

         // Задняя грань
         {QVector3D(1.0f,  1.0f,  -1.02f), QVector4D(1.0f, 0.4, 0.7f, 1.0f)}, // v4
         {QVector3D(0.0f, 0.0f,  -1.02f), QVector4D(1.0f, 0.4, 0.7f, 1.0f)},  // v5
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(1.0f, 0.4, 0.7f, 1.0f)}, // v6
         {QVector3D( -1.0f,  2.5f,  -1.02f), QVector4D(1.0f, 0.4, 0.7f, 1.0f)},  // v7

         // Верхняя грань
         {QVector3D( -1.0f,  2.5f,  -1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)}, // v7
         {QVector3D( -1.0f,  2.5f,  1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)},  // v2
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)}, // v6
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(0.8f, 0.41f, 0.22f, 1.0f)},  // v3

         // Правая грань
         {QVector3D(1.0f,  1.0f,  1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v1
         {QVector3D(1.0f,  1.0f,  -1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v4
         {QVector3D( -1.0f, 4.0f, 1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v3
         {QVector3D( -1.0f, 4.0f, -1.02f), QVector4D(0.6f, 1.0f, 0.0f, 1.0f)}, // v6

         // Левая грань
         {QVector3D(-1.0f,  2.5f,  1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v2
         {QVector3D(-1.0f,  2.5f,  -1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v7
         {QVector3D(0.0f, 0.0f, 1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}, // v0
         {QVector3D(0.0f, 0.0f,  -1.02f), QVector4D(0.5f, 1.0f, 0.0f, 1.0f)}  // v5

     };


     // Indices for drawing cube faces using triangle strips.
     // Triangle strips can be connected by duplicating indices
     // between the strips. If connecting strips have opposite
     // vertex order then last index of the first strip and first
     // index of the second strip needs to be duplicated. If
     // connecting strips have same vertex order then only last
     // index of the first strip needs to be duplicated.
     GLushort indices[] = {
          0,  1,  2,  3,  3,     // Face 0 - triangle strip
          4,  4,  5,  6,  7,  7 , // Face 1 - triangle strip
          8,  8,  9, 10, 11, 11, // Face 2 - triangle strip
         12, 12, 13, 14, 15, 15, // Face 3 - triangle strip
         16, 16, 17, 18, 19, 19 , // Face 4 - triangle strip
         20, 20, 21, 22, 23      // Face 5 - triangle strip

     };

     // Transfer vertex data to VBO 0
     arrayBufRight.bind();
     arrayBufRight.allocate(vertices, 24 * sizeof(VertexData));

     arrayBufMirrorRight.bind();
     arrayBufMirrorRight.allocate(verticesMirror, 24 * sizeof(VertexData));

     // Transfer index data to VBO 1
     indexBufRight.bind();
     indexBufRight.allocate(indices, 34 * sizeof(GLushort));
 }

 void GeometryEngine::initMirrorGeometry() {
     VertexData vertices[] = {

         // Vertex data for face 4
         {QVector3D(-4.0f, 0.0f, -4.0f), QVector2D(-0.1f, -0.1f)}, // v0
         {QVector3D( 1.0f, 0.0f, -4.0f), QVector2D(-0.1f, -0.1f)}, // v1
         {QVector3D(-4.0f, 0.0f,  4.0f), QVector2D(-0.1f, -0.1f)}, // v2
         {QVector3D( 4.0f, 0.0f,  4.0f), QVector2D(-0.1f, -0.1f)}, // v3

         // Vertex data for face 5
         {QVector3D(-4.0f,  -0.01f,  4.0f), QVector2D(0.0f, 0.0f)}, // v4
         {QVector3D( 4.0f,  -0.01f,  4.0f), QVector2D(1.0f, 0.0f)}, // v5
         {QVector3D(-4.0f,  -0.01f, -4.0f), QVector2D(0.0f, 1.0f)}, // v6
         {QVector3D( 4.0f,  -0.01f, -4.0f), QVector2D(1.0f, 1.0f)}  // v7
     };
     // Indices for drawing cube faces using triangle strips.
     // Triangle strips can be connected by duplicating indices
     // between the strips. If connecting strips have opposite
     // vertex order then last index of the first strip and first
     // index of the second strip needs to be duplicated. If
     // connecting strips have same vertex order then only last
     // index of the first strip needs to be duplicated.
     GLushort indices[] = {
          0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
          4,  4,  5,  6,  7,  7  // Face 5 - triangle strip (v20, v21, v22, v23)
     };
     arrayBufMirror.bind();
     arrayBufMirror.allocate(vertices, 8 * sizeof(VertexData));

     // Transfer index data to VBO 1
     indexBufMirror.bind();
     indexBufMirror.allocate(indices, 11 * sizeof(GLushort));

 }

 void GeometryEngine::initAxisGeometry(){
     VertexData vertices[] = {
         {QVector3D(0.0f, 0.0f,  0.0f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)},  // v0
         {QVector3D(4.0f, 0.0f,  0.0f), QVector4D(1.0f, 0.0f, 0.0f, 1.0f)}, // v1
         {QVector3D(0.0f, 0.0f,  0.0f), QVector4D(0.0f, 0.0f, 1.0f, 1.0f)},  // v2
         {QVector3D(0.0f,  4.0f,  0.0f), QVector4D(0.0f, 0.0f, 1.0f, 1.0f)}, // v3
         {QVector3D(0.0f, 0.0f,  0.0f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)},  // v2
         {QVector3D(0.0f,  0.0f,  -4.0f), QVector4D(0.0f, 1.0f, 0.0f, 1.0f)} // v3
     };

     // Transfer vertex data to VBO 0
     arrayBufAxis.bind();
     arrayBufAxis.allocate(vertices, 6 * sizeof(VertexData));

 }



 void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program, QString name, bool mirror){
     if(name == "base") {
        drawNamedGeometry(program, arrayBufBase, indexBufBase, 29);
     } else if(name == "left") {
         if(mirror) {
             drawNamedGeometry(program, arrayBufMirrorLeft, indexBufLeft, 34);
         } else {
             drawNamedGeometry(program, arrayBufLeft, indexBufLeft, 34);
         }
     } else if(name == "right") {
         if(mirror) {
             drawNamedGeometry(program, arrayBufMirrorRight, indexBufRight, 34);
         } else {
             drawNamedGeometry(program, arrayBufRight, indexBufRight, 34);
         }
     }
 }
/*
 void GeometryEngine::drawRight(QOpenGLShaderProgram *program) {
     arrayBufRight.bind();
     indexBufRight.bind();

     // Offset for position
     quintptr offset = 0;

     // Tell OpenGL programmable pipeline how to locate vertex position data
     int vertexLocation = program->attributeLocation("a_position");
     program->enableAttributeArray(vertexLocation);
     program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

     // Offset for texture coordinate
     offset += sizeof(QVector3D);

     // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
     int texcoordLocation = program->attributeLocation("a_texcoord");
     program->enableAttributeArray(texcoordLocation);
     program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

     // Draw cube geometry using indices from VBO 1
     glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
 }
 */

 void GeometryEngine::drawNamedGeometry(QOpenGLShaderProgram *program, QOpenGLBuffer arrayBuf, QOpenGLBuffer indexBuf, int len){
     arrayBuf.bind();
     indexBuf.bind();

     // Offset for position
     quintptr offset = 0;

     // Tell OpenGL programmable pipeline how to locate vertex position data
     int vertexLocation = program->attributeLocation("a_position");
     program->enableAttributeArray(vertexLocation);
     program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

     // Offset for texture coordinate
     offset += sizeof(QVector3D);
     // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
     int texcoordLocation = program->attributeLocation("a_texcoord");
     program->enableAttributeArray(texcoordLocation);
     program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

     // Draw cube geometry using indices from VBO 1
     glDrawElements(GL_TRIANGLE_STRIP, len, GL_UNSIGNED_SHORT, 0);
 }

 void GeometryEngine::drawMirror(QOpenGLShaderProgram *program) {
     arrayBufMirror.bind();
     indexBufMirror.bind();

     // Offset for position
     quintptr offset = 0;

     // Tell OpenGL programmable pipeline how to locate vertex position data
     int vertexLocation = program->attributeLocation("a_position");
     program->enableAttributeArray(vertexLocation);
     program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

     // Offset for texture coordinate
     offset += sizeof(QVector3D);

     // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
     int texcoordLocation = program->attributeLocation("a_texcoord");
     program->enableAttributeArray(texcoordLocation);
     program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

     // Draw cube geometry using indices from VBO 1
     glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, 0);
 }

 void GeometryEngine::drawAxis(QOpenGLShaderProgram *program) {

     arrayBufAxis.bind();

     // Offset for position
     quintptr offset = 0;

     // Tell OpenGL programmable pipeline how to locate vertex position data
     int vertexLocation = program->attributeLocation("a_position");
     program->enableAttributeArray(vertexLocation);
     program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

     // Offset for texture coordinate
     offset += sizeof(QVector3D);
     // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
     int texcoordLocation = program->attributeLocation("a_texcoord");
     program->enableAttributeArray(texcoordLocation);
     program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

     glDrawArrays(GL_LINE_LOOP, 0, 6);

 }
