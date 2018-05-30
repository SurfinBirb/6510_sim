#define _GNU_SOURCE
#include <getopt.h>
#include <string.h>
uint16_t pc(rf regs)
{
    return ((regs.pch << 8) + regs.pcl);
}

uint16_t sp(rf regs)
{
    return (0x0100 + regs.sp);
}

void set_bit(uint8_t number, uint8_t n)
{
    number |= 0x01 << n;
}

void clear_bit(uint8_t number, uint8_t n)
{
    number &= ~(0x01 << n);
}

bool check_bit(uint8_t flags, uint8_t m)
{
    return (flags & (0x01 << m));
}

void set_zero(uint8_t flags, uint8_t m)
{
    if(m == 0x00) set_bit(flags, ZERO);
    else clear_bit(flags, ZERO);
}

void set_neg(uint8_t flags, uint8_t m)
{
    if((m & 128) == 128) set_bit(flags, NEGATIVE);
    else clear_bit(flags, NEGATIVE);
}

void irq_disable(uint8_t flags)
{
    set_bit(flags, IRQ);
}

void irq_enable(uint8_t flags)
{
    clear_bit(flags, IRQ);
}

void set_carry(uint8_t flags)
{
    set_bit(flags, CARRY);
}

void clear_carry(uint8_t flags)
{
    clear_bit(flags, CARRY);
}

void set_decimal(uint8_t flags)
{
    set_bit(flags, DECIMAL);
}

void clear_decimal(uint8_t flags)
{
    clear_bit(flags, DECIMAL);
}

void set_overflow(uint8_t flags)
{
    clear_bit(flags, OVERFLOW);
}

void clear_overflow(uint8_t flags)
{
    clear_bit(flags, OVERFLOW);
}

void change_pc(rf *regs, uint16_t offset, bool sign)
{
    uint16_t pc_reg = pc(*regs);
    if(sign) pc_reg -= offset;
    else pc_reg += offset;
    regs->pch = pc_reg >> 8;
    regs->pcl = pc_reg;
}

// int split (char *str, char c, char ***arr)
// {
//     int count = 1;
//     int token_len = 1;
//     int i = 0;
//     char *p;
//     char *t;
// 
//     p = str;
//     while (*p != '\0')
//     {
//         if (*p == c)
//         count++;
//         p++;
//     }
// 
//     *arr = (char**) malloc(sizeof(char*) * count);
//     if (*arr == NULL) exit(1);
// 
//     p = str;
//     while (*p != '\0')
//     {
//         if (*p == c)
//         {
//             (*arr)[i] = (char*) malloc(sizeof(char) * token_len);
//             if ((*arr)[i] == NULL) exit(1);
//             token_len = 0;
//             i++;
//         }
//         p++;
//         token_len++;
//     }
//     (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
//     if ((*arr)[i] == NULL) exit(1);
// 
//     i = 0;
//     p = str;
//     t = ((*arr)[i]);
//     while (*p != '\0')
//     {
//         if (*p != c && *p != '\0')
//         {
//             *t = *p;
//             t++;
//         }
//         else
//         {
//             *t = '\0';
//             i++;
//             t = ((*arr)[i]);
//         }
//         p++;
//     }
//   return count;
// }

void fetch(es *exec_s)
{
    exec_s->regs.ir = exec_s->mem_map[pc(exec_s->regs)];
}

char* get_mode_name(am mode)
{
    switch (mode)
    {
        case immediate: return "Immediate";
        case absolute: return "Absolute";
        case zero_page: return "Zero page";
        case accumulator: return "Accumulator";
        case implied: return "Implied";
        case indirect_x: return "Indirect X";
        case indirect_y: return "Indirect Y"; 
        case zero_page_x: return "Zero page X";
        case absolute_x: return "Absolute page Y";
        case absolute_y: return "Absolute Y";
        case relative: return "Relative";
        case indirect: return "Indirect";
        case zero_page_y: return "Zero page Y";
        default: return "Unknown addressing mode";
    }
}

