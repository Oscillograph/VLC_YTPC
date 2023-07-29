#ifndef XSPF_H
#define XSPF_H

class XSPFCreator {
public:
	inline static std::string AddHeader()
	{
		std::string output = "";
		output += u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		output += u8"<playlist xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\" version=\"1\">\n";
		output += u8"	<title>Плейлист</title>\n";
		output += u8"	<trackList>\n";
		return output;
	}
	
	inline static std::string AddTrack(
		const std::string& title, 
		const std::string& url, 
		const int& i, 
		const int& caching = 1000	// bigger value (in ms) might help in decreasing buffering
		)
	{
		std::string output = "";

		output += // new track open
		u8"		<track>\n";
		output += // location
		u8"			<location>https://www.youtube.com" + url + "</location>\n";
		output += // title
		u8"			<title>" + title + "</title>\n";
		output += // extension application open
		u8"			<extension application=\"http://www.videolan.org/vlc/playlist/0\">\n";
		output += // id
		u8"				<vlc:id>" + std::to_string(i) + "</vlc:id>\n";
		output += // options
		u8"				<vlc:option>network-caching=" + std::to_string(caching) + "</vlc:option>\n";
		output += // extension application close
		u8"			</extension>\n";
		output += // new track close
		u8"		</track>\n";
		return output;
	}
	
	inline static std::string AddFooter(const int& itemsTotal)
	{
		std::string output = "";
		u8"	</trackList>\n";
		output += u8"	<extension application=\"http://www.videolan.org/vlc/playlist/0\">\n";
		for (int j = 0; j <= itemsTotal; j++){
			output += u8"		<vlc:item tid=\"" + std::to_string(j) + "\"/>\n";
		}
		output += u8"	</extension>\n";
		output += u8"</playlist>\n";
		return output;
	}
	
	inline static bool Create( // return true if something written
		const std::string& source, 
		const std::string& filepath = "./output/playlist.xspf")
	{
		std::ofstream m_OutputFile;
		if (!m_OutputFile.is_open())
		{
			m_OutputFile.open(filepath);
			m_OutputFile << source;
			m_OutputFile.close();
			return true;
		}
		return false;
	}
	
	inline static bool Create( // return true if something written
		const std::string& source, 
		std::ofstream& m_OutputFile,
		const std::string& filepath = "./output/playlist.xspf"
		)
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
	
};

#endif
