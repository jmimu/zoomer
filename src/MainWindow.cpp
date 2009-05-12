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
#include <QMessageBox>

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
	reload_img->setToolTip("Reinitialize colors");
	reload_img->setIcon(QIcon(":/pic/reload_img.png"));
	mainToolBar->addWidget(reload_img);
	QObject::connect(reload_img, SIGNAL(clicked()), this, SLOT(reloadImg()));

	contrast_more = new QPushButton;
	contrast_more->setToolTip("More contrast");
	contrast_more->setIcon(QIcon(":/pic/contrast_more.png"));
	mainToolBar->addWidget(contrast_more);
	QObject::connect(contrast_more, SIGNAL(clicked()), this, SLOT(contrastMore()));

	contrast_less = new QPushButton;
	contrast_less->setToolTip("Less contrast");
	contrast_less->setIcon(QIcon(":/pic/contrast_less.png"));
	mainToolBar->addWidget(contrast_less);
	QObject::connect(contrast_less, SIGNAL(clicked()), this, SLOT(contrastLess()));

	brightness_more = new QPushButton;
	brightness_more->setToolTip("Brighter");
	brightness_more->setIcon(QIcon(":/pic/brightness_more.png"));
	mainToolBar->addWidget(brightness_more);
	QObject::connect(brightness_more, SIGNAL(clicked()), this, SLOT(brightnessMore()));

	brightness_less = new QPushButton;
	brightness_less->setToolTip("Less bright");
	brightness_less->setIcon(QIcon(":/pic/brightness_less.png"));
	mainToolBar->addWidget(brightness_less);
	QObject::connect(brightness_less, SIGNAL(clicked()), this, SLOT(brightnessLess()));

	gamma_more = new QPushButton;
	gamma_more->setToolTip("More gamma");
	gamma_more->setIcon(QIcon(":/pic/gamma_more.png"));
	mainToolBar->addWidget(gamma_more);
	QObject::connect(gamma_more, SIGNAL(clicked()), this, SLOT(gammaMore()));

	gamma_less = new QPushButton;
	gamma_less->setToolTip("Less gamma");
	gamma_less->setIcon(QIcon(":/pic/gamma_less.png"));
	mainToolBar->addWidget(gamma_less);
	QObject::connect(gamma_less, SIGNAL(clicked()), this, SLOT(gammaLess()));

	QFrame *line1=new QFrame(mainToolBar);
	line1->setFrameShape(QFrame::VLine);
	line1->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line1);
	
	previous = new QPushButton;
	previous->setToolTip("Previous picture");
	previous->setIcon(QIcon(":/pic/previous.png"));
	mainToolBar->addWidget(previous);
	QObject::connect(previous, SIGNAL(clicked()), this, SLOT(previous_()));
	
	next = new QPushButton;
	next->setToolTip("Next picture");
	next->setIcon(QIcon(":/pic/next.png"));
	mainToolBar->addWidget(next);
	QObject::connect(next, SIGNAL(clicked()), this, SLOT(next_()));
	
	QFrame *line2=new QFrame(mainToolBar);
	line2->setFrameShape(QFrame::VLine);
	line2->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line2);
	
	validation = new QPushButton;
	validation->setToolTip("Validation");
	validation->setIcon(QIcon(":/pic/validation.png"));
	mainToolBar->addWidget(validation);
	QObject::connect(validation, SIGNAL(clicked()), this, SLOT(validation_()));


	QFrame *line3=new QFrame(mainToolBar);
	line3->setFrameShape(QFrame::VLine);
	line3->setFrameShadow(QFrame::Sunken);
	mainToolBar->addWidget(line3);


	exit = new QPushButton;
	exit->setToolTip("Exit without saving");
	exit->setIcon(QIcon(":/pic/exit.png"));
	mainToolBar->addWidget(exit);
	QObject::connect(exit, SIGNAL(clicked()), this, SLOT(cancel_()));
	
	sa = new QScrollArea( this );
	sa->setWidgetResizable( true );

	resize(800-1,480);
}

MainWindow::~MainWindow()
{
	
}

bool MainWindow::load_config(QString cfgfile)
{
	int result_read=cfg.read(cfgfile.toStdString());
	cfg.validated=false;//validated only if click on validated !
	
	if (result_read==-1)
	{
		QMessageBox::information(this, "Error!", "Configuration file not found, an example has been created: "+cfgfile);
	}else	if (result_read==1)
	{
		QMessageBox::information(this, "Error!", "Error in the configuration file. See stderr for details and check your configuration file: "+cfgfile);
		qApp->quit();
		return false;
	}

	statusBar->showMessage(cfg.message.c_str());
	pw = new PixmapWidget( QString(cfg.filenames[cfg.num_file-1].c_str()), sa );
	
	if (pw->zoomFactor<0) //means that the picture does not exist
	{
		cfg.num_file=0;
		if (!next_()) QMessageBox::information(this, "Error!", "No file found, please check your configuration file: "+cfgfile);
		qApp->quit();
		return false;
	}
	
	setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
	
	sa->setWidget( pw );
	setCentralWidget( sa );
	
	pw->select_x=cfg.point_x;
	pw->select_y=cfg.point_y;

	//have to do it to update scrollbars
	resize(800,480);
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
	cfg.validated=true;
	cfg.write("zoomer.xml");
	qApp->quit();
}

void MainWindow::cancel_()
{
	cfg.validated=false;
	cfg.write("zoomer.xml");
	qApp->quit();
}

bool MainWindow::previous_()
{
	if (cfg.num_file>1)
	{
		cfg.num_file--;
		
		pw->filename=cfg.filenames[cfg.num_file-1].c_str();
		if (!pw->load_img(pw->filename)) //file not found
		{
			if (!previous_()) {cfg.num_file++;return false;}
		}
		
		setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
		
		previous->setDisabled(cfg.num_file==1);
		next->setDisabled(cfg.num_file==cfg.filenames.size());
		return true;
	}
	previous->setDisabled(true);
	return false;
}
bool MainWindow::next_()
{
	if (cfg.num_file<cfg.filenames.size())
	{
		cfg.num_file++;
		
		pw->filename=cfg.filenames[cfg.num_file-1].c_str();
		if (!pw->load_img(pw->filename)) //file not found
		{
			if (!next_())  {cfg.num_file--;return false;}
		}
		setWindowTitle(cfg.filenames[cfg.num_file-1].c_str());
		
		previous->setDisabled(cfg.num_file==1);
		next->setDisabled(cfg.num_file==cfg.filenames.size());
		return true;
	}
	next->setDisabled(true);
	return false;
}
