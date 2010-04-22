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

#include "php_bardecode.h"

/* Handlers */
static zend_object_handlers php_bardecode_object_handlers;

/* Class entries */
zend_class_entry *php_bardecode_sc_entry;
zend_class_entry *php_bardecode_exception_class_entry;

/*
 {{{ BarDecode::setParameter(int type, mixed value)
	Set a parameter
*/
PHP_METHOD(bardecode, setparameter)
{
	php_bardecode_object *intern;
	long type;
	zval *value;
	uint16 lval;
	double dval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &type, &value) == FAILURE) {
		return;
	}

	intern = (php_bardecode_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	switch (type) {
		
	 	case ST_CONTRAST:
		case ST_SKEW_SETTING:
		case ST_MIN_LEN:
		case ST_MAX_LEN:
		case ST_MIN_SPACE_BAR_WIDTH:
		case ST_QUIET_SIZE:
		case ST_MIN_OCCURRENCE:
		case ST_PREF_OCCURRENCE:
		case ST_ORIENTATION_MASK:
		case ST_LINE_JUMP:
		case ST_OVERSAMPLING_SEP:
		case ST_NOISEREDUCTION:
			convert_to_long(value);
			lval = (uint16)Z_LVAL_P(value);
			STSetParameter(intern->bardecoder, type, &lval);
		break;
		
		case ST_READ_CODE39:
		case ST_READ_EAN13:
		case ST_READ_EAN8:
		case ST_READ_UPCA:
		case ST_READ_UPCE:
		case ST_READ_CODE128:
		case ST_READ_CODE25:
		case ST_READ_PDF417:
		case ST_READ_SHORT_CODE128:
		case ST_READ_MICRO_PDF417:
		case ST_READ_CODABAR:
		case ST_READ_DATAMATRIX:
		case ST_CODE39_NEEDS_ASTERISKS:
		case ST_READ_BARCODE_TYPE:
		case ST_USE_OVERSAMPLING:
		case ST_MULTIPLE_READ:
		case ST_DESPECKLE:
		case ST_PAGE_NO:
			convert_to_boolean(value);
			lval = Z_BVAL_P(value);
			STSetParameter(intern->bardecoder, type, &lval);
		break;
		
		case ST_UPPER_RATIO:
		case ST_LOWER_RATIO:
			convert_to_double(value);
			dval = Z_DVAL_P(value);
			STSetParameter(intern->bardecoder, type, &dval);
		break;
		
		case ST_REGEX_MATCH:
			convert_to_string(value);
			STSetParameter(intern->bardecoder, type, Z_STRVAL_P(value));
		break;
		
		case ST_BARCODE_ZONE:
		
			if (Z_TYPE_P(value) != IS_ARRAY) {
				zend_throw_exception(php_bardecode_exception_class_entry, "BARCODE_ZONE must be an array", 1 TSRMLS_CC);
				return;
			}
			/* Check the amount of elements */
			if (zend_hash_num_elements(Z_ARRVAL_P(value)) == 4) {
				int i = 0, mode = 0;
				zval **ppzval;
				BARCODE_RECT rect;
	
				for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(value));
						zend_hash_get_current_data(Z_ARRVAL_P(value), (void **) &ppzval) == SUCCESS;
						zend_hash_move_forward(Z_ARRVAL_P(value))
				) {
					zval tmp_zval, *tmp_pzval;

					tmp_zval = **ppzval;
					zval_copy_ctor(&tmp_zval);
					tmp_pzval = &tmp_zval;
					convert_to_long(tmp_pzval);

					if (i == 0) {
						rect.TopLeftX = Z_LVAL(tmp_zval);
					} else if (i == 1) {
						rect.TopLeftY = Z_LVAL(tmp_zval);
					} else if (i == 2) {
						rect.BottomRightX = Z_LVAL(tmp_zval);
					} else if (i == 3){
						rect.BottomRightY = Z_LVAL(tmp_zval);
					} else {
						break;
					}
					i++;
				}
				if (i != 4) {
					zend_throw_exception(php_bardecode_exception_class_entry, "Failed to read the BARCODE_ZONE array", 1 TSRMLS_CC);
					return;
				}
				STSetParameter(intern->bardecoder, ST_MAPPING_MODE, &mode);
				STSetParameter(intern->bardecoder, ST_BARCODE_ZONE, &rect);
			} else {
				zend_throw_exception(php_bardecode_exception_class_entry, "BARCODE_ZONE must contain four values", 1 TSRMLS_CC);
				return;
			}
		break;
		
		default:
			zend_throw_exception(php_bardecode_exception_class_entry, "Unknown option type provided", 1 TSRMLS_CC);
			return;
		break;
	}
	PHP_BARDECODE_CHAIN_METHOD;
}
/* }}} */

