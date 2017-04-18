/*
 * FileSystem.h
 *
 *  Created on: Jul 30, 2014
 *      Author: alexandru
 */

#ifndef FILESYSTEM_H_
    #define FILESYSTEM_H_

    #include "Thread.h"
    #ifdef ASTOX_OS_MAC
        #include <mach-o/dyld.h>
    #endif
    #ifdef ASTOX_OS_WINDOWS
        #include <tchar.h>
        #include <strsafe.h>
    #endif

    #include "astox.h"
    #include "EventDispatcher.h"


    #ifdef ASTX_ENABLE_BOOST
        #include <boost/filesystem.hpp>
    #endif

    #ifdef ASTOX_INOTIFY_SUPPORT
        #include <sys/inotify.h>
    #endif


//****GENERIC WIN/LINUX FIXES:
    #ifdef ASTOX_OS_WINDOWS



    #else
        #include <fcntl.h>
        #include <sys/types.h>
        #include <sys/stat.h>

        #include <dirent.h>

    #ifdef _HAVE_FTW_H
        #include <ftw.h>
    #endif
        #include <errno.h>
        #include <stdarg.h>
        #include <stdio.h>

        #include <unistd.h>
        #include <dirent.h>
        #include <stdio.h>
    #endif

    #ifndef _XOPEN_SOURCE
        #define _XOPEN_SOURCE 700
    #endif

    #ifndef USE_FDS
        #define USE_FDS 15
    #endif

    #include <stdarg.h>

const int astox_console_colors [] = {
    FG_UNDERLINE, FG_BLINK, FG_INVERT,
    FG_GREY_1, FG_RED_1, FG_GREEN_1, FG_YELLOW_1, FG_BLUE_1, FG_CYAN_1, FG_TURQOISE_1,
    FG_DEFAULT,
    FG_GREY_2, FG_RED_2, FG_GREEN_2, FG_YELLOW_2, FG_BLUE_2, FG_CYAN_2, FG_TURQOISE_2,
    BG_GREY_1, BG_RED_1, BG_GREEN_1, BG_YELLOW_1, BG_BLUE_1, BG_CYAN_1, BG_TURQOISE_1,
    BG_GREY_2, BG_RED_2, BG_GREEN_2, BG_YELLOW_2, BG_BLUE_2, BG_CYAN_2, BG_TURQOISE_2,
    BG_DEFAULT
};

namespace astox {



    class Path {
    private:
        std::string _extension;
        std::string _fileName;
        std::string _fileNameNoExt;
        std::string _parentFolder;
        std::string _fixedPath;
        std::vector<std::string> _tokens;
        bool _isDot;
        bool _isDriveFormat;
        std::string _readablePath;
    public:
        Path(const char * path);
        Path(std::string path);
        Path(std::vector<std::string> _ptokens);
        Path(const char * dirPath, const char * fileName);
        //	Path(const char *fmt, ...);
        bool isDot();
        bool isDriveFormat();
        std::string getExtension();
        std::string getFileName();
        std::string getFileNameNoExt();
        std::string getReadableDirPath();
        std::string toString();
       // std::string getAbsolutePath();
        std::string getFixedPath();
        std::vector<std::string> getTokens();

        void describe();
		std::string resolve(std::string parentDirectory);

		static std::string separatorFix(std::string data);
    protected:
        void processSeparatorFix(std::string data);  //1 -> fix possible separator issues
        void processTokens();  //2 -> detect tokens
        void processFixedPath(); //3 -> fname, extension
    };

    enum FileStats {
        FILE_STATUS_UNKNOWN,
        FILE_STATUS_CHARACTER_DEVICE,
        FILE_STATUS_BLOCK_DEVICE,
        FILE_STATUS_DIRECTORY,
        FILE_STATUS_PIPE,
        FILE_STATUS_SIMLINK,
        FILE_STATUS_REGULAR,
        FILE_STATUS_SOCKET
    };

    class FileInfo {
    private:
        bool _isDir;
        bool _exist;
        bool _isReadable;
        bool _isRegularFile;
        int _fileStatus;

        std::string _fileName;
        Path * _path;
        int _lastSavedTime;
        double _length;
        Date * _lastAccessDate;
        Date * _creationDate;
        Date * _lastModifiedDate;
        Date * _lastStatusChangedDate;
    public:
        FileInfo(Path * path);
        FileInfo(std::string strPath);
    #ifdef ASTX_ENABLE_BOOST
        FileInfo(const boost::filesystem::path& boostPath);
    #endif
    #ifndef ASTOX_OS_WINDOWS
        FileInfo(const struct stat *info, const struct FTW *pathinfo, const int typeflag, const char *filepath);
    #endif





        bool isDirectory();
        bool exist();
        double getLength();
        Path * getPath();
        std::vector<std::string> getTokens();

        int getLastSavedTime();
        Date * getCreationDate();
        Date * getLastAccessDate();
        Date * getLastModifiedDate();
        Date * getStatusChangedDate();
        void describe();
        int getFileStatus();
        String * getFileStatusv();
        Object * toObject();
    protected:
        void digestPath();
		#ifdef ASTOX_OS_WINDOWS //WINDOWS PROCESSOR
			void processWinStat (
			#ifdef ASTOX_COMPILER_UNICODE
					std::wstring wsPath);
			#else
					std::string wsPath);
			#endif
		#else //UNIX PROCESSOR
			void processUnixStat(struct stat *info);
		#endif
    };

    enum FTWR_MODE {
        FS_BASIC,
        FS_BOOST,
        FS_FTW,
        FS_NFTW
    };


    #ifndef ASTOX_OS_WINDOWS
    int fs_nftw_callback(const char *filepath, const struct stat *info,
            const int typeflag, struct FTW *pathinfo);
    int fs_ftw_callback(const char *fpath, const struct stat *sb, int typeflag);
    #endif

    enum RMDIRSTATS {
        RMDIR_BASIC,
        RMDIR_RECURSIVE
    };

    class FileSystem {
    private:
        static std::map<std::string, int> watchedFiles;
        static void watcherFtwrHandler(FileInfo *& data);
        static void ftwrDispatcher(FileInfo *& info, Function * callback = NULL, void (fileInfoHandler) (FileInfo *&) = NULL);
        static double WatcherRefreshRate;
        static double InitialWatcherRefreshRate;
    #ifdef ASTOX_OS_LINUX
        static void inotifyWatcher(char * path);
    #endif
    public:
        static std::string getProcessPath();
        static bool mkdir(std::string name);
        static bool mkdirs(Path path);
        static std::string getRoots();
        static std::string getWorkingDirectory();
        static int ftwr(std::string path, int method = FS_BASIC, Function * callback = NULL, void (fileInfoHandler) (FileInfo *&) = NULL);
        static int ftwrMethod(std::string name);
        static int copyFile(std::string source, std::string destination);
        static bool isDirectory(std::string path);
        static bool rmDirectory(std::string path, int mode = RMDIR_RECURSIVE);
        static bool rmFile(std::string path);
        static bool touch(std::string filename, const char * data = "");
        static void createWatcher(char * path, int repeats = 0, double refreshRate = 1, double incrementalDelta = 0.000001);
    };










    //osascript -e 'tell Application "Finder" to display dialog "Job finished" '

    //Replace popen and pclose with _popen and _pclose for Windows




}






#endif /* FILESYSTEM_H_ */
