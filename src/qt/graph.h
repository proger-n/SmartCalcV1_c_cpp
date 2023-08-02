#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QVector>

extern "C" int head_calc(char* input, int x_exist, double x_value,
                         double* result);

namespace Ui {
class Graph;
}

class Graph : public QDialog {
  Q_OBJECT

 public:
  explicit Graph(QWidget* parent = nullptr);
  ~Graph();

 private:
  Ui::Graph* ui;

 public slots:
  void slot(QString str_inp);
  void slot_min(double min_x);
  void slot_max(double max_x);
};

#endif  // GRAPH_H
