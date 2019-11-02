/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include <string.h>

#include "lwip/apps/httpd_opts.h"
#include "lwip/def.h"
#include "lwip/apps/fs.h"

#include "fsdata.h"

const char* file_name_404_htm = "/404.htm";
const char* file_name_cgi_htm = "/cgi.htm";
const char* file_name_data_ssi = "/data.ssi";
const char* file_name_img_sics_gif = "/img_sics.gif";
const char* file_name_index_htm = "/index.htm";
const char* file_name_jquery_min_js = "/jquery.min.js";
const char* file_name_smoothie_min_js = "/smoothie.min.js";

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
err_t fs_open(struct fs_file *file, const char *name) {
  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
    return ERR_ARG;
  }

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
      return ERR_OK;
    }
  }
  /* file not found */
  return ERR_VAL;
}

/*-----------------------------------------------------------------------------------*/
void fs_close(struct fs_file *file) {
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
int fs_bytes_left(struct fs_file *file) {
  return file->len - file->index;
}
