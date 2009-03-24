/*
 *      MainWindow.cpp
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

#include "MainWindow.h"

#include <QtGui/QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),pw(NULL), cfg()
{
	
	mainToolBar = new QToolBar(this);
  mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
  addToolBar(Qt::TopToolBarArea, mainToolBar);
  statusBar = new QStatusBar(this);
  statusBar->setObjectName(QString::fromUtf8("statusBar"));
  setStatusBar(statusBar);
	
	statusBar->setFont(QFont("Arial", 18));
  statusBar->showMessage("Bonjour !");
  
	reload_img = new QPushButton;
	reload_img->setIcon(QIcon("reload_img.png"));
	mainToolBar->addWidget(reload_img);
	QObject::connect(reload_img, SIGNAL(clicked()), this, SLOT(reloadImg()));

	contrast_more = new QPushButton;
	contrast_more->setIcon(QIcon("contrast_more.png"));
	mainToolBar->addWidget(contrast_more);
	QObject::connect(contrast_more, SIGNAL(clicked()), this, SLOT(contrastMore()));

	contrast_less = new QPushButton;
	contrast_less->setIcon(QIcon("contrast_less.png"));
	mainToolBar->addWidget(contrast_less);
	QObject::connect(contrast_less, SIGNAL(clicked()), this, SLOT(contrastLess()));

	brightness_more = new QPushButton;
	brightness_more->setIcon(QIcon("brightness_more.png"));
	mainToolBar->addWidget(brightness_more);
	QObject::connect(brightness_more, SIGNAL(clicked()), this, SLOT(brightnessMore()));

	brightness_less = new QPushButton;
	brightness_less->setIcon(QIcon("brightness_less.png"));
	mainToolBar->addWidget(brightness_less);
	QObject::connect(brightness_less, SIGNAL(clicked()), this, SLOT(brightnessLess()));

	gamma_more = new QPushButton;
	gamma_more->setIcon(QIcon("gamma_more.png"));
	mainToolBar->addWidget(gamma_more);
	QObject::connect(gamma_more, SIGNAL(clicked()), this, SLOT(gammaMore()));

	gamma_less = new QPushButton;
	gamma_less->setIcon(QIcon("gamma_less.png"));
	mainToolBar->addWidget(gamma_less);
	QObject::connect(gamma_less, SIGNAL(clicked()), this, SLOT(gammaLess()));

	QFrame *line1=new QFrame(mainToolBar);
	line1->setFrameShape(QFrame::VLine);
	line1->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line1);
	
	previous = new QPushButton;
	previous->setIcon(QIcon("previous.png"));
	mainToolBar->addWidget(previous);
	QObject::connect(previous, SIGNAL(clicked()), this, SLOT(previous_()));
	
	next = new QPushButton;
	next->setIcon(QIcon("next.png"));
	mainToolBar->addWidget(next);
	QObject::connect(next, SIGNAL(clicked()), this, SLOT(next_()));
	
	QFrame *line2=new QFrame(mainToolBar);
	line2->setFrameShape(QFrame::VLine);
	line2->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line2);
	
	validation = new QPushButton;
	validation->setIcon(QIcon("validation.png"));
	mainToolBar->addWidget(validation);
	QObject::connect(validation, SIGNAL(clicked()), this, SLOT(validation_()));


	QFrame *line3=new QFrame(mainToolBar);
	line3->setFrameShape(QFrame::VLine);
	line3->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line3);


	exit = new QPushButton;
	exit->setIcon(QIcon("exit.png"));
	mainToolBar->addWidget(exit);
	QObject::connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));
	
	sa = new QScrollArea( this );
	sa->setWidgetResizable( true );

}

MainWindow::~MainWindow()
{
	
}

bool MainWindow::load_config(std::string cfgfile)
{
	cfg.read(cfgfile);
	statusBar->showMessage(cfg.message.c_str());
	pw = new PixmapWidget( QString(cfg.filenames[cfg.num_file-1].c_str()), sa );
	setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
	
	sa->setWidget( pw );
	setCentralWidget( sa );
	
	pw->select_x=cfg.point_x;
	pw->select_y=cfg.point_y;

	//have to do it to update scrollbars
	resize(640,480);
	pw->setZoomFactor(cfg.zoom_factor);
	
	previous->setDisabled(cfg.num_file==1);
	next->setDisabled(cfg.num_file==cfg.filenames.size());
	
	pw->contrast=cfg.contrast;
	pw->brightness=cfg.brightness;
	pw->gamma=cfg.gamma;
	pw->modif_img();
	
	return true;
}

void MainWindow::resizeEvent( QResizeEvent* )
{
	if (pw!=NULL)	pw->setZoomFactor(pw->zoomFactor);
}

void MainWindow::reloadImg()
{
	pw->contrast=100;
	pw->brightness=0;
	pw->gamma=100;
	pw->modif_img();
}
void MainWindow::contrastMore()
{
	pw->contrast+=10;
	pw->modif_img();
}
void MainWindow::contrastLess()
{
	pw->contrast-=10;
	pw->modif_img();
}
void MainWindow::brightnessMore()
{
	pw->brightness+=10;
	pw->modif_img();
}
void MainWindow::brightnessLess()
{
	pw->brightness-=10;
	pw->modif_img();
}
void MainWindow::gammaMore()
{
	pw->gamma+=10;
	pw->modif_img();
}
void MainWindow::gammaLess()
{
	pw->gamma-=10;
	pw->modif_img();
}
void MainWindow::validation_()
{
	cfg.point_x=pw->select_x;
	cfg.point_y=pw->select_y;
	cfg.zoom_factor=pw->zoomFactor;
	cfg.contrast=pw->contrast;
	cfg.brightness=pw->brightness;
	cfg.gamma=pw->gamma;
	cfg.write("zoomer.cfg");
	qApp->quit();
}
void MainWindow::previous_()
{
	if (cfg.num_file>1)
	{
		cfg.num_file--;
		
		pw->filename=cfg.filenames[cfg.num_file-1].c_str();
		pw->load_img(pw->filename);
		setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
		
		previous->setDisabled(cfg.num_file==1);
		next->setDisabled(cfg.num_file==cfg.filenames.size());
	}
}
void MainWindow::next_()
{
	if (cfg.num_file<cfg.filenames.size())
	{
		cfg.num_file++;
		
		pw->filename=cfg.filenames[cfg.num_file-1].c_str();
		pw->load_img(pw->filename);
		setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
		
		previous->setDisabled(cfg.num_file==1);
		next->setDisabled(cfg.num_file==cfg.filenames.size());
	}
}
