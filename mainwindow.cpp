#include "mainwindow.h"
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    const int WIDTH = 800;
    const int HEIGHT = 400;

    this->setMinimumSize(WIDTH,HEIGHT);

    curveCount = -1;
    displayCurve = 0;

    fileMenu = menuBar()->addMenu(tr("&File"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    openAction = new QAction(tr("&Open"), this);
    quitAction = new QAction(tr("&Quit"), this);
    aboutAction = new QAction(tr("&About"), this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(quitAction);
    helpMenu->addAction(aboutAction);

    sbStatus = statusBar();

    aboutDialog = new QMessageBox();//(0, "About this Program", 'This program was written by Patrick Barrett.');
    aboutDialog->setButtonText(1, "Dismiss");
    aboutDialog->setIcon(QMessageBox::Information);
    aboutDialog->setTextFormat(Qt::RichText);
    aboutDialog->setText("<h2>About This Program</h2>");
    aboutDialog->setInformativeText("This program was written by Patrick Barrett");

    plot = new QwtPlot(QwtText("Please Select One or More Files..."), parent);
    plot->setGeometry(0,0,640,400);
    //plot->setAxisScale(QwtPlot::xBottom, 0.0,160.0);
    //plot->setAxisScale(QwtPlot::yLeft,0.0,160.0);
    plot->setCanvasBackground(Qt::white);

    failLine = new QwtPlotMarker();
    failLine->setLineStyle(QwtPlotMarker::HLine);
    failLine->setAxes(plot->xBottom, plot->yLeft);
    failLine->setYValue(3.5);
    failLine->setLinePen(QPen(Qt::red, 1));
    failLine->attach(plot);

    minLine = new QwtPlotMarker();
    minLine->setLineStyle(QwtPlotMarker::HLine);
    minLine->setAxes(plot->xBottom, plot->yLeft);
    minLine->setLinePen(QPen(Qt::blue, 1));

    plotPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                   QwtPlotPicker::VLineRubberBand, QwtPicker::ActiveOnly,
                                   plot->canvas());

    plotPicker->setStateMachine(new QwtPickerClickRectMachine);

    topWidget = new QWidget();

    setCentralWidget(topWidget);

    topBox = new QHBoxLayout(topWidget);
    mainBox = new QVBoxLayout();
    topButtonBox = new QHBoxLayout();
    middleButtonBox = new QHBoxLayout();
    quitButtonBox = new QHBoxLayout();



    pbSelect = new QPushButton(tr("Select File(s)"), this);
    pbExport = new QPushButton(tr("Export Data"), this);
    pbQuit = new QPushButton(tr("Quit"), this);
    pbNext = new QPushButton(tr("->"), this);
    pbPrev = new QPushButton(tr("<-"), this);

    pbExport->setDisabled(true);

    tvOutput = new QTableView();
    tvOutput->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sbOutput = tvOutput->verticalScrollBar();
    sbOutput->setSingleStep(4);

    imData = new QStandardItemModel(0,2);

    tvOutput->setModel(imData);
    imData->setHorizontalHeaderLabels(
             QStringList() << tr("Extension (mm)")
                           << tr("Min Force (N/cm)"));

    pbQuit->setMinimumWidth(150);

    topButtonBox->addWidget(pbSelect);
    topButtonBox->addWidget(pbExport);

    middleButtonBox->addStretch();
    middleButtonBox->addWidget(pbPrev);
    middleButtonBox->addWidget(pbNext);

    quitButtonBox->addStretch(1);
    quitButtonBox->addWidget(pbQuit);

    mainBox->addLayout(topButtonBox);
    mainBox->addLayout(middleButtonBox);
    mainBox->addWidget(plot);
    mainBox->addLayout(quitButtonBox);
    //mainBox->addWidget(sbStatus);

    topBox->addLayout(mainBox);
    topBox->addWidget(tvOutput);

    connect(pbSelect, SIGNAL(clicked()), this, SLOT(selectFiles()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(selectFiles()));
    connect(pbQuit, SIGNAL(clicked()), this, SLOT(close()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(aboutAction, SIGNAL(triggered()), aboutDialog, SLOT(open()));
    //Causes program to not open:
    //connect(fileD, SIGNAL(filesSelected(QStringList)), this, SLOT(processFiles(QStringList)));
    connect(pbPrev, SIGNAL(clicked()), this, SLOT(prevPlotAction()));
    connect(pbNext, SIGNAL(clicked()), this, SLOT(nextPlotAction()));
    connect(plotPicker, SIGNAL(selected(QRectF)), this, SLOT(plotPointSelectedAction(QRectF)));
    //connect(plotPicker, SIGNAL(activated(bool)), this, SLOT(plotPickerActiveAction(bool)));
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
        //TODO: Delete Existing Plots/Data
        this->processFiles(fileList);
    }else
    {
        //Status Message
        sbStatus->showMessage("Error: You must choose one or more files.", 6000);
    }

}

void MainWindow::processFiles(QStringList files)
{
    //Open File And Do Stuff to It.

    //List each file to qDebug
    QStringListIterator itFiles(files);
    while (itFiles.hasNext())
    {
        curveCount++;

        QFile file(itFiles.next().toLocal8Bit().constData());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Cannot Read File: " << file.fileName();
            continue;
        }

        //qDebug() << "File Opened:" << file.fileName();

        CSV *csv = new CSV(&file);

        while(csv->parseLine().at(0) != "(mm)" && !csv->atEnd()) //Get to start of data
        {
            //qDebug() << csv->currentLine();
        }

        if(csv->atEnd())
        {
            qDebug() << "Nothing to read. Make sure file has '(mm)' just before the start of the data";
        }

        QStringList row; //temp row variable
        double firstPoint = 0.0;

        while(!csv->atEnd())
        {
            row = csv->parseLine();

            if(firstPoint == 0.0){
                firstPoint = row.at(0).toDouble();
            }

            if(!row.isEmpty())
            {
                //mpData.insert(row.at(0), row.at(1).toDouble());
                data.append(QPointF(row.at(0).toDouble() - firstPoint, row.at(1).toDouble()));
                //qDebug() << QPointF(row.at(0).toDouble(), row.at(1).toDouble());
            }
        }

        csv->~CSV();

        //QVectorIterator<QPointF> viData(data);

        //while(viData.hasNext()){
        //    qDebug() << viData.next();
        //}

        //qDebug() << "min Value:" << mpData.value(this->minValue(mpData)) << "At:" << this->minValue(mpData);

        //qDebug() << "Ping";

        plotCurves.append(new QwtPlotCurve("Title"));
        //qDebug() << "Ping" << curveCount;

        //plotCurves[curveCount]->setSymbol(QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::darkMagenta), QSize(5, 5) ) );
        plotCurves[curveCount]->setStyle(QwtPlotCurve::Lines);
        plotCurves[curveCount]->setBrush(Qt::NoBrush);
        plotCurves[curveCount]->setPen(QPen(Qt::darkGray));
        //plotCurves[curveCount]->setSize(QSize(5,5));
        //plotCurves[curveCount]->setStyle(QwtPlotCurve::NoCurve);

        plotCurves[curveCount]->setSamples(data);

        data.clear();

        //find mins:
        QPointF minPoint = minValue(plotCurves[curveCount], qreal(5), qreal(1E15));
        force.append(minPoint.y());
        extension.append(minPoint.y());

        QList<QStandardItem*> items;
        items.append(new QStandardItem(QString::number(minPoint.x())));
        items.append(new QStandardItem(QString::number(minPoint.y())));

        imData->appendRow(items);

        output( "Sample " + QString::number(curveCount+1) + ":\tMinimum Force (N/Sample Width): "+QString::number(minPoint.y())+"\tAt point: "+QString::number(minPoint.x()));

        if(plot->itemList().size() < 2){//nothing but the fail line attached

            qDebug() << "Nothing attached, attaching: " << curveCount;

            plot->setTitle(QwtText(QString(curveCount+48+1)));
            plotCurves[curveCount]->attach(plot);
            failLine->attach(plot);
            minLine->setYValue(minPoint.y());
            minLine->attach(plot);
            plot->replot();

            displayCurve = curveCount;

        }

    }

}

QPointF MainWindow::minValue(QwtPlotCurve* curve, qreal min = 0, qreal max = 1E15){ //QReal could cause issues on ARM
    QPointF minimumPoint = QPointF(0,1E15);

    for(unsigned int i = 0; i <= curve->data()->size(); i++){
        if(curve->data()->sample(i).x() > min && curve->data()->sample(i).x() < max){//if in range
            if(curve->data()->sample(i).y() < minimumPoint.y()){//and if is less than prev min
                minimumPoint = curve->data()->sample(i);//set as new min
            }//else skip
        }//else skip
    }

    if(minimumPoint == QPointF(0,1E15)){//if there is no change
        minimumPoint = QPointF(0,0);//change to zero to represent failure
    }

    return minimumPoint;
}

void MainWindow::nextPlotAction(){
    //show next plot
    if(displayCurve >= curveCount){
        displayCurve = curveCount;
    }else{
        displayCurve++;
    }

    if(displayCurve <= 0){
        pbPrev->setDisabled(true);
    }else{
        pbPrev->setEnabled(true);
    }
    if(displayCurve >= curveCount){
        pbNext->setDisabled(true);
    }else{
        pbNext->setEnabled(true);
    }

    plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
    plotCurves[displayCurve]->attach(plot);
    plot->setTitle(QwtText(QString::number(displayCurve+1)));
    failLine->attach(plot);
    minLine->setYValue(force[displayCurve]);
    minLine->attach(plot);
    plot->replot();
}

void MainWindow::prevPlotAction(){
    //show next plot
    if(displayCurve <= 0){
        displayCurve = 0;
    }else{
        displayCurve--;
    }

    if(displayCurve <= 0){
        pbPrev->setDisabled(true);
    }else{
        pbPrev->setEnabled(true);
    }
    if(displayCurve >= curveCount){
        pbNext->setDisabled(true);
    }else{
        pbNext->setEnabled(true);
    }

    plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
    plotCurves[displayCurve]->attach(plot);
    plot->setTitle(QwtText(QString::number(displayCurve+1)));
    failLine->attach(plot);
    minLine->setYValue(force[displayCurve]);
    minLine->attach(plot);
    plot->replot();
}

void MainWindow::plotPointSelectedAction(QRectF limits)
{
    qDebug() << limits;
    if(displayCurve <= curveCount)
    {
        QPointF minPoint = minValue(plotCurves[displayCurve], limits.left(), limits.right());
        if(minPoint == QPointF(0,0)){
            output(tr("An error has occured, please try again."));
        }else{
            output( "Sample " + QString::number(curveCount+1) + ":\tMinimum Force (N/Sample Width): "+QString::number(minPoint.y())+"\tAt point: "+QString::number(minPoint.x()));
            force[displayCurve] = minPoint.y();
            extension[displayCurve] = minPoint.x();
            minLine->setYValue(force[displayCurve]);
            plot->replot();
        }

        imData->setItem(displayCurve,0, new QStandardItem(QString::number(minPoint.x())));
        imData->setItem(displayCurve,1, new QStandardItem(QString::number(minPoint.y())));
    }
}

void MainWindow::plotPickerActiveAction(bool on)
{
    qDebug() << on << plotPicker->isActive() << plotPicker->isEnabled();
    qDebug() << plotPicker->selection();;
}

void MainWindow::output( QString line ) { //append and scroll to end if already at end
    bool atEnd;

    if(sbOutput->value() == sbOutput->maximum()){
        atEnd = true;
    }
    //teOutput->append( line );
    if(atEnd){
        sbOutput->setValue(sbOutput->maximum());
    }
}

