#include "settings.h"
#include "ui_settings.h"
#include "myvalidator.h"
#include <QDialogButtonBox>
#include <QMessageBox>

Settings::Settings(Setting setting, QWidget *parent):
    QDialog(parent), ui_(new Ui::Settings), setting_(setting)
{
    ui_->setupUi(this);

    ui_->ceditMinResult->setValidator(new QIntValidator(0, 1000, this));
    ui_->ceditNGenerations->setValidator(new QIntValidator(10, 1000, this));

    ui_->peditCollapseM->setValidator(new MyValidator(0, 0, 3, this));
    ui_->peditCopyProbability->setValidator(new MyValidator(0, 1, 3, this));
    ui_->peditHoistM->setValidator(new MyValidator(0, 1, 3, this));
    ui_->peditNodeM->setValidator(new MyValidator(0, 1, 3, this));
    ui_->peditSubtreeC->setValidator(new MyValidator(0, 1, 3, this));
    ui_->peditSubtreeM->setValidator(new MyValidator(0, 1, 3, this));

    ui_->geditPopulationSize->setValidator(new QIntValidator(10, 1000, this));
    ui_->geditTounramentSize->setValidator(new QIntValidator(5, 1000, this));
    ui_->geditTreeDepth->setValidator(new QIntValidator(2, 20, this));

    ui_->neditFunctionProbability->setValidator(new MyValidator(0, 1, 3, this));
    ui_->neditMorphoProbability->setValidator(new MyValidator(0, 1, 3, this));
    ui_->neditThreshProbability->setValidator(new MyValidator(0, 1, 3, this));

    QPushButton *okB = ui_->buttonBox->button(QDialogButtonBox::Ok);
    connect(okB, SIGNAL(clicked()), this, SLOT(okButton()));


    setGeneticOperationProbabilities(*setting_.geneticOperationProbabilities);
    setGeneticParam(*setting_.geneticParam);
    setNodeProbabilities(*setting_.geneticNodeProbabilities);
    setStopCriterium(*setting_.stopCriteriumParameters);

    show();
}

Settings::~Settings()
{
    delete ui_;
}

void Settings::setGeneticParam(const GeneticParameters &param)
{
    ui_->geditPopulationSize->setText(QString::number(param.populationSize));
    ui_->geditTounramentSize->setText(QString::number(param.tournamentSize));
    ui_->geditTreeDepth->setText(QString::number(param.treeDepth));
}

void Settings::getGeneticParam(GeneticParameters &param) const
{
    param.populationSize = ui_->geditPopulationSize->text().toInt();
    param.tournamentSize = ui_->geditTounramentSize->text().toInt();
    param.treeDepth = ui_->geditTreeDepth->text().toInt();
}

void Settings::setGeneticOperationProbabilities(const GeneticOperationProbabilities &probabilities)
{
    ui_->peditCollapseM->setText(QString::number(probabilities.collapseMutation));
    ui_->peditCopyProbability->setText(QString::number(probabilities.copy));
    ui_->peditHoistM->setText(QString::number(probabilities.hoistMutation));
    ui_->peditNodeM->setText(QString::number(probabilities.nodeMutation));
    ui_->peditSubtreeC->setText(QString::number(probabilities.subtreeCrossover));
    ui_->peditSubtreeM->setText(QString::number(probabilities.subtreeMutation));
}

void Settings::getGeneticOperationProbabilities(GeneticOperationProbabilities &probabilities) const
{
    probabilities.collapseMutation = ui_->peditCollapseM->text().toDouble();
    probabilities.copy = ui_->peditCopyProbability->text().toDouble();
    probabilities.hoistMutation = ui_->peditHoistM->text().toDouble();
    probabilities.nodeMutation = ui_->peditNodeM->text().toDouble();
    probabilities.subtreeCrossover = ui_->peditSubtreeC->text().toDouble();
    probabilities.subtreeMutation = ui_->peditSubtreeM->text().toDouble();
}

void Settings::setNodeProbabilities(const GeneticNodeProbabilities &probabilities)
{
    ui_->neditFunctionProbability->setText(QString::number(probabilities.function));
    ui_->neditMorphoProbability->setText(QString::number(probabilities.morpho));
    ui_->neditThreshProbability->setText(QString::number(probabilities.thresh));
}

void Settings::getNodeProbabilities(GeneticNodeProbabilities &probabilities) const
{
    probabilities.function = ui_->neditFunctionProbability->text().toDouble();
    probabilities.morpho = ui_->neditMorphoProbability->text().toDouble();
    probabilities.thresh = ui_->neditThreshProbability->text().toDouble();
}

void Settings::setStopCriterium(const StopCriteriumParameters &param)
{
    ui_->ceditMinResult->setText(QString::number(param.minResult));
    ui_->ceditNGenerations->setText(QString::number(param.nGenerations));
}

void Settings::getStopCriterium(StopCriteriumParameters &param) const
{
    param.minResult = ui_->ceditMinResult->text().toInt();
    param.nGenerations = ui_->ceditNGenerations->text().toInt();
}

void Settings::okButton()
{
    getGeneticOperationProbabilities(*setting_.geneticOperationProbabilities);
    getGeneticParam(*setting_.geneticParam);
    getNodeProbabilities(*setting_.geneticNodeProbabilities);
    getStopCriterium(*setting_.stopCriteriumParameters);
}
