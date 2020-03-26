AC_INIT([sybilSBML], [3.1.2], [mayo.roettger@hhu.de])

dnl # --------------------------------------------------------------------
dnl # template config.h.in
dnl # --------------------------------------------------------------------
AC_CONFIG_HEADERS([src/config.h])


dnl # --------------------------------------------------------------------
dnl # global (environment) variables
dnl # --------------------------------------------------------------------
dnl
dnl # PKG_CPPFLAGS       C preprocessor flags -I...
dnl # PKG_CFLAGS         C compiler flags
dnl # PKG_LIBS           libraries -l...


dnl # --------------------------------------------------------------------
dnl # R compiler and flags
dnl # --------------------------------------------------------------------

: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
    echo "could not determine R_HOME"
    exit 1
fi

dnl # GNU R default settings
CC=`"${R_HOME}/bin/R" CMD config CC`
#CPP=`"${R_HOME}/bin/R" CMD config CPP`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`
AC_PROG_CC
AC_PROG_CPP


dnl # --------------------------------------------------------------------
dnl # test for global variables
dnl # --------------------------------------------------------------------

ARGS_OK="FALSE"

if test [ -n "$PKG_CPPFLAGS" -a -n "$PKG_LIBS" ] ; then
    ARGS_OK="TRUE"
else
    if test [ -n "$PKG_CPPFLAGS" -o -n "$PKG_LIBS" ] ; then
        AC_MSG_ERROR([Please set both variables: PKG_CPPFLAGS AND PKG_LIBS])
    fi
fi


dnl # --------------------------------------------------------------------
dnl # get arguments
dnl # --------------------------------------------------------------------

AC_ARG_WITH(sbml-include,
            AC_HELP_STRING([--with-sbml-include=PATH],
                           [location of libSBML include directory]
                          ),
            [SBML_INCLUDE=$withval],
            [SBML_INCLUDE=""])

AC_ARG_WITH(sbml-lib,
            AC_HELP_STRING([--with-sbml-lib=PATH],
                           [location of SBML callable library]
                          ),
            [SBML_LIB=$withval],
            [SBML_LIB=""])


dnl # --------------------------------------------------------------------
dnl # pkg-config
dnl # --------------------------------------------------------------------

dnl # include and library directories
if test [ "$ARGS_OK" = "FALSE" ] ; then
    AC_PATH_PROG([PKG_CONFIG], [pkg-config], [], [$PATH:/usr/local/bin])

    if test [ "${PKG_CONFIG}" != "" ] ; then
        if "${PKG_CONFIG}" --exists libsbml; then

            SBML_LIBS=`${PKG_CONFIG} --libs libsbml`    
            SBML_CFLAGS=`${PKG_CONFIG} --cflags libsbml`    
    
            if test [ -z "$SBML_LIBS" -a -z "$SBML_CFLAGS" ] ; then
                AC_MSG_NOTICE([can not get information about libSBML from pkg-config])
            else
                AC_MSG_NOTICE([set compile and link flags according to pkg-config])
                AC_MSG_NOTICE([compile flags are $SBML_CFLAGS])
                AC_MSG_NOTICE([link flags are $SBML_LIBS])
                PKG_LIBS="${SBML_LIBS}"
                PKG_CPPFLAGS="${SBML_CFLAGS}"
                ARGS_OK="TRUE"
            fi
        else
            AC_MSG_NOTICE([pkg-config file for libSBML is not available])
        fi
    fi
fi


dnl # --------------------------------------------------------------------
dnl # test arguments
dnl # --------------------------------------------------------------------

