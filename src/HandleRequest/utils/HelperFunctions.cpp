#include "../utils.hpp"

bool isFileAccessible(const std::string &path)
{
	std::ifstream infile(path);
	return infile.good();
}

// if file dosn't existe return 1, if file existe and not accessible return 2
// otherwise get file size, type and Lastmodification date and return 0;
int getFileInfo(const std::string &fileName, FileInfo &fileI)
{
	struct stat result;
	if(stat(fileName.c_str(), &result)==0)
	{
		if (!S_ISREG(result.st_mode) && !S_ISLNK(result.st_mode))
			return 1;
		if (isFileAccessible(fileName))
		{
			fileI.size  = result.st_size;
			char buf[100];
			time_t mod_time = result.st_mtime;
			struct tm * curtime = std::gmtime( &mod_time );
			std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", curtime);
			fileI.Mtime = buf;
			fileI.Ftype = getFileType(fileName);
			return 0;
		}
	}
	else if (errno != EACCES)
		return 1;
	return 2;
}


const char *getErrorPage(int error)
{
	if (error == 400) return ERR400;
	else if (error == 405) return ERR405;
	else if (error == 406) return ERR406;
	else if (error == 408) return ERR408;
	else if (error == 411) return ERR411;
	else if (error == 413) return ERR413;
	else if (error == 501) return ERR501;
	else if (error == 505) return ERR505;
	return "";
}


const char* getErrorMessage(int error)
{
	if (error == 400) return "Bad Request";
	else if (error == 405) return "Method Not Allowed";
	else if (error == 406) return "Not Acceptable";
	else if (error == 408) return "Request Timeout";
	else if (error == 411) return "Length Required";
	else if (error == 413) return "Payload Too Large";
	else if (error == 501) return "Not Implemented";
	else if (error == 505) return "HTTP Version Not Supported";
	return "Not Found";
}

std::string getOsName()
{
	#ifdef __APPLE__
	return "(Mac OSX)";
	#elif __MACH__
	return "(Mac OSX)";
	#elif __linux__
	return "(Linux)";
	#elif __FreeBSD__
	return "(FreeBSD)";
	#elif __unix || __unix__
	return "(Unix)";
	#else
	return "Other";
	#endif
}

std::string getDate()
{
	char buf[100];
	time_t now = std::time(0);
	struct tm *tm = std::gmtime(&now);
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buf);
}


// mine types from file extention
std::string getFileType(const std::string &path)
{
	const char *last_dot = strrchr(path.c_str(), '.');
	if (last_dot)
	{
		if (strcmp(last_dot, ".html") == 0) return "text/html";
		else if (strcmp(last_dot, ".htm") == 0) return "text/html";
		else if (strcmp(last_dot, ".shtml") == 0) return "text/html";
		else if (strcmp(last_dot, ".css") == 0) return "text/css";
		else if (strcmp(last_dot, ".xml") == 0) return "text/xml";
		else if (strcmp(last_dot, ".csv") == 0) return "text/csv";
		else if (strcmp(last_dot, ".gif") == 0) return "image/gif";
		else if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
		else if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
		else if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
		else if (strcmp(last_dot, ".js") == 0) return "application/javascript";
		else if (strcmp(last_dot, ".json") == 0) return "application/json";
		else if (strcmp(last_dot, ".png") == 0) return "image/png";
		else if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
		else if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
		else if (strcmp(last_dot, ".txt") == 0) return "text/plain";
		else if (strcmp(last_dot, ".atom") == 0) return "application/atom+xml";
		else if (strcmp(last_dot, ".rss") == 0) return "application/rss+xml";
		else if (strcmp(last_dot, ".webp") == 0) return "image/webp";
		else if (strcmp(last_dot, ".3gpp") == 0) return	"video/3gpp";
		else if (strcmp(last_dot, ".3gp") == 0) return "video/3gpp";
		else if (strcmp(last_dot, ".ts") ==	0) return "video/mp2t";
		else if (strcmp(last_dot, ".mp4") == 0) return "video/mp4";
		else if (strcmp(last_dot, ".mpeg") == 0) return "video/mpeg";
		else if (strcmp(last_dot, ".mpg") == 0) return "video/mpeg";
		else if (strcmp(last_dot, ".mov") == 0) return "video/quicktime";
		else if (strcmp(last_dot, ".webm") == 0) return "video/webm";
		else if (strcmp(last_dot, ".flv") == 0) return "video/x-flv";
		else if (strcmp(last_dot, ".m4v") == 0) return "video/x-m4v";
		else if (strcmp(last_dot, ".mng") == 0) return "video/x-mng";
		else if (strcmp(last_dot, ".asx") == 0) return "video/x-ms-asf";
		else if (strcmp(last_dot, ".asf") == 0) return "video/x-ms-asf";
		else if (strcmp(last_dot, ".wmv") == 0) return "video/x-ms-wmv";
		else if (strcmp(last_dot, ".avi") == 0) return "video/x-msvideo";
		else if (strcmp(last_dot, ".mml") == 0) return "text/mathml";
		else if (strcmp(last_dot, ".txt") == 0) return "text/plain";
		else if (strcmp(last_dot, ".jad") == 0) return "text/vnd.sun.j2me.app-descriptor";
		else if (strcmp(last_dot, ".wml") == 0) return "text/vnd.wap.wml";
		else if (strcmp(last_dot, ".htc") == 0) return "text/x-component";
		else if (strcmp(last_dot, ".png") == 0) return "image/png";
		else if (strcmp(last_dot, ".tif") == 0) return "image/tiff";
		else if (strcmp(last_dot, ".tiff") == 0) return "image/tiff";
		else if (strcmp(last_dot, ".wbmp") == 0) return "image/vnd.wap.wbmp";
		else if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
		else if (strcmp(last_dot, ".jng") == 0) return "image/x-jng";
		else if (strcmp(last_dot, ".bmp") == 0) return "image/x-ms-bmp";
		else if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
		else if (strcmp(last_dot, ".svgz") == 0) return "image/svg+xml";
		else if (strcmp(last_dot, ".mid") == 0) return "audio/midi";
		else if (strcmp(last_dot, ".midi") == 0) return "audio/midi";
		else if (strcmp(last_dot, ".kar") == 0) return "audio/midi";
		else if (strcmp(last_dot, ".mp3") == 0) return "audio/mpeg";
		else if (strcmp(last_dot, ".ogg") == 0) return "audio/ogg";
		else if (strcmp(last_dot, ".m4a") == 0) return "audio/x-m4a";
		else if (strcmp(last_dot, ".ra"	) == 0) return "audio/x-realaudio";

	}
	return "application/octet-stream";
}
