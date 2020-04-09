#ifndef CHARTBUILDER_H
#define CHARTBUILDER_H
#include <QtCharts>
#include "valuemanager.h"
#include "dbmanager.h"

class ChartBuilder
{
public:
    ChartBuilder();
    QChart *buildEIChart(bool hideLegend, valueManager vMgr, dbManager db);
};

#endif // CHARTBUILDER_H
