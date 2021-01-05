/****************************************************************************
	Name : MAHESH BABU
	Declaration of AUTHORSHIP 
	ROLL : 1801CS36
	I here by declare that the entire code is written by me
*****************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include <inttypes.h>
//memory starting from 0

//A table for opcode, Mnemonic and format (value || operand || nill)
//Optable
struct Opcode          
{
        char name[8];
        char code[9];
        char format[6];	//value or offset
        struct Opcode *next;
};

//A table for name, program counter, occurance in the program
//Symbol table
struct Symbol       
{
    char name[50];
    int line;
    struct Symbol *next;
    int occurance;
};
typedef struct Opcode Opcode;
typedef struct Symbol Symbol;

//Twos complement of a 24 bit binary number
void twos_complement(char binary[24], int n){
   char onescomplement[n+1];   
   char twoscomplement[n+1];
   int carry=1; 

   for(int i=0;i<n;i++)  
   {  
       if(binary[i]=='0')  
       	onescomplement[i]='1';  
       else if(binary[i]=='1')  
       	onescomplement[i]='0';  
   }  
   onescomplement[n]='\0';  

	for(int i=n-1; i>=0; i--)  
    {  
        if(onescomplement[i] == '1' && carry == 1)  
        {  
            twoscomplement[i] = '0';  
        }  
        else if(onescomplement[i] == '0' && carry == 1)  
        {  
            twoscomplement[i] = '1';  
            carry = 0;  
        }  
        else  
        {  
            twoscomplement[i] = onescomplement[i];  
        }  
    }  
	twoscomplement[n]='\0';  
    for(int i=0; i<=24; i++){
        binary[i] = twoscomplement[i];
    }
}

//A common function to convert given char array from one base to another
void convert(char s[24],int base1,int base2, char res[24], int neg)
{
    int count=0;
    int num,i,n=0,k=1;
    for(i=strlen(s)-1;i>=0;i--){
        if(s[i]>='A' && s[i]<='Z'){
            num=s[i]-'0'-7;
        }
        else if(s[i]>='a' && s[i]<='z'){
            num = s[i]-'0'-39;
        }
        else
             num=s[i]-'0';
        n=num*k+n;
        k=k*base1;
    }
    // printf("%d\n", n);
    int r;
    while(n!=0)
    {
        r=n%base2;
        num='0'+r;
        // printf("%d\n",num);
        if(num>'9')
        {
            num+=7;
        }
        res[24-count-1]=num;
        count++;
        n=n/base2;
    }
    int m = 24 - count;
    
    for(int i=0; i<m;i++){
        res[i] = '0';
    }
    if(neg==1)twos_complement(res, 24);
}

//Open-chain hashing is done to retrieve Mnemonic data in O(1)
//Optable hash table with repective to mod 13
Opcode* hash_table[13] = {NULL};
Symbol *head=NULL;

//Check if a symbol is present in symbol table
int getSymbol(char *temp){
	Symbol * t = head;
	while(t!=NULL){
		if(!strcmp(temp, t->name)){
			return 1;
		}
		t = t->next;
	}
	return 0;
}


//get address(line) from the symbol table for the respective symbol provided
void getAddressCode(char* temp, char res[24], int *lineno)
{
    Symbol * t = head;
    int num;
    int x=0;
    while(t != NULL)
    {
        if(!strcmp(temp,t->name))
        {
        	t->occurance = t->occurance+1;
            num = t->line;
            *lineno = num;
            break;
        }
        t = t->next;
    }

    char s[20];
    sprintf(s, "%d", abs(num));
    if(num<0)
    	convert(s, 10, 2, res,1);

    if(num>=0)
    	convert(s, 10, 2, res,0);	
}

//hash the Mnemonic and the given index
void insertAtIndex(Opcode *Node,int index)
{
    if(hash_table[index] == NULL)
    {
        hash_table[index] = Node;
        Node->next = NULL;
    }
    else
    {
        Opcode* temp = hash_table[index];
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = Node;
        Node->next=NULL;
    }
}

//hashing index is generated using sum of ascii values of characters in the string
int getHashIndex(char name[])
{
    int sum=0,i=0;
    while(name[i]!='\0')
    {
        sum+=name[i++];
    }
    return sum%13;
}

//A function to insert Opcode table 
void insertIntoHashMap(Opcode *Node)
{
    int index = getHashIndex(Node->name);
    insertAtIndex(Node,index);
}

//check for Mnemonic in optable
int checkforOpcode(char *op){
	Opcode* temp = NULL;
    int index = getHashIndex(op);

    if(hash_table[index] == NULL)
    {	
        // printf("Wrong Opcode");
        return 0;
    }
    else
    {
        temp = hash_table[index];
        int x=0;
        while(temp){
        	if(strcmp(temp->name,op)==0){
        		x=1;
        		break;
        	}
        	temp = temp->next;
        	
        }
        if(!x)return 0;
    }
    return 1;
}

//get Opcode Node for the given Mnemonic in O(1) time
Opcode* getOpcodeNode(char *op)
{
    Opcode* temp = NULL;
    int index = getHashIndex(op);
    if(hash_table[index] == NULL)
    {
        // printf("Wrong Opcode");
        return NULL;
    }

    else
    {
        temp = hash_table[index];
        while(strcmp(temp->name,op)!=0 && temp!=NULL)
        {
            temp = temp->next;
        }
        if(temp == NULL)
        {
            // printf("Opcode not found!");
            return NULL;
        }
        else
        {
            return temp;
        }
    }
}


//Get opcode format given opcode node
char * getOpcodeFormat(Opcode* temp)
{
    return temp->format;
}


//trim whitespace in the starting and ending of a character array
char *trimwhitespace(char *str)
{
  char *end;
 
  while(isspace((unsigned char)*str)) str++;
 
  if(*str == 0)  // All spaces?
    return str;
 
  end = str + strlen(str) - 1;
 
  while(end > str && isspace((unsigned char)*end)) end--;
  // Write new null terminator character
  end[1] = '\0';
  return str;
}

//check for any warnings (Unused Label found)
void checkForWarnings(FILE *log){
	Symbol *t = head;
	int num;
	while(t!=NULL){
		if(t->occurance==0){
			fprintf(log, "Warning : Unused Label found '%s'\n", t->name);
		}
		t = t->next;
	}
}


//Main Program

int main(int argc, char *argv[]){
	// printf("Testing\n");
	
	/**************************************************************************************
 	 pass-1
	***************************************************************************************/

	//Take filename as argument from commandline and create filenames with the name provided
	char fileName[24];
	strcpy(fileName,argv[1]);
	char listingFile[24];
	char objectFile[24];
	char logFile[24];
	fileName[strlen(argv[1])]='\0';
	strncpy(listingFile, fileName, strlen(argv[1])-4);
	strcpy(objectFile,listingFile); 
	strcpy(logFile,objectFile);
	listingFile[strlen(argv[1])-4]='\0';
	objectFile[strlen(argv[1])-4] = '\0';
	logFile[strlen(argv[1])-4] = '\0';

	strcat(logFile,".log");
	strcat(listingFile, ".lst");
	strcat(objectFile, ".o");


	FILE *input, *output;
	int pc=0;
 
	char c1,c2,c3,temp;

	//optable 

    char opcode[100];
    char machine_code[100];
    char format[6];

    //Create nodes for Mnemonic, Opcode, Operand
    Opcode allNodes[21]={{"ldc", "00", "value"},{"adc", "01", "value"}, {"ldl" ,"02", "off"}, {"stl", "03" ,"off"},
						{"ldnl", "04" ,"off"},{"stnl", "05", "off"},{"add", "06", "nill"},{"sub", "07" ,"nill"},
						{"shl", "08", "nill"},{"shr", "09" ,"nill"},{"adj", "0a" ,"value"},{"a2sp", "0b", "nill"},
						{"sp2a" ,"0c", "nill"},{"call", "0d", "off"},{"return" ,"0e", "nill"},{"brz", "0f", "off"},
						{"brlz", "10", "off"},{"br", "11", "off"},{"HALT", "12", "nill"},{"data","13","value"},
						{"SET","14","value"}};

	for(int i=0; i<21; i++){
	    Opcode *Node = &allNodes[i];
	    //Hash all the nodes using open chain hashing 
	    insertIntoHashMap(Node);
	}

	//read the input asm file
    input = fopen(fileName, "r+");
    // output = fopen("output.o", "wb");


    int ilc=-1, base=0;
	char k;
	char checkscan[100];
	const char rem[100] = " :			\n";

	FILE *log;
	log = fopen(logFile, "w");
	int errorsCount=0;
	int linecounter=1;
	//Create symbol table. Store each symbol and it's address (program counter) in the table.
	//If the instruction is SET, then instead of PC, enter the value provided
	while(fgets(checkscan,sizeof checkscan, input) != NULL) /* read each line and process it */
   	{
	      int l=0;
	      char copyCode[100];
	      strcpy(copyCode,checkscan);
	      int comment=0;
	      //check for comment in each line
	      for(; comment<strlen(copyCode) ; comment++){
 			if(copyCode[comment] == ';')break;
 		  }
 		  copyCode[comment] = '\0';
 		  checkscan[comment] = '\0';
	      char *mb;
	      mb = strtok(copyCode,rem);
	      if(mb == NULL || mb[0] == ';' || mb[0]=='\n' || mb[0]==' '){
	      	continue;
	      }
	      while(checkscan[l+1]!='\0'){
	        if(checkscan[l]==':')   //A label
	        {
	            char *token;
 				char *opscan;
 				token = strtok(checkscan, rem);
 				if(token[0]>='0' && token[0]<='9'){
	 				fprintf(log, "Error at line %d: Bogus Label\n", linecounter);
	 				errorsCount++;
	 				continue;
	 			}
 				opscan = strtok(NULL, rem);
 				int set=0;
 				
 				if(opscan!=NULL){
	 				if(strcmp(opscan, "SET")==0){
	 					set=1;
	 				}
 				}	           
	           int i=0;
	           int left=0;
	           int right=l-1;
	           for(; i<l; i++){
	           	if(!isspace(checkscan[i]))break;
	           }
	           int j=l-1;
	           for(;j>=0; j--){
	           	if(!isspace(checkscan[j]))break;
	           }
	           char checkforlabel[100];
	           int mb;
	           for(mb=i; mb<=j; mb++){
	           		checkforlabel[mb-i] = checkscan[mb];
	           }
	           checkscan[mb+1] = '\0';
	           int x = getSymbol(checkforlabel);
	           if(x==1){
	           		fprintf(log, "Error at line %d: Duplicate Label\n", linecounter);
	           		errorsCount++;
	           		break;
	           }
	      
	           Symbol *t;
	           Symbol *temp = malloc(sizeof(Symbol));
	    	   int q;
	           for(q=i;q<=j;q++)
	                temp->name[q-i] = checkscan[q];

	            temp->name[q] = '\0';
	            if(!set)
	            	temp->line = ilc + 1 + base;
	            if(set){
	            	char *oper;
	            	oper = strtok(NULL,rem);
	            	if(oper!=NULL){
		            	int m;
		            	char *ptr;
		            	if(oper[0] == '0' && (oper[1]=='x' || oper[1]=='X') ){
		            		m = strtol(oper,&ptr, 16);	//hexadecimal to decimal
		            	}
		            	else if(oper[0]== '0' && (oper[1]!='x' && oper[1]!='X' && oper[1]!='\0')){
		            		m = strtol(oper,&ptr, 8);	//octal to decimal
		            	}
		            	else{ 
		            		m = strtol(oper, &ptr,10);	
		            	}	
		            	temp->line = m;
	            	} 
	            }
	            temp->next = NULL;
	           if(head == NULL)
	                head = temp;
	           else
	           {
	                t= head;
	                while(t->next!=NULL)
	                    t = t->next;
	                t->next = temp; 
	           }
	           if(opscan==NULL){
 					ilc--;
 			   }
	       }
	       l++;
	     }

	    ilc++;
	    linecounter++;
 	}
 	fclose(input);


