#ifndef SUDOKU_H
#define SUDOKU_H

#include <QMainWindow>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Sudoku; }
QT_END_NAMESPACE

class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    Sudoku(QWidget *parent = nullptr);
    ~Sudoku();

private slots:
    void on_pushButton_reset_clicked();
    void on_pushButton_risolvi_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_forward_clicked();

    void on_pushButton_end_clicked();

    void on_pushButton_start_clicked();

private:
    Ui::Sudoku *ui;
    QVector< QVector<QLineEdit*>> _cells;
    QVector< QVector<int> > _sudo;
    QMessageBox error;
    int step;

    bool isValid(int digit, unsigned int row, unsigned int column);
    bool solve(int row, int col);
    int nextStep(int step);
    int prevStep(int step);
    void print();
    void print(int step);
};
#endif // SUDOKU_H
