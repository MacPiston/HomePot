#include "chartbuilder.h"

ChartBuilder::ChartBuilder()
{

}

QChart *ChartBuilder::buildChart(QString forKey, bool hideLegend)
{
    QPieSeries *newSeries = new QPieSeries();
    newSeries->append("Jane", 1);
    newSeries->append("Joe", 2);
    newSeries->append("Andy", 3);
    newSeries->append("Barbara", 4);
    newSeries->append("Axel", 5);

    QChart *chart = new QChart();
    chart->addSeries(newSeries);
    chart->legend()->setEnabled(!hideLegend);
    return chart;
}
