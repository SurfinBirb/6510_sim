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
	//char *input_file;
	//char *output_file;
//	FILE *input_file_ptr;
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
			//input_file = optarg;
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

// 	if(input_file) input_file_ptr = fopen(input_file,"rb");
// 	if(!input_file_ptr) printf("No such file %s in the directory or not enough rights\n", input_file);
// 	if(debug && output_file) output_file_ptr = fopen(output_file,"wb");
// 	if(debug && !output_file_ptr) printf("No such file %s in the directory or not enough rights\n", output_file);
    mm[0x0] = 0x69; // add 
    mm[0x1] = 0x01; // 1
    mm[0x2] = 0x69; // add
    mm[0x3] = 0x05; // 5
    mm[0x4] = 0xE9; // sub
    mm[0x5] = 0x05; // 6
    mm[0x6] = 0xF0; // branch if 0
    mm[0x7] = 0x07; // pc += this
    mm[0x8] = 0x69; // add
    mm[0x9] = 0x01; // 1
    mm[0xA] = 0x4C; // jmp
    mm[0xB] = 0x0F; // lo
    mm[0xC] = 0x00; // hi
    mm[0xD] = 0x69; // add
    mm[0xE] = 0x02; // 2
    mm[0xF] = 0xEA; //nop <- jump here
    mm[0x10] = 0x02; // halt
    es exec_s = {0, 0, 0, mm, register_file, 0};
	routine(debug, exec_s);
	
	return 0;
}

void print_help(void)
{
	printf("6510");
}