/**************************************************************************************
 	 pass-2
***************************************************************************************/
  
 	input = fopen(fileName, "r+");
 	FILE *listing = fopen(listingFile, "w+");
 	FILE *object = fopen(objectFile,"wb");
 	int counter=0;
 	int countline=0;
 	char linescan[100];
 	const char eliminate[100] = " :			\n";
 	//get each line and search for the Mnemonic in the optable and symbols in symbol table. Form the 32 bit array with first 24 bits for
 	//operand and last 8 bit for opcode. If any error, log them to log file. At last, the 32 bit char array is converted into 32bit uint32 
 	//and is sent to the binary file using fwrite. Hence the object file is created.
 	while( fgets (linescan, sizeof linescan, input ) != NULL ){
 		countline++;
 		int comment=0;
 		char copyCode[100];
	    strcpy(copyCode,linescan);
		char counts[20];
 		char address[24];
 		sprintf(counts, "%d",counter);
 		convert(counts,10,16,address,0);

 		char binary[32];
 		for(; comment<strlen(linescan) ; comment++){
 			if(linescan[comment] == ';')break;
 		}
 		
 		linescan[comment] = '\0';
 		copyCode[comment] = '\0';
 		char *mb;
	    mb = strtok(copyCode,eliminate);

	    if(mb == NULL || mb[0] == ';' || mb[0]=='\n'){
	      	continue;
	    }
 		char takeline[100];
 		strcpy(takeline,linescan);

 		char *token,*opscan; 
 		int islabel=0;
 		for(int i=0; i<strlen(linescan); i++){
 			if(linescan[i] == ':'){
 				islabel=1;	
 				linescan[i] = ' ';
 			}
 		}
 		int isdata=0, isset=0;
 		int bogus=0;
 		int onlyLabel=0;
 		if(islabel){
	 		token = strtok(linescan,eliminate);
	 		if(token==NULL)continue; 
	 		if(token[0]>='0' && token[0]<='9'){
 				// fprintf(log, "Error at line %d: Bogus Label\n", countline);
 				errorsCount++;
 			}
	 		opscan = strtok(NULL,eliminate);
	 		
	 		if(opscan!=NULL){
	 			if(strcmp("data",opscan)==0)isdata=1;
 				if(strcmp("SET",opscan)==0)isset=1;

	 			if(checkforOpcode(opscan) == 0){
	 				fprintf(log, "Error at line %d: Bogus Mnemonic\n", countline);
	 				bogus=1;
	 				errorsCount++;
	 			}
	 		}
	 		for(int abc=18; abc<24; abc++){
 				fprintf(listing, "%c", address[abc]);
 			}
 			fprintf(listing, " ");
 			if(opscan == NULL){
 				onlyLabel=1;
 				for(int z=0; z<8; z++)fprintf(listing, " ");
 			}
 		}
 		if(!islabel){
 			opscan = strtok(linescan,eliminate);
 			if(opscan==NULL)continue;
 			if(opscan!=NULL){
 				if(strcmp("data", opscan)==0)isdata=1;
 				if(strcmp("SET",opscan)==0)isset=1;

	 			if(checkforOpcode(opscan) == 0){
	 				fprintf(log,"Error at line %d: Bogus Mnemonic\n", countline);
	 				bogus=1;
	 				errorsCount++;
	 			}
	 			for(int abc=18; abc<24; abc++){
 					fprintf(listing, "%c", address[abc]);
 				}
 				fprintf(listing, " ");
 			}
 		}
 		if(bogus)continue;
 		char *temp; 
 		if(opscan!=NULL){
 			counter++;
 			// if(strcmp("SET",opscan)==0)continue;
 			if(getOpcodeNode(opscan) ==	 NULL){
	 			fprintf(log,"Error at line %d: Unknown instruction", countline);
	 			errorsCount++;
	 			continue;
	 		}
	 		//fetch for the current node in the optable
 			Opcode* current_node = getOpcodeNode(opscan);
 			if(strcmp("value", getOpcodeFormat(current_node)) == 0 || strcmp("off",getOpcodeFormat(current_node)) == 0){
 				temp = strtok(NULL,eliminate);
 				if(temp == NULL){
 					fprintf(log,"Error at line %d: Missing Operand\n", countline);
           			errorsCount++;
           			continue;
 				}
 				else{
 					int start=18, size=6;
 					if(isdata || isset){
 						start=16;
 						size=8;
 					}	
 					if( (temp[0]>='a' && temp[0]<='z') || (temp[0]>='A' && temp[0]<='Z') ){
 						//label - get the address code from the symbol table and convert it to hex. Dump it to binary
 						if(getSymbol(temp)==0){
			 				fprintf(log,"Error at line %d: No such label\n", countline);
			 				errorsCount++;
			 				continue;
			 			}
 						char res[24],hex[24],deci[24], temp1[24];
 						int m;
 						getAddressCode(temp, res, &m);
 						sprintf(temp1,"%x",m);
 						sprintf(hex, "%x", m);
 						int n = strlen(hex);
 						int st=0;

 						for(int i=0; i<size-n; i++)
 							fprintf(listing, "0");
 						
 						while(n-st-size>0)st++;

 						for(int i=st; i<n; i++){
 							fprintf(listing, "%c", hex[i]);
 						}
 						strcpy(binary,temp1);
 					}
 					else if(temp[0] == '0' && (temp[1] == 'x' || temp[1] == 'X')){
 						//hexadecimal - dump it to binary array
 						char *ptr;
 						int m = strtol(temp,&ptr,16);
 						if(ptr[0]!='\0'){
	            			fprintf(log,"Error at line %d: Not a number\n", countline);
	            			errorsCount++;
	            		}
 						char temp1[24],res[24],hex[24];
 						sprintf(temp1,"%x",m);

 						sprintf(hex, "%x", m);
 						int n = strlen(hex);
 						int st=0;

 						for(int i=0; i<size-n; i++)
 							fprintf(listing, "0");
 						
 						while(n-st-size>0)st++;

 						for(int i=st; i<n; i++){
 							fprintf(listing, "%c", hex[i]);
 						}
 						strcpy(binary,temp1);
 					}
 					else if(temp[0] == '0' && (temp[1] != 'x' && temp[1]!='X' && temp[1]!='\0')){
 						//octal	- convert it into hex and dump it into binary
 						char *ptr;
 						int m = strtol(temp,&ptr,8);
 						if(ptr[0]!='\0'){
	            			fprintf(log,"Error at line %d: Not a number\n", countline);
	            			errorsCount++;
	            		}
 						char temp1[24],res[24],hex[24];
 						
 						sprintf(temp1,"%x",m);

 						sprintf(hex, "%x", m);
 						int n = strlen(hex);
 						int st=0;

 						for(int i=0; i<size-n; i++)
 							fprintf(listing, "0");
 						
 						while(n-st-size>0)st++;

 						for(int i=st; i<n; i++){
 							fprintf(listing, "%c", hex[i]);
 						}
 						strcpy(binary,temp1);
 					}	
 					else{
 						//decimal - convert into hex and dump into binary
 						char *ptr;
 						int m = strtol(temp,&ptr,10);
 						if(ptr[0]!='\0'){
	            			fprintf(log,"Error at line %d: Not a number\n", countline);
	            			errorsCount++;
	            		}
 						char temp1[24],res[24],hex[24];

 						sprintf(temp1,"%x",m);

 						sprintf(hex, "%x", m);
 						int n = strlen(hex);
 						int st=0;

 						for(int i=0; i<size-n; i++)
 							fprintf(listing, "0");
 						
 						while(n-st-size>0)st++;

 						for(int i=st; i<n; i++){
 							fprintf(listing, "%c", hex[i]);
 						}
 						strcpy(binary,temp1);
 					}
 				}
 			}
 			else if(strcmp("nill", getOpcodeFormat(current_node)) == 0){
 				for(int i=0; i<6; i++)
 						fprintf(listing, "0");
 			}	
 			char *extras;
            extras = strtok(NULL,eliminate);
            if(extras != NULL){
           		fprintf(log,"Error at line %d: Extras on the end of the line\n", countline);
           		errorsCount++;
           	}
 			
           	strcat(binary, current_node->code);
 			
 			if(!isset && !isdata){
 				fprintf(listing, "%s", current_node->code);
 			}
 		}
 
 		int rspace=0;
 		for(; rspace<=strlen(takeline); rspace++){
 			if(!isspace(takeline[rspace]))break; 
 		}
 		fprintf(listing, " ");
 		for(; rspace<strlen(takeline); rspace++){
 			if(takeline[rspace] == '\n')break;
 			fprintf(listing, "%c", takeline[rspace]);
 		}
 		fprintf(listing, "\n");

 		//Encode into binary
 		if(!onlyLabel){
 			uint32_t n=0;
 			int m=1;
			int w=0;
			int num;
			for(int i=strlen(binary)-1;i>=0;i--){
				if(w==8)break;
		        if(binary[i]>='A' && binary[i]<='Z'){
		            num=binary[i]-'0'-7;
		        }
		        else if(binary[i]>='a' && binary[i]<='z'){
		            num = binary[i]-'0'-39;
		        }
		        else
		             num=binary[i]-'0';
		        n=num*m+n;
		        m=m*16;
		        w++;
		    }
		    //write into the binary file
			fwrite(&n, sizeof(uint32_t), 1, object);
 		}
 	}

 	checkForWarnings(log);

 	fclose(listing);
    fclose(input);
    fclose(object);


    //if error delete the half generated listing file and object file
    if(errorsCount>0){
    	if(remove(listingFile) == 0){}
    	if(remove(objectFile)==0){}
    }



	//TESTCHECK

	// printf("Check the binary file from here\n");
	// FILE *fin = fopen(objectFile,"rb");
	// uint32_t s;
	// while(fread(&s,sizeof(uint32_t), 1,fin) != 0){
	// 	printf("%" PRIu32 "\n", s);
	// }
	
	// fclose(fin);	
	return 0;
}