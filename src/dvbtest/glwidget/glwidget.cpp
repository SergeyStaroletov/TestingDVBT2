/****************************************************************************
 * @author Sergey Staroletov
 * @license LGPL
 **
 ** Based on the qt gl widget (C) 2016 The Qt Company Ltd.
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file was part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ****************************************************************************/

#include "glwidget.h"
#include <QCoreApplication>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <fstream>
#include <math.h>
#include <vector>

extern bool is_group;

bool GLWidget::m_transparent = false;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_xRot(0), m_yRot(0), m_zRot(0), m_program(0) {
  m_core =
      QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
  // --transparent causes the clear color to be transparent. Therefore, on
  // systems that support it, the widget will become transparent apart from the
  // logo.

  is_group = true;
  if (m_transparent) {
    QSurfaceFormat fmt = format();
    fmt.setAlphaBufferSize(8);
    setFormat(fmt);
  }
}

GLWidget::~GLWidget() { cleanup(); }

QSize GLWidget::minimumSizeHint() const { return QSize(50, 50); }

QSize GLWidget::sizeHint() const { return QSize(400, 400); }

static void qNormalizeAngle(int &angle) {
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_xRot) {
    m_xRot = angle;
    emit xRotationChanged(angle);
    update();
  }
}

void GLWidget::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_yRot) {
    m_yRot = angle;
    emit yRotationChanged(angle);
    update();
  }
}

void GLWidget::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_zRot) {
    m_zRot = angle;
    emit zRotationChanged(angle);
    update();
  }
}

void GLWidget::cleanup() {
  if (m_program == nullptr)
    return;
  makeCurrent();
  m_logoVbo.destroy();
  delete m_program;
  m_program = 0;
  doneCurrent();
}

void GLWidget::setData(unsigned char *buf, int buf_size) {

  qDebug() << "we got a request of new data size = " << buf_size << "\n";
  iq.resize(buf_size);
  memcpy((unsigned char *)(this->iq.data()), buf, buf_size);

  iq.resize(20000);
  collectPoints();
  update();
}

void GLWidget::collectPoints() {

  float max = 0.4;
  int q = 256;
  int q4 = sqrt(q / 4);
  float one = max / q4;

  // prepare points - qam centers
  for (int x = 0; x < q4; x++)
    for (int y = 0; y < q4; y++) {
      ComplexNumber c;
      c.real(one / 2 + x * one);
      c.imag(one / 2 + y * one);
      qams.push_back(c);

      c.real(-x * one - one / 2);
      c.imag(one / 2 + y * one);
      qams.push_back(c);

      c.real(one / 2 + x * one);
      c.imag(-y * one - one / 2);
      qams.push_back(c);

      c.real(-x * one - one / 2);
      c.imag(-y * one - one / 2);
      qams.push_back(c);
    }

  std::vector<ComplexNumber> Z;

  for (int i = 0; i < iq.size() / 5 - 2; i += 2) {
    unsigned char I = iq.at(i);
    unsigned char Q = iq.at(i + 1);
    float pI = I - 127.5;
    float pQ = Q - 127.5;

    ComplexNumber z;
    z.real(pI / 127.5 * max);
    z.imag(pQ / 127.5 * max);

    Z.push_back(z);
  }

  std::vector<ComplexNumber> X;

  R.clear();

  X = Z;

  const int B = 10;
  double PI = std::acos(-1);

  float D[Z.size()][B];
  float S[Z.size()][B];
  ComplexNumber Xz[Z.size()][B];

  int N = 10;

  for (int b = 0; b < B; b++) {
    float PhiB = 1.0 * b / B * PI / 2;
    for (int k = 0; k < Z.size(); k++) {
      ComplexNumber e = 0;
      e.imag(PhiB);
      ComplexNumber z1 = Z[k] * std::exp(e);
      // decision search

      // ComplexNumber Xz;
      // find a nearest qam point
      float min = 999;
      int minPos = 0;
      for (int q = 0; q < qams.size(); q++) {
        float nor = norm(qams[q] - z1);
        nor = nor * nor;
        if (nor < min) {
          min = nor;
          minPos = q;
        }
      }
      Xz[k][b] = z1; // qams[minPos];
      D[k][b] = min;
      // printf("%f\n", min);
    }
    for (int k = N; k < Z.size() - N; k++) {
      float s = 0;
      for (int n = -N; n <= N; n++)
        s += D[k - n][b];
      S[k][b] = s;
    }
  }

  int snrS = 0;
  int snrN = 0;
  for (int k = N; k < Z.size() - 2 * N; k++) {
    float min = 999;
    int minb = 0;
    for (int b = 0; b < B; b++)
      if (S[k][b] < min) {
        min = S[k][b];
        minb = b;
      }
    // X [k][minb] = solution
    for (int b = 0; b < B; b++)

      if (is_group) {
        if (D[k][b] < 0.0000002) {
          R.push_back(Xz[k][b]);
          snrS++;
        } // else snrN++;
      } else {
        R.push_back(Xz[k][b]);
      }
  }
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "uniform vec3 my_color;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.5 + color * 0.5 * NL, 0.0, 1.0);\n"
    "   gl_FragColor.rgb = my_color; gl_FragColor.a = 1.0;\n"
    "}\n";

void GLWidget::initializeGL() {
  // In this example the widget's corresponding top-level window can change
  // several times during the widget's lifetime. Whenever this happens, the
  // QOpenGLWidget's associated context is destroyed and a new one is created.
  // Therefore we have to be prepared to clean up the resources on the
  // aboutToBeDestroyed() signal, instead of the destructor. The emission of
  // the signal will be followed by an invocation of initializeGL() where we
  // can recreate all resources.
  connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
          &GLWidget::cleanup);

  initializeOpenGLFunctions();
  glClearColor(0, 0, 0, m_transparent ? 0 : 1);

  m_program = new QOpenGLShaderProgram;
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     m_core ? vertexShaderSourceCore
                                            : vertexShaderSource);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     m_core ? fragmentShaderSourceCore
                                            : fragmentShaderSource);
  m_program->bindAttributeLocation("vertex", 0);
  m_program->bindAttributeLocation("normal", 1);
  m_program->link();

  m_program->bind();
  m_projMatrixLoc = m_program->uniformLocation("projMatrix");
  m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
  m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
  m_lightPosLoc = m_program->uniformLocation("lightPos");

  // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
  // implementations this is optional and support may not be present
  // at all. Nonetheless the below code works in all cases and makes
  // sure there is a VAO when one is needed.
  m_vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

  // Setup our vertex buffer object.
  m_logoVbo.create();
  m_logoVbo.bind();
  m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

  // Store the vertex attribute bindings for the program.
  setupVertexAttribs();

  // Our camera never changes in this example.
  m_camera.setToIdentity();
  m_camera.translate(0, 0, -1);

  // Light position is fixed.
  m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

  m_program->release();

  /*
  std::ifstream file("/home/sergey/rtl-sdr/src/serg2.iq", std::ios::binary |
  std::ios::ate); std::streamsize size = file.tellg(); file.seekg(0,
  std::ios::beg);

  //if (size > 100000) size = 100000;
  iq.resize(size);
  int n = 100000;

  file.read(( char *)(this->iq.data()), size);

  std::vector<unsigned char> iqq(iq.end() - n, iq.end());

  iq.clear();
  */
}

