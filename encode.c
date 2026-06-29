#include "types.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include <unistd.h>

/* Function Definitions */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int flag=0;
    if(argv[2] != NULL)
    {
        if(strstr(argv[2],".") != NULL)                  //check for . is present or not
        {
            if(strcmp(strstr(argv[2],"."),".bmp") == 0)  //separate the .extn and comapre with .bmp
            {
                flag = 1;
                encInfo->src_image_fname = argv[2];     // if valid update into the structure
            }
        }
    }
    if(flag == 0)
    {
        return e_failure;                               //not valid
    }
    flag =0;
    if(argv[3] != NULL)
    {
        if(strstr(argv[3],".") != NULL)
        {
            if(strcmp(strstr(argv[3],"."),".txt") == 0)            //separate the .extn and comapre with .txt
            {
                strcpy(encInfo -> extn_secret_file,strstr(argv[3],"."));
                flag = 1;
                encInfo->secret_fname = argv[3];                   // if valid update into the structure
                int len=0;
                while((len<4) && (encInfo -> extn_secret_file[len]!='\0'))
                {
                    len++;
                }
                encInfo->size_secret_file_extn = len;
            }
        }
    }
    if(flag == 0)
    {
        return e_failure;                                            //not valid
    }

    if(argv[4] != NULL)
    {
        if(strstr(argv[4],".") != NULL)
        {
            if(strcmp(strstr(argv[4],"."),".bmp") == 0)               //separate the .extn and compare with .bmp
            {
                flag = 1;
                encInfo->stego_image_fname = argv[4];                  // if valid update into the structure
            }
            else
            return e_failure;                                           //not valid
        }
        else
        return e_failure;
    }
    else
    {
        encInfo->stego_image_fname = "stego_img.bmp";
    }
    if(flag == 1)
    {
        printf("Encode :: Validation successfully done\n\n");
        return e_success;
    }
    printf("Encode :: Error occured while doing validation\n");
    return e_failure;
}

/* Encoding the secret file data to stego image
    * Input: FILE info of image, secret file and stego image
    * Output: Encodes the data in secret to stego image
    * Return: e_success or e_failure;
 */
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("Error :: Error occured while opening file\n");
        return e_failure;
    }
	usleep(80000);
    if (check_capacity(encInfo) == e_failure)
    {
        printf("Error :: Error occured while checking capacity\n");
        return e_failure;
    }
    usleep(80000);
    if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error :: Error occured while copying header\n");
        return e_failure;
    }
    usleep(80000);
    if (encode_magic_string(MAGIC_STRING,encInfo) == e_failure)
    {
        printf("Error :: Error occured while copying magic string\n");
        return e_failure;
    }
    usleep(80000);
    if (encode_secret_file_extn_size(encInfo->size_secret_file_extn, encInfo) == e_failure)
    {
        printf("Error :: Error occured while encoding file size\n");
        return e_failure;
    }
    usleep(80000);
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("Error :: Error occured while encoding file extension\n");
        return e_failure;
    }
    usleep(80000);
    if (encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_failure)
    {
        printf("Error :: Error occured while encoding secret file size\n");
        return e_failure;
    }
    usleep(80000);
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("Error :: Error occured while encoding the secret data\n");
        return e_failure;
    }
    usleep(80000);
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error :: Error occured while encoding the copying remaining data\n");
        return e_failure;
    }
    return e_success;                //valid
}

/* Get File pointers for i/p and o/p files
    * Inputs: Src Image file, Secret file and Stego Image file
    * Output: FILE pointer for above files
    * Return Value: e_success or e_failure, on file errors
*/

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR :: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR :: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR :: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    printf("Encode :: successfully open the files\n\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo){
   
	encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    //do the camparisition

    if (encInfo->image_capacity >= (MAX_IMAGE_BUF_SIZE * (strlen(MAGIC_STRING) + MAX_FILE_SUFFIX + (uint)sizeof(encInfo->extn_secret_file) + encInfo->size_secret_file)))
    {
        printf("Encode :: Sufficent space is available in sorce image\n\n");
        return e_success;             //valid space available in source image
    }
    else
    {
        printf("Error :: Not available sufficent space in sorce image\n");
        return e_failure;      //not valid
    }

}

/* Get image size
    * Input: Image file ptr
    * Output: width * height * bytes per pixel (3 in our case)
    * Description: In BMP Image, width is stored in offset 18,
    * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("Encode :: width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("Encode :: height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


uint get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);                 //rewind the file pointer
    long file_size = ftell(fptr);             //store the pointer position
    rewind(fptr);
    printf("\nEncode :: Size of secret data %ld\n\n",file_size);
    printf("Encode :: successfully found size of secret data\n\n");
    return (unsigned int)file_size;            //return the file size
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //rest the both points at zero th position using rewind
    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    char *buffer[54];

    //read 54 bytes for beatiful.bmp
    fread(buffer,sizeof(char),54,fptr_src_image);

    //write 54 bytes into stego_img.bmp
    fwrite(buffer,sizeof(char),54,fptr_dest_image);
    
    printf("Encode :: successfully copied bmp header \n\n");
    return e_success;
}

/* Read and encode the 1 byte to LSB of 8 byte of image file
    * Input: data_to_hide and image_buffer 
    * Output: File names stored in encoded Info
    * Return: e_success or e_failure
 */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int j=0;
    for(int i=7,j=0; i>=0; i--,j++)    //run the loop from 0-7 times
    {
        image_buffer[j] = ((unsigned)((1<<i) & data) >> i) | (image_buffer[j] & (~(1<<0)));
    }
    return e_success;
}

