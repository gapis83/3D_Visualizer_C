#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  //____GIF
  gif_timer = new QTimer(0);  // parent must be null
  connect(gif_timer, SIGNAL(timeout()), this, SLOT(create_screen()));
  //_____

  // set value in file
  this->m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";
  //    load_settings();
  qDebug() << m_sSettingsFile;

  connect(ui->dx, SIGNAL(valueChanged(int)), this, SLOT(dx_valueChanged(int)));
  connect(ui->dy, SIGNAL(valueChanged(int)), this, SLOT(dy_valueChanged(int)));
  connect(ui->dz, SIGNAL(valueChanged(int)), this, SLOT(dz_valueChanged(int)));
  connect(ui->rdx, SIGNAL(valueChanged(int)), this,
          SLOT(rdx_valueChanged(int)));
  connect(ui->rdy, SIGNAL(valueChanged(int)), this,
          SLOT(rdy_valueChanged(int)));
  connect(ui->rdz, SIGNAL(valueChanged(int)), this,
          SLOT(rdz_valueChanged(int)));
  connect(ui->modelScale, SIGNAL(valueChanged(double)), this,
          SLOT(modelScale_valueChanged(double)));
  connect(ui->openFile, SIGNAL(clicked()), this, SLOT(openFile_clicked()));
  connect(ui->Photo, SIGNAL(clicked()), this, SLOT(Photo_clicked()));
  connect(ui->color, SIGNAL(clicked()), this, SLOT(color_clicked()));
  connect(ui->optimization, SIGNAL(clicked()), this,
          SLOT(optimization_clicked()));
}

MainWindow::~MainWindow() {
  //    save_settings();
  delete ui;
  delete gif_timer;
}

void MainWindow::free_vertex_and_facet() {
  if (ui->widget->top.coordinates) {
    free(ui->widget->top.coordinates);
    ui->widget->top.counter_vertex = 0;
  }
  if (ui->widget->polygon.lines) {
    free(ui->widget->polygon.lines);
    ui->widget->polygon.counter_facets = 0;
    ui->widget->polygon.counter_lines = 0;
  }
}

void MainWindow::openFile_clicked() {
  // QString file = QFileDialog::getOpenFileName(this, "Open file", ".", tr( "
  // (*.obj)"));
  QString file = QFileDialog::getOpenFileName(this, tr("Open file"),
                                              QDir::homePath(), tr(" (*.obj)"));

  if (!file.isEmpty()) {
    if (ui->widget->top.counter_vertex > 0) free_vertex_and_facet();
    ui->statusBar->showMessage(file);
    QByteArray ba = file.toLocal8Bit();
    char *str = ba.data();
    int error = 0;
    error = parser(str, &ui->widget->top, &ui->widget->polygon);

    if (error) {
      ui->statusBar->showMessage("file not found");
    } else {
      allowed++;
    }

    ui->coun_vertexes->setText(QString::number(ui->widget->top.counter_vertex));
    ui->count_facets->setText(
        QString::number((ui->widget->polygon.counter_lines - 1) / 2));
    ui->filename->setText(file.right(file.size() - file.lastIndexOf("/") - 1));
  }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0) {
    ui->widget->change_zoom(0.9);
    ui->doubleSpinBox_Scale->setValue(ui->widget->scale);
  } else {
    ui->widget->change_zoom(1.1);
    ui->doubleSpinBox_Scale->setValue(ui->widget->scale);
  }
}

void MainWindow::on_doubleSpinBox_Scale_valueChanged(double valueScale) {
  ui->widget->scale = valueScale;
}

void MainWindow::update_clicked() {
  // move
  ui->widget->step.dx = ui->widget->step.dx - (double)ui->dx->value();
  ui->widget->step.dy = ui->widget->step.dy - (double)ui->dy->value();
  ui->widget->step.dz = ui->widget->step.dz - (double)ui->dz->value();

  if (ui->widget->step.dx == 0 || ui->widget->step.dy == 0 ||
      ui->widget->step.dz == 0) {
    displace(&ui->widget->top, ui->widget->step);
  }

  ui->widget->step.dx = (double)ui->dx->value();
  ui->widget->step.dy = (double)ui->dy->value();
  ui->widget->step.dz = (double)ui->dz->value();

  // rotate
  ui->widget->rotation.rx = ui->widget->rotation.rx - (double)ui->rdx->value();
  ui->widget->rotation.ry = ui->widget->rotation.ry - (double)ui->rdy->value();
  ui->widget->rotation.rz = ui->widget->rotation.rz - (double)ui->rdz->value();

  if (ui->widget->rotation.rx != 0 || ui->widget->rotation.ry != 0 ||
      ui->widget->rotation.rz != 0) {
    rotation_model(&ui->widget->top, ui->widget->rotation);
  }

  ui->widget->rotation.rx = (double)ui->rdx->value();
  ui->widget->rotation.ry = (double)ui->rdy->value();
  ui->widget->rotation.rz = (double)ui->rdz->value();

  // scale
  if (ui->modelScale->value() <= 0) {
    ui->modelScale->setValue(1);
  }

  ui->widget->modelScale = ui->modelScale->value() / ui->widget->modelScale;

  if (ui->widget->modelScale != 0 && ui->modelScale->value() > 0) {
    scaling_image(&ui->widget->top, ui->widget->modelScale);
  }
  ui->widget->modelScale = ui->modelScale->value();
}