void decode(es *exec_s)
{
    uint8_t *mem_map = exec_s->mem_map;
    am mode = instr_set[exec_s->regs.ir].addressing;
    //printf("Instruction opcode = 0x%X; Mnemonic: %s\n", exec_s->regs.ir, instr_set[exec_s->regs.ir].mnemonic);
    //printf("Addressing mode: %s\n", get_mode_name(mode));
    uint16_t high, low, ptr;
    switch (mode)
    {
        case immediate: ;
            exec_s->data = &mem_map[pc(exec_s->regs) + 1];
            //printf("Operand = 0x%X;\n", mem_map[pc(exec_s->regs) + 1]);
            exec_s->pc_incr = 2;
            break;
        case absolute: ;
        high = mem_map[pc(exec_s->regs) + 2];
        low = mem_map[pc(exec_s->regs) + 1];
        exec_s->ptrh = high;
        exec_s->ptrl = low;
        exec_s->data = &mem_map[(high << 8) + low];
        exec_s->pc_incr = 3;
        break;		
        case zero_page: ;
        exec_s->data = &mem_map[mem_map[pc(exec_s->regs) + 1]];
        exec_s->pc_incr = 2;
        break;		
        case accumulator: ;
            exec_s->pc_incr = 1;
            break;		
        case implied:
            exec_s->pc_incr = 1;
            break;
        case indirect_x: ;
        ptr = (exec_s->regs.x + mem_map[pc(exec_s->regs) + 1]) & 0x00FF; // carry is discarded
        low = mem_map[ptr];
        high = mem_map[ptr + 1];
        exec_s->data = &mem_map[(high << 8) + low];
        exec_s->pc_incr = 2;
        break;
        case indirect_y: ;
        ptr = mem_map[pc(exec_s->regs) + 1];
        low = mem_map[ptr] + exec_s->regs.y; // carry is saved
        high = mem_map[ptr + 1];
        exec_s->data = &mem_map[(high << 8) + low];
        exec_s->pc_incr = 2;
        break;
        case zero_page_x: ;
        ptr = (mem_map[pc(exec_s->regs) + 1] + exec_s->regs.x) & 0x00FF;
        exec_s->data = &mem_map[ptr];
        exec_s->pc_incr = 2;
        break;
        case absolute_x: ;
        high = mem_map[pc(exec_s->regs) + 2];
        low = (mem_map[pc(exec_s->regs) + 1] + exec_s->regs.x) & 0x00FF;
        exec_s->data = &mem_map[(high << 8) + low];
        exec_s->pc_incr = 3;
        break;
        case absolute_y: ;
        high = mem_map[pc(exec_s->regs) + 2];
        low = (mem_map[pc(exec_s->regs) + 1] + exec_s->regs.y) & 0x00FF;
        exec_s->data = &mem_map[(high << 8) + low];
        exec_s->pc_incr = 3;
        break;
        case relative: ;
        exec_s->data = &mem_map[pc(exec_s->regs) + 1];
        exec_s->pc_incr = 2;
        break;
        case indirect: ;
        low = mem_map[pc(exec_s->regs) + 1];
        high = mem_map[pc(exec_s->regs) + 2];
        exec_s->ptrh = high;
        exec_s->ptrl = low;
        exec_s->data = &mem_map[(high << 8)+ low];
        exec_s->pc_incr = 3;
        break;
        case zero_page_y: ;
        ptr = (mem_map[pc(exec_s->regs) + 1] + exec_s->regs.y) & 0x00FF;
        exec_s->data = &mem_map[ptr];
        exec_s->pc_incr = 2;
        default:
            return;
    }
}

void execute(es *exec_s)
{
    instr i = instr_set[exec_s->regs.ir];
    i.func(exec_s);
}

bool brk[0x10000] = {0};

