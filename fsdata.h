#include "lwip/apps/fs.h"
#include "lwip/def.h"

#define file_NULL ((struct fsdata_file *)NULL)

#include "Resources/data__404_htm.h"

#include "Resources/data__cgi_htm.h"

#include "Resources/data__data_ssi.h"

#include "Resources/data__img_sics_gif.h"

#include "Resources/data__index_htm.h"

#include "Resources/data__jquery_min_js.h"

#include "Resources/data__smoothie_min_js.h"

#include "Resources/data__freertos_logo_gif.h"

#include "Resources/data__favicon_ico.h"

// TODO: use a single linked list implentation
const struct fsdata_file file__favicon_ico = {
		file_NULL,
		data__favicon_ico,
		data__favicon_ico + PAD_FAVICON_ICO_DATA_OFFSET,
		sizeof(data__favicon_ico) - PAD_FAVICON_ICO_DATA_OFFSET,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__freertos_logo_gif = {
		&file__favicon_ico,
		data__freertos_logo_gif,
		data__freertos_logo_gif + PAD_FREERTOS_LOGO_GIF_DATA_OFFSET,
		sizeof(data__freertos_logo_gif) - PAD_FREERTOS_LOGO_GIF_DATA_OFFSET,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__404_htm = {
		&file__freertos_logo_gif,
		data__404_htm,
		data__404_htm + PAD_404_HTM_DATA_OFFSET,
		sizeof(data__404_htm) - PAD_404_HTM_DATA_OFFSET,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__cgi_htm = {
		&file__404_htm,
		data__cgi_htm,
		data__cgi_htm + PAD_CGI_HTM_DATA_OFFSET,
		sizeof(data__cgi_htm) - PAD_CGI_HTM_DATA_OFFSET,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__data_ssi = {
		&file__cgi_htm,
		data__data_ssi,
		data__data_ssi + 12,
		sizeof(data__data_ssi) - 12,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_SSI,
};

const struct fsdata_file file__img_sics_gif = {
		&file__data_ssi,
		data__img_sics_gif,
		data__img_sics_gif + 16,
		sizeof(data__img_sics_gif) - 16,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__index_htm = {
		&file__img_sics_gif,
		data__index_htm,
		data__index_htm + 12,
		sizeof(data__index_htm) - 12,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__jquery_min_js = {
		&file__index_htm,
		data__jquery_min_js,
		data__jquery_min_js + 16,
		sizeof(data__jquery_min_js) - 16,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

const struct fsdata_file file__smoothie_min_js = {
		&file__jquery_min_js,
		data__smoothie_min_js,
		data__smoothie_min_js + 20,
		sizeof(data__smoothie_min_js) - 20,
		FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
};

#define FS_ROOT (&file__smoothie_min_js)
