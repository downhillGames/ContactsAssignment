/*
 ============================================================================
 Name        : MergeSort.c
 Author      : Tyler Schmidt
 Class       : CS 2060 Sec 01
 Date        : 3/21/2018
 HW          : Chap 17 HW02
 Description : Merge sorting linked list
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dates.h"

/* structure to hold contact data record */
typedef struct contacts
{
	char firstName[26];
	char lastName[31];
	date dob;
	date anniversary;
	char company[76];
	char street1[51];
	char city[51];
	char county[51];
	char state[3];
	char zip[13];
	char phone1[16];
	char phone2[16];
	char email[101];
	char web[101];
	struct contacts * next;
} contact; //608 bytes

/* contact prototypes */
unsigned int fGetLine(FILE *fps, char *line, int max_len);
void setString(char *dest, char *value);
contact * loadContactCsv(char* sourceFile, unsigned long *recordsRead);
int parseContact (char * line, contact * record);
void printContactRecord (contact *record);
int writeToBinary (char* fileName, contact *contacts);
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead);
void freeContacts(contact *head);
void printContacts(contact* head, unsigned long starting, long numToPrint);

/* merge sort prototypes */
void doMergeSort(contact **head, int sortBy, int sortOrder);
void splitList(contact *head, contact **h1, contact **h2);
contact *mergeListsByFirstName(contact *l1, contact *l2, int sortOrder);
contact *mergeListsByLastName(contact *l1, contact *l2, int sortOrder);
void swapNodes(contact **node1, contact **node2);

/* printing prototypes */
void printContactNames(contact *head, int start, long unsigned amount);
void printContactName(contact *record);
contact *moveToContact(contact *head, long unsigned pos);


/* import contacts from large text file,
 * and write into a new binary file */
int main(void)
{
	unsigned long recordCount =0;

	printf("Loading contacts from csv.  Please wait...\n");

	// load contacts from text-based csv file.
	contact * contacts = loadContactCsv ("us-1000000.csv", &recordCount); // load contacts from csv file

	if (contacts == NULL || recordCount==0)
	{
		puts("Loading contact data failed. Goodbye.");
		exit(EXIT_FAILURE);
	}

	printf("There are %lu contacts in our array!\n", recordCount);
	printf("Each contact consists of %lu bytes.\n\n", sizeof(contact));

	writeToBinary("contacts.dat", contacts); // write linked list to binary file

	freeContacts(contacts); // free linked list

	//(about 608 MB)
	printf ("This is %lu bytes in total.\n\n", sizeof(contact)*recordCount);

	// now, write contacts out to a new binary file...
	printf ("Writing %lu records to binary file...\n", recordCount);

	printf ("Finished writing to binary file\n\n");

	//now, let's read in from binary file
	printf ("Reading %lu records from binary file...\n", recordCount);

	contact * contactsB = loadFromBinary("contacts.dat", 1000000, &recordCount); // load contact

	printf ("Finished reading from binary file\n\n");

	if (contactsB == NULL || recordCount==0)
	{
		puts("Loading contact data failed. Goodbye.");
		exit(EXIT_FAILURE);
	}

	printf("There are %lu contacts in our array!\n", recordCount);
	printf("Each contact consists of %lu bytes.\n\n", sizeof(contact));

	printContactNames(contactsB, 403234, 15);// print 15 contacts starting at #403234
	printf("\n");
	printf("Sorting list...");
	//sort by first name (ascending)...
	doMergeSort(&contactsB, 1, 0);

	//sort by last name(descending)...
	doMergeSort(&contactsB, 0, 1);

	printf("\n");
	printf("**** SORTED LIST BELOW ****\n");

	printContactNames(contactsB, 403234, 15); // print 15 contacts starting at #403234


	freeContacts(contactsB); //"Free the mallocs"

	return EXIT_SUCCESS;


		return 0;

}

/*writes contact records to binary file*/
int writeToBinary (char* fileName, contact *contacts)
{
	//Open file for binary write, overwriting if necessary
	FILE *fp = fopen(fileName, "wb");


	struct contacts * current = contacts;
	//walk through linked list
	while (current != NULL)
	{
		fwrite(current, sizeof(contact), 1, fp); //write contacts to linked list
		current = current->next;
	}

	//flush any remaining data in memory
	fflush(NULL);

	//close file
	fclose(fp);

	//Return 1 to signify success
	return 1;
}

