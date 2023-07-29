#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "./include/XSPF.h"
#include "./include/PlaylistParser.h"

int main(int argc, char** argv) 
{
	std::cout <<
	"~~~~~~~~~~~~~~~~~~~~~~ VLC YouTube Playlist Creator ~~~~~~~~~~~~~~~~~~~~\n" <<
	"It takes an html piece of the playlist webpage\n" <<
	"and converts it to XSPF-file to open in VLC.\n\n" <<
	"How to use:\n" <<
	"0. Get html code corresponding to the playlist via your browser's \n" <<
	"   developer interface (this way you copy the actual code constructed \n" <<
	"   by your browser after scripts processing).\n" <<
	"1. Save the code in a TXT-file and place it in the folder called input.\n" <<
	"2. Type Y or y.\n" <<
	"3. Wait for it...\n" <<
	"4. Collect your LEGENDARY files from the folder Output.\n"<<
	"   (Yes, you can process multiple files at a time!)\n" <<
	"5. Repeat as many times as you like.\n"<<
	"6. Type Q or q to exit.\n"<<
	"\n\n\n";
	
	std::string pathInput = "./input"; // input folder
	std::string pathOutput = "./output"; // output folder
	std::string sBuffer1; // just a buffer for strings
	std::vector<std::string> inputFileNames; // store names of input files here
	std::vector<std::string> outputFileNames; // store names of output files here
	
	PlaylistParser* pp = nullptr;
	bool quit = false; // exit flag
	char command; // user command in 1 symbol
	
	while (!quit)
	{
		std::cout << "Enter: ";
		std::cin >> command;
		std::cout << "\nProcessing command...\n";
		switch(command)
		{
		case 'Q':
			case 'q': 
			std::cout << "Exit.\n";
			quit = true;
			break;
		case 'Y':
		case 'y':
			{
				// get the list of files in Input directory
				std::string pathTemp = pathInput;
				inputFileNames.clear();
				outputFileNames.clear();
				for (const auto & filePointer : std::filesystem::directory_iterator(pathTemp))
				{
					inputFileNames.push_back(filePointer.path().string());
					sBuffer1 = pathOutput + "/" + 
					inputFileNames[inputFileNames.size()-1].substr(pathInput.size()+1, std::string::npos);
					
					auto lastDot = sBuffer1.find_last_of(".");
					auto symbols = (lastDot == std::string::npos) ? (sBuffer1.size()) : (lastDot);
					sBuffer1 = sBuffer1.substr(0, symbols) + ".xspf";
					
					outputFileNames.push_back(sBuffer1);
				}
				
				// for each of these files
				for (int i = 0; i < inputFileNames.size(); i++)
				{
					std::cout << "Processing file:" << inputFileNames[i] << ".\n";
					{
						pp = new PlaylistParser(inputFileNames[i], outputFileNames[i]);
					}
				}
				std::cout << "Done.\n";
			}
		}
	}
	
	return 0;
}
