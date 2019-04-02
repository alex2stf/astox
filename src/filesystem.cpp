#include "astox.h"



using namespace std;


namespace astox {
    //**** Path contains only string fixes:

    Path::Path(const char * path) {
#ifdef ASTX_DBG_FS
        astox_debug(__LINE__, __FILENAME__, "Path(const char * %s)", path);
#endif
        processSeparatorFix(path);
        processTokens();
        processFixedPath();
    }

    Path::Path(std::string path) {
#ifdef ASTX_DBG_FS
        astox_debug(__LINE__, __FILENAME__, "Path(std::string %s)", path.c_str());
#endif
        processSeparatorFix(path);
        processTokens();
        processFixedPath();
    }

    Path::Path(std::vector<std::string> _ptokens) {
#ifdef ASTX_DBG_FS
        astox_debug(__LINE__, __FILENAME__, "Path(std::vector<std::string>)");
#endif
        _tokens = _ptokens;
        for (unsigned int i = 0; i < _tokens.size(); i++) {
            _fixedPath.append(_tokens.at(i));
            if (i < _tokens.size() - 1) {
                _fixedPath += (ASTOX_OS_SEPARATOR);
            }
        }

        cout << "std::vector<std::string> Path(" << _fixedPath << ")" << endl;
        processFixedPath();
    };

    Path::Path(const char * dirPath, const char * fileName) {
        std::string data = dirPath;
        if (!data.empty() && data.at(data.size() - 1) != ASTOX_OS_SEPARATOR) {
            data += ASTOX_OS_SEPARATOR;
        }

        std::string fName = fileName;
        if (!fName.empty() && fName.at(0) == ASTOX_OS_SEPARATOR) {
            fName.erase(fName.begin() + 1);
        }
        data.append(fName);
        processSeparatorFix(data);
        processTokens();
        processFixedPath();
    }

	std::string Path::resolve(std::string parentDirectory) {
		using namespace astox::stringAlg;
	
		Path parentPath(parentDirectory);
		std::vector<std::string> parentTokens = parentPath.getTokens();
		std::vector<std::string> tokens = getTokens();
		int parentCount = (int)parentTokens.size();
		int count = 0;

		int i = 0;
		
		for (i = 0; i < tokens.size(); i++) {
			if (tokens.at(i) == "..") {
				parentCount--;
				count++;
			}
		}

		std::string fres;
		for (i = 0; i < parentCount; i++) {
			fres.append(parentTokens.at(i));
			fres += ASTOX_OS_SEPARATOR;
		}

		for (i = count; i < tokens.size() - 1; i++) {
			fres.append(tokens.at(i));
			fres += ASTOX_OS_SEPARATOR;
		}
		fres.append(tokens.at(i));
		return fres;
	};

