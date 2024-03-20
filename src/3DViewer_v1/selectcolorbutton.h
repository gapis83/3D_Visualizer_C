#ifndef SELECTCOLORBUTTON_H
#define SELECTCOLORBUTTON_H

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QPushButton>

class SelectColorButton : public QPushButton {
  Q_OBJECT
 public:
  SelectColorButton(QWidget* parent);

  void setColor(const QColor& color);
  const QColor& getColor() const;

 public slots:
  void updateColor();
  void changeColor();

 private:
  QColor color;
};

#endif  // SELECTCOLORBUTTON_H
