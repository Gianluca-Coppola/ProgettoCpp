#include "sudoku.h"
#include "ui_sudoku.h"

Sudoku::Sudoku(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sudoku)
{
    ui->setupUi(this);
    setFixedSize(410, 540);

    QRegularExpression rx("[1-9]");
    QValidator* validator = new QRegularExpressionValidator(rx, this);

    _cells = QVector<QVector< QLineEdit* >>(9);
    for(unsigned int i = 0; i < 9; ++i)
        _cells[i] = QVector<QLineEdit*>(9);

    _cells[0][0] = ui->block1cell1;
    _cells[0][1] = ui->block1cell2;
    _cells[0][2] = ui->block1cell3;
    _cells[0][3] = ui->block2cell1;
    _cells[0][4] = ui->block2cell2;
    _cells[0][5] = ui->block2cell3;
    _cells[0][6] = ui->block3cell1;
    _cells[0][7] = ui->block3cell2;
    _cells[0][8] = ui->block3cell3;

    _cells[1][0] = ui->block1cell4;
    _cells[1][1] = ui->block1cell5;
    _cells[1][2] = ui->block1cell6;
    _cells[1][3] = ui->block2cell4;
    _cells[1][4] = ui->block2cell5;
    _cells[1][5] = ui->block2cell6;
    _cells[1][6] = ui->block3cell4;
    _cells[1][7] = ui->block3cell5;
    _cells[1][8] = ui->block3cell6;

    _cells[2][0] = ui->block1cell7;
    _cells[2][1] = ui->block1cell8;
    _cells[2][2] = ui->block1cell9;
    _cells[2][3] = ui->block2cell7;
    _cells[2][4] = ui->block2cell8;
    _cells[2][5] = ui->block2cell9;
    _cells[2][6] = ui->block3cell7;
    _cells[2][7] = ui->block3cell8;
    _cells[2][8] = ui->block3cell9;

    _cells[3][0] = ui->block4cell1;
    _cells[3][1] = ui->block4cell2;
    _cells[3][2] = ui->block4cell3;
    _cells[3][3] = ui->block5cell1;
    _cells[3][4] = ui->block5cell2;
    _cells[3][5] = ui->block5cell3;
    _cells[3][6] = ui->block6cell1;
    _cells[3][7] = ui->block6cell2;
    _cells[3][8] = ui->block6cell3;

    _cells[4][0] = ui->block4cell4;
    _cells[4][1] = ui->block4cell5;
    _cells[4][2] = ui->block4cell6;
    _cells[4][3] = ui->block5cell4;
    _cells[4][4] = ui->block5cell5;
    _cells[4][5] = ui->block5cell6;
    _cells[4][6] = ui->block6cell4;
    _cells[4][7] = ui->block6cell5;
    _cells[4][8] = ui->block6cell6;

    _cells[5][0] = ui->block4cell7;
    _cells[5][1] = ui->block4cell8;
    _cells[5][2] = ui->block4cell9;
    _cells[5][3] = ui->block5cell7;
    _cells[5][4] = ui->block5cell8;
    _cells[5][5] = ui->block5cell9;
    _cells[5][6] = ui->block6cell7;
    _cells[5][7] = ui->block6cell8;
    _cells[5][8] = ui->block6cell9;

    _cells[6][0] = ui->block7cell1;
    _cells[6][1] = ui->block7cell2;
    _cells[6][2] = ui->block7cell3;
    _cells[6][3] = ui->block8cell1;
    _cells[6][4] = ui->block8cell2;
    _cells[6][5] = ui->block8cell3;
    _cells[6][6] = ui->block9cell1;
    _cells[6][7] = ui->block9cell2;
    _cells[6][8] = ui->block9cell3;

    _cells[7][0] = ui->block7cell4;
    _cells[7][1] = ui->block7cell5;
    _cells[7][2] = ui->block7cell6;
    _cells[7][3] = ui->block8cell4;
    _cells[7][4] = ui->block8cell5;
    _cells[7][5] = ui->block8cell6;
    _cells[7][6] = ui->block9cell4;
    _cells[7][7] = ui->block9cell5;
    _cells[7][8] = ui->block9cell6;

    _cells[8][0] = ui->block7cell7;
    _cells[8][1] = ui->block7cell8;
    _cells[8][2] = ui->block7cell9;
    _cells[8][3] = ui->block8cell7;
    _cells[8][4] = ui->block8cell8;
    _cells[8][5] = ui->block8cell9;
    _cells[8][6] = ui->block9cell7;
    _cells[8][7] = ui->block9cell8;
    _cells[8][8] = ui->block9cell9;

    //Allows only number between 1 and 9 as input
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j){
            _cells[i][j]->setValidator(validator);
        }

    //instantiate sudoku matrix
    _sudo = QVector<QVector<int>>(9);
    for(unsigned int i = 0; i < 9; ++i)
        _sudo[i] = QVector<int>(9);

    //initialize sudoku matrix with 0s
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j)
            _sudo[i][j] = 0;

    step = 0;
}

Sudoku::~Sudoku()
{
    delete ui;
}

