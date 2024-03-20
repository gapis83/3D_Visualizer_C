#include "glviewer.h"

GLviewer::GLviewer(QWidget* parent) : QOpenGLWidget(parent) {
  //    setWindowTitle("3Dviewer");
  setGeometry(400, 200, 1200, 900);
  top = {0, nullptr};
  polygon = {0, 0, nullptr};
  step = {0, 0, 0};
  rotation = {0, 0, 0};
  modelScale = 1;
  scale = 0.05;

  colorWidget = QColor(Qt::black);
  colorLine = QColor(Qt::darkBlue);
  colorPoint = QColor(Qt::red);

  pointSize = 1;
  lineWidth = 1;
  pointForm = POINT_ROUND;
  lineForm = LINE_SOLID;
  frustum = EMPTY;
}

GLviewer::~GLviewer() {
  //    delete ui;
}

void GLviewer::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
}

void GLviewer::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void GLviewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(colorWidget.redF(), colorWidget.greenF(), colorWidget.blueF(),
               0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glTranslatef(0, 0, 0);
  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);

  drawingElements();
  update();
}

void GLviewer::drawingElements() {
  // Projection//
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  projectionSelect();

  glScaled(scale, scale, scale);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, top.coordinates);

  // Points//
  if (pointForm != EMPTY) {
    pointSettingsAndDraw();
  }
  // Lines//
  if (lineForm != EMPTY) {
    linesSettingsAndDraw();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLviewer::pointSettingsAndDraw() {
  if (pointForm == POINT_ROUND) {
    glEnable(GL_POINT_SMOOTH);
  } else if (pointForm == POINT_QUADRO) {
    glDisable(GL_POINT_SMOOTH);
  }
  glPointSize(pointSize);
  glColor3d(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());
  glDrawElements(GL_POINTS, polygon.counter_lines * 2, GL_UNSIGNED_INT,
                 polygon.lines);
}

void GLviewer::linesSettingsAndDraw() {
  if (lineForm == LINE_DASHED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(4, 0x00FF);
    ////        glLineStipple(2, 255);
    ////        glLineStipple(1, 0x1C47);
  }
  glColor3d(colorLine.redF(), colorLine.greenF(), colorLine.blueF());
  glLineWidth(lineWidth);
  glDrawElements(GL_LINES, polygon.counter_lines * 2, GL_UNSIGNED_INT,
                 polygon.lines);
  glDisable(GL_LINE_STIPPLE);
}

void GLviewer::projectionSelect() {
  if (frustum == EMPTY) {
    glEnable(GL_DEPTH_TEST);
    //            glOrtho(-1, 1, -1, 1, 100, 100);
    glOrtho(-100, 100, 800, 800, 100, 100);
  } else {
    glDisable(GL_DEPTH_TEST);
    glFrustum(-1000, 1000, -1000, 1000, 1000, 1000);
  }
}

void GLviewer::mousePressEvent(QMouseEvent* mo) {
  mPos = mo->pos();
  update();
}

void GLviewer::mouseMoveEvent(QMouseEvent* mo) {
  if (!(mo->pos().x() >= 625 && mo->pos().x() <= 901 && mo->pos().y() >= 630 &&
        mo->pos().y() <= 851)) {
    xRot += 1 / M_PI * (mo->pos().y() - mPos.y());
    yRot += 1 / M_PI * (mo->pos().x() - mPos.x());
  }
  mPos = mo->pos();
  update();
}

void GLviewer::change_zoom(double value) {
  scale *= value;
  update();
}