	std::string Path::separatorFix(std::string data) {
		std::replace(data.begin(), data.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
		return data;
	};

    void Path::processSeparatorFix(std::string data) {
        _fixedPath = separatorFix(data);
		#ifdef ASTOX_OS_WINDOWS
			//delete first separator for windows OS
			if (_fixedPath.at(0) == ASTOX_OS_SEPARATOR) {
				_fixedPath.erase(_fixedPath.begin());
			}
		#endif
    };

    void Path::processTokens() {
        _tokens = stringAlg::splitByChar(_fixedPath, ASTOX_OS_SEPARATOR);
        for (unsigned int i = 0; i < _tokens.size(); i++) {
            if (_tokens.at(i).empty()) {
                _tokens.erase(_tokens.begin() + i);
            }
        }
    };

    void Path::processFixedPath() {
        if (_tokens.size() > 1) {
            _fileName = _tokens.at(_tokens.size() - 1);
        } else {
            _fileName = _fixedPath;
        }
        if (_fileName.size() > 1 && _fileName.at(0) == '.') {
            _isDot = true;
        }

        if (_fileName.find('.') != string::npos) {
            size_t pos = _fileName.find_last_of('.');
            _extension = _fileName.substr(pos + 1, _fileName.size());
            _fileNameNoExt = _fileName.substr(0, pos);
        } else {
            _fileNameNoExt = _fileName;
        }

#ifdef ASTOX_OS_WINDOWS
        if (_tokens.size() > 1) {
            string firstToken = _tokens.at(0);
            _isDriveFormat = (firstToken.at(firstToken.size() - 1) == ':');
        } else {
            _isDriveFormat = false;
        }
#else
        _isDriveFormat = false;
#endif
    };

    std::string Path::getReadableDirPath() {
        std::string result = _fixedPath;
        if (!_fixedPath.empty() && _fixedPath.at(_fixedPath.size() - 1) != ASTOX_OS_SEPARATOR) {
            result += ASTOX_OS_SEPARATOR;
        }

#ifdef ASTOX_OS_WINDOWS
        if (!_fixedPath.empty() && _fixedPath.at(_fixedPath.size() - 1) != '*') {
            result += '*';
        }
#endif
        return result;
    };

	/*
    std::string Path::getAbsolutePath() {
        std::string response = FileSystem::getWorkingDirectory();
		#ifdef ASTOX_OS_WINDOWS
			if (!_isDriveFormat) {
				response += ASTOX_OS_SEPARATOR;
				response.append(_fixedPath);
				return response;
			}
			return _fixedPath;
		#endif
        return response;
    };
	*/

    bool Path::isDriveFormat() {
        return _isDriveFormat;
    }

    std::string Path::toString() {
        return _fixedPath;
    }

    std::vector<std::string> Path::getTokens() {
        return _tokens;
    }

    bool Path::isDot() {
        return _isDot;
    }

    std::string Path::getExtension() {
        return _extension;
    }

    std::string Path::getFileName() {
        return _fileName;
    }

    std::string Path::getFileNameNoExt() {
        return _fileNameNoExt;
    }

    std::string Path::getFixedPath() {
        return _fixedPath;
    };

    void Path::describe() {
        astox_debug(__LINE__, __FILENAME__, "Path::getFileName ====== %s", getFileName().c_str());
        astox_debug(__LINE__, __FILENAME__, "Path::getFileNameNoExt = %s", getFileNameNoExt().c_str());
        astox_debug(__LINE__, __FILENAME__, "Path::getExtension ===== %s", getExtension().c_str());
        astox_debug(__LINE__, __FILENAME__, "Path::toString ========= %s", toString().c_str());
//        astox_debug(__LINE__, __FILENAME__, "Path::getAbsolutePath == %s", getAbsolutePath().c_str());
        astox_debug(__LINE__, __FILENAME__, "Path::getReadableDirPath == %s", getReadableDirPath().c_str());
        astox_debug(__LINE__, __FILENAME__, "Path::isDot ============ %s", isDot() ? "true" : "false");
        astox_debug(__LINE__, __FILENAME__, "Path::isDriveFormat ==== %s", isDriveFormat() ? "true" : "false");
        for (unsigned int i = 0; i < getTokens().size(); i++) {
            astox_debug(__LINE__, __FILENAME__, "Path::getTokens(%i) ===== %s", i, getTokens().at(i).c_str());
        }
    };

    /******************** END PATH ************************/


    FileInfo::FileInfo(std::string strPath) {
#ifdef ASTX_ENABLE_BOOST
        namespace boostfs = ::boost::filesystem;
        _fileStatus = FILE_STATUS_UNKNOWN;
        _path = new Path(strPath);
        digestPath();
        boostfs::path p(strPath);
        _exist = boostfs::exists(p);
        _isRegularFile = boostfs::is_regular_file(p);
        _isDir = boostfs::is_directory(p);
        //describe();
#else
        _fileStatus = FILE_STATUS_UNKNOWN;
        _path = new Path(strPath);
        digestPath();
#endif
    }

    FileInfo::FileInfo(Path * p) {
        _fileStatus = FILE_STATUS_UNKNOWN;
        _path = p;
        digestPath();
    }


#ifdef ASTX_ENABLE_BOOST

FileInfo::FileInfo(const boost::filesystem::path& boostPath) {
        std::string strname = boostPath.string();
        _path = new Path(strname);
        digestPath();

        namespace boostfs = ::boost::filesystem;
        _exist = boostfs::exists(boostPath);
        _isRegularFile = boostfs::is_regular_file(boostPath);
        _isDir = boostfs::is_directory(boostPath);
    };
#endif

    void FileInfo::digestPath() {
#ifdef ASTOX_OS_WINDOWS //WINDOWS PROCESSOR
        _isDir = false;
        _exist = true;

    #ifdef ASTOX_COMPILER_UNICODE
        std::wstring wsPath = astox::stringAlg::stdString2wstring(_path->toString());
        processWinStat(wsPath);
    #else
        std::string wsPath = _path->str();
        processWinStat(wsPath);
    #endif

#else // UNIX PROCESSOR
        struct stat buf;
        _exist = (stat(_path->toString().c_str(), &buf) == 0);
        processUnixStat(&buf);
#endif

#ifdef ASTX_DBG_FS
        describe();
#endif
    };

#ifndef ASTOX_OS_WINDOWS

FileInfo::FileInfo(const struct stat *info, const struct FTW *pathinfo, const int typeflag, const char *filepath) {
        _fileStatus = FILE_STATUS_UNKNOWN;
        _path = new Path(filepath);
        _exist = true;
        processUnixStat((struct stat *) info);
    #ifdef ASTX_DBG_FS
        describe();
    #endif
    };
#endif



#ifdef ASTOX_OS_WINDOWS

void FileInfo::processWinStat(
    #ifdef ASTOX_COMPILER_UNICODE
            std::wstring wsPath
    #else
            std::string wsPath
    #endif
            ) {

        SYSTEMTIME stCreate, stLastAccess, stLastWrite;
        const DWORD attributes = GetFileAttributes(wsPath.c_str());
        _isDir = (FILE_ATTRIBUTE_DIRECTORY & attributes);

        /*if(!_isDir) {
                hFile = CreateFile(wsPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if(hFile == INVALID_HANDLE_VALUE) {
                        printf("Could not open file, error %ul %s\n", GetLastError(), _path->toString().c_str());
                        _exist = false;
                }

                FILETIME lpCreationTime;
                FILETIME lpLastAccessTime;
                FILETIME lpLastWriteTime;

                if(!GetFileTime(hFile, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime)){
                        printf("Something wrong in GetFileTime!\n");
                        _lastAccessDate = _lastModifiedDate = _creationDate = _lastStatusChangedDate = new Date();
                        return;
                }

                else {
                        FileTimeToSystemTime(&lpCreationTime, &stCreate);
                        FileTimeToSystemTime(&lpLastAccessTime, &stLastAccess);
                        FileTimeToSystemTime(&lpLastWriteTime, &stLastWrite);
                }

        }
        else {*/
        WIN32_FILE_ATTRIBUTE_DATA fileAttr;
        if (!GetFileAttributesEx(wsPath.c_str(), GetFileExInfoStandard, &fileAttr)) {
            _exist = false;
			return;
        }

        FileTimeToSystemTime(&fileAttr.ftCreationTime, &stCreate);
        FileTimeToSystemTime(&fileAttr.ftLastAccessTime, &stLastAccess);
        FileTimeToSystemTime(&fileAttr.ftLastWriteTime, &stLastWrite);
        _exist = (attributes != INVALID_FILE_ATTRIBUTES);
        DWORD lpFileSizeHigh = fileAttr.nFileSizeHigh;

        std::tm tCreate, tLastAccess, tLastWrite;
        Date::SystemTime2StructTM(&stCreate, &tCreate);
        Date::SystemTime2StructTM(&stLastAccess, &tLastAccess);
        Date::SystemTime2StructTM(&stLastWrite, &tLastWrite);

        _lastModifiedDate = new Date(&tLastWrite);
        _lastAccessDate = new Date(&tLastAccess);
        _creationDate = new Date(&tCreate);
        _lastStatusChangedDate = _lastModifiedDate;
        _lastSavedTime = (int) stLastAccess.wSecond;


        if (!_isDir && _exist) {
            HANDLE hFile;
            hFile = CreateFile(wsPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE) {
                printf("Could not open file, error %ul %s\n", GetLastError(), _path->toString().c_str());
                _exist = false;
            }
            DWORD fType = GetFileType(hFile);
            switch (fType) {
                case FILE_TYPE_CHAR:
                    break;
                case FILE_TYPE_DISK:
                    break;
                case FILE_TYPE_PIPE:
                    _fileStatus = FILE_STATUS_PIPE;
                    break;
                case FILE_TYPE_REMOTE:
                    break;
                case FILE_TYPE_UNKNOWN:
                    break;
            };
        }




        describe();


    }


#else //UNIX PROCESSOR

    void FileInfo::processUnixStat(struct stat *info) {
        _length = (double) info->st_size;
        _lastModifiedDate = new Date(info->st_mtime);
        _lastAccessDate = new Date(info->st_atime);
    #ifdef ASTOX_OS_MAC
        std::tm creationd;
        localtime_r(&(info->st_birthtime), &creationd);
    #else
        _creationDate = new Date(info->st_ctime);
       // localtime_r(&(info->st_ctime), &creationd);
    #endif
        _lastStatusChangedDate = new Date(info->st_ctime);

        _lastSavedTime = info->st_mtime;





        switch (info->st_mode & S_IFMT) {
            case S_IFBLK:
                _fileStatus = FILE_STATUS_BLOCK_DEVICE;
                break;
            case S_IFCHR:
                _fileStatus = FILE_STATUS_CHARACTER_DEVICE;
                break;
            case S_IFDIR:
                _isDir = true;
                _fileStatus = FILE_STATUS_DIRECTORY;
                break;
            case S_IFIFO:
                _fileStatus = FILE_STATUS_PIPE;
                break;
            case S_IFLNK:
                _fileStatus = FILE_STATUS_SIMLINK;
                break;
            case S_IFREG:
                _isRegularFile = true;
                _fileStatus = FILE_STATUS_REGULAR;
                break;
            case S_IFSOCK:
                _fileStatus = FILE_STATUS_SOCKET;
                break;
            default:
                _fileStatus = FILE_STATUS_UNKNOWN;
                break;
        }
        if (S_ISDIR(info->st_mode)) {
            _isDir = true;
        } else {
            _isDir = false;
        }
    };


#endif

    void FileInfo::describe() {
        _path->describe();
        if (_exist) {
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getLastSavedTime ===== %i", getLastSavedTime());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getCreationDate ====== %s", getCreationDate()->str().c_str());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getLastAccessDate ==== %s", getLastAccessDate()->str().c_str());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getLastModifiedDate == %s", getLastModifiedDate()->str().c_str());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getStatusChangedDate = %s", getStatusChangedDate()->str().c_str());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getFileStatusv ======= %s", getFileStatusv()->str().c_str());
            astox_debug(__LINE__, __FILENAME__, "FileInfo::isDirectory ========== %s", isDirectory() ? "true" : "false");
            astox_debug(__LINE__, __FILENAME__, "FileInfo::exist ================ %s", exist() ? "true" : "false");
            astox_debug(__LINE__, __FILENAME__, "FileInfo::getLength ============ %lf", getLength());
        } else {
            astox_debug(__LINE__, __FILENAME__, "FileInfo::exist ================ %s", exist() ? "true" : "false");
        }
    }

    Date * FileInfo::getCreationDate() {
        return _creationDate;
    };

    Date * FileInfo::getLastAccessDate() {
        return _lastAccessDate;
    };

    Date * FileInfo::getLastModifiedDate() {
        return _lastModifiedDate;
    };

    Date * FileInfo::getStatusChangedDate() {
        return _lastStatusChangedDate;
    };

    int FileInfo::getFileStatus() {
        return _fileStatus;
    };

    String * FileInfo::getFileStatusv() {
//        switch (_fileStatus) {
//            case FILE_STATUS_UNKNOWN:
//                return String::New("unknown");
//            case FILE_STATUS_SOCKET:
//                return String::New("socket");
//            case FILE_STATUS_SIMLINK:
//                return String::New("simlink");
//            case FILE_STATUS_REGULAR:
//                return String::New("regular");
//            case FILE_STATUS_PIPE:
//                return String::New("pipe");
//            case FILE_STATUS_DIRECTORY:
//                return String::New("directory");
//            case FILE_STATUS_CHARACTER_DEVICE:
//                return String::New("characterDevice");
//            case FILE_STATUS_BLOCK_DEVICE:
//                return String::New("blockDevice");
//        }
//        return String::New("unknown");
		return String::New("unknown", Syntax::JS);
    };

    bool FileInfo::exist() {
        return _exist;
    }

    std::vector<std::string> FileInfo::getTokens() {
        return _path->getTokens();
    }

    bool FileInfo::isDirectory() {
        return _isDir;
    }

    double FileInfo::getLength() {
        return _length;
    };

    Path * FileInfo::getPath() {
        return _path;
    }

    int FileInfo::getLastSavedTime() {
        return _lastSavedTime;
    }

    Object * FileInfo::toObject() {
        Object * data = Object::New(Syntax::JS);
//        data->addKey("path", String::New(getPath()->toString()));
//        data->addKey("filename", String::New(getPath()->getFileName()));
//        data->addKey("extension", String::New(getPath()->getExtension()));
//        data->addKey("lastModifiedTime", Number::New(getLastSavedTime()));
//        data->addKey("isDirectory", Boolean::New(_isDir));
//        data->addKey("isDot", Boolean::New(getPath()->isDot()));
//        data->addKey("fileNameNoExt", String::New(getPath()->getFileNameNoExt()));

        Array * tokens = Array::New(Syntax::JS); //TODO modify
//        std::vector<std::string> tokensstr = getPath()->getTokens();
//        for (unsigned int i = 0; i < tokensstr.size(); i++) {
////            tokens->push(String::New(tokensstr.at(i), lxc));
//        }
//        data->addKey("tokens", tokens);
        return data;
    };



