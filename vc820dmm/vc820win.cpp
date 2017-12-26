#include "vc820win.h"
#include "ui_vc820win.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QPainter>


QSerialPort serial;
QTimer timer;
QPixmap *pixmap;

int recvdata[14];
QString display="";

#define P(x,y) QPoint(x,y)

const QPolygon SegA = QPolygon() <<P( 55, 50)<<P(105, 50)<<P( 95, 60)<<P( 65, 60);
const QPolygon SegB = QPolygon() <<P(105, 50)<<P(100,100)<<P( 91, 95)<<P( 95, 60);
const QPolygon SegC = QPolygon() <<P(100,100)<<P( 95,150)<<P( 86,140)<<P( 90,105);
const QPolygon SegD = QPolygon() <<P( 95,150)<<P( 45,150)<<P( 55,140)<<P( 85,140);
const QPolygon SegE = QPolygon() <<P( 45,150)<<P( 50,100)<<P( 59,105)<<P( 55,140);
const QPolygon SegF = QPolygon() <<P( 55, 50)<<P( 50,100)<<P( 60, 95)<<P( 64, 60);
const QPolygon SegG = QPolygon() <<P( 60, 95)<<P( 90, 95)<<P(100,100)<<P( 90,105)<<P( 60,105)<<P( 50,100);
const QPolygon SegS = QPolygon() <<P( 10, 95)<<P( 30, 95)<<P( 30,105)<<P( 10,105);
const QPolygon SegP = QPolygon() <<P(100,150)<<P(110,150)<<P(110,140)<<P(100,140);
const QPolygon SymD = QPolygon() <<P(  0,  0)<<P(  0, -1)<<P( 10, -1)<<P( 10,-10)<<P( 18, -2)<<P( 18,-10)
                                 <<P( 22,-10)<<P( 22, -1)<<P( 30, -1)<<P( 30,  1)<<P( 22,  1)<<P( 22, 10)
                                 <<P( 18, 10)<<P( 18,  2)<<P( 10, 10)<<P( 10,  1)<<P(  0,  1);
const QPolygon SymC = QPolygon() <<P(  5, -3)<<P(  0, -3)<<P(  0,  3)<<P(  5,  3)<<P(  5, -5)<<P( 10, -8)
                                 <<P( 12,  0)<<P( 10,  8)<<P( 12,  0)<<P( 10, -8)<<P( 15,-11)<<P( 18,  0)
                                 <<P( 15, 11)<<P( 18,  0)<<P( 15,-11)<<P( 20,-14)<<P( 23,  0)<<P( 20, 14)
                                 <<P(  5,  5);
const QPolygon SymB = QPolygon() <<P(  0, 10)<<P(  0, -8)<<P(  4, -8)<<P(  4,-10)<<P( 11,-10)<<P( 11, -8)
                                 <<P( 17, -8)<<P( 17,-10)<<P( 26,-10)<<P( 26, -8)<<P( 30, -8)<<P( 30, 10)
                                 <<P(  0, 10);


typedef struct t_element {
    QPoint position;
    QVariant shape;
} t_element;


