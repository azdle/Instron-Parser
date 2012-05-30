#include "mainwindow.h"
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    const int WIDTH = 500;
    const int HEIGHT = 300;

    this->setMinimumSize(WIDTH,HEIGHT);

    plot.setGeometry(0,0,640,400);
    plot.setAxisScale(QwtPlot::xBottom, 0.0,160.0);
    plot.setAxisScale(QwtPlot::yLeft,0.0,160.0);
    plot.setCanvasBackground(Qt::white);

    //plotCurve.setSymbol(QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::darkMagenta), QSize(5, 5) ) );
    plotCurve.setStyle(QwtPlotCurve::Dots);
    plotCurve.setBrush(Qt::NoBrush);
    plotCurve.setPen(QPen(Qt::darkGray));
    //plotCurve.setSize(QSize(5,5));
    plotCurve.setStyle(QwtPlotCurve::NoCurve);

    //data->attach(plot);

    mainBox = new QVBoxLayout(this);
    topButtonBox = new QHBoxLayout();
    quitButtonBox = new QHBoxLayout();

    pbSelect = new QPushButton(tr("Select File"), this);
    pbExport = new QPushButton(tr("Export File"), this);
    pbQuit = new QPushButton(tr("Quit"), this);

    //teOutput = new QTextEdit(tr("Please select an input file..."), this);

    statusBar = new QStatusBar(this);

    pbQuit->setMinimumWidth(150);

    topButtonBox->addWidget(pbSelect);
    topButtonBox->addWidget(pbExport);

    quitButtonBox->addStretch(1);
    quitButtonBox->addWidget(pbQuit);

    mainBox->addLayout(topButtonBox);
    //mainBox->addWidget(teOutput);
    mainBox->addWidget(&plot);
    mainBox->addLayout(quitButtonBox);
    mainBox->addWidget(statusBar);

    connect(pbSelect, SIGNAL(clicked()), this, SLOT(selectFiles()));
    connect(pbQuit, SIGNAL(clicked()), this, SLOT(close()));
    //Causes program to not open:
    //connect(fileD, SIGNAL(filesSelected(QStringList)), this, SLOT(processFiles(QStringList)));
}
MainWindow::~MainWindow()
{
    
}

void MainWindow::selectFiles()
{
    /* //This won't use a native file dialog.
    fileD = new QFileDialog(this, Qt::Dialog);
    fileD->setFileMode(QFileDialog::ExistingFiles);
    fileD->setNameFilter(tr("Data File (*.csv *.exp *.txt)"));
    */

    fileList = QFileDialog::getOpenFileNames( this,
                                              "Select one or more files to open",
                                              "",
                                              "Data File (*.csv *.exp *.txt)");

    if (!fileList.isEmpty())
    {
        this->processFiles(fileList);
    }else
    {
        //Status Message
        statusBar->showMessage("Error: You must choose one or more files.", 6000);
    }

}

void MainWindow::processFiles(QStringList files)
{
    //Open File And Do Stuff to It.

    //List each file to qDebug
    QStringListIterator itFiles(files);
    while (itFiles.hasNext())
    {
        QFile file(itFiles.next().toLocal8Bit().constData());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Cannot Read File: " << file.Text;
            continue;
        }

        qDebug() << "File Opened";

        CSV csv(&file);

        while(csv.parseLine().at(0) != "#data" && !csv.atEnd()) //Get to start of data
        {
        }

        if(csv.atEnd())
        {
            qDebug() << "Nothing to read. Make sure file has '#data'' at the start of the data";
        }

        QStringList row; //temp row variable

        while(!csv.atEnd())
        {
            row = csv.parseLine();

            if(!row.isEmpty())
            {
                //mpData.insert(row.at(0), row.at(1).toDouble());
                data.append(QPointF(row.at(0).toDouble(), row.at(1).toDouble()));
            }
        }

        QVectorIterator<QPointF> viData(data);

        while(viData.hasNext()){
            qDebug() << viData.next();
        }

        //qDebug() << "min Value:" << mpData.value(this->minValue(mpData)) << "At:" << this->minValue(mpData);

        qDebug() << "Ping";


        plotCurve.setSamples(data);

        plotCurve.attach(&plot);

        plot.replot();

        qDebug() << "Ping";

    }

}

QString MainWindow::minValue(QMap<QString, double> map){
    QMapIterator<QString, double> values(map);
    QString minKey;
    double minValue = 1E15; //Set large enough so that the min point will be less.

    while( values.hasNext() )
    {
        if(minValue > values.next().value())
        {
            minValue = values.value();
            minKey = values.key();
        }

        //qDebug() << "minValue: " << minValue << "Value: " << values.value();
    }
    return minKey;
}