dnl # include and library directories
if test [ "$ARGS_OK" = "FALSE" ] ; then

    if test [ -n "$SBML_INCLUDE" -a -n "$SBML_LIB" ] ; then
    
        if test [ -d "$SBML_INCLUDE" ] ; then
            if test [ -d "$SBML_INCLUDE/sbml"] ; then
                PKG_CPPFLAGS="-I${SBML_INCLUDE} -I${SBML_INCLUDE}/sbml"
            else
                PKG_CPPFLAGS="-I${SBML_INCLUDE}"
            fi
        else
            AC_MSG_ERROR([directory $SBML_INCLUDE does not exist])
        fi
    
        if test [ -d "$SBML_LIB" ] ; then
            if test [ -d "$SBML_LIB/sbml" ] ; then
                PKG_LIBS="-L${SBML_LIB} -L${SBML_LIB}/sbml"
            else
                PKG_LIBS="-L${SBML_LIB}"
            fi
        else
            AC_MSG_ERROR([directory $SBML_LIB does not exist])
        fi
        
        ARGS_OK="TRUE"
    else

        dnl # if no arguments are given, try to use /usr/local

        if test [ "$prefix" = "NONE" ] ; then

            if test [ -d "/usr/include" -o -d "/usr/local/include" ] ; then
                PKG_CPPFLAGS="-I/usr/include -I/usr/local/include -I/usr/include/sbml -I/usr/local/include/sbml"
            else
                AC_MSG_ERROR([directories /usr/include and /usr/local/include do not exist])
            fi

            if test [ -d "/usr/local/lib" -o -d "/usr/local/lib64" -o -d "/usr/lib" -o -d "/usr/lib64" ] ; then
                PKG_LIBS=""
                if test [ -d "/usr/local/lib" ] ; then
                    PKG_LIBS="$PKG_LIBS -L/usr/local/lib"
                    if test [ -d "/usr/local/lib/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L/usr/local/lib/sbml"
                    fi
                fi
                if test [ -d "/usr/local/lib64" ] ; then
                    PKG_LIBS="$PKG_LIBS -L/usr/local/lib64"
                    if test [ -d "/usr/local/lib64/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L/usr/local/lib64/sbml"
                    fi
                fi
                if test [ -d "/usr/lib" ] ; then
                    PKG_LIBS="$PKG_LIBS -L/usr/lib"
                    if test [ -d "/usr/lib/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L/usr/lib/sbml"
                    fi
                fi
                if test [ -d "/usr/lib64" ] ; then
                    PKG_LIBS="$PKG_LIBS -L/usr/lib64"
                    if test [ -d "/usr/lib64/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L/usr/lib64/sbml"
                    fi
                fi
                # PKG_LIBS="-L/usr/local/lib -Wl,-rpath /usr/local/lib"
            else
                AC_MSG_ERROR([none of the directories /usr/local/lib, /usr/local/lib64, /usr/lib or /usr/lib64 exists])
            fi

        else
        
            AC_MSG_NOTICE(trying prefix $prefix)
        
            if test [ -d "$prefix/include" ] ; then
                PKG_CPPFLAGS="-I$prefix/include -I$prefix/sbml"
            else
                AC_MSG_ERROR([directory $prefix/include does not exist])
            fi

            if test [ -d "$prefix/lib" -o -d "$prefix/lib64" ] ; then
                PKG_LIBS=""
                if test [ -d "$prefix/lib" ] ; then
                    PKG_LIBS="$PKG_LIBS -L$prefix/lib"
                    if test [ -d "$prefix/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L$prefix/sbml"
                    fi
                fi
                if test [ -d "$prefix/lib64" ] ; then
                    PKG_LIBS="$PKG_LIBS -L$prefix/lib64"
                    if test [ -d "$prefix/lib64/sbml" ] ; then
                        PKG_LIBS="$PKG_LIBS -L$prefix/lib64/sbml"
                    fi
                fi
            else
                AC_MSG_ERROR([none of the directories $prefix/lib of $prefix/lib64 exists])
            fi

        fi

    fi

    dnl # PKG_LIBS="${PKG_LIBS} -lxml2 -lz -lpthread -licucore -lm -lz -lbz2 -lsbml -lstdc++ -lm"
    PKG_LIBS="${PKG_LIBS} -lsbml"

    ARGS_OK="TRUE"

fi

if test [ ! "$ARGS_OK" = "TRUE" ] ; then	
   AC_MSG_ERROR([no libSBML installation found])
fi



dnl # --------------------------------------------------------------------
dnl # check header and library
dnl # --------------------------------------------------------------------

LIBS="${LIBS} ${PKG_LIBS}"
CFLAGS="${CFLAGS} ${PKG_CFLAGS}"
CPPFLAGS="${CPPFLAGS} ${PKG_CPPFLAGS}"

AC_CHECK_HEADER([sbml/SBMLTypes.h], ,
    AC_MSG_ERROR([Could not find SBMLTypes.h:
      sybilSBML requires libSBML from http://www.sbml.org/.
      use --with-sbml-include or PKG_CPPFLAGS to specify the include path.]))

AC_SEARCH_LIBS([readSBML], [sbml], ,
    AC_MSG_ERROR([Could not link to libSBML:
      use --with-sbml-lib or PKG_LIBS to specify the library path and the libraries to pass to the linker.]))

AC_CHECK_LIB([sbml], [readSBML], ,
    AC_MSG_ERROR([Could not link to libSBML:
      use --with-sbml-lib or PKG_LIBS to specify the library path and the libraries to pass to the linker.]))

AC_CHECK_HEADERS([sbml/packages/fbc/common/FbcExtensionTypes.h], ,
 AC_MSG_WARN([Could not find specific FBC header of libSBML:
      make sure your libSBML version is >= 5.18 and including the FBC package.]),
      [#include <sbml/SBMLTypes.h>])

AC_CHECK_HEADERS([sbml/packages/groups/common/GroupsExtensionTypes.h], AC_DEFINE([HAVE_GROUPS_PLUGIN], [1], [define if Groups plugin is available]),
 AC_MSG_WARN([Could not find specific groups package header of libSBML:
      make sure your libSBML version is >= 5.18 and including the groups package.]),
      [#include <sbml/SBMLTypes.h>])

#AC_CHECK_LIB([sbml], [SBMLExtensionRegistry_getRegisteredPackages], ,
#    AC_MSG_ERROR([Could not find specific FBC function of libSBML:
#		make sure your libSBML version is >= 5.16.]))

AC_CHECK_FUNCS([SBase_getPlugin SBMLExtensionRegistry_getRegisteredPackages GeneProductAssociation_setAssociation FbcReactionPlugin_getLowerFluxBound], AC_DEFINE([HAVE_FBC_PLUGIN], [1], [define if FBC plugin is available]),
	AC_MSG_WARN([Could not find specific FBC function of libSBML:
		make sure your libSBML version is >= 5.18 and including the FBC package.]))

#AC_SEARCH_LIBS([SBase_getPlugin], [sbml], ,
#    AC_MSG_ERROR([Could not find specific FBC function of libSBML:
#      make sure your libSBML version is >= 5.16.]))

#AC_SEARCH_LIBS([SBMLExtensionRegistry_getRegisteredPackages], [sbml], ,
#    AC_MSG_ERROR([Could not find specific FBC function of libSBML:
#      make sure your libSBML version is >= 5.16.]))



#AC_SEARCH_LIBS([GeneProductAssociation_setAssociation], [sbml], ,
#    AC_MSG_ERROR([Could not find specific FBC function of libSBML:
#      make sure your libSBML version is >= 5.16.]))



dnl # --------------------------------------------------------------------
dnl # substitute src/Makevars
dnl # --------------------------------------------------------------------

AC_SUBST(PKG_CFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(PKG_CPPFLAGS)
AC_SUBST(HAVE_GROUPS_PLUGIN)
AC_SUBST(HAVE_FBC_PLUGIN)

AC_CONFIG_FILES([src/Makevars.tmp:src/Makevars.in], [
    if test -f src/Makevars && cmp -s src/Makevars.tmp src/Makevars; then
      AC_MSG_NOTICE([creating src/Makevars])
      AC_MSG_NOTICE([src/Makevars is unchanged])
      rm src/Makevars.tmp
    else
      AC_MSG_NOTICE([creating src/Makevars])
      mv src/Makevars.tmp src/Makevars
    fi
  ]
)

AC_OUTPUT

exit 0