void MainWindow::on_spinBox_line_width_valueChanged(int value) {
  ui->widget->lineWidth = value;
}

void MainWindow::on_spinBox_point_size_valueChanged(int value) {
  ui->widget->pointSize = value;
  //     qDebug() << ui->widget->pointSize;
}

void MainWindow::color_clicked() {
  QColorDialog colorDialog;
  colorDialog.move(QGuiApplication::primaryScreen()->geometry().center() -
                   colorDialog.rect().center());
  if (ui->colorPoint->isChecked()) {
    ui->widget->colorPoint = colorDialog.getColor(Qt::gray);
  } else if (ui->colorLine->isChecked()) {
    ui->widget->colorLine = colorDialog.getColor(Qt::gray);
  } else if (ui->colorBackground->isChecked()) {
    ui->widget->colorWidget = colorDialog.getColor(Qt::gray);
  }
}

// ________PHOTO ___GIF
void MainWindow::Photo_clicked() {
  QFileDialog file_dialog_photo(this);
  QString f_name_photo = file_dialog_photo.getSaveFileName(
      this, "Save as...", QDir::currentPath(), "BMP (*.bmp);; JPEG (*.jpeg)");
  QFile file(f_name_photo);
  file.open(QIODevice::WriteOnly);
  QRect rect(0, 0, ui->widget->width(), ui->widget->height());
  QPixmap pixmap = ui->widget->grab(rect);
  pixmap.copy(rect);
  pixmap.toImage().save(&file, "jpg");
  QString q_command_line = "open " + f_name_photo;
  QByteArray temp = q_command_line.toLocal8Bit();
  char *command_line = temp.data();
  system(command_line);
}

void MainWindow::on_stop_and_save_GIF_clicked() {
  gif_timer->start(100);
  create_screen();
}

void MainWindow::save_gif() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save screenshot"), "",
      tr("GIF screenshot (*.gif);;GIF screenshot (*.gif)"));
  QGifImage gif(QSize(640, 480));

  gif.setDefaultTransparentColor(Qt::black);
  gif.setDefaultDelay(100);

  for (QVector<QImage>::Iterator img = mas_image.begin();
       img != mas_image.end(); ++img) {
    gif.addFrame(*img);
  }

  gif.save(fileName);
  mas_image.clear();
  ui->label_Timer_GIF->clear();
  time = 0.0;
}

void MainWindow::create_screen() {
  if (time <= 5.0) {
    mas_image.push_back(ui->widget->grab().toImage());
    time += 0.1;
    ui->label_Timer_GIF->setText(QString::number(time));
  } else if (time >= 5.0) {
    save_gif();
    gif_timer->stop();
  }
}

//_________GIF end

void MainWindow::optimization_clicked() {
  double xMax = 0;
  double xMin = 0;
  double yMax = 0;
  double yMin = 0;
  double zMax = 0;
  double zMin = 0;
  double maxSize = 0;
  double scale = 100;

  for (int i = 3; i < ui->widget->top.counter_vertex; i += 3) {
    if (ui->widget->top.coordinates[i] > xMax)
      xMax = ui->widget->top.coordinates[i];
    if (ui->widget->top.coordinates[i] < xMin)
      xMin = ui->widget->top.coordinates[i];
    if (ui->widget->top.coordinates[i + 1] > yMax)
      yMax = ui->widget->top.coordinates[i];
    if (ui->widget->top.coordinates[i + 1] < yMin)
      yMin = ui->widget->top.coordinates[i];
    if (ui->widget->top.coordinates[i + 2] > zMax)
      zMax = ui->widget->top.coordinates[i];
    if (ui->widget->top.coordinates[i + 2] < zMin)
      zMin = ui->widget->top.coordinates[i];
  }
  maxSize = xMax - xMin;
  if (maxSize < yMax - yMin) maxSize = yMax - yMin;
  if (maxSize < zMax - zMin) maxSize = zMax - zMin;

  while (maxSize * scale > 1 && scale > 0.0001) {
    scale *= 0.9;
  }

  ui->doubleSpinBox_Scale->setValue(scale);
}