const QList<t_element> segs = {
    {P( 80, 30),QVariant("RS232")},
    {P( 30, 30),QVariant("AUTO")},
    {P( 10, 60),QVariant("DC")},
    {P( 10, 80),QVariant("AC")},
    {P(  0,  0),QVariant(SegA)},
    {P(  0,  0),QVariant(SegF)},
    {P(  0,  0),QVariant(SegE)},
    {P(  0,  0),QVariant(SegS)},
    {P(  0,  0),QVariant(SegB)},
    {P(  0,  0),QVariant(SegG)},
    {P(  0,  0),QVariant(SegC)},
    {P(  0,  0),QVariant(SegD)},
    {P( 70,  0),QVariant(SegA)},
    {P( 70,  0),QVariant(SegF)},
    {P( 70,  0),QVariant(SegE)},
    {P(  0,  0),QVariant(SegP)},
    {P( 70,  0),QVariant(SegB)},
    {P( 70,  0),QVariant(SegG)},
    {P( 70,  0),QVariant(SegC)},
    {P( 70,  0),QVariant(SegD)},
    {P(140,  0),QVariant(SegA)},
    {P(140,  0),QVariant(SegF)},
    {P(140,  0),QVariant(SegE)},
    {P( 70,  0),QVariant(SegP)},
    {P(140,  0),QVariant(SegB)},
    {P(140,  0),QVariant(SegG)},
    {P(140,  0),QVariant(SegC)},
    {P(140,  0),QVariant(SegD)},
    {P(210,  0),QVariant(SegA)},
    {P(210,  0),QVariant(SegF)},
    {P(210,  0),QVariant(SegE)},
    {P(140,  0),QVariant(SegP)},
    {P(210,  0),QVariant(SegB)},
    {P(210,  0),QVariant(SegG)},
    {P(210,  0),QVariant(SegC)},
    {P(210,  0),QVariant(SegD)},
    {P(140, 23),QVariant(SymD)},
    {P(330, 70),QVariant("k")},
    {P(330,130),QVariant("n")},
    {P(340,130),QVariant(QString(QChar(0xBC,0x03)))},
    {P(190, 23),QVariant(SymC)},
    {P(335, 70),QVariant("M")},
    {P(330,100),QVariant("%")},
    {P(345,130),QVariant("m")},
    {P(230, 30),QVariant("HOLD")},
    {P(280, 30),QVariant("REL")},
    {P(350, 70),QVariant(QString(QChar(0xA9,0x03)))},
    {P(360,130),QVariant("F")},
    {P(335, 23),QVariant(SymB)},
    {P(340,100),QVariant("Hz")},
    {P(370,130),QVariant("V")},
    {P(380,130),QVariant("A")},
    {P(330,160),QVariant(" ")},
    {P(340,160),QVariant(" ")},
    {P(350,160),QVariant(" ")},
    {P(360,160),QVariant(" ")},
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addPermanentWidget(ui->label);
    ui->statusBar->addPermanentWidget(ui->label_2);

    ui->comboBox->addItem("Example port","ttyUSB9");

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString portname = info.portName();
        QString portdesc = info.description();

        ui->comboBox->addItem(portname + " | " + portdesc,portname);
    }

    connect(&serial,SIGNAL(readyRead()),this,SLOT(readevent()));


    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(cleardisp()));
    timer->start(1000);

    ui->image->adjustSize();

    pixmap = new QPixmap(ui->image->frameRect().width(),ui->image->frameRect().height());
    QPainter p(pixmap);
    p.fillRect(p.viewport(),QColor(0xC0,0xC0,0xC0));
    p.end();

    ui->image->setPixmap(*pixmap);

    ui->image->repaint();

}


MainWindow::~MainWindow()
{
    delete ui;
}

QChar MainWindow::decodedigit(int pattern) {
    //   PEFA DCGB
    // 0 -111 1101 7D
    // 1 -000 0101 05
    // 2 -101 1011 5B
    // 3 -001 1111 1F
    // 4 -010 0111 27
    // 5 -011 1110 3E
    // 6 -111 1110 7E
    // 7 -001 0101 15
    // 8 -111 1111 7F
    // 8 -011 1111 3F
    // L -110 1000 68
    // H -110 0111 67
    QString pat = "\x7d\x05\x5b\x1f\x27\x3e\x7e\x15\x7f\x3f\x68\x67";
    QString sym = "0123456789LH";
    for (int i=0; pat[i] != '\0'; i++) {
        if (pat[i] == (pattern & 0x7F)) {
            return sym[i];
        }
    }
    return ' ';
}

