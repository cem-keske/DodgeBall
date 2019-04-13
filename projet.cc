/**
 * file: projet.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include <algorithm>
#include <memory>
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

	std::vector<std::string> io_files;
	std::unordered_map<std::string, bool> execution_parameters;

	{
	std::vector<std::string> cmd_parameters;
	read_cmd_args(argc, argv, cmd_parameters, io_files);
	init_execution_parameters(cmd_parameters, execution_parameters);
	}	//cmd_parameters' lifetime expired, we don't need it anymore
	
	
	if(execution_parameters["Error"] == true) 
		//Simulation simulation(execution_parameters, io_files);
		Simulator::create_simulation(execution_parameters, io_files);
	else if (io_files.size() > 0) {
		Simulator::create_simulation(execution_parameters, io_files);
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
