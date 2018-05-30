/* #ifndef es *exec_s 
#define es *exec_s uint8_t data, uint8_t *mem_map, rf regs, uint16_t pc_incr
#endif */

typedef enum address_mode
{
	immediate, //2nd byte -- operand; imm, immediate
	absolute, //2nd byte -- lo, 3rd byte -- hi; absolute, abs
	zero_page, //2nd byte -- lo, hi == 00h; zp, zero page
	accumulator, // one byte instruction; accum., imp
	implied, // address is impicitly stated in the instruction; implied, imp
	indirect_x, // lo == zero_page[2nd byte+x], hi == zero_page[2nd byte+x+1]; indX, izx
	indirect_y, // lo == zero_page[2nd byte+y], hi == zero_page[2nd byte+y+1]; indY, izy 
	zero_page_x, // lo == x + 2nd byte, hi == 00h; zpx
	absolute_x, // lo == 2nd byte + x; abx
	absolute_y, // hi == 3rd byte + y; aby
	relative, // lo += 2nd byte, signed; rel
	indirect, // lo == memory[2nd byte]; hi == memory[3rd byte];
	zero_page_y // lo == x + 2nd byte, hi == 00h; zpy
} am;

typedef struct reg_file
{
	uint8_t acc;
	uint8_t x;
	uint8_t y;
	uint8_t pch;
	uint8_t pcl;
	uint8_t sp;
	uint8_t f;
	uint8_t ir;
} rf;

typedef struct exec_struct
{
	uint8_t *data;
	uint8_t ptrh;
	uint8_t ptrl;
	uint8_t *mem_map;
	rf regs;
	uint16_t pc_incr;
} es;

typedef struct instruction
{
	char *mnemonic;
	short cycles;
	uint8_t opcode;
	void (*func)(es *exec_s);
	am addressing;
} instr;

extern void BRK(es *exec_s);
extern void ORA(es *exec_s);
extern void SLO(es *exec_s);
extern void KIL(es *exec_s);
extern void AND(es *exec_s);
extern void EOR(es *exec_s);
extern void ADC(es *exec_s);
extern void SBC(es *exec_s);
extern void CMP(es *exec_s);
extern void CPX(es *exec_s);
extern void CPY(es *exec_s);
extern void DEX(es *exec_s);
extern void DEY(es *exec_s);
extern void INC(es *exec_s);
extern void INX(es *exec_s);
extern void INY(es *exec_s);
extern void ASL(es *exec_s);
extern void ROL(es *exec_s);
extern void LSR(es *exec_s);
extern void ROR(es *exec_s);
extern void LDA(es *exec_s);
extern void STA(es *exec_s);
extern void LDX(es *exec_s);
extern void STX(es *exec_s);
extern void LDY(es *exec_s);
extern void STY(es *exec_s);
extern void TAX(es *exec_s);
extern void TXA(es *exec_s);
extern void TAY(es *exec_s);
extern void TYA(es *exec_s);
extern void TSX(es *exec_s);
extern void TXS(es *exec_s);
extern void PLA(es *exec_s);
extern void PHA(es *exec_s);
extern void PLP(es *exec_s);
extern void PHP(es *exec_s);
extern void BPL(es *exec_s);
extern void BMI(es *exec_s);
extern void BVC(es *exec_s);
extern void BCC(es *exec_s);
extern void BCS(es *exec_s);
extern void BNE(es *exec_s);
extern void BEQ(es *exec_s);
extern void RTI(es *exec_s);
extern void JSR(es *exec_s);
extern void JMP(es *exec_s);
extern void BIT(es *exec_s);
extern void CLC(es *exec_s);
extern void SEC(es *exec_s);
extern void CLD(es *exec_s);
extern void SED(es *exec_s);
extern void CLI(es *exec_s);
extern void SEI(es *exec_s);
extern void CLV(es *exec_s);
extern void NOP(es *exec_s);
extern void RTS(es *exec_s);
extern void RLA(es *exec_s);
extern void SRE(es *exec_s);
extern void RRA(es *exec_s);
extern void SAX(es *exec_s);
extern void LAX(es *exec_s);
extern void DCP(es *exec_s);
extern void ISC(es *exec_s);
extern void ANC(es *exec_s);
extern void ALR(es *exec_s);
extern void ARR(es *exec_s);
extern void XAA(es *exec_s);
extern void AXS(es *exec_s);
extern void AHX(es *exec_s);
extern void SHY(es *exec_s);
extern void SHX(es *exec_s);
extern void TAS(es *exec_s);
extern void LAS(es *exec_s);
extern void BVS(es *exec_s);
extern void DEC(es *exec_s);