#ifdef _FTW_H
    int fs_nftw_callback(const char *filepath, const struct stat *info,
            const int typeflag, struct FTW *pathinfo) {
        FileInfo * file = new FileInfo(info, pathinfo, typeflag, filepath);
        /* const char *const filename = filepath + pathinfo->base; */
        EventDispatcher::dispatch(Event::ON_FS_FTWR, file->toObject());
        delete file;
        return 0;
    }

    int fs_ftw_callback(const char *fpath, const struct stat *sb, int typeflag) {
        Path p(fpath);
        FileInfo * file = new FileInfo(&p);
        EventDispatcher::dispatch(Event::ON_FS_FTWR, file->toObject());
        delete file;
        return 0;
    };
#endif

    bool FileSystem::mkdir(std::string name) {
        std::replace(name.begin(), name.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);

        #ifdef ASTOX_OS_WINDOWS
            #ifdef ASTOX_COMPILER_UNICODE
                std::wstring wsPath = astox::stringAlg::stdString2wstring(name);
                return CreateDirectory(wsPath.c_str(), NULL);
            #else
                return CreateDirectory(name.c_str(), NULL);
            #endif
        #else
            struct stat st;
            if (stat(name.c_str(), &st) != 0) {
                if (::mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST) {
                    return true;
                }
                return false;
            }
            return false;
        #endif
        return true;
    };

    bool FileSystem::mkdirs(Path path) {
        //path.getTokens();
        return false;
    };

    std::string FileSystem::getProcessPath() {
#ifdef ASTOX_OS_MAC
        char path[1024];
        uint32_t size = sizeof (path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            return path;
        } else {
            printf("buffer too small; need size %u\n", size);
            exit(1);
        }
#elif ASTOX_OS_WINDOWS
        TCHAR szFileName[MAX_PATH];
        GetModuleFileName(NULL, szFileName, MAX_PATH);
    #ifdef UNICODE
        std::wstring wStr = szFileName;
        std::string str = std::string(wStr.begin(), wStr.end());
        return str;
    #else
        return szFileName;
    #endif
#else
        //linux
        char arg1[20];
        char exepath[PATH_MAX + 1] = {0};

        sprintf(arg1, "/proc/%d/exe", getpid());
        readlink(arg1, exepath, 1024);
        return string(exepath);
#endif
    }

    int FileSystem::copyFile(std::string source, std::string destination) {
        std::replace(source.begin(), source.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
        std::replace(destination.begin(), destination.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
        ifstream sources(source.c_str(), ios::binary);
        ofstream dests(destination.c_str(), ios::binary);
        dests << sources.rdbuf();

        sources.close();
        dests.close();
        return 1;
    };

    std::string FileSystem::getCWD() {
        char* cwd;
        #ifndef PATH_MAX
            #ifndef MAX_PATH
                #define MAX_PATH 4096
            #endif
            #define PATH_MAX MAX_PATH
        #endif
        char buff[PATH_MAX + 1];
        cwd = getcwd(buff, PATH_MAX + 1);
        #ifdef ASTOX_COMPILER_CYGWIN
            std::string cygdrv = "/cygdrive/";
            if(astox::stringAlg::startsWithIgnoreCase(cwd, cygdrv)) {
                std::string r = cwd;
                r = r.substr(cygdrv.size(), r.size());
                std::string r2;
                r2 += r.at(0);
                r2 += (':');
                r2.append(r.substr(1, r.size()));
                toupper(r2.at(0));
//                astox::stringAlg::replace_all(r2, ASTOX)
                return r2;
            }
        #endif
        return cwd;
    }

    std::string FileSystem::getRoots() {
#ifdef ASTOX_OS_WINDOWS

        DWORD dwMask = 1; // Least significant bit is A: flag
        DWORD dwDrives = GetLogicalDrives();
        char strDrive[4] = {'\0'};
        char strDrivex[4] = {'\0'};
        stxtr drivesr;

        // 26 letters in [A..Z] range
        for (int i = 0; i < 26; i++) {
            //Logically 'AND' the Bitmask with 0x1. we get zero, if its a drive
            if (dwDrives & dwMask) {
                wsprintfA((LPSTR) strDrive, "%c", 'A' + i);
                drivesr += strDrive;
                //printf(" %s", strDrive); //Print out the Drive Letter
                //Just Zero filling the buffer, to prevent overwriting or junks
                for (int j = 0; j < 4; j++) {
                    strDrive[j] = '\0';
                }
            }
            // Shift one bit position to left
            dwMask <<= 1;
        }

        return drivesr;
#else
        return "/";
#endif
    };

    bool FileSystem::touch(std::string filename, const char * data) {
        std::replace(filename.begin(), filename.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
        if (std::ifstream(filename.c_str())) {
            return false;
        }
        std::ofstream o(filename.c_str());
        if (!o) {
            return false;
        }
        o << data << std::endl;
        o.close();
        return true;
    };

    bool FileSystem::rmFile(std::string path) {
        std::replace(path.begin(), path.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
#ifdef ASTOX_OS_WINDOWS
    #ifdef ASTOX_COMPILER_UNICODE
        std::wstring wsPath = astox::stringAlg::stdString2wstring(path);
        return DeleteFile(wsPath.c_str()) != 0;
    #else
        return DeleteFile(path.c_str());
    #endif
#endif
        printf("FileSystem::rmFile not implemented\n");
        return true;
    };

    bool FileSystem::rmDirectory(std::string path, int mode) {
        std::replace(path.begin(), path.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
#ifdef ASTOX_OS_WINDOWS
    #ifdef ASTOX_COMPILER_UNICODE
        std::wstring wsPath = astox::stringAlg::stdString2wstring(path);
        LPCTSTR dir = wsPath.c_str();
        PCZZWSTR n = TEXT("");
        std::wstring refcstrRootDirectory = astox::stringAlg::stdString2wstring(path);
        std::wstring strPattern;
        std::wstring strFilePath;
        LPCTSTR s1 = TEXT("\\*.*");
        LPCTSTR s2 = TEXT("\\");
    #else
        std::string refcstrRootDirectory = path;
        std::string strPattern;
        std::string strFilePath;
        const char * s1 = ("\\*.*");
        const char * s2 = ("\\");
        const char *dir = path.c_str();
    #endif

        bool bDeleteSubdirectories = (mode == RMDIR_RECURSIVE);
        bool bSubdirectory = false;
        HANDLE hFile;
        WIN32_FIND_DATA FileInformation;
        strPattern = refcstrRootDirectory;
        strPattern.append(s1);
        hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);

        if (hFile != INVALID_HANDLE_VALUE) {
            do {
                if (FileInformation.cFileName[0] != '.') {
                    strFilePath.erase();
                    strFilePath = refcstrRootDirectory;
                    strFilePath.append(s2);
                    strFilePath.append(FileInformation.cFileName);

                    if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        if (bDeleteSubdirectories) {
    #ifdef ASTOX_COMPILER_UNICODE
                            std::string s(strFilePath.begin(), strFilePath.end());
                            bool response = FileSystem::rmDirectory(s, mode);
    #else
                            bool response = FileSystem::rmDirectory(strFilePath, mode);
    #endif
                            if (!response) {
                                return false;
                            }
                        } else {
                            bSubdirectory = true;
                        }
                    } else {
                        if (::SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE) {
                            printf("failed %i", ::GetLastError());
                            return false;
                        }

                        if (::DeleteFile(strFilePath.c_str()) == FALSE) {
                            printf("failed %i", ::GetLastError());
                            return false;
                        }
                    }
                }
            } while (::FindNextFile(hFile, &FileInformation) == TRUE);

            ::FindClose(hFile);
            DWORD dwError = ::GetLastError();
            if (dwError != ERROR_NO_MORE_FILES) {
                printf("failed %i", dwError);
                return false;
            } else {
                if (!bSubdirectory) {
                    if (::SetFileAttributes(refcstrRootDirectory.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE) {
                        printf("failed %i", ::GetLastError());
                        return false;
                    }

                    if (::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE) {
                        printf("failed %i", ::GetLastError());
                        return false;
                    }
                }
            }
        }
        return false;
#else
        if (mode == RMDIR_RECURSIVE) {
            DIR *d = opendir(path.c_str());
            size_t path_len = strlen(path.c_str());
            int r = -1;
            if (d) {
                struct dirent *p;
                r = 0;
                while (!r && (p = readdir(d))) {
                    if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                        continue;
                    }
                    Path statp(path.c_str(), p->d_name);
                    rmDirectory(statp.getFixedPath().c_str());
                }
                closedir(d);
            }
        }


        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        if (S_ISDIR(fileInfo.st_mode)) {
            rmdir(path.c_str());
        } else {
            unlink(path.c_str());
        }

        return true;
#endif
    };

    bool FileSystem::isDirectory(std::string path) {
        std::replace(path.begin(), path.end(), ASTOX_OS_SEPARATOR_OTHER, ASTOX_OS_SEPARATOR);
#ifdef ASTOX_OS_WINDOWS
        DWORD attribs = ::GetFileAttributesA(path.c_str());
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            return false;
        }
        return (attribs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        if (S_ISDIR(fileInfo.st_mode)) {
            return true;
        } else {
            return false;
        }
#endif
    };

    int FileSystem::ftwrMethod(std::string name) {
        if (name == "nftw") {
            return FS_NFTW;
        }
        if (name == "ftw") {
            return FS_FTW;
        }
        if (name == "boost") {
            return FS_BOOST;
        }
        return FS_BASIC;
    };

    void FileSystem::ftwrDispatcher(FileInfo *& info, Function * callback, void (fileInfoHandler) (FileInfo *&)) {

        if (callback != NULL) {
            callback->setNumCalls(0);
            callback->call(info->toObject(), Function::npos);
        }
        else if (fileInfoHandler != NULL) {
            fileInfoHandler(info);
        }
        else {
//            EventDispatcher::dispatch(Event::ON_FS_FTWR, info->toObject());
        }
    };

    int FileSystem::ftwr(std::string path, int method, Function * callback, void (fileInfoHandler) (FileInfo *&)) {

        bool exHand = (method == FS_NFTW || method == FS_FTW);

        if (callback != NULL && exHand) {
            method = FS_BASIC;
        }

        astox::stringAlg::trim(path);
        if (path.empty()) {
            astox_debug(__LINE__, __FILENAME__, "FileSystem::ftwr empty path...");
            return -1;
        }

        #ifdef ASTX_ENABLE_BOOST
            if (method == FS_BOOST) {
                namespace boostfs = ::boost::filesystem;
                boostfs::path p(path);
                boostfs::directory_iterator end_itr;
                for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr) {
                    std::string current_file = itr->path().string();
                    if (boostfs::is_regular_file(itr->path())) {
                        FileInfo info(itr->path());
                        EventDispatcher::dispatch(Event::ON_FS_FTWR, info.toObject());
                    }

                    if (boostfs::is_directory(itr->path())) {
                        FileInfo info(itr->path());
                        EventDispatcher::dispatch(Event::ON_FS_FTWR, info.toObject());
                        FileSystem::ftwr(current_file, method);
                    }
                }
                return FS_BOOST;
            }
        #endif

    #ifdef _HAVE_FTW_H
        if (method == FS_NFTW) {
            nftw(path.c_str(), fs_nftw_callback, USE_FDS, FTW_PHYS);
            return FS_NFTW;
        }
        if (method == FS_FTW) {
            ftw(path.c_str(), fs_ftw_callback, 16);
            return FS_FTW;
        }
    #endif

#ifndef ASTOX_OS_WINDOWS
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                string pdname = ent->d_name;
                if (pdname.at(0) != '.') {
                    std::string p = path;
                    if (p.at(p.size() - 1) != ASTOX_OS_SEPARATOR) {
                        p += ASTOX_OS_SEPARATOR;
                    }
                    p.append(ent->d_name);
                    Path * sp = new Path(p);
                    FileInfo * file = new FileInfo(sp);
                    ftwrDispatcher(file, callback, fileInfoHandler);
                    if (FileSystem::isDirectory(p)) {
                        FileSystem::ftwr(p, method, callback, fileInfoHandler);
                    }

                    delete file;
                    delete sp;
                }
            }
            closedir(dir);
        } else {
            Path sp(path);
            FileInfo * file = new FileInfo(&sp);
            ftwrDispatcher(file, callback, fileInfoHandler);
            delete file;
            return FS_BASIC;
        }
#else

        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATA ffd;
        LARGE_INTEGER filesize;
        DWORD dwError = 0;
    #ifdef ASTOX_COMPILER_UNICODE
        std::wstring wsPath = astox::stringAlg::stdString2wstring(path);
        hFind = FindFirstFile(wsPath.c_str(), &ffd);
    #else
        hFind = FindFirstFile(path.c_str(), &ffd);
    #endif

        if (INVALID_HANDLE_VALUE == hFind) {
            printf("INVALID FILE \n");
            return -1;
        }

        do {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
            } else {
                filesize.LowPart = ffd.nFileSizeLow;
                filesize.HighPart = ffd.nFileSizeHigh;
                _tprintf(TEXT("  %s   %lld bytes\n"), ffd.cFileName, filesize.QuadPart);
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
            return -1;
        }

        FindClose(hFind);
#endif
        return FS_BASIC;
    };




    std::map<std::string, int> FileSystem::watchedFiles;
    double FileSystem::WatcherRefreshRate = 1;
    double FileSystem::InitialWatcherRefreshRate = 1;

    void FileSystem::watcherFtwrHandler(FileInfo *& data) {
        std::string path = data->getPath()->getFixedPath();
        int timestamp = data->getLastSavedTime();

        if (watchedFiles.find(path) == watchedFiles.end()) {
            watchedFiles[path] = timestamp;
            #ifdef ASTX_DBG_FS_WATCHER
                astox_debug(__LINE__, __FILENAME__, "%s registered at %i", path.c_str(), timestamp);
            #endif
        } else if (watchedFiles[path] != timestamp) {
//            EventDispatcher::dispatch(Event::ON_FS_FILE_WATCH_CHANGED, data->toObject());
            WatcherRefreshRate = InitialWatcherRefreshRate;
            watchedFiles[path] = timestamp;
            #ifdef ASTX_DBG_FS_WATCHER
                astox_debug(__LINE__, __FILENAME__, "%s dispatched at %i, refresh WatcherRefreshRate = %i", path.c_str(), timestamp, WatcherRefreshRate);
            #endif
        } else {
            #ifdef ASTX_DBG_FS_WATCHER
                astox_debug(__LINE__, __FILENAME__, "%s looping at %i", path.c_str(), timestamp);
            #endif
            watchedFiles[path] = timestamp;
        }

    };





