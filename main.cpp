//
// Created by michal on 19.04.17.
//

#include <iostream>
#include <QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <MainFractalWidget.h>
#include "MainFractalWidget.h"

using namespace std;

int main(int argc, char **argv)
{
    QApplication app( argc, argv );
    MainFractalWidget mainFractalWidget(NULL, "My widget");
    app.setMainWidget( &mainFractalWidget);
    app.connect( &mainFractalWidget, SIGNAL( wantToQuit() ), &app, SLOT(quit() ) );
    mainFractalWidget.show();
    return(app.exec());

    //QLabel *label = new QLabel("label");
    //QPushButton *button = new QPushButton("click me");
    //QObject::connect(button, SIGNAL(clicked()), label, SLOT(close()));

    //label->show();
    //button->show();
    //return app.exec();
}