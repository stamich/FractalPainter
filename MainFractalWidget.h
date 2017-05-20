//
// Created by michal on 20.05.17.
//

#ifndef FRACTALS_MAINFRACTALWIDGET_H
#define FRACTALS_MAINFRACTALWIDGET_H

#include <qwidget.h>

class QPushButton;
class QPaintEvent;
class QLineEdit;
class QProgressBar;

class MainFractalWidget : public QWidget
{
Q_OBJECT

public:
    MainFractalWidget( QWidget *parent=0, const char *name=0);

    ~MainFractalWidget();

public slots:
    void emitWantToQuit();
    void drawFractal();

signals:
    void wantToQuit();
    void wantToZoom();
protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent( QMouseEvent *);
    void mouseReleaseEvent( QMouseEvent *);
    void mouseMoveEvent( QMouseEvent *);

    void drawOnPixmap(void);
    int linesOK(void);

    QLineEdit *line[6];
    QMouseEvent *mouse;
    QPushButton *draw;
    QPushButton *quit;
    QProgressBar *pbar;
    QPixmap *pixmap;
    double params[6];

private:
    void MoveAllWidgets(void);
    void ConnectLines(void);
    double Normal(void);

    int ow, oh;
    double sx, sy, ex, ey;

    UGenerator gener;
};

// ----------------------------------------------------------------------------


#endif //FRACTALS_MAINFRACTALWIDGET_H
