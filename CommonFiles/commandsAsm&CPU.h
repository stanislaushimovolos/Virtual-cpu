//
// Created by Tom on 22.10.2017.
//

enum {
	CMD_push = 1,
	CMD_pushReg,
	CMD_pushRam,
	CMD_pop,
	CMD_popReg,
	CMD_popRam,
	CMD_add,
	CMD_addReg,
	CMD_addRam,
	CMD_sub,
	CMD_subReg,
	CMD_subRam,
	CMD_mul,
	CMD_mulReg,
	CMD_mulRam,
	CMD_div,
	CMD_divReg,
	CMD_divRam,
	CMD_out,
	CMD_outReg,
	CMD_outRam,
	CMD_jmp,
	CMD_je,
	CMD_ret,
	CMD_in,
	CMD_ERR

};

#ifdef  DEF

#ifdef ASM
double funcDbHelper = 0;
int funcIntHelper = 0;
#endif

#define ASSIG_REG strchr ("abcd", (_arrayPtrCmd[codeCounter - 1 ])[0]) - "abcd" + 1;

#define NEXT_ELEM_CODE machCode[codeCounter++]

#define REG_CONDITION \
	_arrayPtrCmd[codeCounter + 1] != NULL &&_arrayPtrCmd[codeCounter + 1][1] == 'x' &&                                                                                 \
				strchr ("abcd", (_arrayPtrCmd[codeCounter + 1])[0]) &&                                      \
				codeCounter  < sizeOfMachCode - 1 &&                                                        \
				(_arrayPtrCmd[codeCounter + 1])[2] == 0

#define RAM_CONDITION                                                                                       \
_arrayPtrCmd[codeCounter + 1] != NULL                                                                       \
									&& codeCounter  < sizeOfMachCode - 1                                    \
									&& sscanf(_arrayPtrCmd[codeCounter + 1], "[%d]", &funcIntHelper )!= 0   \

#define VALUE_CONDITION _arrayPtrCmd[codeCounter + 1] != NULL                                               \
									&& codeCounter  < sizeOfMachCode - 1                                    \
									&& sscanf(_arrayPtrCmd[codeCounter + 1], "%lg", &funcDbHelper )!= 0     \

#define LABEL_CONDITION _arrayPtrCmd[codeCounter + 1] != NULL                                               \
									&& codeCounter  < sizeOfMachCode - 1                                    \
									&& sscanf(_arrayPtrCmd[codeCounter + 1], "%d", &funcIntHelper )!= 0	    \
									&& _labelArr[funcIntHelper] != -1

#define NEXT_CMD (CPU->commands)[++counter]
#define THIS_CMD (CPU->commands)[counter]

#define push stack.push
#define pop stack.pop

#define stack (*(CPU->values))



DEF_CMD (push, CMD_pushReg, {                                                                               \
    if (REG_CONDITION)                                                                                      \
    {                                                                                                       \
        NEXT_ELEM_CODE = CMD_pushReg;                                                                       \
        NEXT_ELEM_CODE = ASSIG_REG;                                                                         \
        continue;                                                                                           \

	}
	}, {
	switch ((int)((CPU->commands)[++counter])){
		case 1:{
			push(CPU->ax);
			break;
		}

		case 2:{
			push(CPU->bx);
			break;
		}

		case 3:{
			push(CPU->cx);
			break;
		}
		case 4:{
			push(CPU->dx);
			break;
		}

		default:
			exit(EXIT_FAILURE);
		}
	counter++;
	continue;
})

DEF_CMD (push, CMD_pushRam, {																			    \
	 if (RAM_CONDITION)	                                                                                    \
		{                                                                                                   \
		NEXT_ELEM_CODE = CMD_pushRam;                                                                       \
		NEXT_ELEM_CODE = (double)funcIntHelper;                                                             \
		funcIntHelper = 0;                                                                                  \
		continue;                                                                                           \
		}                                                                                                   \
	}, {

	if ((int)((CPU->commands)[1 + counter]) < RAM_SIZE){
		push((CPU->ram)[(int)((CPU->commands)[++counter])]);
		counter++;
		continue;
	}
	else{
		printf("nonexistent elem of RAM");
		exit(EXIT_FAILURE);
	}

})

DEF_CMD (push, CMD_push, {																					\
	 if (VALUE_CONDITION)	                                                                                \
		{                                                                                                   \
		funcDbHelper = 0;																					\
		NEXT_ELEM_CODE = CMD_push;                                                                          \
		NEXT_ELEM_CODE = strtod(_arrayPtrCmd[codeCounter - 1], NULL);                                       \
		continue;                                                                                           \
		}}, {push (NEXT_CMD); counter++; continue;})

DEF_CMD (push, CMD_ERR, {
		if (1) {                                                                                            \
		printf("WRONG INPUT");                                                                              \
		exit(EXIT_FAILURE);                                                                                 \
		continue;
		}
																											\
	}, {})

