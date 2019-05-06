/**
 * file: projet.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
 
/**************************************************************************************

=======>  	DDDD	OOOOOO	DDDD	GGGGGG	EEEEEE	  <================================
			D   D	O	 O	D   D	G		E
=======>	D    D	O	 O	D    D	G		E		  <================================
			D	 D  O    O  D    D  G       EEEE
=======>    D    D	O 	 O  D    D	G  GGG	E		  <================================
			D   D	O 	 O  D   D	G    G	E
=======>	DDDD	OOOOOO  DDDD	GGGGGG	EEEEEE 	  <================================

=======================================>  	B19B	AAAAAA	 C		 E		   <=======
											B	B	A	 A	 E		 M        
=======================================>	B   B	A	 A	 M		 R         <=======
											BMAY	AAAAAA	 K		 E         
=======================================>	B   B	A	 A	 E		 Y		   <=======
											B   B	A	 A	 S	 	 A		  
=======================================>	2019	A	 A	 KEEEEE	 ZICIII    <=======

**************************************************************************************/

/**************************************************************************************
                                                                                       
                                                                                                                                                                           
                                       ``.....``                                       
                               `-/oyhmNNNNNNNNNNmdhs+/-                                
                           `/sdNNMMMMMMMMMMMMMMMMMMMMMNNho:`                           
                        .+hNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNh/`                        
                     `/hNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNy:                      
                   `+mNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd/                    
                  /dMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd:                  
                .yNMMMMMMMhyhdNMMMMMMMMMMMMMMMMMMMMMMMMMNdhyhMMMMMMMNs`                
               :mMMMMMMMMh   `.:odNMMNmmdddddddmNNMMNho:.`  `mMMMMMMMMd-               
              +NMMMMMMMMM/       `./-..`````````..-:.`       oMMMMMMMMMN:              
             /NMMMMMMMMMM:                                   oMMMMMMMMMMN:             
            -NMMMMMMMMMMMs                                   hMMMMMMMMMMMm.            
           `dMMMMMMMMMMMm:                                   /mMMMMMMMMMMMy            
           /MMMMMMMMMMMd.                                     -mMMMMMMMMMMM-           
           dMMMMMMMMMMN.                                       :MMMMMMMMMMMy           
          `MMMMMMMMMMMh                                         mMMMMMMMMMMN           
          -MMMMMMMMMMMs                                         hMMMMMMMMMMM`          
          :MMMMMMMMMMMy                                         dMMMMMMMMMMM.          
          -MMMMMMMMMMMm                                        `NMMMMMMMMMMN`          
           NMMMMMMMMMMM/                                       oMMMMMMMMMMMd           
           yMMMMMMMMMMMm-                                     :NMMMMMMMMMMMo           
           -NMMMMMMMMMMMm:                                  `/NMMMMMMMMMMMN`           
            sMMMMMMMMMMMMNy:`                             `:hMMMMMMMMMMMMM+            
            `dMMMMMmyhmNMMMMds/.``                   `.-/ymMMMMMMMMMMMMMMy             
             .dMMMMd/.-:sNMMMMMNmdys+`           .oyhdmMMMMMMMMMMMMMMMMMh`             
              .hMMMMMmo..+dMMMMMMMMN-             :NMMMMMMMMMMMMMMMMMMMy`              
               `sNMMMMMy`.:shNNNNNm+               yMMMMMMMMMMMMMMMMMNo                
                 :dMMMMMy```.+.o+-+                +MMMMMMMMMMMMMMMMd-                 
                  `+mMMMMms-```````                +MMMMMMMMMMMMMMm/                   
                    `+mNMMMMNmdhddm-               +MMMMMMMMMMMNd/`                    
                       :yNMMMMMMMMM:               +MMMMMMMMNms:                       
                         `/smNMMMMM:               +MMMMMNds:`                         
                             ./shmh.               -dmho/.                             
                                                                                       
                                                                                       
**************************************************************************************/

#include <algorithm>
#include <memory>
#include <iostream>
#include "define.h"
#include "simulation.h"
#include "gui.h"



/// ===== CONSTANTS ===== ///

static constexpr int NO_CMDLINE_ARGUMENT(1);
static constexpr int NB_MAX_PARAM(2);	//nb of maximum possible parameters
										//for now we have only "Error" implemented
static constexpr int NB_IO_FILES(2);
static const std::array<std::string, 
						NB_MAX_PARAM> POSSIBLE_PARAMETERS = {"Error", "Step"};

