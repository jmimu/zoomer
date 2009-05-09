/*
 *       PixmapWidget.h
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

#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>
#include <QString>

class QPixmap;
class QScrollArea;

class PixmapWidget : public QWidget
{
	friend class MainWindow;
	
	Q_OBJECT
	
public:
	PixmapWidget( const QString &_filename, QWidget *parent=0 );
	~PixmapWidget();
	QPixmap *m_pm;
	bool load_img(QString _filename);
	void centerView();

public slots:
	void setZoomFactor( float );
	
signals:
	void zoomFactorChanged( float );

protected:
	void paintEvent( QPaintEvent* );
	void wheelEvent( QWheelEvent* );
	void mousePressEvent( QMouseEvent* event);
	//void mouseMoveEvent( QMouseEvent * event);


private:
	QScrollArea *sa;

	float zoomFactor;
	float xoffset;
	float yoffset;
	
	float select_x,select_y;
	QString filename;
	
	QImage *originale;
	QImage *modified;
	int contrast;
	int brightness;
	int gamma;
	void modif_img();
};

#endif // PIXMAPWIDGET_H
