/*
 * RootApi.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: alexandru
 */

#include "ExtApi.h"


#ifdef ASTX_ENABLE_QT_APP
#endif

#ifdef ASTX_ENABLE_SASS

int extapi_sass_output(int error_status, const char* error_message, const char* output_string, const char* outfile) {
    if (error_status) {
        if (error_message) {
            fprintf(stderr, "%s", error_message);
        } else {
            fprintf(stderr, "An error occured; no error message available.\n");
        }
        return 1;
    } else if (output_string) {
        if (outfile) {
            FILE* fp = fopen(outfile, "w");
            if (!fp) {
                perror("Error opening output file");
                return 1;
            }
            if (fprintf(fp, "%s", output_string) < 0) {
                perror("Error writing to output file");
                fclose(fp);
                return 1;
            }
            fclose(fp);
        } else {
            printf("%s", output_string);
        }
        return 0;
    } else {
        fprintf(stderr, "Unknown internal error.\n");
        return 2;
    }
}

void extapi_sass_compileFile(const char* input_path, const char* outfile) {
    int ret;
    struct Sass_Options* options = sass_make_options();
    struct Sass_File_Context* ctx = sass_make_file_context(input_path);
    struct Sass_Context* ctx_out = sass_file_context_get_context(ctx);
    if (outfile) sass_option_set_output_path(options, outfile);
    sass_option_set_input_path(options, input_path);
    sass_file_context_set_options(ctx, options);

    sass_compile_file_context(ctx);

    ret = extapi_sass_output(
            sass_context_get_error_status(ctx_out),
            sass_context_get_error_message(ctx_out),
            sass_context_get_output_string(ctx_out),
            outfile
            );

    if (ret == 0 && sass_option_get_source_map_file(options)) {
        ret = extapi_sass_output(
                sass_context_get_error_status(ctx_out),
                sass_context_get_error_message(ctx_out),
                sass_context_get_source_map_string(ctx_out),
                sass_option_get_source_map_file(options)
                );
    }
    sass_delete_file_context(ctx);
};
#else

void extapi_sass_compileFile(const char* input_path, const char* outfile) {
    ExtApiNoSupport("extapi_sass_compileFile", "ASTX_ENABLE_SASS");
};

int extapi_sass_output(int error_status, const char* error_message, const char* output_string, const char* outfile) {
    ExtApiNoSupport("extapi_sass_output", "ASTX_ENABLE_SASS");
    return 0;
};
#endif

void extapi_describe() {
    printf("compilation date : %s\n", __DATE__);
    printf(" mysql connector : %s\n", STAT_MYSQL_CONN);
#ifdef ASTX_ENABLE_LIBSSH2
    printf(" libssh2 support : %s\n", STAT_MYSQL_CONN);
#else
    printf(" libssh2 NO support : %s\n", STAT_MYSQL_CONN);
#endif
}



#ifdef ASTX_ENABLE_MYSQL_CPP

Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database) {
    Object * response = new Object();

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();

        con = driver->connect(hostName, userName, password);
        /* Connect to the MySQL test database */
        con->setSchema(database);

        stmt = con->createStatement();
        sql::SQLString schemaQuery = "SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='";
        schemaQuery.append(database);
        schemaQuery.append("';");
        res = stmt->executeQuery(schemaQuery);

        std::vector<sql::SQLString> _tableNames;

        while (res->next()) {
            sql::SQLString tableName = res->getString("TABLE_NAME");
            _tableNames.push_back(tableName);
            response->addKey(tableName, new Object());
        }

        for (unsigned int i = 0; i < _tableNames.size(); i++) {
            sql::SQLString _currentStatement = "SELECT * FROM ";
            _currentStatement.append(_tableNames.at(i));
            _currentStatement.append(";");

            res = stmt->executeQuery(_currentStatement);

            int numColumns = res->getMetaData()->getColumnCount();

            std::vector<sql::SQLString> _columnNames;
            for (int j = 0; j < numColumns; j++) {
                sql::SQLString _ctn = res->getMetaData()->getColumnName(j + 1);
                _columnNames.push_back(_ctn);
                Object::cast(response->getKeyLoose(_tableNames.at(i)))->addKey(_ctn, new Array());
            }

            while (res->next()) {
                for (unsigned int k = 0; k < _columnNames.size(); k++) {
                    Array::cast(Object::cast(response->getKeyLoose(_tableNames.at(i)))->getKeyLoose(_columnNames.at(k)))->push(new String(res->getString(_columnNames.at(k))));
                }
            }
        }

        delete res;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILENAME__;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    return response;
}

