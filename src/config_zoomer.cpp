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

#include <stdexcept>

using namespace std;

ConfigZoomer::ConfigZoomer()
	:	message(""),point_x(0.0),point_y(0.0),zoom_factor(1.0),contrast(100),brightness(0),gamma(100),num_file(0),validated(false)
{

}


int ConfigZoomer::read(string cfgfile)//return: 0:ok, -1:no file, 1:problem in the file
{
	//first: check existence of the file
	ifstream conf_file(cfgfile.c_str(), ios::in);
	if(!conf_file)
	{
		std::cerr<<cfgfile<<" file not found!\nCreate an example file...\n";
		init_example();
		write(cfgfile);
		return -1;
	}else{conf_file.close();}

	//read XML content
	TiXmlDocument doc(cfgfile.c_str());
	if ( doc.LoadFile() )
	{
		std::cout << "Reading file " << cfgfile << std::endl;
		
		TiXmlHandle hDoc( &doc );
		TiXmlElement* root;
		try{
			// check that we have "zoomer_cfg" root node...
			// ... and that version is "1.0"
			{
				root = doc.FirstChildElement( "zoomer_cfg" );
				
				if ( !root )
					throw std::string( "Unable to find zoomer_cfg root node !" );
				std::string rootNode( root->Value() );
				if ( rootNode != "zoomer_cfg" )
					throw std::string( "Root node MUST be 'zoomer_cfg' !" );
				
				TiXmlElement* version = root->FirstChildElement("version");
				if ( !version )
					throw std::string( "Unable to find 'version' node !" );
				std::string versionNode( version->GetText() );
				if ( versionNode != "1.0" )
				{
					std::ostringstream oss;
					oss << "'version' MUST be '1.0' ! Value is '" << versionNode << "'." << std::endl;
					throw std::string( oss.str() );
				}
				std::cout << "File " << cfgfile << " is valid ..." << std::endl;
			}

			{
				TiXmlElement* el_message = root->FirstChildElement("message");
				if (!el_message) { std::ostringstream oss;oss << "Unable to find 'message' node !" << std::endl;throw std::string(oss.str());}
				message=el_message->GetText();
				TiXmlElement* el_zoom = root->FirstChildElement("zoom");
				if (!el_zoom) { std::ostringstream oss;oss << "Unable to find 'zoom' node !" << std::endl;throw std::string(oss.str());}
				std::istringstream iss_zoom(el_zoom->GetText());
				iss_zoom>>zoom_factor;
				
				TiXmlElement* el_validated = root->FirstChildElement("validated");
				if (!el_validated) { std::ostringstream oss;oss << "Unable to find 'validated' node !" << std::endl;throw std::string(oss.str());}
				std::istringstream iss_validated(el_validated->GetText());
				iss_validated>>validated;
				
				TiXmlElement* el_image_number = root->FirstChildElement("image_number");
				if (!el_image_number) { std::ostringstream oss;oss << "Unable to find 'image_number' node !" << std::endl;throw std::string(oss.str());}
				num_file=atoi(el_image_number->GetText());
				
				
				TiXmlElement* el_selection = root->FirstChildElement("selection");
				if ( !el_selection )
				{
					std::ostringstream oss;
					oss << "Unable to find 'selection' node !" << std::endl;
					throw std::string( oss.str() );
				}
				{
					TiXmlElement* el_selection_x = el_selection->FirstChildElement("x");
					if ( !el_selection_x )
						throw std::string( "Unable to find 'x' node !" );
					
					//don't use atof because of local settings (want "," instead of ".", why ?)
					std::istringstream iss_point_x(el_selection_x->GetText());
					iss_point_x>>point_x;
					
					TiXmlElement* el_selection_y = el_selection->FirstChildElement("y");
					if ( !el_selection_y )
						throw std::string( "Unable to find 'y' node !" );
					std::istringstream iss_point_y(el_selection_y->GetText());
					iss_point_y>>point_y;
				}
				
				
				TiXmlElement* el_colors = root->FirstChildElement("colors");
				if ( !el_colors )
				{
					std::ostringstream oss;
					oss << "Unable to find 'colors' node !" << std::endl;
					throw std::string( oss.str() );
				}
				{
					TiXmlElement* el_colors_contrast = el_colors->FirstChildElement("contrast");
					if ( !el_colors_contrast )
						throw std::string( "Unable to find 'contrast' node !" );
					contrast=atoi(el_colors_contrast->GetText());
					
					TiXmlElement* el_colors_brightness = el_colors->FirstChildElement("brightness");
					if ( !el_colors_brightness )
						throw std::string( "Unable to find 'brightness' node !" );
					brightness=atoi(el_colors_brightness->GetText());
					
					TiXmlElement* el_colors_gamma = el_colors->FirstChildElement("gamma");
					if ( !el_colors_gamma )
						throw std::string( "Unable to find 'gamma' node !" );
					gamma=atoi(el_colors_gamma->GetText());
				}
				
				
				TiXmlElement* el_image_list = root->FirstChildElement("image_list");
				if ( !el_image_list )
				{
					std::ostringstream oss;
					oss << "Unable to find 'image_list' node !" << std::endl;
					throw std::string( oss.str() );
				}
				{
					TiXmlElement* el_image_list_img = el_image_list->FirstChildElement("img");
					if ( !el_image_list_img )
						throw std::string( "Unable to find 'img' node !" );
					//filenames.push_back(el_image_list_img->GetText());
					filenames.clear();
					while (el_image_list_img)
					{
						filenames.push_back(el_image_list_img->GetText());
						el_image_list_img = el_image_list_img->NextSiblingElement("img");
					}
					
					
				}
				
			}
		}
		catch(const string& s)
		{
			cerr<<s<<"\n";
			return 1;
		}
	}else{
		std::cerr<<"Configuration file is not XML valid!\n";
		return 1;
	}
	
	return 0;
}