void MainWindow::decodedata() {
    char c;

    display = "";
    c = (recvdata[1] << 4) + recvdata[2];
    if ((c & 0x80) != 0) display += "-";
    display += decodedigit(c);

    c = (recvdata[3] << 4) + recvdata[4];
    if ((c & 0x80) != 0) display += ".";
    display += decodedigit(c);

    c = (recvdata[5] << 4) + recvdata[6];
    if ((c & 0x80) != 0) display += ".";
    display += decodedigit(c);

    c = (recvdata[7] << 4) + recvdata[8];
    if ((c & 0x80) != 0) display += ".";
    display += decodedigit(c);

    if ((recvdata[9] & 0b1000) != 0) display += "u";
    if ((recvdata[9] & 0b0100) != 0) display += "n";
    if ((recvdata[9] & 0b0010) != 0) display += "k";
    if ((recvdata[10] & 0b1000) != 0) display += "m";
    if ((recvdata[10] & 0b0010) != 0) display += "M";

    if ((recvdata[10] & 0b0100) != 0) display += "%";
    if ((recvdata[11] & 0b1000) != 0) display += "F";
    if ((recvdata[11] & 0b0100) != 0) display += "Ohm";
    if ((recvdata[12] & 0b1000) != 0) display += "A";
    if ((recvdata[12] & 0b0100) != 0) display += "V";
    if ((recvdata[12] & 0b0010) != 0) display += "Hz";

    ui->textEdit->setText(display);
}


void MainWindow::readevent() {
    char c;
    int nu,nl;

    while (serial.read(&c,1) > 0) {
//        ui->textEdit->insertPlainText( QString("%1 ").arg((unsigned char)(c),2,16,QLatin1Char('*')) );
        nu = (c & 0xF0) >> 4;
        nl = (c & 0x0F);
        if ((nu > 0) && (nu < 15)) {
            recvdata[nu-1] = nl;
            if (nu == 14) {
                updatedisp();
                decodedata();
            }
        }
    }

}


void MainWindow::cleardisp()
{
    QPainter p(pixmap);
    QBrush brush;

    brush.setColor(QColor(0xC0,0xE0,0xC0));
    brush.setStyle(Qt::SolidPattern);

    p.setBrush(brush);
    p.fillRect(p.viewport(),brush);

    p.end();

    ui->image->setPixmap(*pixmap);
}


void MainWindow::updatedisp()
{
    char c;

    cleardisp();

    QPainter p(pixmap);
    QPen pen;
    QBrush brush;
    QFont font("Helvetica");

    pen.setWidth(1);

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);

    font.setBold(true);
    font.setStretch(60);
    font.setPixelSize(20);

    p.setBrush(brush);
    p.setFont(font);

    for (int i=0; i<segs.length(); i++){
        t_element element = segs[i];
        c = recvdata[i / 4];

        if ((c & (1 << (i % 4))) != 0) {

            if (element.shape.type() == QVariant::Polygon) {
                pen.setColor(QColor(0xC0,0xCE,0xC0));
                p.setPen(pen);
                p.drawPolygon(element.shape.value<QPolygon>().translated(element.position));
            }
            if (element.shape.type() == QVariant::String) {
                pen.setColor(Qt::black);
                p.setPen(pen);
                p.drawText(element.position,element.shape.value<QString>());
            }
        }
    }

    p.end();

    ui->image->setPixmap(*pixmap);

    timer->start(5000);
}

void MainWindow::on_pushButton_clicked()
{
    for (int i=0;i<14;i++) {
        recvdata[i]=0xF;
        updatedisp();
    }
}


void MainWindow::on_toolButton_clicked()
{
    ui->textEdit->setText(ui->comboBox->currentData().toString());

    if (serial.isOpen()) {
        serial.close();
    }

    serial.setPortName(ui->comboBox->currentData().toString());
    serial.setBaudRate(QSerialPort::Baud2400);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    ui->label->setText(serial.portName());
    if (serial.open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage("Ready.");
    }
    else
    {
        ui->statusBar->showMessage("Open failed.");
    }

}

void MainWindow::on_toolButton_2_clicked()
{
            serial.setRequestToSend(false);
}

void MainWindow::on_toolButton_3_clicked()
{
            serial.setRequestToSend(true);
}
