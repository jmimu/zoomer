/*
 *      config_zoomer.cpp
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

#include "config_zoomer.h"

using namespace std;

ConfigZoomer::ConfigZoomer()
	:	message(""),point_x(0.0),point_y(0.0),zoom_factor(1.0),contrast(100),brightness(0),gamma(100),num_file(0)
{

}


bool ConfigZoomer::read(string cfgfile)
{
	filenames.clear();
	
	ifstream conf_file(cfgfile.c_str());
	string line;
	
	getline(conf_file,message);
	
	getline(conf_file,line);
	istringstream iss1( line );
	iss1>>point_x;
	
	getline(conf_file,line);
	istringstream iss2( line );
	iss2>>point_y;
	
	getline(conf_file,line);
	istringstream iss3( line );
	iss3>>zoom_factor;
	
	getline(conf_file,line);
	istringstream iss4( line );
	iss4>>contrast;
	
	getline(conf_file,line);
	istringstream iss5( line );
	iss5>>brightness;
	
	getline(conf_file,line);
	istringstream iss6( line );
	iss6>>gamma;
	
	
	getline(conf_file,line);
	istringstream iss7( line );
	iss7>>num_file;
	
	while (getline(conf_file,line))
	{
		if (line!="")
		{
			filenames.push_back(line);
		}
	}
	
	return true;
}


bool ConfigZoomer::write(string cfgfile)
{
	ofstream conf_file(cfgfile.c_str());
	
	conf_file<<message<<endl;
	conf_file<<point_x<<endl;
	conf_file<<point_y<<endl;
	conf_file<<zoom_factor<<endl;

	conf_file<<contrast<<endl;
	conf_file<<brightness<<endl;
	conf_file<<gamma<<endl;

	conf_file<<num_file<<endl;

	for (unsigned int i=0;i<filenames.size();i++)
		conf_file<<filenames[i]<<endl;

	conf_file.close();
	
	return true;
}