/*
 {{{ mixed BarDecode::getParameter(int type, mixed value)
	Get a parameter
*/
PHP_METHOD(bardecode, getparameter)
{
	php_bardecode_object *intern;
	long type;
	uint16 int_param;
	float double_param;
	char *string_param;
	BARCODE_RECT *rect;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &type) == FAILURE) {
		return;
	}

	intern = (php_bardecode_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	switch (type) {
		
	 	case ST_CONTRAST:
		case ST_SKEW_SETTING:
		case ST_MIN_LEN:
		case ST_MAX_LEN:
		case ST_MIN_SPACE_BAR_WIDTH:
		case ST_QUIET_SIZE:
		case ST_MIN_OCCURRENCE:
		case ST_PREF_OCCURRENCE:
		case ST_ORIENTATION_MASK:
		case ST_LINE_JUMP:
		case ST_OVERSAMPLING_SEP:
		case ST_NOISEREDUCTION:
			STGetParameter(intern->bardecoder, type, &int_param);
			RETURN_LONG(int_param);
		break;
		
		case ST_READ_CODE39:
		case ST_READ_EAN13:
		case ST_READ_EAN8:
		case ST_READ_UPCA:
		case ST_READ_UPCE:
		case ST_READ_CODE128:
		case ST_READ_CODE25:
		case ST_READ_PDF417:
		case ST_READ_SHORT_CODE128:
		case ST_READ_MICRO_PDF417:
		case ST_READ_CODABAR:
		case ST_READ_DATAMATRIX:
		case ST_CODE39_NEEDS_ASTERISKS:
		case ST_READ_BARCODE_TYPE:
		case ST_USE_OVERSAMPLING:
		case ST_MULTIPLE_READ:
		case ST_DESPECKLE:
		case ST_PAGE_NO:
			STGetParameter(intern->bardecoder, type, &int_param);
			RETURN_BOOL(int_param);
		break;
		
		case ST_UPPER_RATIO:
		case ST_LOWER_RATIO:
			STGetParameter(intern->bardecoder, type, &double_param);
			RETURN_DOUBLE(double_param);
		break;
		
		case ST_REGEX_MATCH:
			STGetParameter(intern->bardecoder, type, &string_param);
			if (string_param != (char *)NULL) {
				RETURN_STRING(string_param, 1);
			} else {
				RETURN_STRING("", 1);
			}
		break;
		
		case ST_BARCODE_ZONE:
			zend_throw_exception(php_bardecode_exception_class_entry, "Retrieving BARCODE_ZONE is not supported", 1 TSRMLS_CC);
			return;
		break;
		
		default:
			zend_throw_exception(php_bardecode_exception_class_entry, "Unknown option", 1 TSRMLS_CC);
			return;
		break;
	}
}
/* }}} */