/*
 * void wait_for_input()
 * {
 *    char *line;
 *    bool _continue = 0;
 *    int option_index = 0;
 *    do
 *    {
 *        printf("dbg> ");
 *        size_t len = 0;
 *        ssize_t nread;
 *        nread = getline(&line, &len, stdin);
 *        printf("Line: %s", line);
 *        if(nread == -1)
 *        {
 *            printf("Oi, something went wrong\n");
 *            exit(-1);
 *        }       
 *        char **av;
 *        int ac = split(line, ' ', &av);
 *        if('\n' != av[0][0])
 *        {
 *            int length = ac + 1;
 *            char **tmp = malloc((length)*sizeof(char*));
 *            tmp[0] = malloc(4*sizeof(char));
 *            for(int i = 1; i < length; i++)
 *            {
 *                tmp[i] = malloc(strlen(av[i-1]) + 1);
 *            }
 *            //tmp[ac-1] = malloc(sizeof(char));
 *            strcpy(tmp[0], "dbg");
 *            for(int i = 1; i < length; i++)
 *            {
 *                strcpy(tmp[i], av[i-1]);
 *            }
 *            //strcpy(tmp[ac-1], "");
 *            av = tmp;
 *            //optind = 0; // reset getopt
 *            //opterr = 1;
 *            int c = 0;
 *            av[length-1][strlen(av[length-1])-1] = '\0';
 *            uint16_t address;
 *            char *end_ptr = 0;
 *            printf("count = %d\n", ac);
 *            for(int i = 0; i <= ac; i++) printf("Arr: %s; length = %lu\n",av[i],strlen(av[i]));
 *            option_index = 0;
 *            static const struct option long_options[] = {
 *                   {"breakpoint",     required_argument, 0, 'b'},
 *                   {"delete",  required_argument, 0, 'd'},
 *                   {"ls",  no_argument, 0, 'l'},
 *                   {"continue", no_argument, 0, 'c'},
 *                   {"exit", no_argument, 0, 'e'},
 *                   {"help", no_argument, 0, 'h'},
 *                   {"run", no_argument, 0, 'r'},
 *                   {"",0,0,0}
 *            };
 *            printf("getopt = %d\n", getopt_long(length, av, "b:cd:ehlr", long_options, &option_index));
 *            while ((c = getopt_long(length, av, "b:cd:ehlr", long_options, &option_index)) != -1)
 *            {
 *                printf("%c", c);
 *                switch(c)
 *                {
 *                    case 'b': ;
 *                        printf("breakpoint set at %s", optarg);
 *                        address = (uint16_t) strtol(optarg, &end_ptr, 16);
 *                        if((end_ptr == optarg))
 *                        {
 *                            printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", optarg);
 *                        }
 *                        else brk[address] = true;               
 *                    break;
 *                    case 'c': ;
 *                    printf("c");
 *                        _continue = true;
 *                    break;
 *                    case 'd': ;
 *                    printf("breakpoint removed at %s", optarg);
 *                        address = (uint16_t) strtol(optarg, &end_ptr, 16);
 *                        if((end_ptr == optarg))
 *                        {
 *                            printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", optarg);
 *                        }
 *                        else brk[address] = false;
 *                    break;
 *                    case 'e': ;
 *                    printf("e");
 *                        exit(0);
 *                    break;
 *                    case 'h': ;
 *                    break;
 *                    case 'l': ;
 *                        printf("Breakpoint list: \n");
 *                        for(int i = 0x0000; i <= 0xFFFF; i++)
 *                        {
 *                            if(brk[i]) printf("0x%04X\n", i);
 *                        }
 *                    break;
 *                }
 *            }
 *        }
 *    } while (!_continue);
 * }
 */
// const char *argp_program_version = "veersion";
// const char *argp_program_bug_address = "<vak34@gmail.com>";
// static char doc[] = "Description";
// static char args_doc[] = "Doc";
// static struct argp_option options[] = {
//     {"breakpoint", 'b', "b_address", 0, "Set a breakpoint"},
//     {"delete", 'd', "d_address", 0, "Delete breakpoint"},
//     {"ls", 'l', 0, 0, "List of breakpoints"},
//     {"continue", 'c', 0, 0, "Continue"},
//     {"exit", 'e', 0, 0, "Exit"},
//     {"help", 'h', 0, 0, "Show help"},
//     {"run", 'r', 0, 0, "Run"},
//     {0}
// }
// 
// struct arguments {
//     char *b_address;
//     char *d_address;
//     bool cont; 
// }
// 
// static error_t parse_opt(int key, char *arg, struct argp_state *state) {
//     struct arguments *arguments = state -> input;
//     switch (key) {
//     case 'b': arguments -> b_address = state -> b_address; break;
//     case 'd': arguments -> d_address = state -> d_address; break;
//     case 'c': arguments -> cont = true; break;
//     case 'e': exit(0); break;
//     
//     case 'l':
//         printf("Breakpoint list: \n");
//         for(int i = 0x0000; i <= 0xFFFF; i++)
//         {
//             if(brk[i]) printf("0x%04X\n", i);
//         }
//     case ARGP_KEY_ARG: return 0;
//     default: return ARGP_ERR_UNKNOWN;
//     }   
//     return 0;
// }
// 
// static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };
// 
// void input_argp(void)
// {
//     char *line;
//     bool _continue = 0;
//     do
//     {
//         printf("dbg> ");
//         size_t len = 0;
//         ssize_t nread;
//         nread = getline(&line, &len, stdin);
//         printf("Line: %s", line);
//     } while (!_continue)
//     
// }

