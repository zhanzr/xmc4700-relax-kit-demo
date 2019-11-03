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
#ifndef LWIP_HDR_APPS_FS_H
#define LWIP_HDR_APPS_FS_H

#include "httpd_opts.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FS_READ_EOF     -1
#define FS_READ_DELAYED -2

#define FS_FILE_FLAGS_HEADER_INCLUDED     0x01
#define FS_FILE_FLAGS_HEADER_PERSISTENT   0x02
#define FS_FILE_FLAGS_HEADER_HTTPVER_1_1  0x04
#define FS_FILE_FLAGS_SSI                 0x08

struct fs_file {
  const char *data;
  int len;
  int index;
  /* pextension is free for implementations to hold private (extensional)
     arbitrary data, e.g. holding some file state or file system handle */
  void *pextension;

  u8_t flags;
};

err_t fs_open(struct fs_file *file, const char *name);
void fs_close(struct fs_file *file);
int fs_bytes_left(struct fs_file *file);

struct fsdata_file {
  const struct fsdata_file *next;
  const uint8_t *name;
  const uint8_t *data;
  int len;
  u8_t flags;
};

typedef struct str_static_file {
	const char* file_name;
	const char* file_data;
} static_file;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_APPS_FS_H */