#ifndef CHARTBUILDER_H
#define CHARTBUILDER_H
#include <QtCharts>


class ChartBuilder
{
public:
    ChartBuilder();
    QChart *buildChart(QString forKey, bool hideLegend);
};

#endif // CHARTBUILDER_H
