#ifndef RES_GEN_001_HPP
#define RES_GEN_001_HPP

#include "../MACROS.hpp"
#include "../Includes.hpp"
#include "Response.hpp"
#include "../configuration/ServersInterface.hpp"

extern ServersInterface ServI;

struct	FileInfo;
/*
** this class is used to generate response :
** -> error response
** -> redirect response
** -> cach response
*/

class ResGenerator
{
	public:
		ResGenerator();
		~ResGenerator();
		Response *getErrorResponse(size_t server, int error,
			const std::string &Host = "",
			bool keepAlive = 0,
			const std::string &Header = ""
			);
		Response *getRedirecResponse(size_t server, int code,
			const std::string &Location,
			const std::string &Host = "",
			bool keepAlive = 0
			);
		Response* getErrorResponse( Response *res, int error, const std::string &Host );
		Response *get304Response(size_t server, const std::string &time, bool kAlive);
		Response *get200Response(size_t server, const std::string &fileName, FileInfo &Fdata);
		Response *get204Response(size_t server, bool keepAlive);
		Response *GetDirListingResponse(const std::string &PATH, const std::string &name,  size_t server, bool connection);
	private:
		void setDefaultErrorPage(Response *res, const char *errpage);
		Response *createResponse(size_t server, int code);
		void setConnection(Response *res, bool keepAlive);
		bool setUserErrorPage(Response *res, size_t server, const std::string &host, int error);
};

#endif