#ifdef ASTOX_INOTIFY_SUPPORT

    #define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
    #define EVENT_SIZE  ( sizeof (struct inotify_event) )
    #define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

    #define IN_ALL_EVENTS	(IN_ACCESS | IN_MODIFY | IN_ATTRIB | IN_CLOSE_WRITE | \
			 IN_CLOSE_NOWRITE | IN_OPEN | IN_MOVED_FROM | \
			 IN_MOVED_TO | IN_DELETE | IN_CREATE | IN_DELETE_SELF)

    void FileSystem::inotifyWatcher(char * path) {
        int fd, wd, i = 0;
        fd = inotify_init();
        char buffer[EVENT_BUF_LEN];

        /*checking for error*/
        if (fd < 0) {
            perror("inotify_init");
        }
        wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
        if (wd == -1) {
            perror("inotify_init");
        }
    #ifdef ASTX_DBG_FS_WATCHER
        astox_debug(__LINE__, __FILENAME__, "START WATCH USING inotify EVENTS on %s", path);
    #endif
        for (;;) {
            int length = read(fd, buffer, EVENT_BUF_LEN);
            /*checking for error*/
            if (length < 0) {
                perror("read");
            }
            /*actually read return the list of change events happens. Here,
             * read the change event one by one and process it accordingly.*/
            while (i < length) {
                struct inotify_event *event = (struct inotify_event *) &buffer[ i ];
                if (event->len) {
                    if (event->mask & IN_ALL_EVENTS) {
                        if (event->mask & IN_ISDIR) {
                            printf("Directory %s something.....\n", event->name);
                        } else {
                            printf("File %s something...\n", event->name);
                        }
                    }
                }
                i += EVENT_SIZE + event->len;
            }
        }
        inotify_rm_watch(fd, wd);
        close(fd);
    };
#endif

    void FileSystem::createWatcher(char * path, int repeats, double refreshRate, double incrementalDelta) {
        int counter = 0;
        WatcherRefreshRate = refreshRate;
        InitialWatcherRefreshRate = refreshRate;

        while (counter <= repeats) {
            FileSystem::ftwr(path, FS_BASIC, NULL, FileSystem::watcherFtwrHandler);
			#ifdef ASTX_DBG_FS_WATCHER
				astox_debug(__LINE__, __FILENAME__, "FINISHED LOOP %i/%i with refresh rate = %lf", counter, repeats, WatcherRefreshRate);
			#endif
			#ifdef ASTOX_OS_WINDOWS
				Sleep(1000 * (DWORD) WatcherRefreshRate);
			#else
				 usleep(1000 * 1000 * WatcherRefreshRate);
			#endif
            if (repeats > 0) {
                ++counter;
            }
            WatcherRefreshRate += incrementalDelta;
        }

    };











}//exit namespace











