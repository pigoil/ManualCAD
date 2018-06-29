#include "parameterdialog.h"
#include "ui_parameterdialog.h"

ParameterDialog::ParameterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterDialog)
{
    ui->setupUi(this);
}

ParameterDialog::~ParameterDialog()
{
    delete ui;
}

float ParameterDialog::value()
{
    return ui->doubleSpinBox->value();
}
