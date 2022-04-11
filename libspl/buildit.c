/* Public Domain.  Targets GNU systems. */
/* Compiles .cpp and .c to ./bin */
/* This is a bootstrap type file, so minumum dependancies and features. */
/* Use at your own risk */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_INCLUDES 512
#define PATH_CH_SIZE 128
#define MAX_LINES_TO_SCAN 300

static char _gccName[PATH_CH_SIZE];
static char _lnName[PATH_CH_SIZE];

static char _outputName[PATH_CH_SIZE];
static char _gppflags[PATH_CH_SIZE];
static char _lnflags[PATH_CH_SIZE];
static char _lexflags[PATH_CH_SIZE];
static char _yyflags[PATH_CH_SIZE];
static char _version[PATH_CH_SIZE];

static char _outputPath[PATH_CH_SIZE];
static char _outputNameBuf[PATH_CH_SIZE];

struct sourceInfo
{
	char filename[PATH_CH_SIZE];
	time_t fileLastAccessedTime;

	char objFilename[PATH_CH_SIZE];
	time_t objLastAccessedTime;

	int includes[MAX_INCLUDES];
	int includesPos;

	short isC;
	short isCpp;
	short isLex;
	short isYacc;
	short isCs;		/* C# */

	short scanComplete;

	short buildCheck;
};

static struct sourceInfo _srcTable[MAX_INCLUDES];
static int _srcTablePos = 0;

static int _csLink = 0;
static int _buildOneForLibZeroForExe = 0;

/* STRING FUNCTIONS */

static int doesStringEndWith( const char *cp, const char *cpend )
{
	int m_len = strlen(cp);
	int pos = m_len - strlen(cpend);
	int x;

	if ( pos <= 0 )
	{
		return 0;
	}

	for ( x = pos; x < m_len; x++ )
	{
		if ( cp[x] != *cpend++ )
		{
			return 0;
		}
	}
	
	return 1;
}

static int lastIndexOf( const char *cp, const char *cpfind)
{
	int start = strlen(cp);
	int cpfindlen = strlen(cpfind);
	int pos;

	for ( pos = start; pos > -1; pos-- )
	{
		int cnt = 0;
		while (cnt < cpfindlen && cp[pos - cnt] == cpfind[cpfindlen - (cnt + 1)] )
		{
			cnt++;
		}
		if (cnt == cpfindlen)
		{
			return pos - cnt + 1;
		}
	}

	return -1;
}

static int indexOf( const char *cstr, const char *srch, const int start )
{
	int mlen = strlen(cstr);

	for ( int pos = start; pos < mlen; pos++ )
	{
		int end;
		int cppos = 1;

		if ( cstr[pos] == srch[0] )
		{
			for( end = pos+1; end < mlen && srch[cppos] != '\0'; end++ )
			{
				if ( cstr[end] != srch[cppos] )
				{
					break;
				}
				cppos++;
			}
			if ( srch[cppos] == '\0' )
			{
				return pos;
			}
		}
	}

	return -1;
}

static int skipWS(const char *cp, int pos)
{
	while (cp[pos] == ' ' || cp[pos] == '\t')
	{
		pos++;
	}

	return cp[pos] == '\0' ? -1 : pos;
}

/* END STRING */

static int needsBuilt(const int tablePos, time_t srcModTime)
{
	int x;
	int includeX;

	if (_srcTable[tablePos].buildCheck)
	{
		return 0;
	}

	_srcTable[tablePos].buildCheck = 1;

	for (x = 0; x < _srcTable[tablePos].includesPos; x++)
	{
		includeX = _srcTable[tablePos].includes[x];

		if (_srcTable[includeX].fileLastAccessedTime > srcModTime)
		{
			printf("Out of date due to %s\n", _srcTable[includeX].filename);
			return 1;
		}

		if (needsBuilt(includeX, srcModTime))
		{
			return 1;
		}
	}

	return 0;
}

