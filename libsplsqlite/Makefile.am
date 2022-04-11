SUBDIRS = debug release
#AnsiTerminal

EXTRA_DIST = readme1st.txt		\
	spl.cbp				\
	spl.sln				\
	spl.cproj			\
	ChangeLog			\
	TODO docs/quickref.html		\
	docs/jsbnf.txt			\
	docs/Reference/Collection.html	\
	docs/Reference/Configuration.html	\
	docs/Reference/Data.html	\
	docs/Reference/DebugBuild.html	\
	docs/Reference/Index.html	\
	docs/Reference/IO.html		\
	docs/Reference/Math.html	\
	docs/Reference/Net.html		\
	docs/Reference/Status.html	\
	docs/Reference/Term.html	\
	docs/Reference/Text.html	\
	docs/Reference/Threading.html	\
	docs/Reference/Types.html	\
	docs/Reference/Web.html		\
	docs/Reference/Xml.html		\
	splUnitTests.vcproj		\
	spl.anjuta 				\
	gpl-3.0.txt				\
	NEWS					\
	spl/configwin32.h		\
	spl/cleanwindows.h		\
	test/utf8test.xml		\
	test/utf8testverify.xml \
	test/StateMachine1.xml	\
	test/midsummerNightsDreamWithAVeryLongFilenameToConfuseTheStringHandlingRoutines.xml  \
	examples/splw3/aclocal.m4         \
	examples/splw3/Configuration.h \
	examples/splw3/libtool \
	examples/splw3/AnyKeyHandler.cpp  \
	examples/splw3/ltmain.sh  \
	examples/splw3/AnyKeyHandler.h     \
	examples/splw3/configure \
	examples/splw3/configure.ac          \
	examples/splw3/Makefile.am \
	examples/splw3/Makefile.in \
	examples/splw3/app.config          \
	examples/splw3/AUTHORS \
	examples/splw3/missing \
	examples/splw3/autogen.sh          \
	examples/splw3/ContentHandlers.cpp   \
	examples/splw3/MyHttpServer.cpp \
	examples/splw3/Cache.cpp           \
	examples/splw3/ContentHandlers.h     \
	examples/splw3/MyHttpServer.h \
	examples/splw3/Cache.h             \
	examples/splw3/COPYING               \
	examples/splw3/NEWS \
	examples/splw3/CacheItem.cpp       \
	examples/splw3/debug/Makefile.am          \
	examples/splw3/debug/Makefile.in             \
	examples/splw3/README \
	examples/splw3/CacheItem.h         \
	examples/splw3/depcomp               \
	examples/splw3/release/Makefile.am \
	examples/splw3/release/Makefile.in \
	examples/splw3/ChangeLog           \
	examples/splw3/HtmlHandler.cpp       \
	examples/splw3/SplSimpleWebServer.cpp \
	examples/splw3/config.guess        \
	examples/splw3/HtmlHandler.h         \
	examples/splw3/SplSimpleWebServer.sln \
	examples/splw3/config.h.in         \
	examples/splw3/index.html            \
	examples/splw3/SplSimpleWebServer.vcproj \
	examples/splw3/config.sub          \
	examples/splw3/INSTALL              \
	examples/splw3/Configuration.cpp   \
	examples/splw3/install-sh            

ADDLIBS =

if PLATFORM_OSS
ADDLIBS += -spthread
AM_CXXFLAGS = -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
AM_CPPFLAGS = -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
AM_CFLAGS = -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
AM_LDFLAGS = -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE

RunTestsMain_CPPFLAGS = -I.. -DDEBUG -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
RunTestsMain_CFLAGS = -I.. -DDEBUG -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
RunTestsMain_CXXFLAGS = -I.. -DDEBUG -WTandem_float -D_XOPEN_SOURCE_EXTENDED -D_TANDEM_SOURCE  -Wnowarn=734
else
RunTestsMain_CPPFLAGS = -I.. -DDEBUG
RunTestsMain_CFLAGS = -I.. -DDEBUG
RunTestsMain_CXXFLAGS = -I.. -DDEBUG
endif

if HAVE_LIBM
ADDLIBS += -lm
endif

if HAVE_LIBPTHREAD
ADDLIBS += -lpthread
endif

#if HAVE_LIBTDS
#ADDLIBS += -ltds
#endif

#if HAVE_LIBSYBDB
#ADDLIBS += -lsybdb
#endif

if HAVE_LIBMYSQLC
ADDLIBS += -lmysqlclient
endif

#if HAVE_LIBDBLIB
#ADDLIBS += -lsybdb
#endif

TESTS = RunTestsMain
check_PROGRAMS = RunTestsMain
RunTestsMain_SOURCES = test/RunTestsMain.cpp
RunTestsMain_LDFLAGS = -lc $(ADDLIBS) -pthread -Ldebug/.libs -lspld -ldl

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = spl.pc spld.pc
