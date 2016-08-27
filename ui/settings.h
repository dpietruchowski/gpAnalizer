#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "application/application.h"

namespace Ui {
class Settings;
}

struct Setting
{
    GeneticParameters geneticParam;
    GeneticOperationProbabilities geneticOperationProbabilities;
    GeneticNodeProbabilities geneticNodeProbabilities;
    StopCriteriumParameters stopCriteriumParameters;
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public:
    void setLineEdits();

private:
    void setGeneticParam(const GeneticParameters& param);
    void getGeneticParam(GeneticParameters &param) const;
    void setGeneticOperationProbabilities(const GeneticOperationProbabilities&
                                          probabilities);
    void getGeneticOperationProbabilities(GeneticOperationProbabilities &
                                          probabilities) const;
    void setNodeProbabilities(const GeneticNodeProbabilities& probabilities);
    void getNodeProbabilities(GeneticNodeProbabilities &probabilities) const;
    void setStopCriterium(const StopCriteriumParameters& param);
    void getStopCriterium(StopCriteriumParameters & param) const;

private:
    Ui::Settings *ui_;
    Setting setting_;

private slots:
    void okButton();
signals:
    void getSettings(const Setting&);
};

#endif // SETTINGS_H
