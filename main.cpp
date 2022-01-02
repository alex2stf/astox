
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <occi.h>
#include <cliora.h>

static text* username = (text*)"omoney_uat";
static text* password = (text*)"Oracle_123";

struct address
{
    OCIString* state;
    OCIString* zip;
};

using namespace astox;

int main()
{

    text* seldept = (text*)"SELECT * FROM APPLICATION_PROPERTIES";

    text* dbname = (text*)"(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=172.18.56.153)(PORT=1759)))(CONNECT_DATA=(SERVICE_NAME=omoney)))";

    OraSqlConnection orasqlConnection;
    orasqlConnection.connect(username, password, dbname);
    //orasqlConnection.execute(seldept);
    
    if (!orasqlConnection.tableExists((char *)"test_table")) {
        printf("table test does not exists");
    }

    return 0;

   

    OCIEnv* envhp = NULL;
    OCIError* errhp = NULL;
    OCIServer* srvhp = NULL;
    OCISvcCtx* svchp = NULL;
    OCISession* authp = (OCISession*)0;
    OCIStmt* stmthp1 = NULL;
   // OCIDefine* defnp = (OCIDefine*)0;
    sword    empno;




    sword errcode = 0;

    errcode = OCIEnvCreate(&envhp, OCI_DEFAULT, 0, 0, 0, 0, 0, 0);


    if (errcode != 0) {
        printf("OCIEnvCreate failed with errcode = %d.\n", errcode);
        OCITerminate(OCI_DEFAULT);
        exit(1);
    }
    printf("env created");
    (void)OCIHandleAlloc((dvoid*)envhp, (dvoid**)&errhp, OCI_HTYPE_ERROR,
        (size_t)0, (dvoid**)0);
    printf("alloc 1");



    (void)OCIHandleAlloc((dvoid*)envhp, (dvoid**)&srvhp, OCI_HTYPE_SERVER,
        (size_t)0, (dvoid**)0);

    printf("alloc 2");

    OCIHandleAlloc((dvoid*)envhp, (dvoid**)&svchp, OCI_HTYPE_SVCCTX,  (size_t)0, (dvoid**)0);

    printf("alloc 3\n");

    OCIServerAttach(srvhp, errhp, (text*)dbname,
        (sb4)strlen((char*)dbname), 0);

    printf("OCIServerAttach");

    (void)OCIAttrSet((dvoid*)svchp, OCI_HTYPE_SVCCTX, (dvoid*)srvhp,
        (ub4)0, OCI_ATTR_SERVER, (OCIError*)errhp);

    (void)OCIHandleAlloc((dvoid*)envhp, (dvoid**)&authp,
        (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid**)0);

    (void)OCIAttrSet((dvoid*)authp, (ub4)OCI_HTYPE_SESSION,
        (dvoid*)username, (ub4)strlen((char*)username),
        (ub4)OCI_ATTR_USERNAME, errhp);

    (void)OCIAttrSet((dvoid*)authp, (ub4)OCI_HTYPE_SESSION,
        (dvoid*)password, (ub4)strlen((char*)password),
        (ub4)OCI_ATTR_PASSWORD, errhp);

    
    OCISessionBegin(svchp, errhp, authp, OCI_CRED_RDBMS,
        (ub4)OCI_DEFAULT);

    OCIAttrSet((dvoid*)svchp, (ub4)OCI_HTYPE_SVCCTX,
        (dvoid*)authp, (ub4)0,
        (ub4)OCI_ATTR_SESSION, errhp);

        
    OCIHandleAlloc((dvoid*)envhp, (dvoid**)&stmthp1, OCI_HTYPE_STMT, (size_t)0, (dvoid**)0);

    OCIStmtPrepare(stmthp1, errhp, seldept,
        (ub4)strlen((char*)seldept),
        (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);


    OCIRef* type_ref = (OCIRef*)0;

    address* addr = (address*)NULL;
    OCIType* addr_tdo = (OCIType*)0;
    OCIDefine* defn1p = (OCIDefine*)0, * defn2p = (OCIDefine*)0;
    


    /*
    OCIObjectPin(envhp, errhp, type_ref, (OCIComplexObject*)0,
        OCI_PIN_ANY, OCI_DURATION_SESSION, OCI_LOCK_NONE,
        (dvoid**)&addr_tdo);

    OCIDefineObject(defn2p, errhp, addr_tdo, (dvoid**)&addr,
        (ub4*)0, (dvoid**)0, (ub4*)0);

    OCIObjectPin(envhp, errhp, type_ref, (OCIComplexObject*)0,
        OCI_PIN_ANY, OCI_DURATION_SESSION, OCI_LOCK_NONE,
        (dvoid**)&addr);

   */



    char owner[100][31];
    sb2 owner_ind[100];
    ub2 owner_len[100];
    ub2 owner_code[100];

    OCIDefineByPos(stmthp1, &defn1p, errhp, 1, (void*)owner, 31, SQLT_STR, (void*)owner_ind,
        owner_len, owner_code, OCI_DEFAULT);



   OCIStmtExecute(svchp, stmthp1, errhp, (ub4)1, (ub4)0,
       (CONST OCISnapshot*) NULL, (OCISnapshot*)NULL, OCI_DEFAULT);

   OCIStmtFetch(stmthp1, errhp, (ub4)1, (ub4)OCI_FETCH_NEXT, (ub4)OCI_DEFAULT);


   printf("k = %s v = %.10s\n",
       owner,
       "xxxxx");

      

   /*
    (status = OCIBindByName(stmthp, &bnd5p, errhp, (text *) ":EMPNO",
             -1, (dvoid *) &empno,
             (sword) sizeof(empno), SQLT_INT, (dvoid *) 0,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)))
             */



    printf("Hello world!\n");

    return 0;
}
