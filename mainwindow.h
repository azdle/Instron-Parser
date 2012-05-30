#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QWidget>
#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QFile>
#include<QFileDialog>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QTextEdit>
#include<QString>
#include<QStringList>
#include<QStringListIterator>
#include<QDebug>
#include<QTableView>
#include<QStandardItemModel>
#include<QStatusBar>
#include<QMapIterator>
#include<qwt_plot.h>
#include<qwt_plot_curve.h>
#include<qwt_symbol.h>
#include<csv.h>

class MainWindow : public QWidget
{
    Q_OBJECT
    
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void selectFiles();
        void processFiles(QStringList);

    private:
        QFile csvfile;
        QLabel *label;
        QFileDialog *fileD;
        QStringList fileList;
        //QMap<QString, double> mpData;
        QVector<double> extension;
        QVector<double> force;

        QStatusBar *statusBar;

        QVBoxLayout *mainBox;
        QHBoxLayout *topButtonBox;
        QHBoxLayout *quitButtonBox;

        QPushButton *pbSelect;
        QPushButton *pbExport;
        QPushButton *pbQuit;

        QTextEdit *teOutput;
        QwtPlot plot;
        QwtPlotCurve plotCurve;
        QVector<QPointF> data;


        QString minValue(QMap<QString, double>);
};


#endif // MAINWINDOW_H
