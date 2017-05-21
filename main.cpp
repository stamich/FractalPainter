//
// Created by michal on 19.04.17.
//

#include <iostream>
#include <QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMainWindow>
//#include <MainFractalWidget.h>
#include "MainFractalWidget.h"

using namespace std;

int main(int argc, char **argv)
{
    QApplication *app = new QApplication( argc, argv );
    MainFractalWidget mainFractalWidget(NULL, "My widget");
    QMainWindow x;
    x.setCentralWidget( &mainFractalWidget);
    x.show();
    x.connect( &mainFractalWidget, SIGNAL( wantToQuit() ), app, SLOT(quit() ) );
    mainFractalWidget.show();
    return app->exec();

    //QLabel *label = new QLabel("label");
    //QPushButton *button = new QPushButton("click me");
    //QObject::connect(button, SIGNAL(clicked()), label, SLOT(close()));

    //label->show();
    //button->show();
    //return app.exec();
}
//int main(int argc,char *argv[])
//{
//    QApplication *my=new QApplication(argc,argv);
//    QMainWindow x;
//    QString ss="3D'E";
//    QLabel *ee=new QLabel(ss,0);
//    x.setCentralWidget(ee);
//    x.show();
//    return my->exec();
//}