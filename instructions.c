void ORA(es *exec_s)
{
	exec_s->regs.acc |= *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void AND(es *exec_s)
{
	exec_s->regs.acc &= *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void EOR(es *exec_s)
{
	exec_s->regs.acc ^= *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	//change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ADC(es *exec_s)
{
	int16_t acc_val = (int16_t) exec_s->regs.acc; // & 0x7F;
    #ifdef DEBUG
        printf("Accumulator content = 0x%X;\n", exec_s->regs.acc);
        printf("exec_s->data = 0x%X;\n", *exec_s->data);
    #endif
	int16_t mem_val = (int16_t) *exec_s->data; // & 0x7F;
    #ifdef DEBUG
    printf("Memory content = 0x%X;\n", mem_val);
    #endif
    if(check_bit(&(exec_s->regs.f), CARRY)) acc_val++;
    int16_t result = acc_val + mem_val;
    exec_s->regs.acc = (int8_t) (result & 0xFF);
    if((acc_val^result)&(mem_val^result)&0x80) set_overflow(&(exec_s->regs.f));
    else clear_overflow(&(exec_s->regs.f));
    if(result & 0x100) set_carry(&(exec_s->regs.f));
    else clear_carry(&(exec_s->regs.f));
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
    #ifdef DEBUG
    printf("Next pc = 0x%X\n", pc(exec_s->regs));
    #endif
}

void SBC(es *exec_s)
{
    int16_t acc_val = (int16_t) exec_s->regs.acc; // & 0x7F;
    #ifdef DEBUG
        printf("Accumulator content = 0x%X;\n", exec_s->regs.acc);
        printf("exec_s->data = 0x%X;\n", *exec_s->data);
    #endif
	int16_t mem_val = (int16_t) *exec_s->data; // & 0x7F;
    #ifdef DEBUG
    printf("Memory content = 0x%X;\n", mem_val);
    #endif
    if(!check_bit(&(exec_s->regs.f), CARRY)) acc_val++;
    int16_t result = acc_val + (0xFF - mem_val);
    exec_s->regs.acc = (int8_t) (result & 0xFF);
	if((acc_val^result)&(mem_val^result)&0x80) set_overflow(&(exec_s->regs.f));
    else clear_overflow(&(exec_s->regs.f));
    if(result & 0x100) clear_carry(&(exec_s->regs.f));
    else set_carry(&(exec_s->regs.f));
    set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
    #ifdef DEBUG
    printf("Next pc = 0x%X\n", pc(exec_s->regs));
    #endif
}

void CMP(es *exec_s)
{
	uint16_t result = exec_s->regs.acc - *exec_s->data;
	set_zero(&(exec_s->regs.f), (uint8_t) result);
	set_neg(&(exec_s->regs.f), (uint8_t) result);
    set_carry(&(exec_s->regs.f));
    if(result & 0x100) clear_carry(&(exec_s->regs.f));
    else set_carry(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CPX(es *exec_s)
{
	uint8_t result = exec_s->regs.x - *exec_s->data;
	set_zero(&(exec_s->regs.f), result);
	set_neg(&(exec_s->regs.f), result);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CPY(es *exec_s)
{
	uint8_t result = exec_s->regs.y - *exec_s->data;
	set_zero(&(exec_s->regs.f), result);
	set_neg(&(exec_s->regs.f), result);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void DEC(es *exec_s)
{
	(*exec_s->data)--;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void DEX(es *exec_s)
{
	exec_s->regs.x--;
	set_zero(&(exec_s->regs.f), exec_s->regs.x);
	set_neg(&(exec_s->regs.f), exec_s->regs.x);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void DEY(es *exec_s)
{
	exec_s->regs.y--;
	set_zero(&(exec_s->regs.f), exec_s->regs.y);
	set_neg(&(exec_s->regs.f), exec_s->regs.y);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void INC(es *exec_s)
{
	(*exec_s->data)++;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void INX(es *exec_s)
{
	exec_s->regs.x++;
	set_zero(&(exec_s->regs.f), exec_s->regs.x);
	set_neg(&(exec_s->regs.f), exec_s->regs.x);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void INY(es *exec_s)
{
	exec_s->regs.y++;
	set_zero(&(exec_s->regs.f), exec_s->regs.y);
	set_neg(&(exec_s->regs.f), exec_s->regs.y);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ASL(es *exec_s)
{
	*exec_s->data <<= 1;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ROL(es *exec_s)
{
	*exec_s->data = (*exec_s->data << 1) | (*exec_s->data >> ((sizeof(*exec_s->data) << 3) - 1));
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void LSR(es *exec_s)
{
	*exec_s->data >>= 1;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ROR(es *exec_s)
{
	*exec_s->data = (*exec_s->data >> 1) | (*exec_s->data << ((sizeof(*exec_s->data) << 3) - 1));
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

//Move commands

void LDA(es *exec_s)
{
	exec_s->regs.acc = *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void STA(es *exec_s)
{
	*exec_s->data = exec_s->regs.acc;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void LDX(es *exec_s)
{
	exec_s->regs.x = *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.x);
	set_neg(&(exec_s->regs.f), exec_s->regs.x);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void STX(es *exec_s)
{
	*exec_s->data = exec_s->regs.x;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void LDY(es *exec_s)
{
	exec_s->regs.y = *exec_s->data;
	set_zero(&(exec_s->regs.f), exec_s->regs.y);
	set_neg(&(exec_s->regs.f), exec_s->regs.y);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void STY(es *exec_s)
{
	*exec_s->data = exec_s->regs.x;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TAX(es *exec_s)
{
	exec_s->regs.x = exec_s->regs.acc;
	set_zero(&(exec_s->regs.f), exec_s->regs.x);
	set_neg(&(exec_s->regs.f), exec_s->regs.x);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TXA(es *exec_s)
{
	exec_s->regs.acc = exec_s->regs.x;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TAY(es *exec_s)
{
	exec_s->regs.y = exec_s->regs.acc;
	set_zero(&(exec_s->regs.f), exec_s->regs.y);
	set_neg(&(exec_s->regs.f), exec_s->regs.y);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TYA(es *exec_s)
{
	exec_s->regs.acc = exec_s->regs.x;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TSX(es *exec_s)
{
	exec_s->regs.x = exec_s->regs.sp;
	set_zero(&(exec_s->regs.f), exec_s->regs.x);
	set_neg(&(exec_s->regs.f), exec_s->regs.x);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TXS(es *exec_s)
{
	exec_s->regs.sp = exec_s->regs.x;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void PLA(es *exec_s)
{
	exec_s->regs.acc = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	set_zero(&(exec_s->regs.f), exec_s->regs.acc);
	set_neg(&(exec_s->regs.f), exec_s->regs.acc);
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void PHA(es *exec_s)
{
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.acc;
	exec_s->regs.sp--;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void PLP(es *exec_s)
{
	exec_s->regs.f = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void PHP(es *exec_s)
{
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.f;
	exec_s->regs.sp--;
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

//Jump/Flag commands

void BPL(es *exec_s)
{
	if(check_bit(&(exec_s->regs.f), NEGATIVE)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BMI(es *exec_s)
{
	if(!check_bit(&(exec_s->regs.f), NEGATIVE)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BVC(es *exec_s)
{
	if(!check_bit(&(exec_s->regs.f), OVERFLOW)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BVS(es *exec_s)
{
	if(check_bit(&(exec_s->regs.f), OVERFLOW)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BCC(es *exec_s)
{
	if(check_bit(&(exec_s->regs.f), CARRY)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BCS(es *exec_s)
{
	if(!check_bit(&(exec_s->regs.f), CARRY)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
}

void BNE(es *exec_s)
{
	if(check_bit(&(exec_s->regs.f), ZERO)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
    #ifdef DEBUG
    printf("Next pc = 0x%X\n", pc(exec_s->regs));
    printf("data = 0x%x\n", *exec_s->data);
    #endif
}

void BEQ(es *exec_s)
{
	if(!check_bit(&(exec_s->regs.f), ZERO)) change_pc(&(exec_s->regs), exec_s->pc_incr);
	else change_pc(&(exec_s->regs), *exec_s->data + exec_s->pc_incr);
    #ifdef DEBUG
    printf("Next pc = 0x%X\n", pc(exec_s->regs));
    #endif
}

void BRK(es *exec_s)
{
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.pcl;
	exec_s->regs.sp--;
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.pch;
	exec_s->regs.sp--;
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.f;
	exec_s->regs.sp--;
	irq_disable(&(exec_s->regs.f));
	set_bit(&(exec_s->regs.f), BREAK);
	exec_s->regs.pch = 0xFF;
	exec_s->regs.pcl = 0xFE; 
}

void RTI(es *exec_s)
{
	exec_s->regs.f = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	exec_s->regs.pch = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	exec_s->regs.pcl = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	irq_enable(&(exec_s->regs.f));
	clear_bit(&(exec_s->regs.f), BREAK);
}

void JSR(es *exec_s)
{
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.pch;
	exec_s->regs.sp--;
	exec_s->mem_map[sp(exec_s->regs)] = exec_s->regs.pcl;
	exec_s->regs.sp--;
	exec_s->regs.pcl = exec_s->ptrl;
	exec_s->regs.pch = exec_s->ptrh;
}

void RTS(es *exec_s)
{
	exec_s->regs.pcl = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
	exec_s->regs.pch = exec_s->mem_map[sp(exec_s->regs)];
	exec_s->regs.sp++;
}

void JMP(es *exec_s)
{
	exec_s->regs.pcl = exec_s->ptrl;
	exec_s->regs.pch = exec_s->ptrh;
}

void BIT(es *exec_s)
{
	uint8_t result = (exec_s->regs.acc & *exec_s->data);
	set_zero(&(exec_s->regs.f), result);
	set_neg(&(exec_s->regs.f), (*exec_s->data));
	if(check_bit(exec_s->data, OVERFLOW)) set_overflow(&(exec_s->regs.f));
	else clear_overflow(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CLC(es *exec_s)
{
	clear_carry(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SEC(es *exec_s)
{
	set_carry(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CLD(es *exec_s)
{
	clear_decimal(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SED(es *exec_s)
{
	set_decimal(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CLI(es *exec_s)
{
	irq_enable(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SEI(es *exec_s)
{
	irq_disable(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void CLV(es *exec_s)
{
	clear_overflow(&(exec_s->regs.f));
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void NOP(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

// Illegal opcodes?

void SLO(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void RLA(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SRE(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void RRA(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SAX(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void LAX(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void DCP(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ISC(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ANC(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ALR(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void ARR(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void XAA(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void AXS(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void AHX(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SHY(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void SHX(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void TAS(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void LAS(es *exec_s)
{
	change_pc(&(exec_s->regs), exec_s->pc_incr);
}

void KIL(es *exec_s) 
{
	printf("KIL instruction at %02x%02x, emulation terminated...\n", exec_s->regs.pch, exec_s->regs.pcl);
	exit(0);
}
