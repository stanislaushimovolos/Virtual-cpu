#include <iostream>
#include <cstring>

#include "../CommonFiles/fileworking.h"

#define ASM_VERSION "v2.1"
#define SIGNATURE "SA"
#define ASM_NAME "ASM"

#define INIT_ARR_SZ 100
#define MAX_LABEL_AMOUNT 100

const char inputFilename[] = "../CommonFiles/asmCode.txt";
const char outFilename[] = "../CommonFiles/machCode.txt";
const char labelName[] = "label";


void *writeCode (const double *machCode, const size_t sizeOfMachCode);

char *createPtrArrLabelArr (char *const code, char ***const arrayPtrCmd, size_t *sizePtrArr);

void assemble (char **const _arrayPtrCmd, double *const machCode, size_t sizeOfMachCode, char *labelArr);


int main () {

	size_t cmdAmount = INIT_ARR_SZ;

	char *code = getBufferFromFileGetSzOfBuf (inputFilename);

	char **arrayPtrCmd = (char **) calloc (cmdAmount + 1, sizeof (char **));

	char *labelArr = createPtrArrLabelArr (code, &arrayPtrCmd, &cmdAmount);

	double *machCode = (double *) calloc (2 * cmdAmount + 1, sizeof (double));

	assemble (arrayPtrCmd, machCode, cmdAmount, labelArr);

	writeCode (machCode, cmdAmount);

	free (code);
	free (machCode);
	free (arrayPtrCmd);
	free (labelArr);


	return 0;
}

void assemble (char **const _arrayPtrCmd, double *const machCode, size_t sizeOfMachCode, char *_labelArr) {

	assert (machCode);
	assert (_arrayPtrCmd);

	int codeCounter = 0;

	while (_arrayPtrCmd[codeCounter] != 0 && codeCounter < sizeOfMachCode) {

#define  DEF_CMD(name, num, codeAsm, codeCpu)\
    if (strcmp (_arrayPtrCmd[codeCounter], #name) == 0){\
                                                        \
                                                        \
        codeAsm                                         \
                                                        \
    }

#include "../CommonFiles/commandsAsm&CPU.h"

		printf ("WRONG INPUT");
		exit (EXIT_FAILURE);
	}
}

#undef DEF_CMD

char *createPtrArrLabelArr (char *const code, char ***const arrayPtrCmd, size_t *sizePtrArr) {

	assert (code);
	assert (arrayPtrCmd);

	int cmdCounter = 0;
	size_t _labelAmount = MAX_LABEL_AMOUNT;
	char *LeXem = strtok (code, " \n");
	char *labelArr = (char *) calloc (MAX_LABEL_AMOUNT + 1, sizeof (char));

	for (int i = 0; i < _labelAmount; i++)
		labelArr[i] = -1;

	char lbNum = 0;

	while (LeXem != NULL) {

		if (lbNum > _labelAmount - 2) {
			labelArr = (char *) realloc (labelArr, *sizePtrArr += *sizePtrArr / 2);
		}

		if (*sizePtrArr < *sizePtrArr - 2) {
			*arrayPtrCmd = (char **) realloc (*arrayPtrCmd, _labelAmount += _labelAmount / 2);
		}

		if (strcmp (LeXem, labelName) == 0) {
			LeXem = strtok (NULL, " \n");
			lbNum = (char) strtol (LeXem, NULL, 10);
			LeXem = strtok (NULL, " \n");
			labelArr[lbNum] = (char) (cmdCounter + 1);
			continue;
		}

		(*arrayPtrCmd)[cmdCounter] = LeXem;

		LeXem = strtok (NULL, " \n");
		cmdCounter++;
	}
	*sizePtrArr = (size_t) cmdCounter;

	return labelArr;
}


void *writeCode (const double *machCode, const size_t sizeOfMachCode) {

	assert (machCode);

	FILE *outFile = fopen (outFilename, "w");

	fprintf (outFile, "%s\n", SIGNATURE);
	fprintf (outFile, "%s ", ASM_NAME);
	fprintf (outFile, "%s\n", ASM_VERSION);
	for (int i = 0; i < sizeOfMachCode; i++) {

		fprintf (outFile, "%lg ", machCode[i]);
	}
}