PHP_METHOD(bardecode, read)
{
	php_bardecode_object *intern;
	char *filename;
	int filename_len, bar_count, i;

	char **bar_codes, **bar_codes_type;
    
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern    = (php_bardecode_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	bar_count = STReadBarCode(intern->bardecoder, filename, 0, &bar_codes, &bar_codes_type);

	if (bar_count < 0) {
		zend_throw_exception(php_bardecode_exception_class_entry, "Unable to open the file", bar_count TSRMLS_CC);
		return;
	}

	array_init(return_value);
	for (i = 0; i < bar_count; i++) {
		zval *bar_array;
		
		zval *coordinates;
		int page;
		int top_left_x, top_left_y;
		int bottom_right_x, bottom_right_y;
		
		MAKE_STD_ZVAL(bar_array);
		array_init(bar_array);

		add_assoc_string(bar_array, "code", bar_codes[i], 1);
		add_assoc_string(bar_array, "type", bar_codes_type[i], 1);
		
		add_assoc_long(bar_array, "confidence", STGetBarCodeConfidence(intern->bardecoder, i));
		add_assoc_long(bar_array, "direction", STGetBarCodeDirection(intern->bardecoder, i));
		
		MAKE_STD_ZVAL(coordinates);
		array_init(coordinates);
		
		page = STGetBarCodePos(intern->bardecoder, i, &top_left_x, &top_left_y, &bottom_right_x, &bottom_right_y);
		
		add_assoc_long(coordinates, "page", page);
		add_assoc_long(coordinates, "top_left_x", top_left_x);
		add_assoc_long(coordinates, "top_left_y", top_left_y);
		add_assoc_long(coordinates, "bottom_right_x", bottom_right_x);
		add_assoc_long(coordinates, "bottom_right_y", bottom_right_y);
		
		add_assoc_zval(bar_array, "coordinates", coordinates);
		add_next_index_zval(return_value, bar_array);
	}
	return;
}

static function_entry php_bardecode_class_methods[] =
{
	PHP_ME(bardecode, setparameter,	NULL, 	ZEND_ACC_PUBLIC)
	PHP_ME(bardecode, getparameter,	NULL, 	ZEND_ACC_PUBLIC)
	PHP_ME(bardecode, read,		NULL, 	ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};


/* Initialization */
static void php_bardecode_object_free_storage(void *object TSRMLS_DC)
{
	php_bardecode_object *intern = (php_bardecode_object *)object;
	
	if (!intern) {
		return;
	}
	
	STFreeBarCodeSession(intern->bardecoder);
	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}

static zend_object_value php_bardecode_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_bardecode_object *intern;
	
	/* Allocate memory for it */
	intern = emalloc(sizeof(php_bardecode_object));
	memset(&intern->zo, 0, sizeof(php_bardecode_object));
	
	/* Initialize reader */
	intern->bardecoder = STCreateBarCodeSession();
	
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));
	
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_bardecode_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &php_bardecode_object_handlers;
	return retval;
}