/*loads contact records from binary file*/
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead)
{
	FILE *fp = fopen(sourceFile, "rb");

	   //If pointer is not valid, print error
		if( fp  == NULL)  //if fp != NULL file is valid
		{
			perror(sourceFile);
		}


		/*calculate how many records are in the file */

			//Seek to the end of the file (move to the end)
			fseek(fp, 0L, SEEK_END);

			//# of records read is equal to the size of fp dividied by struct contact
			(*recordsRead) = ftell(fp) / sizeof(contact);

			fseek(fp, 0L, SEEK_SET);

			struct contacts * head = NULL;        // pointers for linked list
			struct contacts * prev, *current;
		// If recordsRead is greater than maxRecords  then set recordsRead equal to maxRecords
		if ((*recordsRead) > maxRecords)
		{
			(*recordsRead) = maxRecords;
		}
	//pointer that holds access to memory we allocate to hold contacts

		for (int i = 0; i < 1000000; i++)
		{
			current = (struct contacts *) malloc(sizeof(struct contacts));

			if (head == NULL) /* first structure */
				head = current;
			else /* subsequent structures */
				prev->next = current;

			current->next = NULL;

			fread(current, sizeof(contact), 1, fp);
			// place values we read from the file into a contact record
			prev = current;


		}
		//close file
		fclose(fp);

	//return memory pointer to contacts
	return head;
}


/* sends contact information to console */
void printContactRecord (contact *record)
{
	//print contact information line to console
	printf("*** Contact Information ***\n");

	//print first Name to console
	printf("First Name: %s\n", record->firstName);

	//print last name to console
	printf("Last Name: %s\n", record->lastName);

	//print company to console
	printf("Company: %s\n", record->company);

	//print address to console
	printf("Address: %s\n", record->street1);

	//print city to console
	printf("City: %s\n", record->city);

	//print state to console
	printf("State: %s\n", record->state);

	//print zip to console
	printf("Zip: %s\n", record->zip);

	//print email to console
	printf("Email: %s\n", record->email);

	//print website to console
	printf("Web: %s\n", record->web);

	//print phone number to console
	printf("Phone: %s\n\n", record->phone1);
}

/* sets value into a char array */
void setString(char *dest, char *value)
{
	strcpy(dest,value);
}

/* Reads an entire line from a text file.
   Returns how many characters are in the line*/
unsigned int fGetLine(FILE *fps, char *line, int max_len)
{
	int tempChar;
	unsigned int tempCharIdx = 0U;

    /* get a character from the file pointer */
    while((tempChar = fgetc(fps)))
    {
        /* avoid buffer overflow error */
        if (tempCharIdx == max_len)
        {
            fprintf(stderr, "Warning: line is too long.\n");
        }

        /* test character value */
        if (tempChar == EOF) // reached end of file?
        {
            line[tempCharIdx] = '\0';
            break;
        }
        else if (tempChar == '\n' // compensate for Windows CRLF or Linux LF
        		|| tempChar == '\r') {
            line[tempCharIdx] = '\0';
            break;
        }
        else
        	 if (tempCharIdx < max_len)
            line[tempCharIdx++] = (char)tempChar;
        	 else
        	 {
        		 // reached max size of line buffer - truncate the line
        		 line[tempCharIdx] = '\0';
        		 tempCharIdx++; // do not exit loop
        		 //continue to read until end of line in file
        		 // (even though the line is truncated)
        	 }
    }

    // return how many characters read
    return tempCharIdx;
}


/* Load contacts from a (text) comma delimited file*/
contact * loadContactCsv(char* sourceFile, unsigned long *recordsRead)
{

	//	return NULL;

	// open csv file
	FILE *fps = fopen(sourceFile, "r");

    unsigned long mi = 0; // index into our allocated contact memory

    if(fps != NULL) // make sure we have a valid file pointer
    {
        char line[256]; // we can read a line with max 256 characters (including null terminator)

		struct contacts * head = NULL;
		struct contacts * prev, *current;
        // Read each contact from the text file one line at a time.
        // Note: 1 "record" = 1 line = 1 contact

		while (fGetLine(fps, line, sizeof(line)) > 0 )
			//		while (fget(fps) != NULL && line != '\0')
		{

			if (mi != 0)
			{
				current = (struct contacts *) malloc(sizeof(struct contacts));

				if (head == NULL) /* first structure */
					head = current;
				else /* subsequent structures */
					prev->next = current;

				current->next = NULL;
				parseContact(line, current); // place into contact struct
				prev = current;

			}
			mi++; // increment the array index...
			}
			// let caller know how many actual records were read...
			(*recordsRead) = mi -1;

			// close the file
			fclose(fps);

			// Return pointer to memory (the heap) where contacts are stored
			return head;

    }
    else
    {
			//print out if error
			perror(sourceFile);


        return NULL; //return null to indicate that a problem was encountered
    }
}



/*free memory from linked list*/
void freeContacts(contact *head)
{
	struct contacts* tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}



}
/*prints user defined number of contact info from starting # */
void printContacts(contact* head, unsigned long starting, long numToPrint)
{
	contact *current;
	current = head;
	unsigned long ending = starting + numToPrint;
	// print info starting of contacts starting with input ...
	for (long i = 0 ; i < ending; i++)
	{
		if(i >= starting && i <= ending)
		{
			printContactRecord(current);

		}
		current = current->next;
	}
}





