#include "cliora.h"

namespace astox {

	static void checkerr(OCIError* errhp,
		sword status)

	{
		text errbuf[512];
		sb4 errcode = 0;

		switch (status)
		{
		case OCI_SUCCESS:
			break;
		case OCI_SUCCESS_WITH_INFO:
			(void)printf("Error - OCI_SUCCESS_WITH_INFO\n");
			break;
		case OCI_NEED_DATA:
			(void)printf("Error - OCI_NEED_DATA\n");
			break;
		case OCI_NO_DATA:
			(void)printf("Error - OCI_NODATA\n");
			break;
		case OCI_ERROR:
			(void)OCIErrorGet((dvoid*)errhp, (ub4)1, (text*)NULL, &errcode,
				errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
			(void)printf("Error - %.*s\n", 512, errbuf);
			break;
		case OCI_INVALID_HANDLE:
			(void)printf("Error - OCI_INVALID_HANDLE\n");
			break;
		case OCI_STILL_EXECUTING:
			(void)printf("Error - OCI_STILL_EXECUTE\n");
			break;
		case OCI_CONTINUE:
			(void)printf("Error - OCI_CONTINUE\n");
			break;
		default:
			break;
		}
	}

	OraSqlConnection::OraSqlConnection()
	{
	}

	void OraSqlConnection::connect(text* username, text* password, text* dbname)
	{
		sword errcode = 0;

		errcode = OCIEnvCreate(&ocienv, OCI_DEFAULT, 0, 0, 0, 0, 0, 0);
		if (errcode != 0) {
			printf("OCIEnvCreate failed with errcode = %d.\n", errcode);
			OCITerminate(OCI_DEFAULT);
			exit(1);
		}
		printf("env created");
		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&errhp, OCI_HTYPE_ERROR, (size_t)0, (dvoid**)0);
		printf("alloc 1");

		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&srvhp, OCI_HTYPE_SERVER, (size_t)0, (dvoid**)0);

		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&svchp, OCI_HTYPE_SVCCTX, (size_t)0, (dvoid**)0);

		printf("alloc 3\n");

		OCIServerAttach(srvhp, errhp, (text*)dbname, (sb4)strlen((char*)dbname), 0);

		printf("OCIServerAttach");

		
		//put context
		OCIAttrSet((dvoid*)svchp, OCI_HTYPE_SVCCTX, (dvoid*)srvhp, (ub4)0, OCI_ATTR_SERVER, (OCIError*)errhp);

		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&ocisession, (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid**)0);

		OCIAttrSet((dvoid*)ocisession, (ub4)OCI_HTYPE_SESSION,
			(dvoid*)username, (ub4)strlen((char*)username),
			(ub4)OCI_ATTR_USERNAME, errhp);

		//parola
		OCIAttrSet((dvoid*)ocisession, (ub4)OCI_HTYPE_SESSION, (dvoid*)password, (ub4)strlen((char*)password), (ub4)OCI_ATTR_PASSWORD, errhp);
	
		OCISessionBegin(svchp, errhp, ocisession, OCI_CRED_RDBMS,
			(ub4)OCI_DEFAULT);

		OCIAttrSet((dvoid*)svchp, (ub4)OCI_HTYPE_SVCCTX,
			(dvoid*)ocisession, (ub4)0,
			(ub4)OCI_ATTR_SESSION, errhp);
	}

	//https://cpp.hotexamples.com/examples/-/-/OCIDefineByPos/cpp-ocidefinebypos-function-examples.html
	void OraSqlConnection::execute(text* query)
	{
		OCIStmt* statement = NULL;
		


		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&statement, OCI_HTYPE_STMT, (size_t)0, (dvoid**)0);


		OCIStmtPrepare(statement, errhp, query, (ub4)strlen((char*)query), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);

		
		OCIDefine* defn1p = (OCIDefine*)0;
		char owner[255];

		char app_value[255];
		sb2 owner_ind[255];
		ub2 owner_len[255];
		ub2 owner_code[255];

		OCIDefineByPos(statement, &defn1p, errhp, 1, (void*)owner, 31, SQLT_STR, (void*)owner_ind,
			owner_len, owner_code, OCI_DEFAULT);



		OCIStmtExecute(svchp, statement, errhp, (ub4)1, (ub4)0,
			(CONST OCISnapshot*) NULL, (OCISnapshot*)NULL, OCI_DEFAULT);

		OCIStmtFetch(statement, errhp, (ub4)1, (ub4)OCI_FETCH_NEXT, (ub4)OCI_DEFAULT);

		printf("k = %s owner_ind=%u  v = %.10s\n",
			owner, owner_code,
			"xxxxx");

		 OCIStmtRelease(statement, errhp,
			(OraText*)NULL, 0, OCI_DEFAULT);
	}

	void OraSqlConnection::close()
	{
	}

	bool OraSqlConnection::tableExists(char* name)
	{
		OCIStmt* statement = NULL;
		OCIError* statementErr = NULL;

		OCIHandleAlloc((dvoid*)ocienv, (dvoid**)&statement, OCI_HTYPE_STMT, (size_t)0, (dvoid**)0);
		text* query = (text*)"SELECT count(*) FROM APPLICATION_PROPERTIES";

		sword res = OCIStmtPrepare(statement, errhp, query, (ub4)strlen((char*)query), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);

		OCIDefine* defn2p = (OCIDefine*)0;

		int count[1];

		OCIDefineByPos(statement, &defn2p, errhp, 1, (void*)count, 1, SQLT_STR, (void*)0,
			0, 0, OCI_DEFAULT);

		res = OCIStmtExecute(svchp, statement, errhp, (ub4)1, (ub4)0,
			(CONST OCISnapshot*) NULL, (OCISnapshot*)NULL, OCI_DEFAULT);

		if (errhp) {
			checkerr(errhp, res);
		}

		OCIStmtRelease(statement, errhp, (OraText*)NULL, 0, OCI_DEFAULT);
		return false;
	}

	OraSqlConnection::~OraSqlConnection()
	{
		close();
	}


	/* ----------------------------------------------------------------- */
/*  Free the specified handles                                       */
/* ----------------------------------------------------------------- */
	void free_handles (
		OCIServer* srvhp,
	OCISvcCtx* svchp,
	OCIServer* srvhpm,
	OCISession* authp,
	OCIStmt* stmthp,
	OCIStmt* stmthp1,
	OCIStmt* stmthp2,
		OCIError* errhp,
		OCIEnv* envhp
	)
	{
		if (srvhp)
			(void)OCIHandleFree((dvoid*)srvhp, (ub4)OCI_HTYPE_SERVER);
		if (svchp)
			(void)OCIHandleFree((dvoid*)svchp, (ub4)OCI_HTYPE_SVCCTX);
		if (errhp)
			(void)OCIHandleFree((dvoid*)errhp, (ub4)OCI_HTYPE_ERROR);
		if (authp)
			(void)OCIHandleFree((dvoid*)authp, (ub4)OCI_HTYPE_SESSION);
		if (stmthp)
			(void)OCIHandleFree((dvoid*)stmthp, (ub4)OCI_HTYPE_STMT);
		if (stmthp1)
			(void)OCIHandleFree((dvoid*)stmthp1, (ub4)OCI_HTYPE_STMT);
		if (stmthp2)
			(void)OCIHandleFree((dvoid*)stmthp2, (ub4)OCI_HTYPE_STMT);

		if (envhp)
			(void)OCIHandleFree((dvoid*)envhp, (ub4)OCI_HTYPE_ENV);

		return;
	}


	




}