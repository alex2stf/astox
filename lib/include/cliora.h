#ifndef ASTOX_CLI_ORA_H
#define ASTOX_CLI_ORA_H

#include <occi.h>



namespace astox {



	class OraSqlConnection
	{
	public:
		OraSqlConnection();
		~OraSqlConnection();
		void connect(text* username, text* password, text* dbname);

		void execute(text* query);

		void close();

		bool tableExists(char* name);
	

	private:
		OCISession* ocisession = (OCISession*)0;
		OCIEnv* ocienv = NULL;

		OCIError* errhp = NULL;
		OCIServer* srvhp = NULL;
		OCISvcCtx* svchp = NULL;
		OCIDefine* defnp = (OCIDefine*)0;
		

	};

	

}

#endif // !ASTOX_CLI_ORA_H