static int make()
{
	int x;
	int buildIt = 0;
	char cmdBuf[256];
	char linkCmdBuf[8000];

	for (x = 0; x < _srcTablePos; x++)
	{
		_srcTable[x].buildCheck = 0;
	}

	/*sprintf(linkCmdBuf, "%s %s -o %s.so.%s ", _lnName, _lnflags, _outputName, _version);*/
	sprintf(linkCmdBuf, "%s %s -o %s.so ", _lnName, _lnflags, _outputName);

	for (x = 0; x < _srcTablePos; x++)
	{
		if (_srcTable[x].objFilename[0] == '\0')
		{
			continue;
		}

		if (_srcTable[x].fileLastAccessedTime > _srcTable[x].objLastAccessedTime)
		{
			buildIt = 1;
		}
		else
		{
			/*printf("Scanning .h files for %s\n", _srcTable[x].filename);*/
			buildIt = needsBuilt(x, _srcTable[x].objLastAccessedTime);
		}
		
		strcat(linkCmdBuf, _srcTable[x].objFilename);
		strcat(linkCmdBuf, " ");

		if (! buildIt)
		{
			continue;
		}

		if (_srcTable[x].isC || _srcTable[x].isCpp)
		{
			sprintf(cmdBuf, "%s %s -c %s -o %s\n", _gccName, _gppflags, _srcTable[x].filename, _srcTable[x].objFilename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
		}
		else if (_srcTable[x].isLex)
		{
			sprintf(cmdBuf,"flex %s %s\n", _lexflags, _srcTable[x].filename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
			strcpy(_srcTable[x].objFilename, "./");
			strcat(_srcTable[x].objFilename, _outputPath);
			strcpy(_srcTable[x].objFilename, "/lex.yy.o");
			sprintf(cmdBuf, "%s %s -c lex.yy.c -o %s\n", _gccName, _gppflags, _srcTable[x].filename, _srcTable[x].objFilename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
		}
		else if (_srcTable[x].isYacc)
		{
			sprintf(cmdBuf, "bison %s %s -o yacc.yy.c\n", _yyflags, _srcTable[x].filename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
			strcpy(_srcTable[x].objFilename, "./");
			strcat(_srcTable[x].objFilename, _outputPath);
			strcpy(_srcTable[x].objFilename, "/yacc.yy.o");
			sprintf(cmdBuf, "%s %s -c lex.yy.c -o %s\n", _gccName, _gppflags, _srcTable[x].filename, _srcTable[x].objFilename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
		}
		else if (_srcTable[x].isCs)
		{
			sprintf(cmdBuf, "csc -c %s -o %s\n", _srcTable[x].filename, _srcTable[x].objFilename);
			printf("%s", cmdBuf);
			if (system(cmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }
		}
	}

	/* Link */
	printf("%s\n", linkCmdBuf);
	if (system(linkCmdBuf)) { printf("FAILED\n", cmdBuf); return 0; }

	return 1;
}

static int findFileInTable(const char *filename)
{
	int x;

	/* printf("finding %s\n", filename); */

	for (x = 0; x < _srcTablePos; x++)
	{
		if (0 == strcmp(filename, _srcTable[x].filename))
		{
			return x;
		}

		/* trim "./" */
		if (0 == strcmp(filename, &(_srcTable[x].filename)[2]))
		{
			return x;
		}
	}

	return -1;
}

/* Scan first MAX_LINES_TO_SCAN, find any #include, lookup and not include file. */
static int scanDependancy(const int tablePos)
{
	FILE* filePointer;
	int bufferLength = 255;
	char buffer[bufferLength];
	int linecount = 0;
	int pos;
	int posend;
	char chDelim[2];

	if (_srcTable[tablePos].scanComplete)
	{
		return 1;
	}

	_srcTable[tablePos].scanComplete = 1;

	chDelim[1] = '\0';

	/*printf("scanning %s\n", _srcTable[tablePos].filename);*/

	filePointer = fopen(_srcTable[tablePos].filename, "r");

	while(fgets(buffer, bufferLength, filePointer) && linecount++ < MAX_LINES_TO_SCAN) 
	{
		if 
		(
			buffer[0] == '#' && 
			buffer[1] == 'i' && 
			buffer[2] == 'n' && 
			buffer[3] == 'c' && 
			buffer[4] == 'l' &&
			buffer[5] == 'u' &&
			buffer[6] == 'd' &&
			buffer[7] == 'e'
		)
		{
			chDelim[0] = '\0';
		
			if (pos = skipWS(buffer, 8))
			{
				if (buffer[pos] == '"')
				{
					chDelim[0] = buffer[pos];
				}
				else if (buffer[pos] == '<')
				{
					chDelim[0] = '>';
				}

				if (chDelim[0] != '\0')
				{
					pos++;
					posend = indexOf(buffer, chDelim, pos);

					if (posend > 0)
					{
						buffer[posend] = '\0';

						_srcTable[tablePos].includes[_srcTable[tablePos].includesPos] = findFileInTable(&buffer[pos]);

						if (_srcTable[tablePos].includes[_srcTable[tablePos].includesPos] > -1)
						{
							int incpos = _srcTable[tablePos].includesPos;

							_srcTable[tablePos].includesPos++;

							/*printf("found %s\n", &buffer[pos]);*/

							scanDependancy(incpos);
						}
					}
				}
			}
		}
	}

	fclose(filePointer);

	_srcTable[tablePos].scanComplete = 1;

	return 1;
}

static int scanDependancies()
{
	int x;

	for (x = 0; x < _srcTablePos; x++)
	{
		if (_srcTable[x].scanComplete)
		{
			continue;
		}

		if (! scanDependancy(x))
		{
			return 0;
		}
	}

	return 1;
}

static int createObjectFilename(const char *basePath)
{
	int slashBeforeFilenamePos = lastIndexOf(basePath, "/");
	int lastDotPos;
	struct stat file_stats;

	_outputNameBuf[0] = '.';
	_outputNameBuf[1] = '/';
	_outputNameBuf[2] = '\0';

	strcpy(_outputNameBuf, _outputPath);
	strcat(_outputNameBuf, &basePath[slashBeforeFilenamePos]);

	lastDotPos = lastIndexOf(_outputNameBuf, ".");
	_outputNameBuf[lastDotPos+1] = 'o';
	_outputNameBuf[lastDotPos+2] = '\0';

	strcpy(_srcTable[_srcTablePos].objFilename, _outputNameBuf);

	if( access( _outputNameBuf, F_OK ) != -1 ) 
	{
		/* file exists */
		if(0 != stat(_outputNameBuf, &file_stats))
		{
			printf("Error %d getting file stat on %s\n", errno, _outputNameBuf);
			return errno;
		}

		_srcTable[_srcTablePos].objLastAccessedTime = file_stats.st_mtime;
	} 
	else
	{
		/* file doesn't exist */
		_srcTable[_srcTablePos].objLastAccessedTime = 0;
	}

	return 1;
}

static int scanFiles(const char *basePath)
{
	char path[1000];
	struct dirent *dp;
	struct stat file_stats;
	int noObjFile;
	
	DIR *dir = opendir(basePath);

	if (!dir)
	{
		/* Unable to open directory stream */

		if (errno == 20)
		{
			if (_srcTablePos >= MAX_INCLUDES)
			{
				printf("Number of includes exceeds hardcoded max of %d\n", MAX_INCLUDES);
				return 20;
			}

			if(0 != stat(basePath, &file_stats))
			{
				printf("Error %d getting file stat on %s\n", errno, basePath);
				return errno;
			}

			_srcTable[_srcTablePos].includesPos = 0;
			_srcTable[_srcTablePos].isC = 0;
			_srcTable[_srcTablePos].isCpp = 0;
			_srcTable[_srcTablePos].isCs = 0;
			_srcTable[_srcTablePos].isLex = 0;
			_srcTable[_srcTablePos].isYacc = 0;
			_srcTable[_srcTablePos].scanComplete = 0;

			_srcTable[_srcTablePos].fileLastAccessedTime = file_stats.st_mtime;
			strcpy(_srcTable[_srcTablePos].filename, basePath);
			_srcTable[_srcTablePos].objFilename[0] = '\0';

			noObjFile = 0;

			if (doesStringEndWith(basePath, ".h"))
			{
				noObjFile = 1;	
			}
			else if (doesStringEndWith(basePath, ".c"))
			{
				_srcTable[_srcTablePos].isC = 1;
			}
			else if (doesStringEndWith(basePath, ".cpp"))
			{
				_srcTable[_srcTablePos].isCpp = 1;
			}
			else if (doesStringEndWith(basePath, ".cc"))
			{
				_srcTable[_srcTablePos].isCpp = 1;
			}
			else if (doesStringEndWith(basePath, ".cs"))
			{
				_srcTable[_srcTablePos].isCs = 1;
				_csLink = 1;
			}
			//else if (doesStringEndWith(basePath, ".l"))
			//{
			//	_srcTable[_srcTablePos].isLex = 1;
			//}
			//else if (doesStringEndWith(basePath, ".y"))
			//{
			//	_srcTable[_srcTablePos].isYacc = 1;
			//}
			else
			{
				return 0;
			}

			if (! noObjFile)
			{
				if (! createObjectFilename(basePath))
				{
					return 20;
				}
			}

			_srcTablePos++;

			return 0;
		}
		else
		{
			printf("Error %d reading directory or file %s\n", errno, basePath);
			return errno;
		}
	}

	while ((dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			/* Construct new path from our base path */
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			int ret = scanFiles(path);
			if (ret != 0)
			{
				printf("Error scanning files\n");
				closedir(dir);
				return ret;
			}
		}
	}

	closedir(dir);

	return 0;
}

static int ensureBinDir()
{
	DIR *dir = opendir(_outputPath);

	if(dir && 0 == errno)
	{
		closedir(dir);
		return 0;
	}

	if (0 != mkdir(_outputPath, 0777))
	{
		printf("Error %d creating output %s dir\n", errno, _outputPath);
		return errno;
	}

	return 0;
}

static void parseArgs(const int argc, char **argv)
{
	int showhelp = 0;
	int x;
	char *cp;
	char flag;

	if (argc == 1)
	{
		showhelp = 1;
	}

	for (x = 0; x < argc; x++)
	{
		cp = argv[x];

		if (cp[0] != '-')
		{
			strcpy(_outputName, cp);
		}
		else
		{
			cp++;
			flag = cp[0];
			cp++;

			if (cp[0] == '=')
			{
				cp++;
			}

			switch (flag)
			{
				case '?':
				case 'h':
					showhelp = 1;
					break;
				case 'c':
					strcat(_gppflags, cp);
					break;
				case 'l':
					strcpy(_lnflags, cp);
					break;
				case 'x':
					strcpy(_lexflags, cp);
					break;
				case 'y':
					strcpy(_yyflags, cp);
					break;
				case 'p':
					strcpy(_outputPath, cp);
					break;
				case 't':
					if (cp[0] == 'l')
					{
						_buildOneForLibZeroForExe = 1;
					}
					else if (cp[0] == 'e')
					{
						_buildOneForLibZeroForExe = 0;
					}
					else
					{
						printf("Invalid output type of %s, should be -texe or -tlib\n", cp);
						exit(20);
					}
					
					break;
				case 'v':
					strcpy(_version, cp);
					break;
				default:
					printf("Unknown switch %s\n", cp);
					exit(20);
			}
		}
	}

	if (showhelp)
	{
		//printf("buildit -h -? -cCOMPILEARGS -lLINKARGS -tOUTTYPE -v0.0.0 outputname\n");
		printf("buildit -h -? -pOBJPATH -cCOMPILEARGS -lLINKARGS -xLEXARGS -yYACCARGS -tOUTTYPE outputname\n");
		printf("# OBJPATH defaults to -pbi\n");
		printf("# OUTTYPE is either lib or exe, exe is the default.\n");
		printf("# Object files will be built in ./bin.\n");
		printf("# No files in the root directory will be compiled or checked as a dependency.\n");
		printf("# First %d lines of header and source files will be scanned for includes.\n", MAX_LINES_TO_SCAN);
		printf("# Header files should be organized such that #include <a/b.h> maps to ./a/h.h.\n");
		printf("# Or #include \"a/b.h\" maps to ./a/h.h.\n");
		//printf("# Re-rooting, ie -I, header file location for dependency checking not supported.\n");
		//printf("# Will also compile cs files, but do not mix with c/cc/cpp.\n");
		exit(20);
	}
}

int main(int argc, char **argv)
{
	/* Directory path to list files */
	char *path = ".";
	struct dirent *dp;
	DIR *dir;

	strcpy(_gccName, "g++");
	strcpy(_lnName, "g++");

	_lnflags[0] = '\0';
	_lexflags[0] = '\0';
	_yyflags[0] = '\0';

	strcpy(_gppflags, "-I. ");

	strcpy(_outputPath, "bin");
	strcpy(_outputName, "out");
	strcpy(_version, "0.0.0");

	parseArgs(argc, argv);

	if (0 != ensureBinDir())
	{
		return 20;
	}

	dir = opendir(path);
	if (!dir)
	{
		printf("Error reading directory or file %d\n", errno);
		return errno;
	}

	/* skip files in root */
	while ((dp = readdir(dir)) != NULL)
	{
		if (dp->d_type != DT_DIR)
		{
			continue;
		}
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
		{
			continue;
		}

		/*printf("entering %s\n", dp->d_name); */

		if (scanFiles(dp->d_name))
		{
			return 20;
		}
	}

	closedir(dir);

	/* all source files are now in _srcTable.  */
	/*printf ("%d files found.\n", _srcTablePos); */

	if (! scanDependancies())
	{
		return 20;
	}

	if (! make())
	{
		return 20;
	}

	return 0;
}
