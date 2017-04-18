/*
 * RootApi.h
 *
 *  Created on: Oct 10, 2014
 *      Author: alexandru
 */

#ifndef EXTAPI_H_
    #define EXTAPI_H_
    #include "Macros.h"
    #include "FileSystem.h"
    #include "Socket.h"
    #include "astox.h"



    #define ExtApiNoSupport(data, enbleFlag){ printf("-method not supported by extapi_: (%s)\n	compile code using (%s) flag\n", data, enbleFlag);}

    #ifdef ASTX_ENABLE_MYSQL_CPP
        #include "mysql_connection.h"
        #include <cppconn/driver.h>
        #include <cppconn/exception.h>
        #include <cppconn/resultset.h>
        #include <cppconn/statement.h>
        #include <cppconn/sqlstring.h>
        #define  STAT_MYSQL_CONN "mysql c++ connector"
Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database);

    #elif ASTX_ENABLE_MYSQL_CC
        #include <mysql.h>
        #include <mysql/mysql.h>
        #define  STAT_MYSQL_CONN "mysql c connector"
Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database);
    #else
        #define  STAT_MYSQL_CONN "no connector"
astox::Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database);
    #endif


    #ifdef ASTX_ENABLE_LIBSSH2
        #include <libssh2.h>
        #include <libssh2_sftp.h>
        #ifdef ASTOX_OS_WINDOWS
void __wsaStartup();
        #endif
    #endif

struct ssh2_data {
    #ifdef ASTX_ENABLE_LIBSSH2
    LIBSSH2_SESSION *session;
    #else
    int * session;
    #endif
    int sock;
    const char *username;
    const char *password;
    const char * servername;
    const char * privateKey;
    const char * publickey;
};
ssh2_data ssh2_makeData(const char *username, const char *password, const char * servername,
        const char * privateKey = "/home/username/.ssh/id_rsa.pub",
        const char * publickey = "/home/username/.ssh/id_rsa");
int ssh2_scpConnect(ssh2_data * data);
int ssh2_scpClose(ssh2_data * data);
int ssh2_scpWrite(ssh2_data * data, const char *loclfile, const char *scppath);


    #ifdef ASTX_ENABLE_CURL
        #include <curl/curl.h>
void curl_doGet(char * url);
    #else
void curl_doGet(char * url);
    #endif



void extapi_describe();





    #ifdef ASTX_ENABLE_SASS
        #include <libsass/sass2scss.h>
        #include <libsass/sass_context.h>

        #ifndef SASSC_VERSION
            #define SASSC_VERSION "[NA]"
        #endif

        #define BUFSIZE 512
        #ifdef _WIN32
            #define PATH_SEP ';'
        #else
            #define PATH_SEP ':'
        #endif

int extapi_sass_output(int error_status, const char* error_message, const char* output_string, const char* outfile);
void extapi_sass_compileFile(const char* input_path, const char* outfile);
    #else
void extapi_sass_compileFile(const char* input_path, const char* outfile);
int extapi_sass_output(int error_status, const char* error_message, const char* output_string, const char* outfile);
    #endif

    #ifdef ASTX_ENABLE_QT_APP
        #include <QtWidgets/QApplication>
        #include <QtWidgets/QMessageBox>
        #include <QWindow>
        #include <QDebug>

void extapi_qt_windowed_app(QApplication app);
    #endif



#endif /* ROOTAPI_H_ */