//write the XML config file
bool ConfigZoomer::write(string cfgfile)
{
	cout<<"Write configuration to "<<cfgfile<<"\n";
	TiXmlDocument doc;
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * root = new TiXmlElement( "zoomer_cfg" );
	
	TiXmlElement * el_version = new TiXmlElement( "version" );
	root->LinkEndChild( el_version );
	TiXmlText * text_version = new TiXmlText( "1.0" );
	el_version->LinkEndChild( text_version );
	
	TiXmlElement * el_message = new TiXmlElement( "message" );
	root->LinkEndChild( el_message );
	TiXmlText * text_message = new TiXmlText( message.c_str() );
	el_message->LinkEndChild( text_message );
	
	TiXmlElement * el_selection = new TiXmlElement( "selection" );
	root->LinkEndChild( el_selection );
	TiXmlElement * el_selection_x = new TiXmlElement( "x" );
	el_selection->LinkEndChild( el_selection_x );
	std::ostringstream oss_x;oss_x << point_x;
	TiXmlText * text_selection_x = new TiXmlText(oss_x.str().c_str() );
	el_selection_x->LinkEndChild( text_selection_x );
	TiXmlElement * el_selection_y = new TiXmlElement( "y" );
	el_selection->LinkEndChild( el_selection_y );
	std::ostringstream oss_y;oss_y << point_y;
	TiXmlText * text_selection_y = new TiXmlText( oss_y.str().c_str() );
	el_selection_y->LinkEndChild( text_selection_y );
	
	
	TiXmlElement * el_colors = new TiXmlElement( "colors" );
	root->LinkEndChild( el_colors );
	TiXmlElement * el_colors_contrast = new TiXmlElement( "contrast" );
	el_colors->LinkEndChild( el_colors_contrast );
	std::ostringstream oss_contrast;oss_contrast << contrast;
	TiXmlText * tecontrastt_colors_contrast = new TiXmlText(oss_contrast.str().c_str() );
	el_colors_contrast->LinkEndChild( tecontrastt_colors_contrast );
	TiXmlElement * el_colors_brightness = new TiXmlElement( "brightness" );
	el_colors->LinkEndChild( el_colors_brightness );
	std::ostringstream oss_brightness;oss_brightness << brightness;
	TiXmlText * tecontrastt_colors_brightness = new TiXmlText( oss_brightness.str().c_str() );
	el_colors_brightness->LinkEndChild( tecontrastt_colors_brightness );
	TiXmlElement * el_colors_gamma = new TiXmlElement( "gamma" );
	el_colors->LinkEndChild( el_colors_gamma );
	std::ostringstream oss_gamma;oss_gamma << gamma;
	TiXmlText * tecontrastt_colors_gamma = new TiXmlText( oss_gamma.str().c_str() );
	el_colors_gamma->LinkEndChild( tecontrastt_colors_gamma );
	
	TiXmlElement * el_zoom = new TiXmlElement( "zoom" );
	root->LinkEndChild( el_zoom );
	std::ostringstream oss_zoom;oss_zoom << zoom_factor;
	TiXmlText * text_zoom = new TiXmlText( oss_zoom.str().c_str() );
	el_zoom->LinkEndChild( text_zoom );
	
	TiXmlElement * el_validated = new TiXmlElement( "validated" );
	root->LinkEndChild( el_validated );
	std::ostringstream oss_validated;oss_validated << validated;
	TiXmlText * text_validated = new TiXmlText( oss_validated.str().c_str() );
	el_validated->LinkEndChild( text_validated );
	
	TiXmlElement * el_image_number = new TiXmlElement( "image_number" );
	root->LinkEndChild( el_image_number );
	std::ostringstream oss_image_number;oss_image_number << num_file;
	TiXmlText * text_image_number = new TiXmlText( oss_image_number.str().c_str() );
	el_image_number->LinkEndChild( text_image_number );
	
	TiXmlElement * el_image_list = new TiXmlElement( "image_list" );
	root->LinkEndChild( el_image_list );
	for (unsigned int i=0;i<filenames.size();i++)
	{
		TiXmlElement * el_img = new TiXmlElement( "img" );
		el_image_list->LinkEndChild( el_img );
		TiXmlText * text_img = new TiXmlText(filenames[i].c_str());
		el_img->LinkEndChild( text_img );
	}
	
	doc.LinkEndChild( decl );
	doc.LinkEndChild( root );

	
	doc.SaveFile(cfgfile.c_str());
	return true;
}

void ConfigZoomer::init_example()
{
	message="This is an example file: please click upper-left corner of B";
	point_x=150.0;
	point_y=100.0;
	zoom_factor=1.0;
	contrast=100;
	brightness=0;
	gamma=100;
	num_file=1;
	filenames.push_back("ex/A.png");
	filenames.push_back("ex/B.png");
	filenames.push_back("ex/C.png");
}
