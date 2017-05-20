//
// Created by michal on 20.04.17.
//

#include "MainFractalWidget.h"
#include "Generators.h"

#include <qpushbutton.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpen.h>
#include <qbrush.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <math.h>

#include <fstream>

MainFractalWidget::MainFractalWidget( QWidget *parent, const char *name)
        : QWidget(parent, name)
{
    int i;
    Kernel k(4);
    k[0]=12;	k[1]=34;	k[2]=56;	k[3]=78;


    params[0] = -2.0;		params[1] = -1.5;
    params[2] =  1.0;		params[3] =  1.5;
    params[4] = 32.0;		params[5] = 32.0;

    resize( 500, 448 );

    gener.Init(k);

    draw = new QPushButton( "Draw it!", this);
    quit = new QPushButton( "Quit", this );
    pbar = new QProgressBar( this );
    pixmap = new QPixmap( width()-128, height()-48 );


    line[0]= new QLineEdit(this);	line[0]->setMaxLength(10);	line[0]->resize(50, 30);	line[0]->move(0, 48 + 0);
    line[1]= new QLineEdit(this);	line[1]->setMaxLength(10);	line[1]->resize(50, 30);	line[1]->move(50, 48 + 0);

    line[2]= new QLineEdit(this);	line[2]->setMaxLength(10);	line[2]->resize(50, 30);	line[2]->move(0, 48 + 30);
    line[3]= new QLineEdit(this);	line[3]->setMaxLength(10);	line[3]->resize(50, 30);	line[3]->move(50, 48 + 30);

    line[4]= new QLineEdit(this);	line[4]->setMaxLength(10);	line[4]->move(0, 48 + 60);
    line[5]= new QLineEdit(this);	line[5]->setMaxLength(10);	line[5]->move(0, 48 + 90);

    for (i=0; i<6; i++)
    {
        QString s;
        s.setNum(params[i]);
        line[i]->setText( s );
    }

    pbar -> resize( width()-128, 48 );
    draw -> resize( 128, 48 );
    quit -> resize( 128, 48 );
    pixmap -> fill();



    ow=width();
    oh=height();
    MoveAllWidgets();

    connect( 	quit,	SIGNAL( clicked() ),
                this,	SLOT( emitWantToQuit() ) );

    connect( 	draw,	SIGNAL( clicked() ),
                this,	SLOT( drawFractal() ) );
}

void MainFractalWidget::emitWantToQuit()
{
    if ( QMessageBox::warning(this, "Program asks:", "Are You sure?",
                              QMessageBox::Yes | QMessageBox::Default,
                              QMessageBox::No | QMessageBox::Escape ) == QMessageBox:: Yes )
            emit wantToQuit();
}

void MainFractalWidget::drawFractal()
{
    if ( ! linesOK() )
    {
        QMessageBox::information( this, "Program tells...",
                                  "Wrong sequence of characters.\n"
                                          "Cannot convert to number." );
    }
    else
    {
        drawOnPixmap();
        repaint();
    }
}

MainFractalWidget::~MainFractalWidget()
{
    delete draw;
    delete quit;
    delete pixmap;
    delete pbar;
    for (int i=0; i<6; i++)
        delete line[i];
}

// Protected methods

void MainFractalWidget::mousePressEvent( QMouseEvent *mouse)
{
    if ( mouse->button()==QMouseEvent::LeftButton )
    {
        int mx=mouse->x()-128;
        int my=mouse->y()-48;
        int X=width()-128;
        int Y=height()-48;
        double dRe = (params[2]-params[0])/(double)(X-1);
        double dIm = (params[3]-params[1])/(double)(Y-1);

        sx=params[0]+dRe*mx;
        sy=params[1]+dIm*my;

        params[0]=sx-0.25*dRe*X;	params[1]=sy-0.25*dIm*Y;
        params[2]=sx+0.25*dRe*X;	params[3]=sy+0.25*dIm*Y;

        for (int i=0; i<4; i++)
        {
            QString s;
            s.setNum(params[i]);
            line[i]->setText( s );
        }
    }

    drawOnPixmap();
    repaint();
}

void MainFractalWidget::mouseReleaseEvent( QMouseEvent *mouse)
{
}

void MainFractalWidget::mouseMoveEvent( QMouseEvent *mouse)
{
    if ( mouse->x()==0 )
    {}
}

void MainFractalWidget::drawOnPixmap(void)
{
    QPainter p;
    pixmap -> fill();

    p.begin(pixmap);

    int X=width()-128, Y=height();
    int current=0;
    pbar->setTotalSteps( Y );

    double R = params[5]*params[5];
    double dRe = (params[2]-params[0])/(double)(X-1);
    double dIm = (params[3]-params[1])/(double)(Y-1);

    double dColor=127.0/params[4];
    ofstream outlog("outlog.txt");

    for (int y=0; y<Y; y++)
    {
        for (int x=0; x<X; x++)
        {
            double re = params[0] + dRe * (double)x;
            double im = params[1] + dIm * (double)y;
            double c_re = re, c_im = im;

            for (int i=0; i<params[4]; i++)
            {
                double r2 = re * re;
                double i2 = im * im;
                double r=r2+i2;

                if ( (r > R) && (i<params[4]) )
                {
                    QPen pen( QColor((int)(dColor*i), (int)(dColor*(i/4)), (int)(dColor*log(r)) ));
                    p.setPen(pen);
                    p.drawPoint( x, Y - y +1 );
                    // outlog << "( " << x << ", " << y << ", " << r << " ) "<< endl;
                    break;
                }
                im = 2.0 * re * im + c_im;
                re = r2 - i2 + c_re;
            }
        }

        pbar->setProgress( current++ );
    }
    p.end();
    pbar -> reset();
}

int MainFractalWidget::linesOK(void)
{
    for (int i=0; i<6; i++)
    {
        bool ok;
        QString s = line[i] -> text();
        params[i] = s.toDouble(&ok);
        if ( ! ok )
            return(0);
    }
    return(1);
}

void MainFractalWidget::paintEvent(QPaintEvent *pe)
{
    if ((width()!=ow)&&(height()!=oh))
    {
        ow=width();
        oh=height();
        MoveAllWidgets();
    }

    bitBlt(this, 128, 0, pixmap);
}

// Strict private methods

void MainFractalWidget::MoveAllWidgets(void)
{
    draw -> move( 0, 0 );
    quit -> move( 0, oh - 48 );
    pbar -> move( 128, oh - 48 );
    pbar -> resize( ow - 128, 48 );
    pixmap -> resize( ow - 128, oh - 48 );
    pixmap ->fill();
}

double MainFractalWidget::Normal(void)
{
    double result=0.0;
    result = sqrt(-2.0*log(gener.Value()))*sin(2.0*M_PI*gener.Value());
    return(result);
}