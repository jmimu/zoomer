#include <QApplication>

#include "MainWindow.h"
#include "config_zoomer.h"

int main( int argc, char **argv )
{
	QApplication a( argc, argv );
	
	MainWindow *mw = new MainWindow();

	ConfigZoomer cfg;

	mw->show();
	
	mw->load_config("zoomer.xml");
	
	
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	
	return a.exec();
}
