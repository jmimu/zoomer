/*
 *      MainWindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScrollArea>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QPushButton>

#include "PixmapWidget.h"
#include "config_zoomer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool load_config(QString cfgfile);
private:
		void resizeEvent( QResizeEvent* );
		QScrollArea *sa;
		PixmapWidget *pw;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QPushButton *reload_img;
    QPushButton *contrast_more;
    QPushButton *contrast_less;
    QPushButton *brightness_more;
    QPushButton *brightness_less;
    QPushButton *gamma_more;
    QPushButton *gamma_less;
    QPushButton *validation;

    QPushButton *previous;
    QPushButton *next;
    QPushButton *extra;
    QPushButton *exit;
    
    ConfigZoomer cfg;
    
public slots:
    void reloadImg();
    void contrastMore();
    void contrastLess();
    void brightnessMore();
    void brightnessLess();
    void gammaMore();
    void gammaLess();
    void validation_();
    void cancel_();
    void extra_();
    bool previous_();
    bool next_();

};

#endif // MAINWINDOW_H
