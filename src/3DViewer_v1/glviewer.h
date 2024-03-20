#ifndef GLVIEWER_H
#define GLVIEWER_H

#define GL_SILENCE_DEPRECATION

#include <QColorDialog>
#include <QFileDialog>
#include <QTimer>
#include <QtOpenGL>
#include <QtOpenGLWidgets>
#include <QtWidgets/QWidget>
#include <iostream>

extern "C" {
#include "../parser/s21_viewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class GLviewer;
}
QT_END_NAMESPACE

class GLviewer : public QOpenGLWidget {
  Q_OBJECT

 public:
  GLviewer(QWidget *parent = nullptr);
  ~GLviewer();

  Vertex top;
  Facet polygon;

  double scale;
  double modelScale;
  Step step;
  Rotation rotation;

  QColor colorWidget;
  QColor colorLine;
  QColor colorPoint;

  int pointSize;
  int pointForm;
  int lineWidth;
  int lineForm;
  int frustum;

  enum {
    EMPTY,
    POINT_ROUND = 1,
    POINT_QUADRO,
    LINE_SOLID = 1,
    LINE_DASHED,
    IS_CHECKED = 1

  };
  void pointSettingsAndDraw();
  void linesSettingsAndDraw();
  void projectionSelect();

  void change_zoom(double value);

 private:
  //    Ui::GLviewer *ui;

  float xRot, yRot, zRot;
  QPoint mPos;

  void drawingElements();
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};
#endif  // GLVIEWER_H
