#include "narrative.h"
#include <cmath>

// 123.0/255.0, 57.0/255.0, 144.0/255.0     -> purple
// 28.0/255.0, 120.0/255.0, 159.0/255.0     -> blue
// 97.0/255.0, 149.0/255.0, 5.0/255.0       -> green
// 247.0/255.0, 154.0/255.0, 1.0/255.0      -> yellow
// 208.0/255.0, 81.0/255.0, 38.0/255.0      -> red
// 52.0/255.0, 56.0/255.0, 57.0/255.0       -> dark gray bg
// 193.0/255.0, 194.0/255.0, 196.0/255.0    -> light gray fg

QVector4D GREEN = QVector4D(97.0/255.0, 149.0/255.0, 5.0/255.0, 1.0);

Narrative::Narrative(const QGLFormat &format, QWidget *parent):
    QGLWidget(format, parent), vbo(QGLBuffer::VertexBuffer)
{
}

Narrative::~Narrative()
{

}

QSize Narrative::sizeHint() const
{
    return QSize(1024, 1024);
}

void Narrative::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(Qt::white);

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "vertexShader.vert");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "fragmentShader.frag");

//    vertices << 1  << 0 << -2
//             << 0  << 1 << -2
//             << -1 << 0 << -2;

    float inner = 0.5, outer = 1.0;
    unsigned int pts = 100;
    for( unsigned int i = 0; i <= pts; ++i )
    {
        float angle  = (    i  / (float)pts * 5 / 7 ) * 3.14159f * 2.0f;
        float angle1 = ( (i+1) / (float)pts * 5 / 7 ) * 3.14159f * 2.0f;
        vertices << inner * cos( angle1 )  << inner * sin( angle1 ) << -2;
        vertices << inner * cos( angle )  << inner * sin( angle ) << -2;
        vertices << outer * cos( angle )  << outer * sin( angle ) << -2;

        vertices << inner * cos( angle1 )  << inner * sin( angle1 ) << -2;
        vertices << outer * cos( angle )  << outer * sin( angle ) << -2;
        vertices << outer * cos( angle1 )  << outer * sin( angle1 ) << -2;

    }

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(), vertices.size()*sizeof(float));

    shaderProgram.bind();
    shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    shaderProgram.enableAttributeArray("vertex");


    shaderProgram.release();
    vbo.release();
    vao.release();

}

void Narrative::resizeGL(int w, int h)
{
    if(h == 0)
        h = 1;

    pMatrix.setToIdentity();
    //pMatrix.perspective(60.0, (float)w/(float)h, 0.001, 1000);
    pMatrix.ortho(-2, 2, -2, 2, 0.001, 1000);

    glViewport(0, 0, w, h);
}

void Narrative::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    shaderProgram.bind();
    shaderProgram.setUniformValue("mvpMatrix", pMatrix*mMatrix*vMatrix);
    shaderProgram.setUniformValue("color", GREEN);


    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);

    vao.release();

    shaderProgram.release();


}
