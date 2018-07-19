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
	CMD_jne,
	CMD_jb,
	CMD_jl,
	CMD_ret,
	CMD_in,
	CMD_ERR,
	CMD_call,
	CMD_end,
	CMD_sqrt

};

#define ASSIGN_REG strchr ("abcd", (tokens[codeCounter - 1 ])[0]) - "abcd" + 1;


#define NEXT_ELEM_CODE data->binaryCode[codeCounter++]



#define REG_CONDITION \
	tokens[codeCounter + 1] != NULL && tokens[codeCounter + 1][1] == 'x' &&                                                                                 \
				strchr ("abcd", (tokens[codeCounter + 1])[0]) &&                                      	\
				codeCounter  < tokensNumber - 1 &&                                                    	\
				(tokens[codeCounter + 1])[2] == 0



#define RAM_CONDITION                                                                                 	\
tokens[codeCounter + 1] != NULL                                                                       	\
									&& codeCounter  < tokensNumber - 1                                	\
									&& sscanf(tokens[codeCounter + 1], "[%d]", &integerTemp )!= 0  		\



#define VALUE_CONDITION tokens[codeCounter + 1] != NULL                                              	\
									&& codeCounter  < tokensNumber - 1                                  \
									&& sscanf(tokens[codeCounter + 1], "%lg", &doubleTemp )!= 0     	\



#define LABEL_CONDITION tokens[codeCounter + 1] != NULL                                               	\
									&& codeCounter  < tokensNumber - 1                                  \
									&& sscanf(tokens[codeCounter + 1], "%d", &integerTemp )!= 0	    	\
									&& _labels[integerTemp] != 0                                   \



/*#define NEXT_CMD (CPU->commands)[++counter]
#define THIS_CMD (CPU->commands)[counter]

#define push stack.push
#define pop stack.pop

#define stack (*(CPU->values))*/



