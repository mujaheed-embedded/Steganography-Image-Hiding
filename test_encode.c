#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;

    if(argc > 1){
        // Encoding
        if ((check_operation_type(argv)) == e_encode)
        {
            if(argc < 4){
                usleep(500000);
                
                printf("Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\n");
                return 1;
            }

            // Read and validate the arguments entered
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                // Encoding secret data

                if (do_encoding(&encInfo) == e_failure)
                {
                    usleep(500000);
                    fprintf(stderr, "Error: %s function failed\n", "do_encoding()");
                    return 1;
                }
                usleep(500000);
                printf("\nINFO: ## Encoding Done Successfully ##\n");
            }
            else
            {
                usleep(500000);
                fprintf(stderr, "Error: %s function failed\n", "read_and_validation_encode_args()");
            
            
                usleep(500000);
                printf("Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\n");
                return 1;
            }
        }
        // Decoding
        else if (check_operation_type(argv) == e_decode)
        {
            	DecodeInfo decInfo;
                
                if(argc < 3){
                   
                    
                    usleep(500000);
                    printf("Decoding: ./a.out -d <.bmp file> [output file]\n");
                    return 1;
                }
                // To read and store file names
                if (read_and_validate_decode_args(argv, &decInfo) == e_success){
                    // Decode the stego image
                    if (do_decoding(&decInfo) == e_failure){
                        usleep(500000);
                        fprintf(stderr, "Error: %s function failed\n", "do_decoding()");
                        return e_failure;
                    }
                    usleep(500000);
                    printf("\nINFO: ## Decoding Done Successfully ##\n");
                }
                else{
                    usleep(500000);
                    fprintf(stderr, "Error: %s function failed\n", "read_and_validate_decode_args()");
                   
                    
                    usleep(500000);
                    printf("Decoding: ./a.out -d <.bmp file> [output file]\n");
                    return e_failure;
                }
        }
        else
        {
            
            
            usleep(500000);
            printf("Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\n Decoding: ./a.out -d <.bmp file> [output file]\n");
            return 1;
        }
    }
    else{
        
        
        usleep(500000);
        printf(" Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\nDecoding: ./a.out -d <.bmp file> [output file]\n");
        return 1;
    }

    return 0;
}

/* Check for operation to be performed
 * Input: Command line arguments
 * Output: Operation to be performed
 * Return: integer constant coresponding to operation
 */
OperationType check_operation_type(char *argv[])
{
	
	if ((strcmp(argv[1],"-e")) == 0)   //comapare if -e is passes or not
	{
		return e_encode;
	}
	else if ((strcmp(argv[1], "-d")) == 0)  //comapare if -d is passes or not
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}