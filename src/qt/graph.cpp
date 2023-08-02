#include "graph.h"

#include "ui_graph.h"

Graph::Graph(QWidget *parent) : QDialog(parent), ui(new Ui::Graph) {
  ui->setupUi(this);
}

Graph::~Graph() { delete ui; }

void Graph::slot(QString str_inp) { ui->textEdit->setText(str_inp); }

void Graph::slot_min(double min_x) { ui->doubleSpinBox_min_x->setValue(min_x); }

void Graph::slot_max(double max_x) {
  ui->doubleSpinBox_max_x->setValue(max_x);
  double x_min = ui->doubleSpinBox_min_x->value();
  double x_max = ui->doubleSpinBox_max_x->value();
  double h = (x_max - x_min) / 10000;
  double y_min = 0;
  double y_max = 0;
  QVector<double> x, y;

  QString tmp = ui->textEdit->toPlainText();
  QByteArray ba = tmp.toLocal8Bit();
  char *str = ba.data();

  double Y;

  ui->widget->xAxis->setRange(x_min, x_max);
  for (double X = x_min; X <= x_max; X += h) {
    int res = head_calc(str, 1, X, &Y);
    if (res == 0) {
      x.push_back(X);
      y.push_back(Y);
      if (Y > y_max || y_max != y_max) y_max = Y;
      if (Y < y_min || y_min != y_min) y_min = Y;
    }
  }

  ui->widget->yAxis->setRange(y_min, y_max);
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x, y);
  ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->widget->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
  ui->widget->replot();
}
