/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / bardecode                                            |
   +----------------------------------------------------------------------+
   | Copyright (c) 2009 Mikko Koppanen                                    |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   +----------------------------------------------------------------------+
*/

#ifndef _PHP_BARDECODE_H_
# define _PHP_BARDECODE_H_

#define PHP_BARDECODE_EXTNAME "bardecode"
#define PHP_BARDECODE_EXTVER  "0.0.1-dev"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef ZTS
# include "TSRM.h"
#endif

#include "php.h"
#include "Zend/zend_exceptions.h"

#include <barcode.h>

/* Structure for bardecode object. */
typedef struct _php_bardecode_object  {
	zend_object zo;
	void *bardecoder;
} php_bardecode_object;

#define PHP_BARDECODE_CHAIN_METHOD RETURN_ZVAL(getThis(), 1, 0); 

#ifndef BARCODE_RECT
typedef struct {
	long TopLeftX;
	long TopLeftY;
	long BottomRightX;
	long BottomRightY;
} BARCODE_RECT ;
#endif

#endif
