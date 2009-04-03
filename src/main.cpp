/*
 *      main.cpp
 *      
 *      Copyright 2009 JMM <jmimu@free.fr>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include <QApplication>

#include "MainWindow.h"
#include "config_zoomer.h"

#define ZOOMER_VERSION 0.14

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
