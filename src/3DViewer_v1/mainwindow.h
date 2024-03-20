#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>

#include "glviewer.h"

//_________GIF
#include <math.h>

#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QSettings>
#include <QTextEdit>
#include <QTimer>
#include <cstdint>
#include <vector>

#include "QtGifImage-master/src/gifimage/qgifimage.h"
//__________________

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void free_vertex_and_facet();

  void load_settings();

  void save_settings();

  //___ GIF
  QVector<QImage> mas_image;
  //____
  QString m_sSettingsFile;

 protected:
  void keyPressEvent(QKeyEvent *event) override {
    if (event->modifiers() == Qt::ControlModifier &&
        event->key() == Qt::Key_W) {
      close();
    }
  }

 private slots:

  void openFile_clicked();

  void on_doubleSpinBox_Scale_valueChanged(double valueScale);

  void update_clicked();

  void on_spinBox_line_width_valueChanged(int value);

  void on_spinBox_point_size_valueChanged(int value);

  void color_clicked();

  //_______GIF
  void Photo_clicked();

  void create_screen();

  void on_stop_and_save_GIF_clicked();

  void save_gif();
  //_______

  void optimization_clicked();

  void on_comboBox_point_form_currentIndexChanged(int index);

  void on_comboBox_line_form_currentIndexChanged(int index);

  void on_radioButton_frustum_clicked(bool checked);

  void on_radioButton_ortho_clicked();

  void on_load_setting_clicked();

  void on_save_settings_clicked();

  void on_dial_rdx_valueChanged(int value);

  void on_dial_rdy_valueChanged(int value);

  void on_dial_rdz_valueChanged(int value);

  void on_horizontalSlider_dx_valueChanged(int value);

  void on_horizontalSlider_dy_valueChanged(int value);

  void on_horizontalSlider_dz_valueChanged(int value);

  void dx_valueChanged(int arg1);

  void dy_valueChanged(int arg1);

  void dz_valueChanged(int arg1);

  void rdx_valueChanged(int arg1);

  void rdy_valueChanged(int arg1);

  void rdz_valueChanged(int arg1);

  void modelScale_valueChanged(double arg1);

 private:
  Ui::MainWindow *ui;
  void wheelEvent(QWheelEvent *event) override;

  //_____GIF
  QTimer *timer;
  QTimer *gif_timer;
  QThread *somethread;
  double time = 0;
  int xyz = 1;
  int flag_record = 0;
  int allowed = 0;
  //_-----
};
#endif  // MAINWINDOW_H
