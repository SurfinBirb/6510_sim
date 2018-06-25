#define _GNU_SOURCE
#define MAXARGS 128
#define LINELEN 256
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <argp.h>

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

int bp = 1;
bool brk[0x10000] = {0};

int getnonblank()
{
	int ch;
	for (;;) { 
		ch  = getchar();
		if (ch == '\n') break;
		if (ch == EOF) break;
		if (ch > ' ') break;
	}
	return ch;
}

int get_args( char * argv[], char line[] )
{
	int argc;
	int len;
	int ch;
    argv[0] = "dbg";
	len = 0;
	argc = 1;
	for (;;) {
		ch = getnonblank();
		if (ch == '\n') break;
		if (ch == EOF) break;

		argv[argc] = &line[len];
		if (argc >= (MAXARGS-1)) {
			fputs( "too many arguments", stdout );
		} else {
			argc++;
		}
		line[len] = ch;
		if (len >= (LINELEN-1)) {
			fputs( "line too long", stdout );
		} else {
			len++;
		}

		for (;;) { 
			ch  = getchar();
			if (ch == '\n') break;
			if (ch == EOF) break;
			if (ch <= ' ') break;

			line[len] = ch;
			if (len >= (LINELEN-1)) {
				fputs( "line too long", stdout );
			} else {
				len++;
			}
		}

		line[len] = '\0';
		if (len >= (LINELEN-1)) {
			fputs( "line too long", stdout );
		} else {
			len++;
		}

		if (ch == '\n') break;
		if (ch == EOF) break;
	}
	argv[argc] = NULL;

	if ((argc == 0) && (ch == EOF)) return -1;
	return argc;
}


const char *argp_program_version = "1.0";
const char *argp_program_bug_address = "<vak34@gmail.com>";
static char doc[] = "This is MOS Technology 6510 simulator in debug mode.";
static char args_doc[] = " ";
static struct argp_option options[] = {
    {"breakpoint", 'b', "address", 0, "Set a breakpoint",0},
    {"delete", 'd', "address", 0, "Delete breakpoint",0},
    {"ls", 'l', 0, 0, "List of breakpoints",0},
    {"continue", 'c', 0, 0, "Continue",0},
    {"exit", 'e', 0, 0, "Exit",0},
    {"run", 'r', 0, 0, "Run",0},
    {"print", 'p', "Memory location", 0, "Print contents of the memory location", 0},
    {0}
};

struct arguments {
    int print;
    int special;
    uint16_t content;
    char *a;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'b': ;
        char *end_ptr;
        int adr = strtoul(arg, &end_ptr, 16);
        if(arg != end_ptr && adr < 0x10000 && adr >= 0x0)
        {
            brk[(uint16_t) adr] = true;
            printf("Breakpoint set at 0x%s\n", arg);
        }
        else
        {
            printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", arg);
        }
        break;
    case 'd': ;
        end_ptr = NULL;
        adr = strtoul(arg, &end_ptr, 16);
        if(arg != end_ptr && adr < 0x10000 && adr >= 0x0)
        {
            brk[(uint16_t) adr] = false;
            printf("Breakpoint removed at 0x%s\n", arg);
        }
        else
        {
            printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", arg);
        }
        break;
    case 'c': bp = 0; break;
    case 'e': exit(0); break;
    case 'l':
        printf("Breakpoint list: \n");
        for(int i = 0x0000; i <= 0xFFFF; i++)
        {
            if(brk[i]) printf("0x%04X\n", i);
        }
        break;
    case 'r': 
        bp = 0;
        for(int i = 0x0000; i <= 0xFFFF; i++)
            brk[i] = false;
        break;
    case 'p': ;
        arguments->print = 1;
        if(strcmp("x", arg)== 0 || strcmp("X", arg)== 0) arguments->content = 1; else
        if(strcmp("y", arg)== 0 || strcmp("Y", arg)== 0) arguments->content = 2; else
        if(strcmp("a", arg)== 0 || strcmp("A", arg)== 0) arguments->content = 3; else
        if(strcmp("sp", arg)== 0 || strcmp("SP", arg)== 0) arguments->content = 4; else
        if(strcmp("f", arg)== 0 || strcmp("F", arg)== 0) arguments->content = 5; else
        if(strcmp("h", arg)== 0 || strcmp("H", arg)== 0) arguments->content = 6; else
        if(strcmp("l", arg)== 0 || strcmp("L", arg)== 0) arguments->content = 7; else
        {
            arguments->special = 1;
            arguments->a = arg;
        }
        break;
    case ARGP_KEY_ARG: return 0;
    default: return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

void routine(bool debug, es exec_s)
{
    if(debug)
    {
        while(1)
        {
            while(bp)
            {
                printf("dbg> ");
                char argv_buf[512];
                int ac;
                char * av[MAXARGS];
                ac = get_args(av, argv_buf);
                struct arguments arguments;
                arguments.print = 0;
                arguments.content = 0;
                arguments.special = 0;
                arguments.a = "";
                argp_parse (&argp, ac, av, 0, 0, &arguments);
                if(arguments.print){
                    if(arguments.special){
                        char *end_ptr = NULL;
                        int adr = strtoul(arguments.a, &end_ptr, 16);
                        if(arguments.a != end_ptr && adr < 0x10000 && adr >= 0x0)
                        {
                            printf("Memory content of 0x%X -- 0x%x\n", adr, exec_s.mem_map[adr]);
                        }
                        else
                        {
                        printf("%s is not a valid address (set address in 0x0000 - 0xFFFF range)\n", arguments.a);
                        }
                    }
                    else
                    {
                        switch(arguments.content){
                            case 1: ;
                            printf("x: 0x%x\n", exec_s.regs.x);
                            break;
                            case 2: ;
                            printf("y: 0x%x\n", exec_s.regs.y);
                            break;
                            case 3: ;
                            printf("a: 0x%x\n", exec_s.regs.acc);
                            break;
                            case 4: ;
                            printf("sp: 0x%x\n", exec_s.regs.sp);
                            break;
                            case 5: ;
                            printf("f: 0x%x\n", exec_s.regs.f);
                            break;
                            case 6: ;
                            printf("pc high byte: 0x%x\n", exec_s.regs.pch);
                            break;
                            case 7: ;
                            printf("pc low byte: 0x%x\n", exec_s.regs.pcl);
                            break;
                            default:
                            printf("Wrong memory location\n");
                        }
                    }
                }
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
