#include "types.h"
#include "common.h"
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    int flag=0;
    if(argv[2] != NULL)
    {
        if(strstr(argv[2],".") != NULL)
        {
            if(strcmp(strstr(argv[2],"."),".bmp") == 0)  // separate the . extn and compare it with .bmp
            {
                flag = 1;
                decInfo->src_image_fname = argv[2];      //if valid update into structure
            }
        }
    }
    if(flag == 0)
    {
        printf("Decode :: Error occured while doing validation\n\n");
        return e_failure;     //not valid
    }

    if(argv[3] != NULL)
    {
        const char *delim = "."; 
        decInfo -> dest_image_fname = strtok(argv[3], delim);   // separate the . extn 
            
        return e_success;
        printf("Decode :: Validation successfully done\n\n");   // validation done
    }
    else
    {
        decInfo->dest_image_fname = "result";           // put the output file into dest_image_fname
        return e_success;
        printf("Decode :: Validation successfully done\n\n");
    }
}
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo) ==  e_failure)
    {
        return e_failure;        //not valid
    }
    usleep(80000);
    if(decode_magic_string(decInfo) == e_failure)
    {
        return e_failure;        //not valid
    }
    usleep(80000);
    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        return e_failure;        //not valid
    }
    usleep(80000);
    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        return e_failure;
    }
    usleep(80000);
    if(decode_secret_file_size(decInfo) == e_failure)
    {
        return e_failure;
    }
    usleep(80000);
    if(decode_secret_file_data(decInfo) == e_failure)
    {
        return e_failure;      //not valid
    }
    usleep(80000);
    return e_success;        // valid

}
Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n\n", decInfo->src_image_fname);

    	return e_failure;
    }
    printf("Decode :: successfully open the source files\n\n");
    return e_success;     // file opened successfully
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);
    char temp[10];
    char magic[10]; 
       int flag = 0,res=0;
       printf("Decode :: Enter the Magic string : ");
       scanf(" %s",magic);     //read the input magic string from user
        char size_int[32];
        fread(size_int, sizeof(int), MAX_IMAGE_BUF_SIZE, decInfo->fptr_src_image);
        int size = decode_lsb_to_size(size_int);
        for (int i = 0; i < size; i++)
        {
            fread(decInfo->image_data, 8, 1, decInfo->fptr_src_image);   //read from fptr_src_image
            temp[i] = decode_lsb_to_byte(decInfo->image_data);
            if (temp[i] != magic[i])
            {
                flag = 1;
                break;  
            }
        }
    if (flag == 0)
    {
        printf("\nDecode :: Magic string matched\n\n");
        return e_success;   //if user entered magic string matched proceed
    }
    else
    {
        printf("\nDecode :: Magic string Mismatched\n\n");
        return e_failure; //user entered magic string mismatched
    }
}
char decode_lsb_to_byte(char *image_buffer)
{
    char data[10];   //char array for 10 bytes
    data[0] = 0;  
    int j=0;
    for(int i=7,j=0; i>=0; i--,j++)
    {
        data[0] = ((image_buffer[j] & (1<<0)) << i) | data[0];  // store the data into zeroth index
    } 
    return data[0];  //return data
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char size_int[32];
    
    fread(size_int, sizeof(int), MAX_IMAGE_BUF_SIZE, decInfo->fptr_src_image);  // read from source file
    decInfo -> size_extn_file = decode_lsb_to_size(size_int);
    printf("Decode :: Size of extern : %d\n\n",decInfo -> size_extn_file);
    printf("Decode :: secret extern size successfully decoded\n\n");
    return e_success;   //valid return e_success
}
int decode_lsb_to_size(char *image_buffer)
{
    int data = 0;
    int j=0;
    for(int i=31,j=0; i>=0; i--,j++)
    {
        data = ((image_buffer[j] & (1<<0)) << i) | data;  //store the data into a variable
    } 
    return data;  //return the data
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char temp[4];
    for(int i=0; i < decInfo -> size_extn_file; i++)
    {
        fread(decInfo->image_data, 8, 1, decInfo->fptr_src_image);    //read from src_image
        temp[i] = decode_lsb_to_byte(decInfo->image_data);   //store the image data into and array
    }
    int new_len = strlen(decInfo -> dest_image_fname)+1+strlen(temp)+1;
    char temp1[new_len];
    strcpy(temp1,decInfo -> dest_image_fname);   //copy the dest_image_fname into temp1 array
    
    decInfo->dest_image_fname = strcat(temp1,temp);  // concatenate the both array data into dest_image
    
    printf("Decode :: extn: %s\n\n",temp);

    decInfo->fptr_dest_image = fopen(decInfo->dest_image_fname, "w");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n\n", decInfo->dest_image_fname);

    	return e_failure;
    }
    printf("Decode :: successfully open the destination file files\n\n");
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char size_int[32];  //character array for 32 bytes
    
    fread(size_int, 4, 8, decInfo->fptr_src_image);  //read from source file
    decInfo -> size_secret_file = decode_lsb_to_size(size_int);
    printf("Decode :: file size: %ld\n\n",decInfo -> size_secret_file);
    printf("Decode :: secret extern size successfully decoded\n\n");
    return e_success;  //valid
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char temp[4];
    for(int i=0; i < decInfo -> size_secret_file ; i++)
    {
        //read from src_image
        fread(decInfo->image_data, 8, 1, decInfo->fptr_src_image);
        temp[0] = decode_lsb_to_byte(decInfo->image_data);
        fwrite(temp,1,1,decInfo->fptr_dest_image);  //write into dest_image
    }
    fclose(decInfo->fptr_dest_image);  //close the file 
    fclose(decInfo->fptr_src_image);
    printf("Decode :: Secret data succsesfully copied your file\n\n");
    return e_success;
}
