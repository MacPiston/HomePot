#include "chartbuilder.h"

ChartBuilder::ChartBuilder()
{

}

QChart *ChartBuilder::buildEIChart(valueManager vMgr, dbManager db)
{
    QPieSeries *newSeries = new QPieSeries();
    newSeries->append("Income", vMgr.getTotalIncome(db));
    newSeries->append("Expense", vMgr.getTotalExpense(db));
    newSeries->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(newSeries);
    chart->legend()->hide();
    chart->setTitle("Summary");
    chart->setTheme(QChart::ChartThemeDark);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setAnimationDuration(450);
    chart->setBackgroundRoundness(9);
    return chart;
}