struct instruction instr_set[256] =
{
	{"BRK", 7, 0x00, BRK, implied},
	{"ORA", 6, 0x01, ORA, indirect_x},
    {"KIL", 0, 0x02, KIL, implied},
    {"SLO", 8, 0x03, SLO, indirect_x},
    {"NOP", 3, 0x04, NOP, zero_page},
    {"ORA", 3, 0x05, ORA, zero_page},
    {"ASL", 5, 0x06, ASL, zero_page},
    {"SLO", 5, 0x07, SLO, zero_page},
    {"PHP", 3, 0x08, PHP, implied},
    {"ORA", 2, 0x09, ORA, immediate},
    {"ASL", 2, 0x0A, ASL, accumulator},
    {"ANC", 2, 0x0B, ANC, immediate},
    {"NOP", 4, 0x0C, NOP, absolute},
    {"ORA", 4, 0x0D, ORA, absolute},
    {"ASL", 6, 0x0E, ASL, absolute},
    {"SLO", 6, 0x0F, SLO, absolute},
    {"BPL", 2, 0x10, BPL, relative},
    {"ORA", 5, 0x11, ORA, indirect_y},
    {"KIL", 0, 0x12, KIL, implied},
    {"SLO", 8, 0x13, SLO, indirect_y},
    {"NOP", 4, 0x14, NOP, zero_page_x},
    {"ORA", 4, 0x15, ORA, zero_page_x},
    {"ASL", 6, 0x16, ASL, zero_page_x},
    {"SLO", 6, 0x17, SLO, zero_page_x},
    {"CLC", 2, 0x18, CLC, implied},
    {"ORA", 4, 0x19, ORA, absolute_y},
    {"NOP", 2, 0x1A, NOP, implied},
    {"SLO", 7, 0x1B, SLO, absolute_y},
    {"NOP", 4, 0x1C, NOP, absolute_x},
    {"ORA", 4, 0x1D, ORA, absolute_x},
    {"ASL", 7, 0x1E, ASL, absolute_x},
    {"SLO", 7, 0x1F, SLO, absolute_x},
    {"JSR", 6, 0x20, JSR, absolute},
    {"AND", 6, 0x21, AND, indirect_x},
    {"KIL", 0, 0x22, KIL, implied},
    {"RLA", 8, 0x23, RLA, indirect_x},
    {"BIT", 3, 0x24, BIT, zero_page},
    {"AND", 3, 0x25, AND, zero_page},
    {"ROL", 5, 0x26, ROL, zero_page},
    {"RLA", 5, 0x27, RLA, zero_page},
    {"PLP", 4, 0x28, PLP, implied},
    {"AND", 2, 0x29, AND, immediate},
    {"ROL", 2, 0x2A, ROL, accumulator},
    {"ANC", 2, 0x2B, ANC, immediate},
    {"BIT", 4, 0x2C, BIT, absolute},
    {"AND", 4, 0x2D, AND, absolute},
    {"ROL", 6, 0x2E, ROL, absolute},
    {"RLA", 6, 0x2F, RLA, absolute},
    {"BMI", 2, 0x30, BMI, relative},
    {"AND", 5, 0x31, AND, indirect_y},
    {"KIL", 0, 0x32, KIL, implied},
    {"RLA", 8, 0x33, RLA, indirect_y},
    {"NOP", 4, 0x34, NOP, zero_page_x},
    {"AND", 4, 0x35, AND, zero_page_x},
    {"ROL", 6, 0x36, ROL, zero_page_x},
    {"RLA", 6, 0x37, RLA, zero_page_x},
    {"SEC", 2, 0x38, SEC, implied},
    {"AND", 4, 0x39, AND, absolute_y},
    {"NOP", 2, 0x3A, NOP, implied},
    {"RLA", 7, 0x3B, RLA, absolute_y},
    {"NOP", 4, 0x3C, NOP, absolute_x},
    {"AND", 4, 0x3D, AND, absolute_x},
    {"ROL", 7, 0x3E, ROL, absolute_x},
    {"RLA", 7, 0x3F, RLA, absolute_x},
    {"RTI", 6, 0x40, RTI, implied},
    {"EOR", 6, 0x41, EOR, indirect_x},
    {"LSR", 0, 0x42, LSR, indirect_x},
    {"SRE", 8, 0x43, SRE, indirect_x},
    {"NOP", 3, 0x44, NOP, zero_page},
    {"EOR", 3, 0x45, EOR, zero_page},
    {"LSR", 5, 0x46, LSR, zero_page},
    {"SRE", 5, 0x47, SRE, zero_page},
    {"PHA", 3, 0x48, PHA, implied},
    {"EOR", 2, 0x49, EOR, immediate},
    {"LSR", 2, 0x4A, LSR, accumulator},
    {"ALR", 2, 0x4B, ALR, immediate},
    {"JMP", 3, 0x4C, JMP, absolute},
    {"EOR", 4, 0x4D, EOR, absolute},
    {"LSR", 6, 0x4E, LSR, absolute},
    {"SRE", 6, 0x4F, SRE, absolute},
    {"BVC", 2, 0x50, BVC, relative},
    {"EOR", 5, 0x51, EOR, indirect_y},
    {"KIL", 0, 0x52, KIL, indirect_y},
    {"SRE", 8, 0x53, SRE, indirect_y},
    {"NOP", 4, 0x54, NOP, zero_page_x},
    {"EOR", 4, 0x55, EOR, zero_page_x},
    {"LSR", 6, 0x56, LSR, zero_page_x},
    {"SRE", 6, 0x57, SRE, zero_page_x},
    {"CLI", 2, 0x58, CLI, implied},
    {"EOR", 4, 0x59, EOR, absolute_y},
    {"NOP", 2, 0x5A, NOP, absolute_y},
    {"SRE", 7, 0x5B, SRE, absolute_y},
    {"NOP", 4, 0x5C, NOP, absolute_x},
    {"EOR", 4, 0x5D, EOR, absolute_x},
    {"LSR", 7, 0x5E, LSR, absolute_x},
    {"SRE", 7, 0x5F, SRE, absolute_x},
    {"RTS", 6, 0x60, RTS, implied},
    {"ADC", 6, 0x61, ADC, indirect_x},
    {"KIL", 0, 0x62, KIL, indirect_x},
    {"RRA", 8, 0x63, RRA, indirect_x},
    {"NOP", 3, 0x64, NOP, zero_page},
    {"ADC", 3, 0x65, ADC, zero_page},
    {"ROR", 5, 0x66, ROR, zero_page},
    {"RRA", 5, 0x67, RRA, zero_page},
    {"PLA", 4, 0x68, PLA, implied},
    {"ADC", 2, 0x69, ADC, immediate},
    {"ROR", 2, 0x6A, ROR, accumulator},
    {"ARR", 2, 0x6B, ARR, immediate},
    {"JMP", 5, 0x6C, JMP, indirect},
    {"ADC", 4, 0x6D, ADC, absolute},
    {"ROR", 6, 0x6E, ROR, absolute},
    {"RRA", 6, 0x6F, RRA, absolute},
    {"BVS", 2, 0x70, BVS, relative},
    {"ADC", 5, 0x71, ADC, indirect_y},
    {"KIL", 0, 0x72, KIL, indirect_y},
    {"RRA", 8, 0x73, RRA, indirect_y},
    {"NOP", 4, 0x74, NOP, zero_page_x},
    {"ADC", 4, 0x75, ADC, zero_page_x},
    {"ROR", 6, 0x76, ROR, zero_page_x},
    {"RRA", 6, 0x77, RRA, zero_page_x},
    {"SEI", 2, 0x78, SEI, implied},
    {"ADC", 4, 0x79, ADC, absolute_y},
    {"NOP", 2, 0x7A, NOP, absolute_y},
    {"RRA", 7, 0x7B, RRA, absolute_y},
    {"NOP", 4, 0x7C, NOP, absolute_x},
    {"ADC", 4, 0x7D, ADC, absolute_x},
    {"ROR", 7, 0x7E, ROR, absolute_x},
    {"RRA", 7, 0x7F, RRA, absolute_x},
    {"NOP", 2, 0x80, NOP, immediate},
    {"STA", 6, 0x81, STA, indirect_x},
    {"NOP", 2, 0x82, NOP, immediate},
    {"SAX", 6, 0x83, SAX, indirect_x},
    {"STY", 3, 0x84, STY, zero_page},
    {"STA", 3, 0x85, STA, zero_page},
    {"STX", 3, 0x86, STX, zero_page},
    {"SAX", 3, 0x87, SAX, zero_page},
    {"DEY", 2, 0x88, DEY, implied},
    {"NOP", 2, 0x89, NOP, immediate},
    {"TXA", 2, 0x8A, TXA, implied},
    {"XAA", 2, 0x8B, XAA, immediate},
    {"STY", 4, 0x8C, STY, absolute},
    {"STA", 4, 0x8D, STA, absolute},
    {"STX", 4, 0x8E, STX, absolute},
    {"SAX", 4, 0x8F, SAX, absolute},
    {"BCC", 2, 0x90, BCC, relative},
    {"STA", 6, 0x91, STA, indirect_y},
    {"KIL", 0, 0x92, KIL, indirect_y},
    {"AHX", 6, 0x93, AHX, indirect_y},
    {"STY", 4, 0x94, STY, zero_page_x},
    {"STA", 4, 0x95, STA, zero_page_x},
    {"STX", 4, 0x96, STX, zero_page_y},
    {"SAX", 4, 0x97, SAX, zero_page_y},
    {"TYA", 2, 0x98, TYA, implied},
    {"STA", 5, 0x99, STA, absolute_y},
    {"TXS", 2, 0x9A, TXS, implied},
    {"TAS", 5, 0x9B, TAS, absolute_y},
    {"SHY", 5, 0x9C, SHY, absolute_x},
    {"STA", 5, 0x9D, STA, absolute_x},
    {"SHX", 5, 0x9E, SHX, absolute_y},
    {"AHX", 5, 0x9F, AHX, absolute_y},
    {"LDY", 2, 0xA0, LDY, immediate},
    {"LDA", 6, 0xA1, LDA, indirect_x},
    {"LDX", 2, 0xA2, LDX, immediate},
    {"LAX", 6, 0xA3, LAX, indirect_x},
    {"LDY", 3, 0xA4, LDY, zero_page},
    {"LDA", 3, 0xA5, LDA, zero_page},
    {"LDX", 3, 0xA6, LDX, zero_page},
    {"LAX", 3, 0xA7, LAX, zero_page},
    {"TAY", 2, 0xA8, TAY, implied},
    {"LDA", 2, 0xA9, LDA, immediate},
    {"TAX", 2, 0xAA, TAX, implied},
    {"LAX", 2, 0xAB, LAX, absolute},
    {"LDY", 4, 0xAC, LDY, absolute},
    {"LDA", 4, 0xAD, LDA, absolute},
    {"LDX", 4, 0xAE, LDX, absolute},
    {"LAX", 4, 0xAF, LAX, absolute},
    {"BCS", 2, 0xB0, BCS, relative},
    {"LDA", 5, 0xB1, LDA, indirect_y},
    {"KIL", 0, 0xB2, KIL, indirect_y},
    {"LAX", 5, 0xB3, LAX, indirect_y},
    {"LDY", 4, 0xB4, LDY, zero_page_x},
    {"LDA", 4, 0xB5, LDA, zero_page_x},
    {"LDX", 4, 0xB6, LDX, zero_page_y},
    {"LAX", 4, 0xB7, LAX, zero_page_y},
    {"CLV", 2, 0xB8, CLV, implied},
    {"LDA", 4, 0xB9, LDA, absolute_y},
    {"TSX", 2, 0xBA, TSX, implied},
    {"LAS", 4, 0xBB, LAS, absolute_y},
    {"LDY", 4, 0xBC, LDY, absolute_x},
    {"LDA", 4, 0xBD, LDA, absolute_x},
    {"LDX", 4, 0xBE, LDX, absolute_y},
    {"LAX", 4, 0xBF, LAX, absolute_y},
    {"CPY", 2, 0xC0, CPY, immediate},
    {"CMP", 6, 0xC1, CMP, indirect_x},
    {"NOP", 2, 0xC2, NOP, immediate},
    {"DCP", 8, 0xC3, DCP, indirect_x},
    {"CPY", 3, 0xC4, CPY, zero_page},
    {"CMP", 3, 0xC5, CMP, zero_page},
    {"DEC", 5, 0xC6, DEC, zero_page},
    {"DCP", 5, 0xC7, DCP, zero_page},
    {"INY", 2, 0xC8, INY, implied},
    {"CMP", 2, 0xC9, CMP, immediate},
    {"DEX", 2, 0xCA, DEX, implied},
    {"AXS", 2, 0xCB, AXS, immediate},
    {"CPY", 4, 0xCC, CPY, absolute},
    {"CMP", 4, 0xCD, CMP, absolute},
    {"DEC", 6, 0xCE, DEC, absolute},
    {"DCP", 6, 0xCF, DCP, absolute},
    {"BNE", 2, 0xD0, BNE, relative},
    {"CMP", 5, 0xD1, CMP, indirect_y},
    {"KIL", 0, 0xD2, KIL, indirect_y},
    {"DCP", 8, 0xD3, DCP, indirect_y},
    {"NOP", 4, 0xD4, NOP, zero_page_x},
    {"CMP", 4, 0xD5, CMP, zero_page_x},
    {"DEC", 6, 0xD6, DEC, zero_page_x},
    {"DCP", 6, 0xD7, DCP, zero_page_x},
    {"CLD", 2, 0xD8, CLD, implied},
    {"CMP", 4, 0xD9, CMP, absolute_y},
    {"NOP", 2, 0xDA, NOP, absolute_y},
    {"DCP", 7, 0xDB, DCP, absolute_y},
    {"NOP", 4, 0xDC, NOP, absolute_x},
    {"CMP", 4, 0xDD, CMP, absolute_x},
    {"DEC", 7, 0xDE, DEC, absolute_x},
    {"DCP", 7, 0xDF, DCP, absolute_x},
    {"CPX", 2, 0xE0, CPX, immediate},
    {"SBC", 6, 0xE1, SBC, indirect_x},
    {"NOP", 2, 0xE2, NOP, immediate},
    {"ISC", 8, 0xE3, ISC, indirect_x},
    {"CPX", 3, 0xE4, CPX, zero_page},
    {"SBC", 3, 0xE5, SBC, zero_page},
    {"INC", 5, 0xE6, INC, zero_page},
    {"ISC", 5, 0xE7, ISC, zero_page},
    {"INX", 2, 0xE8, INX, implied},
    {"SBC", 2, 0xE9, SBC, immediate},
    {"NOP", 2, 0xEA, NOP, implied},
    {"SBC", 2, 0xEB, SBC, immediate},
    {"CPX", 4, 0xEC, CPX, absolute},
    {"SBC", 4, 0xED, SBC, absolute},
    {"INC", 6, 0xEE, INC, absolute},
    {"ISC", 6, 0xEF, ISC, absolute},
    {"BEQ", 2, 0xF0, BEQ, relative},
    {"SBC", 5, 0xF1, SBC, indirect_y},
    {"KIL", 0, 0xF2, KIL, indirect_y},
    {"ISC", 8, 0xF3, ISC, indirect_y},
    {"NOP", 4, 0xF4, NOP, zero_page_x},
    {"SBC", 4, 0xF5, SBC, zero_page_x},
    {"INC", 6, 0xF6, INC, zero_page_x},
    {"ISC", 6, 0xF7, ISC, zero_page_x},
    {"SED", 2, 0xF8, SED, implied},
    {"SBC", 4, 0xF9, SBC, absolute_y},
    {"NOP", 2, 0xFA, NOP, absolute_y},
    {"ISC", 7, 0xFB, ISC, absolute_y},
    {"NOP", 4, 0xFC, NOP, absolute_x},
    {"SBC", 4, 0xFD, SBC, absolute_x},
    {"INC", 7, 0xFE, INC, absolute_x},
    {"ISC", 7, 0xFF, ISC, absolute_x}
};