void Sudoku::on_pushButton_risolvi_clicked()
{
    //Sets sudoku cells to read-only, discern which cells should contain the solution and sets their color to red.
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j){
            _cells[i][j]->setReadOnly(true);
            if(_cells[i][j]->text() == ""){
                _cells[i][j]->setModified(false);
                _cells[i][j]->setStyleSheet("color: red");
            }else{
                _sudo[i][j] = _cells[i][j]->text().toInt();
            }
        }
    //Check if inserted Sudoku is solvable.
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j)
            if(!isValid(_sudo[i][j], i, j)){
                error.critical(0,"Error","Input is not valid.");
                error.setFixedSize(300,150);
                return;
            }
    //Tries to solve sudoku
    if(!solve(0, 0)){
        error.critical(0,"Error","The sudoku has no solution.");
        error.setFixedSize(300,150);
    }else{
        step = prevStep(81);
        ui->pushButton_forward->setEnabled(false);
        ui->pushButton_end->setEnabled(false);
        if(prevStep(step) > 0){
            ui->pushButton_back->setEnabled(true);
            ui->pushButton_start->setEnabled(true);
        }
        print();
    }
}

bool Sudoku::solve(int row, int col)
{
    int nextR = row;
    int nextC = col;
    if(col == 8)
        nextR++;
    if(col == 8)
        nextC = 0;
    else
        nextC++;

    //if we reach row 9, we finished the sudoku.
    if(row == 9)
        return true;

    //if current cell contains user set digit, go to next cell
    QLineEdit* current = _cells[row][col];
    if(current->isModified())
        return solve(nextR, nextC);

    for(unsigned int val = 1; val <= 9; ++val){
        _sudo[row][col] = val;
        if(isValid(_sudo[row][col], row, col) && solve(nextR, nextC)){
            return true;
        }
        _sudo[row][col] = 0;
    }

    return false;
}


void Sudoku::on_pushButton_reset_clicked()
{
    ui->pushButton_back->setEnabled(false);
    ui->pushButton_forward->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    step = 0;
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j){
            _sudo[i][j] = 0;
            _cells[i][j]->setText("");
            _cells[i][j]->setReadOnly(false);
            _cells[i][j]->setStyleSheet("color: black");
        }
}

bool Sudoku::isValid(int digit, unsigned int row, unsigned int col)
{

    for(unsigned int i = 0; i < 9; ++i)
        if(digit != 0 && i != row && digit == _sudo[i][col])
            return false;


    for(unsigned int j = 0; j < 9; ++j)
        if(digit != 0 && j != col && digit == _sudo[row][j])
            return false;


    unsigned int row_start = row - (row % 3);
    unsigned int col_start = col - (col % 3);

    for(unsigned int i = row_start; i < row_start + 3; ++i)
        for(unsigned int j = col_start; j < col_start + 3; ++j){
            if(digit != 0 && i != row && j != col && digit == _sudo[i][j])
                return false;
        }

    return true;
}



void Sudoku::print()
{
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j)
            if(!_cells[i][j]->isModified())
                _cells[i][j]->setText(QString::number(_sudo[i][j]));
}

void Sudoku::print(int step)
{
    for(unsigned int i = 0; i < 9; ++i)
        for(unsigned int j = 0; j < 9; ++j)
            if(!_cells[i][j]->isModified()){
                if(step >= 0 && static_cast<int>((i*9)+j) <= step){
                    _cells[i][j]->setText(QString::number(_sudo[i][j]));
                }else {
                    _cells[i][j]->setText("");
                }
            }
}


void Sudoku::on_pushButton_back_clicked()
{
    //take previous non-fixed step
    step--;
    while(_cells[step/9][step%9]->isModified() && step > 0)
        step--;

    if(prevStep(step) < 0){
        ui->pushButton_back->setEnabled(false);
        ui->pushButton_start->setEnabled(false);
    }
    if(step < 80){
        ui->pushButton_forward->setEnabled(true);
        ui->pushButton_end->setEnabled(true);
    }
    print(step);
}

void Sudoku::on_pushButton_forward_clicked()
{
    //takes next non-fixed step
    step++;
    while(_cells[step/9][step%9]->isModified() && step < 80)
        step++;

    if(step >= 0){
        ui->pushButton_back->setEnabled(true);
        ui->pushButton_start->setEnabled(true);
    }
    if(step == 80 || nextStep(step) > 80){
        ui->pushButton_forward->setEnabled(false);
        ui->pushButton_end->setEnabled(false);
    }

    print(step);

}

int Sudoku::nextStep(int step){

    int next = step + 1;
    while(next <= 80 && _cells[next/9][next%9]->isModified())
        next++;
    return next;

}

int Sudoku::prevStep(int step){

    int prev = step - 1;
    while(prev >= 0 && _cells[prev/9][prev%9]->isModified())
        prev--;
    return prev;

}




void Sudoku::on_pushButton_end_clicked()
{
    step = prevStep(81);
    ui->pushButton_back->setEnabled(true);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_forward->setEnabled(false);
    print(step);
}

void Sudoku::on_pushButton_start_clicked()
{
    step = -1;
    ui->pushButton_back->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(true);
    ui->pushButton_forward->setEnabled(true);
    step = nextStep(step);
    print(step);
}