void MainWindow::on_comboBox_point_form_currentIndexChanged(int index) {
  ui->widget->pointForm = index;
  ui->widget->update();
}

void MainWindow::on_comboBox_line_form_currentIndexChanged(int index) {
  ui->widget->lineForm = index;
  ui->widget->update();
}

void MainWindow::on_radioButton_frustum_clicked(bool checked) {
  ui->widget->frustum = checked;
}

void MainWindow::on_radioButton_ortho_clicked() {
  ui->widget->frustum = ui->widget->EMPTY;
}

void MainWindow::load_settings() {
  QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
  ui->comboBox_point_form->setCurrentIndex(
      settings.value("comboBox_point_form").toInt());
  ui->comboBox_line_form->setCurrentIndex(
      settings.value("comboBox_line_form").toInt());

  ui->spinBox_point_size->setValue(
      settings.value("spinBox_point_size").toInt());
  ui->spinBox_line_width->setValue(
      settings.value("spinBox_line_width").toInt());

  ui->colorBackground->setChecked(settings.value("colorBackground").toBool());
  ui->colorLine->setChecked(settings.value("colorLine").toBool());
  ui->colorPoint->setChecked(settings.value("colorPoint").toBool());

  ui->widget->colorLine = settings.value("colorLine_c").value<QColor>();
  ui->widget->colorPoint = settings.value("colorPoint_c").value<QColor>();
  ui->widget->colorWidget = settings.value("colorWidget_c").value<QColor>();
}

void MainWindow::save_settings() {
  QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
  settings.setValue("comboBox_point_form",
                    ui->comboBox_point_form->currentIndex());
  settings.setValue("spinBox_point_size", ui->spinBox_point_size->value());
  settings.setValue("comboBox_line_form",
                    ui->comboBox_line_form->currentIndex());
  settings.setValue("spinBox_line_width", ui->spinBox_line_width->value());

  settings.setValue("colorBackground", ui->colorBackground->isChecked());
  settings.setValue("colorLine", ui->colorLine->isChecked());
  settings.setValue("colorPoint", ui->colorPoint->isChecked());

  settings.setValue("colorLine_c", ui->widget->colorLine);
  settings.setValue("colorPoint_c", ui->widget->colorPoint);
  settings.setValue("colorWidget_c", ui->widget->colorWidget);
}

void MainWindow::on_save_settings_clicked() {
  save_settings();
  QMessageBox::information(this, "Сохранение настроек",
                           "Сохранение настроек выполнено успешно");
}

void MainWindow::on_load_setting_clicked() { load_settings(); }

void MainWindow::on_dial_rdx_valueChanged(int value) {
  if (allowed != 0) {
    ui->rdx->setValue(value);
    update_clicked();
  }
}

void MainWindow::on_dial_rdy_valueChanged(int value) {
  if (allowed != 0) {
    ui->rdy->setValue(value);
    update_clicked();
  }
}

void MainWindow::on_dial_rdz_valueChanged(int value) {
  if (allowed != 0) {
    ui->rdz->setValue(value);
    update_clicked();
  }
}

void MainWindow::on_horizontalSlider_dx_valueChanged(int value) {
  if (allowed != 0) {
    ui->dx->setValue(value);
    update_clicked();
  }
}

void MainWindow::on_horizontalSlider_dy_valueChanged(int value) {
  if (allowed != 0) {
    ui->dy->setValue(value);
    update_clicked();
  }
}

void MainWindow::on_horizontalSlider_dz_valueChanged(int value) {
  if (allowed != 0) {
    ui->dz->setValue(value);
    update_clicked();
  }
}

void MainWindow::dx_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->dx->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::dy_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->dy->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::dz_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->dz->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::rdx_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->rdx->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::rdy_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->rdy->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::rdz_valueChanged(int arg1) {
  if (allowed != 0) {
    ui->rdz->setValue(arg1);
    update_clicked();
  }
}

void MainWindow::modelScale_valueChanged(double arg1) {
  if (allowed != 0) {
    ui->modelScale->setValue(arg1);
    update_clicked();
  }
}
