#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

// Two directories:
// input - for html files to parse
// output - for xspf files to write

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
		
		// m_URLRegex = std::regex("\/watch\?v=([a-zA-Z0-9\-]+)");
		// m_TitleRegex = 
		
		std::cout << "Reading file: " << input << "\n";
		if (GetRawHTML(rawHTML, input)) 
		{
			// initialize XSPF file source
			xspfSource += u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			xspfSource += u8"<playlist xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\" version=\"1\">\n";
			xspfSource += u8"	<title>Плейлист</title>\n";
			xspfSource += u8"	<trackList>\n";
			int i = 0;
			
			std::cout << "Parsing. . .\n";
			if (GetVideoTitles(rawHTML))
			{
				if (m_TitleMatches.ready())
					std::cout << "Found " << m_TitleMatches.size() << " titles.\n";
				if (GetVideoURLs(rawHTML))
				{
					std::cout << "Found " << m_urlMatches.size() << " urls.\n";
					for (i = 0; i < m_TitleMatches.size(); i++)
					{
						tempTitle = m_TitleMatches[i];
						tempURL = m_urlMatches[2*i]; // there are 2 URLs per title
						
						// add an element to output string
						xspfSource += // new track
						"		<track>\n";
						xspfSource += // location
						"			<location>" + tempURL + "</location>\n";
						xspfSource += // title
						"			<title>" + tempTitle + "</title>\n";
						xspfSource += // extension application open
						"			<extension application=\"http://www.videolan.org/vlc/playlist/0\">\n";
						xspfSource += // id
						"				<vlc:id>" + std::to_string(i) + "</vlc:id>";
						xspfSource += // options
						"				<vlc:option>network-caching=1000</vlc:option>\n";
						xspfSource += // extension application close
						"			</extension>";
						xspfSource += // new track close
						"		</track>\n";
						i++;
					}
				}
			}
			
			// close XSPF file source
			xspfSource += "	</tracklist>\n";
			xspfSource += "	<extension application=\"http://www.videolan.org/vlc/playlist/0\">\n";
			for (int j = 0; j <= i; j++){
				xspfSource += "		<vlc:item tid=\"" + std::to_string(j) + "\"/>\n";
			}
			xspfSource += "	</extension>\n";
			xspfSource += "</playlist>\n";
			
			std::cout << " Parsing complete.\n";
			std::cout << "Writing to: " << output << "\n";
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
	
	bool GetVideoTitles(const std::string& source)
	{
		std::regex_search(source, m_TitleMatches, m_TitleRegex);
		if (m_TitleMatches.ready())
			return true;
		return false;
	}
	
	bool GetVideoURLs(const std::string& source)
	{
		std::regex_search(source, m_urlMatches, m_URLRegex);
		if (m_urlMatches.ready())
			return true;
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
	
	std::regex m_TitleRegex = std::regex(u8"aria-label=\".+ Game Engine series");
	std::regex m_URLRegex = std::regex(u8"/watch\?v=([a-zA-Z0-9-]+)");
	
	std::smatch m_TitleMatches;
	std::smatch m_urlMatches;
};


int main(int argc, char** argv) 
{
	std::string iFile = "./input/html-input.txt";
	std::string oFile = "./output/playlist.xspf";
	
	std::cout << "Calling PlaylistParser.\n";
	PlaylistParser* pp = new PlaylistParser(iFile, oFile);
	std::cout << "Done.\n";
	delete pp;
	
	return 0;
}