Status encode_int_to_lsb(int size, char *image_buffer) {
    int j=0;
    for(int i=31,j=0; i>=0; i--,j++)   //run the loop from 0-31 times
    {
        image_buffer[j] = ((unsigned)((1<<i) & size) >> i) | (image_buffer[j] & (~(1<<0)));
    }
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char y[10];
    printf("Do you want to create your own magic string 'Y'[Press any key to ignore]: ");
    scanf("%s",y);   
    if(strcasecmp(y,"y") == 0)
    {
        char temp[20];
        printf("\nEnter your own magic string(Don't forget whatever you enter) : ");
        scanf("%s",temp);           //input to create ur own magic string
        int size = strlen(temp);    //find the length of ur entered magic string

        char size_int[32];
        fread(size_int, 32, 1, encInfo->fptr_src_image);     //read 32 bytes from fptr_src_image
        if (encode_int_to_lsb(size, size_int) == e_failure)
        {
            return e_failure;
        }
        fwrite(size_int, 32, 1 , encInfo->fptr_stego_image);  //write 32 bytes into fptr_stego_image

        for(int i=0; i < size; i++)
        {
        fread(encInfo -> image_data,1,8,encInfo ->fptr_src_image);
        if(encode_byte_to_lsb(temp[i] ,encInfo -> image_data) == e_success)
        {
            fwrite(encInfo -> image_data,8,1,encInfo -> fptr_stego_image);
        }
        }
        fprintf(stderr, "\nEncode :: Own magic string encoded %s\n\n", encInfo->stego_image_fname);  //own magic string encoded
    }
    else{
        fprintf(stderr, "\nEncode :: Default magic string encoded %s\n\n", encInfo->stego_image_fname);  //default magic string encode

        char size_int[32];
        fread(size_int, 32, 1, encInfo->fptr_src_image);   //read 32 bytes from source image
        if (encode_int_to_lsb(2, size_int) == e_failure)
        {
            return e_failure;
        }
        fwrite(size_int, 32, 1 , encInfo->fptr_stego_image);
        
        for(int i=0; i < strlen(magic_string); i++)
        {
            fread(encInfo -> image_data,1,8,encInfo ->fptr_src_image);             //read 8 bytes from image_data
            if(encode_byte_to_lsb(magic_string[i] ,encInfo -> image_data) == e_success)
            {
                fwrite(encInfo -> image_data,8,1,encInfo -> fptr_stego_image);    //write into stego image
            }
        }
    }
    printf("Encode :: Magic string successfully encoded\n\n");
    return e_success;
}

/* Encode secret file extension size */
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo) 
{
    char size_int[32];
    fread(size_int, sizeof(int), MAX_IMAGE_BUF_SIZE, encInfo->fptr_src_image);  //read from fptr_src_image
    if (encode_int_to_lsb(size, size_int) == e_failure)
    {
        return e_failure;
    }
    fwrite(size_int, sizeof(int), MAX_IMAGE_BUF_SIZE, encInfo->fptr_stego_image); //write into stego_image
    printf("Encode :: secret extern size successfully encoded\n\n");
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   for(int i = 0; i < encInfo->size_secret_file_extn; i++)
    {
        fread(encInfo -> image_data,1,8,encInfo ->fptr_src_image);  //read from source iage
        if(encode_byte_to_lsb(file_extn[i] ,encInfo -> image_data) == e_success)
        {
            fwrite(encInfo -> image_data , 8 , 1 , encInfo -> fptr_stego_image);  //write into stego image
        }
    }
    printf("Encode :: secret extension successfully encoded\n\n");
    return e_success;
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char size_int[32];
    size_t bytesread;
    // Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at 7th index
    fread(size_int, 32, 1, encInfo->fptr_src_image);
    if (encode_int_to_lsb(file_size, size_int) == e_failure)
    {
        return e_failure;
    }
    fwrite(size_int, 32, 1 , encInfo->fptr_stego_image);   //write into stego_image
    printf("Encode :: secret file size successfully encoded\n\n");
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo) 
{ 
    while (1)
    {
        size_t bytesRead = fread(encInfo->secret_data, 1, 1, encInfo->fptr_secret);  //read from fptr_secret
        
        if (bytesRead < 1) 
        {
            if (feof(encInfo->fptr_secret)) 
            {
                break;  // End of file
            } 
            else 
            {
                printf("Error :: Error reading secret file.\n");
                return e_failure;
            }
        }

        fread(encInfo->image_data, 1, 8, encInfo->fptr_src_image); 
        
        if (encode_byte_to_lsb(encInfo->secret_data[0], encInfo->image_data) == e_success) 
        {
            fwrite(encInfo->image_data, 8, 1, encInfo->fptr_stego_image);  //write into stego_image
        } 
        else 
        {
            printf("Error :: Error occured  encoding data into image.\n");
            return e_failure;
        }
    }
    printf("Encode :: secret file data successfully encoded\n\n");
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	int ch;
	while ((ch = getc(fptr_src)) != EOF)  //read character by character from fptr_src till EOF
    {
		if (ferror(fptr_src))
		{
			fprintf(stderr, "Error: unable to read from %s", "Source image file");
			clearerr(fptr_src);        //clear error
			return e_failure;
		}
      	putc(ch, fptr_dest);
		if (ferror(fptr_dest))
		{
			fprintf(stderr, "Error: unable to write to  %s", "Destination image file");
			clearerr(fptr_dest);
			return e_failure;
		}
    }
    printf("Encode :: Copy remaining inage data successfully\n\n");
	return e_success;
}
