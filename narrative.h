#ifndef NARRATIVE_H
#define NARRATIVE_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


class Narrative : public QGLWidget
{
    Q_OBJECT
public:
    Narrative(const QGLFormat &format, QWidget *parent = 0);
    ~Narrative();
    QSize   sizeHint() const;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QMatrix4x4                  pMatrix;
    QGLShaderProgram            shaderProgram;
    QVector<float>              ringArcObj;
    QVector<float>              ringArcObj1;
    QOpenGLBuffer               vbo;
    QOpenGLBuffer               vbo1;
    QOpenGLVertexArrayObject    vao;

    QVector<QOpenGLBuffer>      vbo_v;
    QVector<QVector<float>>     obj_v;

    void shaderDrawArcObj(QOpenGLBuffer &vbo, QVector<float> &obj,  QVector4D &color);
};

#endif // NARRATIVE_H