DEF_CMD (push, CMD_pushReg, {
    if (REG_CONDITION)
    {
        NEXT_ELEM_CODE = CMD_pushReg;
        NEXT_ELEM_CODE = ASSIGN_REG;
        continue;

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


DEF_CMD (push, CMD_pushRam, {
	 if (RAM_CONDITION)
		{
		NEXT_ELEM_CODE = CMD_pushRam;
		NEXT_ELEM_CODE = (double)integerTemp;
		integerTemp = 0;
		continue;
		}
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

DEF_CMD (push, CMD_push, {
	 if (VALUE_CONDITION)
		{
		doubleTemp = 0;
		NEXT_ELEM_CODE = CMD_push;
		NEXT_ELEM_CODE = strtod(tokens[codeCounter - 1], NULL);
		continue;
		}},
		{
			push (NEXT_CMD); counter++; continue;
		})

DEF_CMD (push, CMD_ERR, {
			printf("Push command operand: %s\n", tokens[codeCounter + 1]);
            return throw_error(PUSH_ERR, "Error, unknown operand.", "",
                    __PRETTY_FUNCTION__, __LINE__, __FILE__);

	}, {})

DEF_CMD (pop, CMD_popReg, {
	if(REG_CONDITION)
	{
		NEXT_ELEM_CODE = CMD_popReg;
        NEXT_ELEM_CODE = ASSIGN_REG;
        continue;
	}
	}, {

/*	pop (registers [(int)((CPU->commands)[++counter])]); */

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

DEF_CMD (pop, CMD_popRam, {
	if(RAM_CONDITION){
		NEXT_ELEM_CODE = CMD_popRam;
		NEXT_ELEM_CODE = (double)integerTemp;
        integerTemp = 0;
		continue;
	}
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

DEF_CMD (pop, CMD_pop, {
	if(1){
		NEXT_ELEM_CODE = CMD_pop;
		integerTemp = 0;
		continue;
	}
	}, {pop(); counter++; continue;})




#define SMPL_INSTR(name) {                                                                                  \
	                                                                                                        \
		NEXT_ELEM_CODE = CMD_##name;                                                                        \
		integerTemp = 0;                                                                                  	\
		continue;                                                                                           \
	                                                                                                        \
	}

#define DEF_CMD_ARITH(name, CMD_name, operation)                                                            \
DEF_CMD (name, CMD_name, SMPL_INSTR(name),  {                                                               \
																											\
	double value1 = 0;                                                                                      \
	double value2 = 0;                                                                                      \
																											\
	pop(value1);                                                                                            \
	pop(value2);                                                                                            \
																											\
																											\
	operation;                                                                                              \
																											\
	counter++;                                                                                              \
	continue;                                                                                               \
	}                                                                                                       \
	)

DEF_CMD_ARITH (add, CMD_add, push(value1 + value2) )

DEF_CMD_ARITH (sub, CMD_sub, push (value2 - value1) )

DEF_CMD_ARITH (mul, CMD_mul, push (value1 * value2) )

DEF_CMD_ARITH (div, CMD_div, push (value2 / value1) )

DEF_CMD (out, CMD_out, SMPL_INSTR(out),  {

	double value = 0;

	pop (value);
	printf("%lg\n", value);

	counter++;
	continue;
}
)

DEF_CMD (in, CMD_in, SMPL_INSTR(in),  {

	double value = 0;

	std::cin >> value;

	push(value);

	counter++;
	continue;
})

DEF_CMD (sqrt, CMD_sqrt, SMPL_INSTR(sqrt),  {

	double value = 0;

	pop(value);

	push(sqrt(value));

	counter++;
	continue;
})


#define JMP_CODE counter = CPU->commands[counter + 1] - 1;

DEF_CMD (jmp, CMD_jmp,{
	if (LABEL_CONDITION)
	{
		NEXT_ELEM_CODE = CMD_jmp;
		NEXT_ELEM_CODE = (double)(_labels[integerTemp]);

		continue;
	}
	else{
        printf("Labels value: %d\n", strtol(tokens[codeCounter + 1],NULL, 10));
        return throw_error(LABEL_NON_EXIST, "Error, non-existent label.", "",
            __PRETTY_FUNCTION__, __LINE__, __FILE__);
	}
},
{
	JMP_CODE;
	continue;
}
)

#define DEF_CMD_JMP_INSTR(name, condition)                                                                  \
DEF_CMD (name, CMD_##name, {                                                                                \
	if (LABEL_CONDITION)	                                    											\
	{                                                                                                       \
		NEXT_ELEM_CODE = CMD_##name;                                                                        \
		NEXT_ELEM_CODE = (double)(data->labels[integerTemp]);                                             	\
		                                                                                                    \
		continue;                                                                                           \
	}                                                                                                       \
	else{                                                                                                   \
		printf("Labels value: %d\n", strtol(tokens[codeCounter + 1],NULL, 10));                             \
        return throw_error(LABEL_NON_EXIST, "Error, non-existent label.", "",                               \
        __PRETTY_FUNCTION__, __LINE__, __FILE__);                                                           \
		                                                                                                    \
	}                                                                                                       \
},                                                                                                          \
{                                                                                                           \
																											\
	double value1 = 0;                                                                                      \
	double value2 = 0;                                                                                      \
																											\
	pop(value1);                                                                                            \
	pop(value2);                                                                                            \
																											\
	if (condition) {                                                                                        \
		JMP_CODE;                                                                                           \
		push(value2);                                                                                       \
		push(value1);                                                                                       \
		continue;                                                                                           \
	}                                                                                                       \
	counter += 2;                                                                                           \
	push(value2);                                                                                           \
	push(value1);                                                                                           \
	continue;                                                                                               \
}                                                                                                           \
)

DEF_CMD_JMP_INSTR(je, value1 == value2 )

DEF_CMD_JMP_INSTR(jne, value1 != value2 )

DEF_CMD_JMP_INSTR(jb, value1 < value2 )

DEF_CMD_JMP_INSTR(jl, value1 > value2 )

#undef push

#undef pop

DEF_CMD (call, CMD_call, {
if (LABEL_CONDITION)
{
	NEXT_ELEM_CODE = CMD_call;
	NEXT_ELEM_CODE = (double)(data->labels[integerTemp]);

	continue;
	}
else{
    printf("Labels value: %d\n", strtol(tokens[codeCounter + 1],NULL, 10));                             \
    return throw_error(LABEL_NON_EXIST, "Error, non-existent label.", "",                               \
        __PRETTY_FUNCTION__, __LINE__, __FILE__);                                                       \
	continue;
}
}, {
	(*(CPU->refunds)).push(counter + 2 );
	JMP_CODE;
	continue;
}
)

DEF_CMD (ret, CMD_ret, SMPL_INSTR(ret), {
	int value = 0;

	(*(CPU->refunds)).pop(value);
	counter = value ;
	continue;
}
)

DEF_CMD (end, CMD_end, SMPL_INSTR(end), {
	exit(EXIT_SUCCESS);
}
)


#undef DEF_CMD_JMP_INSTR

#undef NEXT_CMD

#undef THIS_CMD

#undef stack

#undef ASSIGN_REG

#undef NEXT_ELEM_CODE

#undef REG_CONDITION

#undef RAM_CONDITION

#undef VALUE_CONDITION

#undef JMP_CODE

#undef SMPL_INSTR

#undef DEF_CMD_ARITH