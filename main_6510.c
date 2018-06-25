#define _XOPEN_SOURCE 700
#include "main_6510.h"

void print_help(void);

extern void routine(bool debug, es exec_s);

int main (int argc, char **argv)
{
//     printf("%d\n",argc);
//     for (int i = 0; i < argc; ++i)
//     {
//         printf("%ld  %s\n", strlen(argv[i]), argv[i]);
//     }
    
    
	int c;
	bool debug = false;
	char *input_file = NULL;
	//char *output_file;
	FILE *input_file_ptr;
//	FILE *output_file_ptr;
	uint8_t mm[0x10000] = {0};
	rf register_file = {
		.acc = 0x00,
		.x = 0x00,
		.y = 0x00,
		.pch = 0x00,
		.pcl = 0x00,
		.sp = 0x00,
		.f = 0xA0
	};
    int opt_index = 0;
    static const struct option l_options[] = {
                   {"debug",  no_argument, 0, 'd'},
                   {"file",  required_argument, 0, 'f'},
                   {"output", required_argument, 0, 'o'},
                   {"help", no_argument, 0, 'h'},
                   { 0 }
            };
	while ((c = getopt_long(argc, argv, "df:ho:", l_options, &opt_index)) != -1)
    switch (c)
    {
		case 'd':
			debug = true;
			break;
		case 'f':
			input_file = optarg;
			break;
		case 'h':
			print_help();
			break;
		case 'o':
			//output_file = optarg;
			break;
		case '?':
			if(optopt == c)
			{
				printf("No args provided for -%c option", optopt);
			}
			break;
		default:
			return 0;
	}

 	if(input_file)
    {
        input_file_ptr = fopen(input_file,"rb");
        if(!input_file_ptr) printf("No such file %s in the directory or not enough rights\n", input_file);
        else
        {
            fseek(input_file_ptr, 0, SEEK_END);
            long size = ftell(input_file_ptr) - 2;
            rewind(input_file_ptr);
            uint8_t lo, hi;
            char adr_buf[2];
            fread(adr_buf, sizeof(char), 2, input_file_ptr);
            lo = (uint8_t) adr_buf[0];
            hi = (uint8_t) adr_buf[1];
            uint8_t *load_ptr = &mm[(((uint16_t) hi) << 8) + lo];
            fread(load_ptr, sizeof(char), size, input_file_ptr);
        }
    }
// 	if(debug && output_file) output_file_ptr = fopen(output_file,"wb");
// 	if(debug && !output_file_ptr) printf("No such file %s in the directory or not enough rights\n", output_file);
    
    es exec_s = {0, 0, 0, mm, register_file, 0};
	routine(debug, exec_s);
	
	return 0;
}

void print_help(void)
{
	printf("6510");
}