/// ===== FUNCTION DECLARATIONS ===== ///

static void read_cmd_args(int, char*[], 
						  std::vector<std::string>&, std::vector<std::string>&);
static void init_execution_parameters(std::vector<std::string> const&, 
									  std::unordered_map<std::string, bool>&);
static int open_gui();

/// ===== MAIN FUNCTION ===== ///

int main(int argc, char* argv[]) {

	Segment seg(0,0,1,1);
	Rectangle rec({0,0},2,2);
	
	std::cout << Tools::intersect(rec,seg,0) << std::endl;
	seg = Segment(-0.1,0, -0.1,1);
	std::cout  << Tools::intersect(rec,seg,0.2) << " : result" << std::endl<< std::endl;
	seg = Segment(3,0,0,3);
	std::cout << Tools::intersect(rec,seg,0.2)<< " : result" << std::endl<< std::endl;
	seg = Segment(4.1,0,0,4.1);
	std::cout << Tools::intersect(rec,seg,0.1/sqrt(2)- 0.00001)<< " : result" << std::endl<< std::endl;
	seg= Segment(2.50000, 1.09,3,0);
	std::cout<< Tools::intersect(rec,seg,0.5)<< " : result" << std::endl<< std::endl;
	seg= Segment(-1,0,2,3);
	std::cout  << Tools::intersect(rec,seg,0.5)<< " : result" << std::endl<< std::endl;
	seg= Segment(-1,0,0,2.54);
	std::cout << Tools::intersect(rec,seg,0.2)<< " : result" << std::endl<< std::endl;
	seg= Segment(-1,0,-0.2000001,0);
	std::cout <<  Tools::intersect(rec,seg,0.2)<< " : result" << std::endl << std::endl;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	std::vector<std::string> io_files;
	std::unordered_map<std::string, bool> execution_parameters;

	{
	std::vector<std::string> cmd_parameters;
	read_cmd_args(argc, argv, cmd_parameters, io_files);
	init_execution_parameters(cmd_parameters, execution_parameters);
	}	//cmd_parameters' lifetime expired, we don't need it anymore
	
	// initialize execution parameters in Simulator
	Simulator::exec_parameters(execution_parameters);
	
	if(execution_parameters["Error"] == true) { 
		Simulator::create_simulation(io_files);
	} else if (execution_parameters["Step"] == true) {
		if(io_files.size() > 0) {
			Simulator::create_simulation(io_files);
		} else {
			std::cout << "No I/O file. Aborting... " << std::endl; 
		}
	} else if (io_files.size() > 0) {
		Simulator::create_simulation(io_files);
		open_gui();
	} else {				
		open_gui();
	}
		
}

/// ===== FUNCTION DEFINITIONS ===== ///

/** 
 * Reads command line arguments from args and sets "io_files" with given .txt files and
 * "cmd_parameters" with given execution parameters.
 */
static void read_cmd_args(int argc, char* argv[], 
						  std::vector<std::string>& cmd_parameters, 
						  std::vector<std::string>& io_files) {	

	if(argc == NO_CMDLINE_ARGUMENT) return;	//no need to read args if there are none
	unsigned int nb_parameters(argc);
	
	unsigned int io_counter(0);	//counts io parameters
	bool io_format_found(false);

	std::string argv_param;
	
	for(size_t i(1); i<nb_parameters; ++i) {	// argv[0] is not relevant
		argv_param = argv[i];
		io_format_found = argv_param.find(".txt") != std::string::npos;
		if(io_format_found && io_counter < NB_IO_FILES) {
			io_files.push_back(argv[i]);
			++io_counter;
		} else {
			cmd_parameters.push_back(argv_param);
		}
	}	
}

/**
 * Initialises an unordered map, pairing possible execution parameters with 
 * corresponding boolean indicating their state.
 * 
 * (This function will be useful when we add more execution parameters in the future!)
 */
static void init_execution_parameters(std::vector<std::string> const &cmd_parameters, 
									  std::unordered_map<std::string, bool> 
									  &exec_parameters) {
	for(const auto &param : POSSIBLE_PARAMETERS) {
		//if a possible parameter is not invoked, mark it false; else mark it true
		if(std::find(cmd_parameters.begin(), cmd_parameters.end(), param) == 
		   cmd_parameters.end())	
			exec_parameters.emplace(param,false);
		else
			exec_parameters.emplace(param,true);
	}
}

static int open_gui() {
	auto app = Gtk::Application::create();
		
	Gui_Window proWindow;
	return app->run(proWindow);
}
