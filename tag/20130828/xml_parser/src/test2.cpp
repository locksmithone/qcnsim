
#include "tinyxml2.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <iterator>
#include "quakes.h"
#include "hosts.h"
#include "stdlib.h"
#include <sstream>
#include <ctime>



void read_hosts(char* qcn_xml, char* host_xml, std::vector<Host> *hosts );
void read_quake(Quake* q1, char* quake_xml);

//************************************
//1st input: qcn xml	            **
//2nd input: host xml	            **
//3rd input: quake xml              ** 
//4th input: name of the output file**
//5th input: simulaton length	    ** 
//************************************

int main(int argc, char *argv[]){
		
	//check to make sure all the files are put in as arguements
	if(argc != 6){
		printf("<qcn xml> <host xml> <quake xml> <output filename> <sim length>\n");
		return 1;
	}
	//It is assumed that they are put in the proper order
	printf(" 1: %s\n",argv[1]);
	printf(" 2: %s\n",argv[2]);
	printf(" 3: %s\n",argv[3]);
	int sim_len = atoi(argv[5]);


	//************************
	//**  READING IN QUAKES **
	//************************
	
	//make a quake object
	Quake *q1;
	Quake q2;
	q1 = &q2;
	//call the fcn
	read_quake( q1, argv[3]);


	//*******************
	//**READING IN HOSTS**
	//********************
	

	//holds all the hosts
	std::vector<Host> *hosts1;
	std::vector<Host> hosts;
	hosts1 = &hosts;
	
	//call the fcn
	read_hosts(argv[1], argv[2], hosts1);


	//**********************
	//**MAKING OUTPUT FILE**
	//**********************
	
	//first thing is to put in the quake information
	
	//set up the quake part 
	std::string str = "<results> \n <quakes> \n <quake latitude='";
	//read in the quake's information
	//need to use this so we can go from float to string
	std::ostringstream buffer;
	buffer << q2.latitude;
	str.append(buffer.str());
	str.append("' longitude='");
	//these 2 lines clear the buffer
	buffer.str("");
	buffer.clear();
	buffer << q2.longitude;
	str.append(buffer.str());
	str.append("' magnitude='");
	buffer.str("");
        buffer.clear();
	buffer << q2.magnitude;
        str.append(buffer.str());
	str.append("' depth='");
        buffer.str("");
        buffer.clear();
        buffer << q2.depth;
        str.append(buffer.str());
	str.append("' time='");
        buffer.str("");
        buffer.clear();
        buffer << q2.time;
        str.append(buffer.str());
	//finish off the quake part
	str.append("'> \n </quake> \n </quakes> \n");
	
	//set up the trigger portion
	str.append("<triggers>\n");
	

	//************************
	//**   RANDOM DATA!!!	**
	//************************


	//add some fake data to the hosts
	srand((unsigned)time(0));
	//just makes a fake time for the trigger
	int fake_time = rand()%sim_len;
	hosts[0].trig_time.push_back(fake_time);
	//makes a fake magnotude values from 0-10
        float fake_mag = (float)rand()/((float)RAND_MAX/10.00);
	hosts[0].trig_fmag.push_back(fake_mag);
	fake_time = rand()%sim_len;
	hosts[1].trig_time.push_back(fake_time);
	fake_mag = (float)rand()/((float)RAND_MAX/10.00);
        hosts[1].trig_fmag.push_back(fake_mag);
	fake_time = rand()%sim_len;
	hosts[2].trig_time.push_back(fake_time);
	fake_mag = (float)rand()/((float)RAND_MAX/10.00);
        hosts[2].trig_fmag.push_back(fake_mag);




	//puts in the trigger information
	
	//loop through the list of hosts
	int i;
	//loop through the triggers in each host 
	int j;
	for ( i =0; i < hosts.size(); i ++){
		for ( j = 0; j < hosts[i].trig_time.size(); j ++){
			//add the trigger time
			str.append("<trigger time='");
			buffer.str("");
		        buffer.clear();
        		buffer << hosts[i].trig_time[j];
        		str.append(buffer.str());
			//add the lat
			str.append("' lat='");
			buffer.str("");
                        buffer.clear();
                        buffer << hosts[i].lat;
                        str.append(buffer.str());
			//add the lng
			str.append("' lng='");
			buffer.str("");
                        buffer.clear();
                        buffer << hosts[i].lng;
                        str.append(buffer.str());
			//add the fmag
			str.append("' fmag='");
			buffer.str("");
                        buffer.clear();
                        buffer << hosts[i].trig_fmag[j];
                        str.append(buffer.str());
			// close it
			str.append("' /> \n");
		}
	}
	
	//end the trigger portion
	str.append("</triggers>\n");

	//end the xml
	str.append("</results>");

	
	tinyxml2::XMLDocument doc4;
        //you need to be sure your string is in proper xml format or it won't parse it
        //it might also make it slightly differntly from your input
	doc4.Parse( str.c_str());	
	//create the outputfile
	std::string out_name = argv[4];
	out_name.append(".xml");
	char * please = (char*) out_name.c_str();
	FILE* output_file = fopen(please, "w");
	//make the printer so we can save out xml to the file
	tinyxml2::XMLPrinter printer(output_file);
	//print the xml onto the ouput file
	doc4.Print(&printer);
	fclose(output_file);
	return 0;

}

void read_quake(Quake *q1,char* quake_xml){
	// get the quake xml
	tinyxml2::XMLDocument qdoc;
	qdoc.LoadFile(quake_xml);
	//get element that holds the values
	tinyxml2::XMLElement* quake_data = qdoc.FirstChildElement()->FirstChildElement( "quake" );
        //assign the values
        quake_data->QueryFloatAttribute( "latitude", &(q1->latitude));
        quake_data->QueryFloatAttribute( "longitude", &(q1->longitude));
        quake_data->QueryFloatAttribute( "magnitude", &(q1->magnitude));
        quake_data->QueryFloatAttribute( "depth", &(q1->depth));
        quake_data->QueryFloatAttribute( "time", &(q1->time));
	
}

void read_hosts(char* qcn_xml, char* host_xml, std::vector<Host> *hosts){
	//there are 2 different xml files that hold the data for the hosts. 
	//read in qcn file
	tinyxml2::XMLDocument doc;
	doc.LoadFile( qcn_xml );
	//read in host file
	tinyxml2::XMLDocument doc2;
	doc2.LoadFile( host_xml );
	//used to iterate through the files
	tinyxml2::XMLElement* qcn_node = doc.FirstChildElement()->FirstChildElement( "marker" );
	tinyxml2::XMLElement* host_node = doc2.FirstChildElement()->FirstChildElement( "row_host" );
	  
	float holder;
	//while it exists
	while (qcn_node){
		//add a host to the vector
	        hosts->push_back(Host());
	        //sets the host's id 
	        hosts->back().id = (atoi((host_node ->FirstChildElement( "id" )-> GetText())));
	        //set the host's create time
	        hosts->back().create_time = (float)(atof((host_node ->FirstChildElement ( "create_time" ) -> GetText())));	                             
                //sets the host's lat
	        qcn_node ->QueryFloatAttribute( "lat", &holder);	                                                                                     
                hosts->back().lat = holder;	                                                                                                             
                //set host's long
	        qcn_node ->QueryFloatAttribute( "lng ", &holder);                    
                hosts->back().lng = holder;
		//Iterate
	        qcn_node = qcn_node ->NextSiblingElement( "marker" );
	        host_node = host_node ->NextSiblingElement( "row_host" );
	}
}

