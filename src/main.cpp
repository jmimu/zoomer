#include <QApplication>

#include "MainWindow.h"
#include "config_zoomer.h"

#define ZOOMER_VERSION 0.11

int main( int argc, char **argv )
{
	cout<<"zoomer, version "<<ZOOMER_VERSION<<"\nCopyright (C) 2009 IGN/TS.\n"
				<<"This is free software; see the source code for copying conditions.\n"
				<<"There is ABSOLUTELY NO WARRANTY; not even for MERCHANTIBILITY or\n"
				<<"FITNESS FOR A PARTICULAR PURPOSE.\n";

	QApplication a( argc, argv );
	
	MainWindow *mw = new MainWindow();

	ConfigZoomer cfg;

	mw->show();
	
	mw->load_config("zoomer.xml");
	
	
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	
	return a.exec();
}
