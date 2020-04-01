#include "chartbuilder.h"

ChartBuilder::ChartBuilder()
{

}

QChart *ChartBuilder::buildChart(QString forKey, bool hideLegend, valueManager vMgr, dbManager db)
{
    QPieSeries *newSeries = new QPieSeries();
    newSeries->append("Income", vMgr.generateTotalIncome(db));
    newSeries->append("Expense", vMgr.generateTotalExpense(db));

    QChart *chart = new QChart();
    chart->addSeries(newSeries);
    chart->legend()->setEnabled(!hideLegend);
    return chart;
}
