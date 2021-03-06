/*
 *      config_zoomer.h
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


#ifndef CONFIG_ZOOMER_H
#define CONFIG_ZOOMER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tinyxml/tinyxml.h"

using namespace std;

/**
 * Zoomer configuration, using XML
 * */

class ConfigZoomer
{
	public:
		ConfigZoomer();
		int read(string cfgfile);
		bool write(string cfgfile);
		void init_example();
		
		string message;
		float point_x;
		float point_y;
		float zoom_factor;
		int contrast;
		int brightness;
		int gamma;
		unsigned int num_file;//number of the picture (first = 1)
		vector <string> filenames;
		bool validated;
};

#endif // CONFIG_ZOOMER_H