#elif ASTX_ENABLE_MYSQL_CC

Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database) {

    Object * response = new Object();
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
    }

    if (mysql_real_connect(con, "localhost", "root", "smartx", NULL, 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
    }

    if (mysql_query(con, "SELECT * FROM `test_db`.`test_table1`;")) {

    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {

    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;


    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }


    mysql_free_result(result);
    mysql_close(con);

    return response;
}
#else

astox::Object * mysql_DbToObject(const char * hostName, const char * userName, const char * password, const char * database) {
    printf("method not suppoerted");
    return astox::Object::npos;
}
#endif





#ifdef ASTX_ENABLE_LIBSSH2
using namespace std;
using namespace astox;
    #ifdef ASTOX_OS_WINDOWS

    void __wsaStartup() {
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2, 0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
};
    #endif

ssh2_data ssh2_makeData(const char *username, const char *password, const char * servername,
        const char * privateKey, const char * publickey) {
    ssh2_data data;
    data.username = username;
    data.password = password;
    data.servername = servername;
    data.privateKey = privateKey;
    data.publickey = publickey;
    return data;
};

int ssh2_scpConnect(ssh2_data * data) {

    #ifdef ASTOX_OS_WINDOWS
    __wsaStartup();
    #endif

    unsigned long hostaddr;
    int i, auth_pw = 1;
    struct sockaddr_in sin;

    //	LIBSSH2_SESSION *session = NULL;

    int rc;

    size_t nread;
    char *ptr;
    struct stat fileinfo;


    hostaddr = inet_addr(data->servername);
    rc = libssh2_init(0);

    if (rc != 0) {
        fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    data->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == data->sock) {
        fprintf(stderr, "failed to create socket!\n");
        return -1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(data->sock, (struct sockaddr*) (&sin),
            sizeof (struct sockaddr_in)) != 0) {
        fprintf(stderr, "failed to connect!\n");
        return -1;
    }

    data->session = libssh2_session_init();
    if (!data->session) {
        return -1;
    }

    rc = libssh2_session_handshake(data->session, data->sock);
    if (rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return -1;
    }

    #ifdef ASTX_DBG_SSH2
    const char *fingerprint = libssh2_hostkey_hash(data->session, LIBSSH2_HOSTKEY_HASH_SHA1);
    astox_debug_start_with(__LINE__, __FILENAME__, "ssh2_scpConnect:: ");
    for (i = 0; i < 20; i++) {
        astox_debug_printf("%02X", (unsigned char) fingerprint[i]);
    }
    astox_debug_end();
    #endif

    if (auth_pw) {
        /* We could authenticate via password */
        if (libssh2_userauth_password(data->session, data->username, data->password)) {
            fprintf(stderr, "Authentication by password failed.\n");
            return ssh2_scpClose(data);
        }
    } else {
        /* Or by public key */
        if (libssh2_userauth_publickey_fromfile(data->session, data->username,
                data->publickey,
                data->privateKey,
                data->password)) {
            fprintf(stderr, "\tAuthentication by public key failed\n");
            return ssh2_scpClose(data);

        }
    }
    return 0;
};

int ssh2_scpClose(ssh2_data * data) {
    if (data->session) {
        libssh2_session_disconnect(data->session, "Normal Shutdown, Thank you for playing");
        libssh2_session_free(data->session);
    }
    #ifdef ASTOX_OS_WINDOWS
    closesocket(data->sock);
    #else
    close(data->sock);
    #endif

    #ifdef	ASTX_DBG_SSH2
    astox_debug(__LINE__, __FILENAME__, "ssh2_scpClose");
    #endif
    libssh2_exit();
    return 0;
};

int ssh2_scpWrite(ssh2_data * data, const char *loclfile, const char *scppath) {


    LIBSSH2_CHANNEL *channel;
    FILE *local;
    int rc;
    size_t nread;
    char *ptr;
    struct stat fileinfo;

    local = fopen(loclfile, "rb");
    if (!local) {
        fprintf(stderr, "Can't open local file %s\n", loclfile);
        return -1;
    }
    stat(loclfile, &fileinfo);
    /* Send a file via scp. The mode parameter must only have permissions! */
    channel = libssh2_scp_send(data->session, scppath, fileinfo.st_mode & 0777, (unsigned long) fileinfo.st_size);

    if (!channel) {
        char *errmsg;
        int errlen;
        int err = libssh2_session_last_error(data->session, &errmsg, &errlen, 0);
        fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
        return ssh2_scpClose(data);
    }
    #ifdef	ASTX_DBG_SSH2
    astox_debug(__LINE__, __FILENAME__, "ssh2_scpWrite: send file %s", loclfile);
    #endif
    char mem[1024];
    do {
        nread = fread(mem, 1, sizeof (mem), local);
        if (nread <= 0) {
            /* end of file */
            break;
        }
        ptr = mem;

        do {
            /* write the same data over and over, until error or completion */
            rc = libssh2_channel_write(channel, ptr, nread);

            if (rc < 0) {
                fprintf(stderr, "ERROR %d\n", rc);
                break;
            } else {
                /* rc indicates how many bytes were written this time */
                ptr += rc;
                nread -= rc;
            }
        } while (nread);

    } while (1);

    #ifdef	ASTX_DBG_SSH2
    astox_debug(__LINE__, __FILENAME__, "ssh2_scpWrite: Sending EOF at %s", scppath);
    #endif
    libssh2_channel_send_eof(channel);
    #ifdef	ASTX_DBG_SSH2
    astox_debug(__LINE__, __FILENAME__, "ssh2_scpWrite: Waiting for EOF");
    #endif
    libssh2_channel_wait_eof(channel);
    #ifdef	ASTX_DBG_SSH2
    astox_debug(__LINE__, __FILENAME__, "ssh2_scpWrite: Waiting for channel to close");
    #endif
    libssh2_channel_wait_closed(channel);
    libssh2_channel_free(channel);
    channel = NULL;
    if (local) {
        fclose(local);
    }
    return 0;
}
#else

ssh2_data ssh2_makeData(const char *username, const char *password, const char * servername,
        const char * privateKey, const char * publickey) {
    ExtApiNoSupport("ssh2_makeData", "ASTX_ENABLE_LIBSSH2");
    ssh2_data s;
    int n = 1;
    s.session = &n;
    s.sock = 1;
    s.username = s.password = s.servername = s.privateKey = s.publickey = "";
    return s;
};

int ssh2_scpConnect(ssh2_data * data) {
    ExtApiNoSupport("ssh2_scpConnect", "ASTX_ENABLE_LIBSSH2");
    return 0;
};

int ssh2_scpClose(ssh2_data * data) {
    ExtApiNoSupport("ssh2_scpClose", "ASTX_ENABLE_LIBSSH2");
    return 0;
};

int ssh2_scpWrite(ssh2_data * data, const char *loclfile, const char *scppath) {
    ExtApiNoSupport("ssh2_scpWrite", "ASTX_ENABLE_LIBSSH2");
    return 0;
};

#endif


#ifdef ASTX_ENABLE_CURL

void curl_doGet(char * url) {
    printf("DO THE DEW");
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Data-Encrypted: false");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        printf("curl_easy_init failed");
    }
};
#else

void curl_doGet() {
    ExtApiNoSupport("curl_doGet", "ASTX_ENABLE_CURL");
};
#endif



