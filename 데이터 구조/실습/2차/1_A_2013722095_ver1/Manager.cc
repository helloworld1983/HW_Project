#include "Manager.h"

Manager::Manager(int bpOrder){
	fout.open("log.txt", ios::app);
	avl = new AVLTree(&fout);
	bp = new BpTree(&fout, bpOrder);	
}

Manager::~Manager(){
	delete avl;
	delete bp;
	fout.close();
}

void Manager::run(const char* command_txt){

	ifstream fin;		fin.open(command_txt); // open command file
	int	load = -1;											// load flag
		
	if(!fin){ // failed to open command file
		cout<<"[ERROR] command file open error!"<<endl;
		return;
	}

	char*	str='\0';		// token
	char	buf[128]={0};
	char	buf2[128]={0};

	while(fin.getline(buf, 64)){ // get text line into the 'buf' array until the end of file
		strcpy(buf2, buf);			// copy buf into buf2
		str=strtok(buf, " \n");		// devide buf array by '\n'
		if (strcmp(str, "EXIT") == 0)
			return;
		if(strcmp(str, "LOAD") == 0){ // LOAD command
			if(load == 1){					// if program already implemented load command, print error code
				printErrorCode(100); continue;
			}else if((str=strtok('\0', " \n")) != '\0' || !LOAD()){ // if token is null or couldn't implement LOAD function, print error code
				printErrorCode(100); continue;
			}
			load = 1;
			cout << "======== LOAD ========" << endl;
			cout << "Success" << endl;
			cout << "======================" << endl;
			fout << "======== LOAD ========" << endl;
			fout << "Success" << endl;
			fout << "======================" << endl;
			continue;
		}
		else if (strcmp(str, "SEARCH_AVL") == 0)
		{
			str = strtok('\0', "\n");
			if (SEARCH_AVL(str) == false)// excute SEARCH_AVL function / if it return false, print error code
				printErrorCode(300);
			continue;
			
		}
		else if(strcmp(str, "SEARCH_BP") == 0)// excute SEARCH_BP function 
		{
			char *tok1, *tok2, *tok3 = '\0';
			tok1 = strtok('\0', " ");
			tok2 = strtok('\0', " ");
			tok3 = strtok('\0', " ");
			if (tok1 == '\0' || tok2 == '\0' || tok3 == '\0') // case : didn't insert all infomation for searching / print errorcode
			{
				printErrorCode(200);
				continue;
			}
			double a = atof(tok1);
			double b = atof(tok2);
			int year = atoi(tok3);
			if (SEARCH_BP(a, b, year) == false)  // if it return false(can't find student), print error code
				printErrorCode(200);
			continue;
		}
		else if(strcmp(str, "RANK") == 0)// excute RANK function 
		{
			if (RANK()== false)
				printErrorCode(400);
			continue;
		}
		else if(strcmp(str, "PRINT_AVL") == 0)// excute PRINT_AVL function 
		{
			if (PRINT_AVL() == false)
				printErrorCode(600);
			continue;
		}
		else if(strcmp(str, "PRINT_BP") == 0)// excute PRINT_BP function 
		{
			if (PRINT_BP() == false)
				printErrorCode(500);
			continue;
		}
		fout<<endl;
	}	
	fin.close();
}

bool Manager::LOAD(){
	
	char*	str='\0';
	char	buf[128]={0};
	char	buf2[128]={0};

	ifstream fin;
	fin.open("grade_list.txt");
	
	if(!fin){ // failed to opend grade_list file
		printErrorCode(100);
		return false;
	}
		
	while(fin.getline(buf, 64)){
		StudentData* pStu;
		strcpy(buf2, buf);

		str=strtok(buf, " ");
		str=strtok('\0', " ");
		str=strtok('\0', " ");
		/* make node accoding to student's grade*/
		if(strcmp(str, "1") == 0){
			pStu = new Freshman();	

		}else if(strcmp(str, "2") == 0){
			pStu = new Sophomore();

		}else if(strcmp(str, "3") == 0){
			pStu = new Junior();

		}else if(strcmp(str, "4") == 0){
			pStu = new Senior();

		}
		/* insert student's information*/
		str=strtok(buf2, " ");
		pStu->setStudentID(atoi(str));
		str=strtok('\0', " ");
		pStu->setName(str);
		str=strtok('\0', " ");
		pStu->setYear(atoi(str));
		pStu->setAvgGrade(strtok('\0', " "), strtok('\0', " "), strtok('\0', " "));

		/*insert node at each Data Structure*/
		avl->Insert(pStu);
		bp->Insert(pStu);
	}
	return true;
}

bool Manager::SEARCH_AVL(char* name){
	if(!avl->Search(name))	return false;
	else							return true;
}

bool Manager::SEARCH_BP(double a, double b, int year){
	if(!bp->Search(a, b, year))			return false;
	else											return true;
}

bool Manager::RANK(){
	if(!avl->Rank())		return false;
	else					return true;
}

bool Manager::PRINT_AVL(){
	if(!avl->Print())	return false;
	else					return true;
}

bool Manager::PRINT_BP(){
	if(!bp->Print())		return false;
	else					return true;
}

void Manager::printErrorCode(int n){
	cout << "======== ERROR ========" << endl;
	cout << n << endl;
	cout << "=======================" << endl;
	fout<<"======== ERROR ========"<<endl;
	fout<<n<<endl;
	fout<<"======================="<<endl;
}