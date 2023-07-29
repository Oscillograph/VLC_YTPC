#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H

#include "./XSPF.h"
#include <regex>

class PlaylistParser 
{
public:
	PlaylistParser(
		std::string input = "./input/html-input.txt", 
		std::string output = "./output/playlist.xspf"
		) 
	: m_InputFilePath(input), m_OutputFilePath(output)
	{
		std::string rawHTML = "";
		std::string tempTitle = "";
		std::string tempURL = "";
		std::string xspfSource = "";
		
		if (GetRawHTML(rawHTML, input)) 
		{
			// initialize XSPF file source
			xspfSource += XSPFCreator::AddHeader();
			int i = 0;
			
			while (GetVideoTitles(rawHTML))
			{	
				if (GetVideoURLs(rawHTML))
				{
					tempTitle = m_TitleMatches[0];
					tempTitle = tempTitle.substr(12, tempTitle.size());
					tempURL = m_urlMatches[0]; // there are 2 URLs per title
					
					// add an element to output string
					xspfSource += XSPFCreator::AddTrack(tempTitle, tempURL, i);
				}
				i++;
				rawHTML = m_TitleMatches.suffix().str();
			}
			
			// close XSPF file source
			xspfSource += XSPFCreator::AddFooter(i);
			
			// write output string to XSPF file
			CreateXSPF(xspfSource, output);
		}
	}
	
	
	~PlaylistParser(){
		if (m_InputFile.is_open())
		{
			m_InputFile.close();
		}
		if (m_OutputFile.is_open())
		{
			m_OutputFile.close();
		}
	}
	
	bool GetRawHTML( // return true if something read
		std::string& output, 
		std::string filepath = "./input/html-input.txt"
		)
	{
		std::string lineRead;
		const char delimLine = '\n';
		
		if (!m_InputFile.is_open())
		{
			m_InputFile.open(filepath);
			if(m_InputFile.is_open())
			{
				while (std::getline(m_InputFile, lineRead, delimLine))
				{
					output += (lineRead + "\n");
				}
				m_InputFile.close(); // no need to leave it open
				return true;
			}
		}
		
		return false;
	}
	
	bool GetVideoTitles(std::string& source)
	{
		if (std::regex_search(source, m_TitleMatches, m_TitleRegex))
		{
			return true;
		}
		return false;
	}
	
	bool GetVideoURLs(std::string& source)
	{
		if (std::regex_search(source, m_urlMatches, m_URLRegex))
		{
			return true;
		}
		return false;
	}
	
	bool CreateXSPF( // return true if something written
		const std::string& source, 
		std::string filepath = "./output/playlist.xspf")
	{
		if (!m_OutputFile.is_open())
		{
			m_OutputFile.open(filepath);
			m_OutputFile << source;
			m_OutputFile.close();
			return true;
		}
		return false;
	}
	
private:
	std::string m_InputFilePath;
	std::string m_OutputFilePath;
	
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;
	
	std::regex m_TitleRegex = std::regex(u8"(aria-label=\".+ Автор:) | (aria-label=\".+ Автор:)");
	std::regex m_URLRegex = std::regex("/watch\\?v=[a-zA-Z0-9\\-_]+");
	
	std::smatch m_TitleMatches;
	std::smatch m_urlMatches;
};

#endif