DEF_CMD (pop, CMD_popReg, {                                                                                 \
	if(REG_CONDITION){                                                                                      \
		NEXT_ELEM_CODE = CMD_popReg;                                                                        \
        NEXT_ELEM_CODE = ASSIG_REG;                                                                         \
        continue;                                                                                           \
	}                                                                                                       \
	}, {
	switch ((int)((CPU->commands)[++counter])){
		case 1:{
			pop(CPU->ax);
			break;
		}

		case 2:{
			pop(CPU->bx);
			break;
		}

		case 3:{
			pop(CPU->cx);
			break;
		}
		case 4:{
			pop(CPU->dx);
			break;
		}

		default:
			exit(EXIT_FAILURE);
		}
		counter++;
		continue;
})

DEF_CMD (pop, CMD_popRam, {                                                                                 \
	if(RAM_CONDITION){                                                                                      \
		NEXT_ELEM_CODE = CMD_popRam;                                                                        \
		NEXT_ELEM_CODE = (double)funcIntHelper;                                                             \
		funcIntHelper = 0;                                                                                  \
		continue;                                                                                           \
	}                                                                                                       \
	}, {

	if ((int)((CPU->commands)[1 + counter]) < RAM_SIZE){
		pop((CPU->ram)[(int)((CPU->commands)[++counter])]);
		counter++;
		continue;
	}
	else{
		printf("nonexistent elem of RAM");
		exit(EXIT_FAILURE);
		}
	})

DEF_CMD (pop, CMD_pop, {                                                                                    \
	if(1){                                                                                                  \
		NEXT_ELEM_CODE = CMD_pop;                                                                           \
		funcIntHelper = 0;                                                                                  \
		continue;                                                                                           \
	}                                                                                                       \
	}, {pop(); counter++; continue;})

#define SMPL_INSTR(name) {                                                                                  \
	if(1){                                                                                                  \
		NEXT_ELEM_CODE = CMD_##name;                                                                        \
		funcIntHelper = 0;                                                                                  \
		continue;                                                                                           \
	}                                                                                                       \
	}

DEF_CMD (add, CMD_add, SMPL_INSTR(add),  {

	double value1 = 0;
	double value2 = 0;

	pop(value1);
	pop(value2);
	push (value1 + value2);

	counter++;
	continue;

	}
	)

DEF_CMD (sub, CMD_sub, SMPL_INSTR(sub),  {

	double value1 = 0;
	double value2 = 0;

	pop(value1);
	pop(value2);
	push (value2 - value1);

	counter++;
	continue;
	}
	)

DEF_CMD (mul, CMD_mul, SMPL_INSTR(mul),  {

	double value1 = 0;
	double value2 = 0;

	pop(value1);
	pop(value2);
	push (value1 * value2);

	counter++;
	continue;

}
)

DEF_CMD (div, CMD_div, SMPL_INSTR(div),  {

	double value1 = 0;
	double value2 = 0;

	pop(value1);
	pop(value2);
	push (value2 / value1);

	counter++;
	continue;

}
)

DEF_CMD (out, CMD_out, SMPL_INSTR(out),  {

	double value = 0;

	pop (value);
	std::cout << value << std::endl;

	counter++;
	continue;
}
)

DEF_CMD (ret, CMD_ret, SMPL_INSTR(ret),  {})

DEF_CMD (in, CMD_in, SMPL_INSTR(in),  {

	double value = 0;

	std::cin >> value;

	push(value);

	counter++;
	continue;
})

#undef SMPL_INSTR

#define DEF_CMD_JMP_INSTR(name, codeCpu)                                                                    \
DEF_CMD (name, CMD_##name, {                                                                                \
	if (LABEL_CONDITION)	                                    											\
	{                                                                                                       \
		NEXT_ELEM_CODE = CMD_##name;                                                                        \
		NEXT_ELEM_CODE = (double)(_labelArr[funcIntHelper]);                                                \
		                                                                                                    \
		continue;                                                                                           \
	}                                                                                                       \
	else{                                                                                                   \
		funcIntHelper = 0;                                                                                  \
		printf("WRONG INPUT");                                                                              \
		exit(EXIT_FAILURE);                                                                                 \
		continue;                                                                                           \
	}                                                                                                       \
}, codeCpu)

DEF_CMD_JMP_INSTR (jmp, {})

DEF_CMD_JMP_INSTR (je, {})

#undef DEF_CMD_JMP_INSTR

#undef NEXT_CMD

#undef THIS_CMD

#undef push

#undef pop

#undef stack

#undef ASSIG_REG

#undef NEXT_ELEM_CODE

#undef REG_CONDITION

#undef RAM_CONDITION

#undef VALUE_CONDITION

#endif