void GLWidget::setupVertexAttribs() {
  m_logoVbo.bind();
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glEnableVertexAttribArray(0);
  f->glEnableVertexAttribArray(1);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                           reinterpret_cast<void *>(3 * sizeof(GLfloat)));
  m_logoVbo.release();
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  m_world.setToIdentity();
  m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
  m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
  m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
  m_program->bind();
  m_program->setUniformValue(m_projMatrixLoc, m_proj);
  m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
  QMatrix3x3 normalMatrix = m_world.normalMatrix();
  m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

  m_program->setUniformValue("my_color", QVector3D(0.0, 0.7, 0.0));

  glBegin(GL_LINES);
  glVertex2f(-1.0f, 0.0f);
  glVertex2f(1.0f, 0.0f);
  glVertex2f(0.0f, -1.0f);
  glVertex2f(0.0f, 1.0f);
  glEnd();

  m_program->setUniformValue("my_color", QVector3D(0.0, 1, 0.0));

  float max = 0.4;
  int q = 256;
  int q4 = sqrt(q / 4);
  float one = max / q4;

  glPointSize(5);

  GLfloat emission[] = {0.0, 0.3, 0.3, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

  // glColor3f(1.0, 1.0, 0.0);

  // draw QAM points

  glBegin(GL_POINTS);
  for (int x = 0; x < q4; x++)
    for (int y = 0; y < q4; y++) {
      glVertex2f(one / 2 + x * one, one / 2 + y * one);

      glVertex2f(-x * one - one / 2, one / 2 + y * one);

      glVertex2f(one / 2 + x * one, -y * one - one / 2);

      glVertex2f(-x * one - one / 2, -y * one - one / 2);
    }
  glEnd();

  glPointSize(2);

  m_program->setUniformValue("my_color", QVector3D(1, 0.0, 0.0));

  // just draw all we prepared before

  glBegin(GL_POINTS);
  for (auto z : R) {
    glVertex2f(z.real(), z.imag());
  }
  glEnd();

  // todo: calc snr? printf( "SNR: %lf\n", 10 * log10(1.0*snrS / snrN)  );

  m_program->release();
}

void GLWidget::resizeGL(int w, int h) {
  m_proj.setToIdentity();
  m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event) { m_lastPos = event->pos(); }

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - m_lastPos.x();
  int dy = event->y() - m_lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(m_xRot + 8 * dy);
    setYRotation(m_yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(m_xRot + 8 * dy);
    setZRotation(m_zRot + 8 * dx);
  }
  m_lastPos = event->pos();
}
