#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QFile>
#include<QFileDialog>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QTableWidget>
#include<QString>
#include<QStringList>
#include<QStringListIterator>
#include<QDebug>
#include<QTableView>
#include<QStandardItemModel>
#include<QStatusBar>
#include<QMapIterator>
#include<QScrollBar>
#include<QMenu>
#include<QMenuBar>
#include<QAction>
#include<QMessageBox>
#include<qwt_plot.h>
#include<qwt_plot_curve.h>
#include<qwt_symbol.h>
#include<qwt_plot_item.h>
#include<qwt_plot_marker.h>
#include<qwt_symbol.h>
#include<qwt_picker.h>
#include<qwt_plot_picker.h>
#include<qwt_picker_machine.h>
#include<qwt_series_data.h>
#include<csv.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void selectFiles();
        void processFiles(QStringList);
        void nextPlotAction();
        void prevPlotAction();
        void plotPointSelectedAction(QRectF);
        void plotPickerActiveAction(bool on);
        void output(QString);

    private:
        //QMenuBar *menuBar;
        QMenu *fileMenu;
        QMenu *helpMenu;
        QAction *openAction;
        QAction *quitAction;
        QAction *aboutAction;
        QMessageBox *aboutDialog;

        QFile csvfile;
        QLabel *label;
        QFileDialog *fileD;
        QStringList fileList;

        QStatusBar *sbStatus;

        QWidget *topWidget;

        QHBoxLayout *topBox;
        QVBoxLayout *mainBox;
        QHBoxLayout *topButtonBox;
        QHBoxLayout *middleButtonBox;
        QHBoxLayout *quitButtonBox;

        QPushButton *pbSelect;
        QPushButton *pbExport;
        QPushButton *pbQuit;
        QPushButton *pbNext;
        QPushButton *pbPrev;

        QTableView *tvOutput;
        QStandardItemModel *imData;
        QScrollBar *sbOutput;
        QwtPlot *plot;
        QwtPlotMarker *failLine;
        QwtPlotMarker *minLine;
        QwtPlotPicker *plotPicker;
        QVector<QwtPlotCurve*> plotCurves;
        QVector<QPointF> data;
        QVector<qreal> extension;
        QVector<qreal> force;
        int curveCount;
        int displayCurve;

        QPointF minValue(QwtPlotCurve*, qreal, qreal);
};


#endif // MAINWINDOW_H