/* parse fields from line and place values into contact record */
int parseContact (char * line, contact * record)
{
	const char s[2] = {','}; // separator used to tokenize each line we read from csv file
    char *token; // holds words between commas in each line

	/* get first name */
    token = strtok(line, s);
    if (token != NULL)
		setString(record->firstName,token);

		/* get last name */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->lastName,token);

	/* get company name */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->company,token);

	/* get street address */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->street1,token);

	/* get city name */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->city,token);

	/* get county name */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->county,token);

	/* get state name */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->state,token);

	/* get zip */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->zip,token);

	/* get phone1 */
	token = strtok(NULL, s);
	if (token != NULL)
			setString(record->phone1,token);

	/* get phone2 */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->phone2,token);

	/* get email */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->email,token);

	/* get web */
	token = strtok(NULL, s);
	if (token != NULL)
		setString(record->web,token);

	//default dates to ZERO
	record->dob = ZERO_DATE;
	record->anniversary = ZERO_DATE;

	return 1;
}

/* splits linked linked list in half using fast/slow method */
void splitList(contact *head, contact **h1, contact **h2)
{
	*h1 = head; *h2 = NULL;
	contact *slow = head;
	contact *fast = head->next;
	// find split
	while (fast)
	{
		fast = fast->next;
		if (fast == NULL)
			break;
		fast = fast->next;
		slow = slow->next;
	}
	//assign second half
	*h2 = slow->next;
	// chop first half & set next to NULL
	slow->next = NULL;
}

/* merges and compares two contacts by last name*/
contact *mergeListsByLastName(contact *l1, contact *l2, int sortOrder)
{
	contact *head = NULL;
	contact **lastRef = &head;
	while (1) //while true
	{
		//assign Nodes
		if (l1 == NULL || l2 == NULL)
		{

			if (l1 > l2)
				*lastRef = l1;
			else
				*lastRef = l2;

			break;
		}
		// sort by ascending = 0
		if (sortOrder == 0) {
			if (strcmp(l1->lastName, l2->lastName) <= 0) // compare contact last names
				swapNodes(&l1, lastRef);
			else
				swapNodes(&l2, lastRef);
		}
		// sort by descending = 1
		else {
			if (strcmp(l2->lastName, l1->lastName) <= 0) // compare contact last names
				swapNodes(&l1, lastRef);
			else
			swapNodes(&l2, lastRef);
		}
		// update last element
		lastRef = &((*lastRef)->next);
	}
	return head;
}
/* merges and compares two contacts by first name*/
contact *mergeListsByFirstName(contact *l1, contact *l2, int sortOrder)
{
	contact *head = NULL;
	contact **firstRef = &head;
	while (1) // while true
	{
		//assign nodes
		if (l1 == NULL || l2 == NULL)
		{
			if (l1 > l2)
				*firstRef = l1;
			else
				*firstRef = l2;

			break;
		}
		// sort by ascending = 0
		if (sortOrder == 0) {
			if (strcmp(l1->firstName, l2->firstName) <= 0) // compare contact first names
				swapNodes(&l1, firstRef);
			else
				swapNodes(&l2, firstRef);
		}
		// sort by descending = 1
		else {
			if (strcmp(l2->firstName, l1->firstName) <= 0) // compare contact first names
				swapNodes(&l1, firstRef);
			else
				swapNodes(&l2, firstRef);
		}
		//update last element
		firstRef = &((*firstRef)->next);
	}
	return head;
}

/* swaps two nodes in a linked list */
void swapNodes(contact **node1, contact **node2)
{

	contact *temp = *node1; //hold temp node
	*node1 = (*node1)->next;
	temp->next = *node2;
	*node2 = temp;
}


/*performs merge sort on linked list*/
void doMergeSort(contact **head, int sortBy, int sortOrder)
{
	// do not process empty or single-node list
	if (head == NULL
		|| *head == NULL
		|| (*head)->next == NULL)
		return;
	contact *first, *second;

	// split the list in half
	splitList(*head, &first, &second);

	// perform recursive merge sort on both halves
	doMergeSort(&first, sortBy, sortOrder);
	doMergeSort(&second, sortBy, sortOrder);

	// Merge both halves back together,
	// sorting as indicated by sortBy and sortOrder.
	if (sortBy == 0)
		*head = mergeListsByLastName(first, second,sortOrder);
	else
		*head = mergeListsByFirstName(first, second, sortOrder);
}
/*prints contact name in lastName, firstName format*/
void printContactName(contact *record)
{
	printf("%s, %s\n", record->lastName, record->firstName);

}
/*prints contact name in lastName, firstName format*/
void printContactNames(contact *head, int start, long unsigned amount) {

	contact *current;
	current =  moveToContact(head, start); //move to user defined starting point

	//print info starting of contacts starting with input
	for (long i = 0; i < amount; i++)
	{
		printContactName(current);
		current = current->next;
	}
}
/*moves to contact in user defined starting position*/
contact *moveToContact(contact *head, long unsigned pos)
{
	contact *current;
	current = head;
	 //loop current is in starting pos
	for (unsigned long i = 0; i < pos; i++)
	{
		if(i !=0)
			current = current->next;
	}
	return current;
}
