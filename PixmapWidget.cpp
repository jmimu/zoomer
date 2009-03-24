/*
 *       PixmapWidget.cpp
 *      
 *      2009 JMM <jmimu@free.fr>,
 *      Taken from Johan Thelin (e8johan@gmail.com)
 *      on the website : http://www.digitalfanatics.org/qt4/articles/zoomer.html
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


#include <QPixmap>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QImage>
#include "math.h"
#include <iostream>

#include "PixmapWidget.h"
#include "qimage_manip.h"

PixmapWidget::PixmapWidget( const QString &_filename, QWidget *parent ) : QWidget( parent ),originale(NULL),modified(NULL)
{
	contrast=100;
	brightness=0;
	gamma=100;

	sa=(QScrollArea *)parent;
	filename=_filename;
	m_pm = new QPixmap( _filename );
	load_img(filename);
	zoomFactor = 1;
	select_x=0.0;
	select_y=0.0;
	//setMinimumSize( m_pm->width()*zoomFactor, m_pm->height()*zoomFactor );
	
}


PixmapWidget::~PixmapWidget()
{
	delete m_pm;
	delete modified;
	delete originale;
}

void PixmapWidget::modif_img()
{
	//std::cout<<" ctr "<<contrast<<"  brit "<<brightness<<"   gamm "<<gamma<<"\n";
	
	delete(modified);
	modified=new QImage(*originale);
	
	*modified=changeGamma(*modified,gamma);
	*modified=changeContrast(*modified,contrast);
	*modified=changeBrightness(*modified,brightness);
	
	m_pm->detach();
	(*m_pm)=QPixmap::fromImage(*modified,Qt::ColorOnly);//fromImage is static !!
	repaint();
	
}

bool PixmapWidget::load_img(QString _filename)
{
	filename=_filename;
	delete originale;
	bool ret=m_pm->load(filename);
	m_pm->detach();
	originale=new QImage(m_pm->toImage());
	modif_img();
	return ret;
}

void PixmapWidget::centerView()
{
	//gestion des scrollbar pour centrer sur la selection
	//std::cout<<"hz :   curr "<<sa->horizontalScrollBar()->value()<<"   new "<<sa->horizontalScrollBar()->maximum()* select_x/m_pm->width()<<"   max "<<sa->horizontalScrollBar()->maximum()<<"   sel "<<select_x<<"   size "<<m_pm->width()<<"\n";
	//std::cout<<"vert :   curr "<<sa->verticalScrollBar()->value()<<"   new "<<sa->verticalScrollBar()->maximum()* select_y/m_pm->height()<<"   max"<<sa->verticalScrollBar()->maximum()<<"   sel "<<select_y<<"   size "<<m_pm->height()<<"\n";
	
	sa->horizontalScrollBar()->setValue( sa->horizontalScrollBar()->maximum()* select_x/m_pm->width());
	sa->verticalScrollBar()->setValue( sa->verticalScrollBar()->maximum()* select_y/m_pm->height());
	
	repaint();
}

void PixmapWidget::setZoomFactor( float f )
{
	int w, h;
	
//	if( f == zoomFactor )
//		return;

	zoomFactor = f;
	emit( zoomFactorChanged( zoomFactor ) );

	w = m_pm->width()*zoomFactor;
	h = m_pm->height()*zoomFactor;
	setMinimumSize( w, h );
	
	QWidget *p = dynamic_cast<QWidget*>( parent() );
	if( p )
	{
		resize( p->width(), p->height() );
	}
	
	centerView();
	//std::cout<<"fin setZoomFactor  maxx : "<<sa->horizontalScrollBar()->maximum()<<"\n";;
	//repaint();
}

void PixmapWidget::paintEvent( QPaintEvent *event )
{
	bool drawBorder = false;
	
	if( width() > m_pm->width()*zoomFactor )
	{
		xoffset = (width()-m_pm->width()*zoomFactor)/2;
		drawBorder = true;
	}
	else
	{
		xoffset = 0;
	}
	
	if( height() > m_pm->height()*zoomFactor )
	{
		yoffset = (height()-m_pm->height()*zoomFactor)/2;
		drawBorder = true;
	}
	else
	{
		yoffset = 0;
	}

	QPainter p( this );
	p.save();
	p.translate( xoffset, yoffset );
	p.scale( zoomFactor, zoomFactor );
	p.drawPixmap( 0, 0, *m_pm );
	p.restore();
	if( drawBorder )
	{
		p.setPen( Qt::black );
		p.drawRect( xoffset-1, yoffset-1, m_pm->width()*zoomFactor+1, m_pm->height()*zoomFactor+1 );
	}
	

	float r1=10;
	float r2=3;
	float r3=0.5;
	p.setPen(QPen(Qt::blue, 3));
	p.drawEllipse(xoffset+(select_x-r1)*zoomFactor,yoffset+(select_y-r1)*zoomFactor,r1*2*zoomFactor,r1*2*zoomFactor);
	p.setPen(QPen(Qt::green, 2));
	p.drawEllipse(xoffset+(select_x-r2)*zoomFactor,yoffset+(select_y-r2)*zoomFactor,r2*2*zoomFactor,r2*2*zoomFactor);
	p.setPen(QPen(Qt::red, 1));
	p.drawEllipse(xoffset+(select_x-r3)*zoomFactor,yoffset+(select_y-r3)*zoomFactor,r3*2*zoomFactor,r3*2*zoomFactor);
	p.setPen(Qt::black);
	p.drawLine(xoffset+(select_x-2)*zoomFactor,yoffset+select_y*zoomFactor,xoffset+(select_x+2)*zoomFactor,yoffset+select_y*zoomFactor);
	p.drawLine(xoffset+select_x*zoomFactor,yoffset+(select_y-2)*zoomFactor,xoffset+select_x*zoomFactor,yoffset+(select_y+2)*zoomFactor);

	//std::cout<<" maxx : "<<sa->horizontalScrollBar()->maximum()<<"\n";
	//centerView();
}

void PixmapWidget::wheelEvent( QWheelEvent *event )
{
	float f;

	//f = zoomFactor + 0.001*event->delta();
	if (event->delta()>0) f=zoomFactor*1.2;
	else if (event->delta()<0) f=zoomFactor/1.2;
	else f=zoomFactor;
	//std::cout<<f<<std::endl;
	if( f < 32.0/m_pm->width() )
		f = 32.0/m_pm->width();

	setZoomFactor( f );
}

void PixmapWidget::mousePressEvent( QMouseEvent *event )
{
	std::cout<<((event->x()-xoffset)/zoomFactor)<<" "<<((event->y()-yoffset)/zoomFactor)<<"    "<<std::endl;
	select_x=(event->x()-xoffset)/zoomFactor;
	select_y=(event->y()-yoffset)/zoomFactor;
	repaint();
}