void comd_line_stub(char *line, int *bp, es exec_s)
{
    uint16_t adr;
    if(line[0] == 'l'  && line[1] == 's')
    {
        printf("Breakpoint list: \n");
        for(int i = 0x0000; i <= 0xFFFF; i++)
        {
            if(brk[i]) printf("0x%04X\n", i);
        }
        *bp = 1;
        return;
    } else
        if(line[0] == 'b')
        {
            char adr_str[5] = {line[2], line[3], line[4], line[5], '\0'};
            char *end_ptr;
            //printf("\n 1 \n");
            adr = (uint16_t) strtoul(adr_str, &end_ptr, 16);
            //printf("\n 2 : 0x%x\n", adr);
            if((end_ptr == adr_str))
            {
                printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", adr_str);
            }
            else 
            {
                brk[adr] = true;
                printf("Breakpoint set at 0x%s\n", adr_str);
            }
            *bp = 1;
            return;
        } else
            if(line[0] == 'd')
            {
                char adr_str[5] = {line[2], line[3], line[4], line[5], '\0'};
                char *end_ptr;
                adr = (uint16_t) strtol(adr_str, &end_ptr, 16);
                if((end_ptr == adr_str))
                {
                    printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", optarg);
                }
                else
                {
                    brk[adr] = false;
                    printf("Breakpoint removed at 0x%s\n", adr_str);
                }
                *bp = 1;
                return;            
            } else
                if(line[0] == 'c')
                {
                    *bp = 0;
                    return;
                } else
                    if(line[0] == 'p')
                    {
                        switch(line[2])
                        {
                            case 'X': printf("X : %d\n", exec_s.regs.x); break;
                            case 'Y': printf("Y : %d\n", exec_s.regs.y); break;
                            case 'A': printf("Acc : %d\n", exec_s.regs.acc); break;
                            case 'S': printf("S : %d\n", exec_s.regs.sp); break;
                            case 'F': printf("F : %d\n", exec_s.regs.f); break;
                            case 'H': printf("PCH : %d\n", exec_s.regs.pch); break;
                            case 'L': printf("PCL : %d\n", exec_s.regs.pcl); break;
                            default: ;
                                char adr_str[5] = {line[2], line[3], line[4], line[5], '\0'};
                                char *end_ptr;
                                adr = (uint16_t) strtol(adr_str, &end_ptr, 16);
                                if((end_ptr == adr_str))
                                {
                                    printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", optarg);
                                }
                                else
                                {
                                    printf("Memory content at 0x%x is %x\n", adr, exec_s.mem_map[adr]);
                                }
                        }
                        *bp = 1;
                        return;
                    } else
                    {
                        printf("REEEEEE\n");
                        *bp = 1;
                        return;
                    }
}

void routine(bool debug, es exec_s)
{
    if(debug)
    {
        int bp = 1;
        while(1)
        {
            char *line;
            while(bp)
            {
                printf("dbg> ");
                size_t len = 0;
                ssize_t nread;
                nread = getline(&line, &len, stdin);
                //printf("Line: %s", line);
                if(nread == -1)
                {
                    printf("Oi, something went wrong\n");
                    exit(-1);
                }
                comd_line_stub(line, &bp, exec_s);
            }
            if(brk[pc(exec_s.regs)])
            {
                bp = 1;
                printf("Breakpoint 0x%x. FC, what do? \n", pc(exec_s.regs));
            }
            fetch(&exec_s);
            decode(&exec_s);
            execute(&exec_s);
        }
    }
    else
    {
        while(1)
        {
            fetch(&exec_s);
            decode(&exec_s);
            execute(&exec_s);
        }
    }
}
