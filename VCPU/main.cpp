#include <iostream>
#include <cstring>
#include <cmath>

#include "../Stack/stack.h"

#include "../CommonFiles/fileworking.h"

#include "CPU.h"



#define CPU_VERSION "v2.1"
#define SIGNATURE "SA"
#define ASM_NAME "ASM"

#define RAM_SIZE 256
#define MAX_RET_AMOUNT 200
#define INIT_AMOUNT_OF_COMMANDS 200
#define MAX_VALUE_STACK_SIZE 200

const char codeFilename[] = "../CommonFiles/machcode.txt";


int main () {

	size_t szOfCmdArr = INIT_AMOUNT_OF_COMMANDS;

	char *code = getBufferFromFileGetSzOfBuf (codeFilename);
	double *arrOfCMD = (double *) calloc (szOfCmdArr, sizeof (double));

	VCPU Cpu = {};

	controlVerSig (&code);

	createCommandArr (code, &arrOfCMD, &szOfCmdArr);

	createStack (refunds, int, "%d", MAX_RET_AMOUNT);
	createStack (values, double, "%lg", MAX_VALUE_STACK_SIZE);

	Construct_Cpu (szOfCmdArr, &Cpu, &values, &refunds, arrOfCMD);


	executePrg (&Cpu);

	Destruct_Cpu (&Cpu);

	return 0;
}


void controlVerSig (char **code) {

	assert (code);

	char *LeXem = strtok (*code, " \n");

#define COND_CHEAK(name)                        \
     if (strcmp (LeXem, name) != 0) {           \
        printf("Incorrect data");               \
        exit (EXIT_FAILURE);                    \
        }                                       \

	COND_CHEAK (SIGNATURE)
	LeXem = strtok (NULL, " \n");

	COND_CHEAK (ASM_NAME)
	LeXem = strtok (NULL, " \n");

	COND_CHEAK (CPU_VERSION)

#undef COND_CHEAK

	*code = LeXem + strlen (LeXem) + 1;

}

void *createCommandArr (char *const code, double **const arrayPtrCmd, size_t *sizePtrArr) {

	assert (code);
	assert (arrayPtrCmd);
	assert (sizePtrArr);

	int cmdCounter = 0;
	char *LeXem = strtok (code, " \n");

	while (LeXem != NULL) {

		if (*sizePtrArr < *sizePtrArr - 2) {
			*arrayPtrCmd = (double *) realloc (*arrayPtrCmd, *sizePtrArr += *sizePtrArr / 2);
		}

		(*arrayPtrCmd)[cmdCounter] = strtod (LeXem, NULL);
		LeXem = strtok (NULL, " \n");
		cmdCounter++;
	}
	*sizePtrArr = (size_t) cmdCounter;

	free(code);

}


void
Construct_Cpu (size_t sizeOfCmdArr, VCPU *Cpu, Stack<double> *_lValues, Stack<int> *_lRefunds, double *_lcommands) {

	assert (Cpu);
	assert (_lcommands);
	assert (_lRefunds);
	assert (_lValues);

	Cpu->ax = 0;
	Cpu->bx = 0;
	Cpu->cx = 0;
	Cpu->dx = 0;

	Cpu->SzOfCode = sizeOfCmdArr;

	Cpu->ram = (double *) calloc (RAM_SIZE, sizeof (double));

	Cpu->values = _lValues;
	Cpu->refunds = _lRefunds;
	Cpu->commands = _lcommands;
}


void Destruct_Cpu (VCPU *Cpu) {
	free (Cpu->commands);
	free (Cpu->values);
	free (Cpu->refunds);
	free (Cpu->ram);
}

void executePrg (VCPU *CPU) {

	assert (CPU);

	int counter = 0;

#define  DEF_CMD(name, num, codeAsm, codeCpu)           \
            case num:{                                  \
            codeCpu                                     \
            std:: cout << num << std::endl;}

	while (counter < CPU->SzOfCode) {

		switch ((int) CPU->commands[counter]) {

#include "../CommonFiles/commandsAsm&CPU.h"

			default: {
				printf ("WRONG INPUT");
				exit (EXIT_FAILURE);
			}
		}
	}
}

#undef DEF_CMD

void dumpCPU (VCPU CPU) {
	printf ("ax = %lg\n", CPU.ax);
	printf ("bx = %lg\n", CPU.bx);
	printf ("cx = %lg\n", CPU.cx);
	printf ("dx = %lg\n", CPU.dx);

	printf ("RAM\n");

	for (int i = 0; i < RAM_SIZE; i++)
		if (CPU.ram[i] != 0)
			printf ("%2d %24lg\n", i, CPU.ram[i]);
	(CPU.values)->dump ();
}