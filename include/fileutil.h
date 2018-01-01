

#ifndef _L_FILE_UTIL_H_
#define _L_FILE_UTIL_H_

#include "platform.h"

#ifdef _WIN32
// windows
#include <io.h>
#include <direct.h>

#else
// linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#endif

#include <stdarg.h> 
#include <queue>
#include <string>
#include "lpublic.h"


#ifdef _WIN32
// windows
#define LMKDIR(f)   mkdir((f))
#else
// linux
#define LMKDIR(f)   mkdir((f), 0777)
#endif

#define LU_MAX_FILESYS_NAME	260	/* longest Windows or Posix filename + 1 */

#define LU_FS_ISPLITOER(x) ((x) == L'/' || (x) == L'\\' || (x) == L':')
#define LU_FS_ISSEP(x)	((x) == L'/' || (x) == L'\\')
#define LU_FS_PREF	L'\\'

#define LU_FS_COLON	L':'
#define LU_FS_PERIOD	L'.'
#define LU_FS_SLASH	L'/'
#define LU_FS_BSLASH	L'\\'

namespace LU_FILE
{

class LConsole 
{
public:
    LConsole() {}
    ~LConsole() {}
    inline int info(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        printf(format, ap);
        va_end(ap);
        printf("\n");
        return 1;
    }

};

inline int ErrLog(const char *format, ...)
{
    va_list ap;
    fprintf(stderr, "%s: Line %d\n", __FILE__, __LINE__);
    va_start(ap, format);
    fprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    return 1;
}

std::queue<std::string> SplitPath(const char *path)
{
    std::queue<std::string> q;
    char ch[LU_MAX_FILESYS_NAME + 1];
    int pos = 0;
    int isabsolutepath = 0;          // 是否是绝对路径
    LU_PROCESS_ERROR(path);
    
    memset(ch, 0, LU_MAX_FILESYS_NAME + 1);

    if(LU_FS_SLASH == path[0])
        isabsolutepath = 1;

    for (int i = 0; i < strlen(path); ++i)
    {
        if (LU_FS_COLON == path[0])
            isabsolutepath = 1;

        if (!LU_FS_ISPLITOER(path[i]) && pos < LU_MAX_FILESYS_NAME)
        {
            ch[pos++] = path[i];
        }
        else
        {
            if(ch[0])
            {
                ch[pos] = '\0';
                q.push(ch);
                memset(ch, 0, pos);
                pos = 0;
            }
        }
    }

    if (ch[0])
    {
        q.push(ch);
    }

    if (isabsolutepath == 1 && q.size() > 0)
    {
        auto& it = q.front();
        it.insert((std::string::size_type)0,1, LU_FS_SLASH);
    }

Exit0:
    return q;
}

template<typename T>
inline int MKdirIsNotExist(std::string fpath, const T& console)
{
    int nResult = 1;
    int nRetCode = 0;
    int flag = -1;
    std::string temp;
    // console->info("share ptr test! file:{0}", fpath.c_str());

    auto q = SplitPath(fpath.c_str());
    while(!q.empty())
    {
        auto& it = q.front();
        temp.append(it.c_str());
        if (access(temp.c_str(), 0) == -1)
        {
            flag = LMKDIR(temp.c_str());
            if (0 != flag)
            {
                console->info("directory [{0}] create faild.", temp.c_str());
                nResult = 0;
                break;
            }
            console->info("directory [{0}] create success.", temp.c_str());
        }
        temp.append("/");
        q.pop();
    }

    return nResult;
} 

template <int n = 0>
inline int MKdirIsNotExist(std::string fpath, const int& p = n)
{
    int nResult = 0;

    LConsole lc;
    nResult = MKdirIsNotExist(fpath, &lc);

    return nResult;
} 

template<typename T>
inline int RMdirIsExist(std::string fpath, const T& console)
{
    int nResult = 0;
    int nRetCode = 0;
    int flag = -1;
    // console->info("share ptr test! file:{0}", fpath.c_str());
    nRetCode = access(fpath.c_str(), 0);
    LU_PROCESS_ERROR(nRetCode == 0);

	flag = rmdir(fpath.c_str()); 
    LU_PROCESS_ERROR(-1 != flag);

    nResult = 1;
Exit0:
    if(1 == nResult)
        console->info("directory [{0}] remove success.", fpath.c_str());
    else
        console->info("directory [{0}] remove faild.", fpath.c_str());
    return nResult;
} 

template <int n = 0>
inline int RMdirIsExist(std::string fpath, const int& p = n)
{
    int nResult = 0;

    LConsole lc;
    nResult = RMdirIsExist(fpath, &lc);

    return nResult;
} 

};



#endif //_L_FILE_UTIL_H_