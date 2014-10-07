#ifndef NARRATIVE_H
#define NARRATIVE_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QGLBuffer>
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
    QVector<float>              vertices;
    QGLBuffer                   vbo;
    QOpenGLVertexArrayObject    vao;

};

#endif // NARRATIVE_H
