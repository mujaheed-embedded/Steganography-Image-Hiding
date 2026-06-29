
#include <stdio.h>
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *src_image_fname;
    FILE *fptr_src_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    //output info
    char *dest_image_fname;
    FILE *fptr_dest_image;
    char extn_secret_file[4];
    long size_secret_file;
    int size_extn_file;
} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_files_decode(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

char decode_lsb_to_byte(char *image_buffer);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode a size into LSB of image data array */
int decode_lsb_to_size(char *image_buffer);

#endif