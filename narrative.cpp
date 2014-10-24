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
QVector4D LIGHTGRAY = QVector4D(193.0/255.0, 194.0/255.0, 196.0/255.0, 1.0);

Narrative::Narrative(const QGLFormat &format, QWidget *parent):
    QGLWidget(format, parent), vbo(QOpenGLBuffer::VertexBuffer), vbo1(QOpenGLBuffer::VertexBuffer)
{
    vbo_v.clear();
    obj_v.clear();
}

Narrative::~Narrative()
{
}

QSize Narrative::sizeHint() const
{
    return QSize(1024, 1024);
}

QVector<float> ringArc(float inner, float outer, float ratio, unsigned int sample = 100){

    QVector<float> ver;
    for( unsigned int i = 0; i <= sample; ++i )
    {
        float angle  = (   i   / (float)sample * ratio ) * 3.14159f * 2.0f;
        ver << inner * cos( angle )  << inner * sin( angle  ) << -2;
        ver << outer * cos( angle )  << outer * sin( angle  ) << -2;

    /*
        float angle1 = ( (i+1) / (float)sample * ratio ) * 3.14159f * 2.0f;
        ver << inner * cos( angle1 ) << inner * sin( angle1 ) << -2;
        ver << inner * cos( angle )  << inner * sin( angle  ) << -2;
        ver << outer * cos( angle )  << outer * sin( angle  ) << -2;

        ver << inner * cos( angle1 ) << inner * sin( angle1 ) << -2;
        ver << outer * cos( angle  ) << outer * sin( angle  ) << -2;
        ver << outer * cos( angle1 ) << outer * sin( angle1 ) << -2; */
    }
    return ver;
}

void setupVBO(QOpenGLBuffer &vbo, QVector<float> &obj){
    vbo.create();
    vbo.bind();
    vbo.allocate(obj.data(), obj.size()* sizeof(float));
    vbo.release();
}

void Narrative::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(Qt::white);

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "vertexShader.vert");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "fragmentShader.frag");
    shaderProgram.link();
    shaderProgram.bind();

    float ratio = 5./7.;
    float inner = 0.3, outer = 0.35;
    ringArcObj = ringArc(inner, outer, ratio);

    inner = 0.5; outer = 0.85;
    ringArcObj1 = ringArc(inner, outer, ratio);

    vao.create();
    vao.bind();

    setupVBO(vbo, ringArcObj);
    setupVBO(vbo1, ringArcObj1);

    vao.release();
    shaderProgram.release();
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

void Narrative::shaderDrawArcObj(QOpenGLBuffer &vbo, QVector<float> &obj, QVector4D &color){
    vbo.bind();
    shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    shaderProgram.enableAttributeArray("vertex");
    shaderProgram.setUniformValue("arcColor", color);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (obj.size())/3);
    vbo.release();
}

void Narrative::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    shaderProgram.bind();
    shaderProgram.setUniformValue("mvpMatrix", pMatrix*mMatrix*vMatrix);

    vao.bind();
    shaderDrawArcObj(vbo, ringArcObj, GREEN);
    shaderDrawArcObj(vbo1, ringArcObj1, LIGHTGRAY);
    vao.release();

    shaderProgram.release();


}