PHP_MINIT_FUNCTION(bardecode)
{
	zend_class_entry ce;
	memcpy(&php_bardecode_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/*
		Initialize exceptions (bardecode exception)
	*/
	INIT_CLASS_ENTRY(ce, "bardecodeexception", NULL);
	php_bardecode_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_bardecode_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;

	/*
		Initialize the class (bardecode read)
	*/
	INIT_CLASS_ENTRY(ce, "bardecode", php_bardecode_class_methods);
	ce.create_object = php_bardecode_object_new;
	php_bardecode_object_handlers.clone_obj = NULL;
	php_bardecode_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

#define PHP_BARDECODE_REGISTER_CONST_LONG(const_name, value) \
	zend_declare_class_constant_long(php_bardecode_sc_entry, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);
	
	PHP_BARDECODE_REGISTER_CONST_LONG("UPPER_RATIO", ST_UPPER_RATIO);
	PHP_BARDECODE_REGISTER_CONST_LONG("LOWER_RATIO", ST_LOWER_RATIO);
	PHP_BARDECODE_REGISTER_CONST_LONG("LINE_JUMP", ST_LINE_JUMP);
	PHP_BARDECODE_REGISTER_CONST_LONG("MIN_OCCURRENCE", ST_MIN_OCCURRENCE);
	PHP_BARDECODE_REGISTER_CONST_LONG("TIFF_WARNINGS", ST_TIF_WARNINGS);
	PHP_BARDECODE_REGISTER_CONST_LONG("MULTIPLE_READ", ST_MULTIPLE_READ);
	PHP_BARDECODE_REGISTER_CONST_LONG("ORIENTATION", ST_ORIENTATION);
	PHP_BARDECODE_REGISTER_CONST_LONG("PREF_OCCURRENCE", ST_PREF_OCCURRENCE);
	PHP_BARDECODE_REGISTER_CONST_LONG("DESPECKLE", ST_DESPECKLE);
	PHP_BARDECODE_REGISTER_CONST_LONG("NOISEREDUCTION", ST_NOISEREDUCTION);
	PHP_BARDECODE_REGISTER_CONST_LONG("QUIET_SIZE", ST_QUIET_SIZE);
	PHP_BARDECODE_REGISTER_CONST_LONG("DEBUG", ST_DEBUG);
	PHP_BARDECODE_REGISTER_CONST_LONG("PAGE_NO", ST_PAGE_NO);
	PHP_BARDECODE_REGISTER_CONST_LONG("TIFF_SPLIT", ST_TIFF_SPLIT);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_BARCODE_TYPE", ST_READ_BARCODE_TYPE);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_CODE39", ST_READ_CODE39);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_EAN13", ST_READ_EAN13);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_EAN8", ST_READ_EAN8);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_UPCA", ST_READ_UPCA);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_UPCE", ST_READ_UPCE);
	PHP_BARDECODE_REGISTER_CONST_LONG("SHOW_CHECK_DIGIT", ST_SHOW_CHECK_DIGIT);
	PHP_BARDECODE_REGISTER_CONST_LONG("CODE39_NEEDS_ASTERISKS", ST_CODE39_NEEDS_ASTERISKS);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_CODE128", ST_READ_CODE128);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_CODE25", ST_READ_CODE25);
	PHP_BARDECODE_REGISTER_CONST_LONG("CONTRAST", ST_CONTRAST);
	PHP_BARDECODE_REGISTER_CONST_LONG("BARCODE_ZONE", ST_BARCODE_ZONE);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_PATCHCODES", ST_READ_PATCHCODES);
	PHP_BARDECODE_REGISTER_CONST_LONG("ORIENTATION_MASK", ST_ORIENTATION_MASK);
	PHP_BARDECODE_REGISTER_CONST_LONG("USE_OVERSAMPLING", ST_USE_OVERSAMPLING);
	PHP_BARDECODE_REGISTER_CONST_LONG("OVERSAMPLING_SEP", ST_OVERSAMPLING_SEP);
	PHP_BARDECODE_REGISTER_CONST_LONG("MIN_LEN", ST_MIN_LEN);
	PHP_BARDECODE_REGISTER_CONST_LONG("MAX_LEN", ST_MAX_LEN);
	PHP_BARDECODE_REGISTER_CONST_LONG("TIFF_SPLIT_MODE", ST_TIFF_SPLIT_MODE);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_CODABAR", ST_READ_CODABAR);
	PHP_BARDECODE_REGISTER_CONST_LONG("CONVERT_UPCE_TO_UPCA", ST_CONVERT_UPCE_TO_UPCA);
	PHP_BARDECODE_REGISTER_CONST_LONG("SKEW_SETTING", ST_SKEW_SETTING);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_CODE25_NI", ST_READ_CODE25_NI);
	PHP_BARDECODE_REGISTER_CONST_LONG("DUPLICATE_BARCODES", ST_DUPLICATE_BARCODES);
	PHP_BARDECODE_REGISTER_CONST_LONG("MAPPING_MODE", ST_MAPPING_MODE);
	PHP_BARDECODE_REGISTER_CONST_LONG("MIN_SEPARATION", ST_MIN_SEPARATION);
	PHP_BARDECODE_REGISTER_CONST_LONG("EXTENDED_CODE39", ST_EXTENDED_CODE39);
	PHP_BARDECODE_REGISTER_CONST_LONG("CODE39_CHECKSUM", ST_CODE39_CHECKSUM);
	PHP_BARDECODE_REGISTER_CONST_LONG("ERROR_CORRECTION", ST_ERROR_CORRECTION);
	PHP_BARDECODE_REGISTER_CONST_LONG("NUMERIC_BARCODE", ST_NUMERIC_BARCODE);
	PHP_BARDECODE_REGISTER_CONST_LONG("REGEX_MATCH", ST_REGEX_MATCH);
	PHP_BARDECODE_REGISTER_CONST_LONG("MIN_SPACE_BAR_WIDTH", ST_MIN_SPACE_BAR_WIDTH);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_PDF417", ST_READ_PDF417);
	PHP_BARDECODE_REGISTER_CONST_LONG("MEDIAN_FILTER", ST_MEDIAN_FILTER);
	PHP_BARDECODE_REGISTER_CONST_LONG("CODE25_MIN_OCCURRENCE_LENGTH", ST_CODE25_MIN_OCCURRENCE_LENGTH);
	PHP_BARDECODE_REGISTER_CONST_LONG("WEIGHT_LONG_BARCODES", ST_WEIGHT_LONG_BARCODES);
	PHP_BARDECODE_REGISTER_CONST_LONG("CODE25_CHECKSUM", ST_CODE25_CHECKSUM);
	PHP_BARDECODE_REGISTER_CONST_LONG("ENCODING", ST_ENCODING);
	PHP_BARDECODE_REGISTER_CONST_LONG("GAMMA_CORRECTION", ST_GAMMA_CORRECTION);
	PHP_BARDECODE_REGISTER_CONST_LONG("SKEW_LINE_JUMP", ST_SKEW_LINE_JUMP);
	PHP_BARDECODE_REGISTER_CONST_LONG("MAX_RECT_OVERLAP", ST_MAX_RECT_OVERLAP);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_SHORT_CODE128", ST_READ_SHORT_CODE128);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_MICRO_PDF417", ST_READ_MICRO_PDF417);
	PHP_BARDECODE_REGISTER_CONST_LONG("SHORT_CODE128_MIN_LENGTH", ST_SHORT_CODE128_MIN_LENGTH);
	PHP_BARDECODE_REGISTER_CONST_LONG("PDF417DEBUG", ST_PDF417DEBUG);
	PHP_BARDECODE_REGISTER_CONST_LONG("READ_DATAMATRIX", ST_READ_DATAMATRIX);
	PHP_BARDECODE_REGISTER_CONST_LONG("SHOW_CODABAR_START_STOP", ST_SHOW_CODABAR_START_STOP);
	
#undef PHP_BARDECODE_REGISTER_CONST_LONG

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(bardecode)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(bardecode)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "bardecode module", "enabled");
	php_info_print_table_row(2, "bardecode module version", PHP_BARDECODE_EXTVER);
	php_info_print_table_end();
}

static function_entry php_bardecode_functions[] =
{
	{ NULL, NULL, NULL }
};

zend_module_entry bardecode_module_entry =
{
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_BARDECODE_EXTNAME,
	php_bardecode_functions,	/* Functions */
	PHP_MINIT(bardecode),		/* MINIT */
	PHP_MSHUTDOWN(bardecode),	/* MSHUTDOWN */
	NULL,						/* RINIT */
	NULL,						/* RSHUTDOWN */
	PHP_MINFO(bardecode),		/* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_BARDECODE_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BARDECODE
ZEND_GET_MODULE(bardecode)
#endif