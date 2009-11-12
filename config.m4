PHP_ARG_WITH(bardecode, whether to enable the bardecode extension,
[ --with-bardecode[=DIR]   Enables the bardecode extension.], no)

PHP_ARG_WITH(bardecode-libtiff, Location to libtiff installation,
[ --with-bardecode-libtiff[=DIR]   Location to libtiff installation.], no)

if test $PHP_BARDECODE != "no"; then

  AC_MSG_CHECKING(for barcode.h header)

  for i in $PHP_BARDECODE /usr /usr/local /opt;
  do
  	test -r $i/include/barcode.h && BARDECODE_PATH=$i && break
  done

  if test -z "$BARDECODE_PATH"; then
  	AC_MSG_ERROR(Cannot locate barcode.h header. Please check your bardecode installation)
  fi

  AC_MSG_RESULT(found in $BARDECODE_PATH/include/barcode.h)

  dnl $PHP_LIBDIR seems to be set to "lib" but we need to check lib64 as well
  for BARDECODE_LIB_PATH in $PHP_LIB_PATH lib lib64; do
  	PHP_CHECK_LIBRARY(bardecode, STCreateBarCodeSession, [
                      BARDECODE_FOUND_LIB_PATH=$BARDECODE_PATH/$BARDECODE_LIB_PATH
                      PHP_ADD_LIBRARY_WITH_PATH(bardecode, $BARDECODE_FOUND_LIB_PATH, BARDECODE_SHARED_LIBADD)
                      PHP_ADD_INCLUDE($BARDECODE_PATH/include)			
  	],[
  	],[
  		-L$BARDECODE_PATH/$BARDECODE_LIB_PATH -ltiff -ljpeg
  	])
  done

  if test -z $BARDECODE_FOUND_LIB_PATH; then
  	AC_MSG_ERROR(Unable to found bardecode library installation)
  fi

  PHP_CHECK_LIBRARY(tiff, TIFFSetWarningHandler, [
                    PHP_ADD_LIBRARY_WITH_PATH(tiff, $BARDECODE_FOUND_LIB_PATH, BARDECODE_SHARED_LIBADD)
  ],[
  	AC_MSG_ERROR([libtiff4 not found or error. Check config.log for details])
  ],[
  	-L$PHP_BARDECODE_TIFF/lib 
  ])

  AC_DEFINE(HAVE_BARDECODE,1,[ ])
  PHP_SUBST(BARDECODE_SHARED_LIBADD)
  PHP_NEW_EXTENSION(bardecode, bardecode.c, $ext_shared)
fi
