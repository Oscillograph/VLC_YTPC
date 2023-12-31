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
			
			while (GetVideoTitle(rawHTML))
			{	
				if (GetVideoURL(rawHTML))
				{
					tempTitle = m_TitleMatches[0];
					// 12 from the left for aria-label="
					// 12 from the right for Автор: which is:
					// 1 for ":" + 2*5 cyrillic letters + 1 for blank space
					tempTitle = tempTitle.substr(12, tempTitle.size()-(12 + 12));
					tempURL = m_urlMatches[0];
					
					// add an element to output string
					xspfSource += XSPFCreator::AddTrack(tempTitle, tempURL, i);
				}
				i++;
				rawHTML = m_TitleMatches.suffix().str();
			}
			
			// close XSPF file source
			xspfSource += XSPFCreator::AddFooter(i);
			
			// write output string to XSPF file
			XSPFCreator::Create(xspfSource, m_OutputFilePath);
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
	
	bool GetVideoTitle(const std::string& source)
	{
		if (std::regex_search(source, m_TitleMatches, m_TitleRegex))
		{
			return true;
		}
		return false;
	}
	
	bool GetVideoURL(const std::string& source)
	{
		if (std::regex_search(source, m_urlMatches, m_URLRegex))
		{
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
