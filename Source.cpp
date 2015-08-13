#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<cstring>
#include<vector>
#include<sstream>
#include<streambuf>

using namespace std;

int getNum(stringstream& iss);
string toLowerStr(string str);
void consumeSpace(stringstream& iss);
void multiply(stringstream& iss, ofstream& ofs);
void divide(stringstream& iss, ofstream& ofs);
void modulo(stringstream& iss, ofstream& ofs);
void addition(stringstream& iss, ofstream& ofs);
void subtraction(stringstream& iss, ofstream& ofs);
void factor(stringstream& iss,ofstream& ofs);
void morefactors(stringstream& iss,ofstream& ofs);
void moreterms(stringstream& iss,ofstream& ofs);
void term(stringstream& iss,ofstream& ofs);
void expr(stringstream& iss,ofstream& ofs);
void print_stm(stringstream& iss,ofstream& ofs);
void read_stm(stringstream& iss,ofstream& ofs);
void if_stm(stringstream& iss,ofstream& ofs);
void while_stm(stringstream& iss,ofstream& ofs);
void init_stm(stringstream& iss,ofstream& ofs, string s);
bool stm(stringstream& iss,ofstream& ofs);
void stmt_list(stringstream& iss,ofstream& ofs);
void opt_stmts(stringstream& iss, ofstream& ofs);
void start(stringstream& iss,ofstream& ofs);



vector<string> variables;	//Stores the initialiized variablenames 
int labelcounter = 1;		//Stores the current label number

/*
	Main Function
	Splits the given program into tokens
	and	starts it
*/

int main(){
	string filename;

	//Takes the name input using std::in
	cout<<"Please enter the filename: ";
	cin>>filename;

	ifstream ifs((filename+".my").c_str());
	ofstream ofs((filename+".asm").c_str());
	ofs<<"code segment"<<" \n";
	
	//reading all the file to a string
	string str((istreambuf_iterator<char>(ifs)),
                 istreambuf_iterator<char>());
	for (int i = 0; i < str.length(); ++i){
		if(str[i]==' '||str[i]=='\n'){
			str[i]='&';
		}
	}
	stringstream iss1(str);
	string endOf="=+-/%*();";
	string tokenized="";
	char ch;
	while(iss1>>ch){
		if(ch=='&'){
			tokenized+=' ';
			consumeSpace(iss1);
		}else if(strchr(endOf.c_str(),ch) != NULL){
			tokenized+=' ';
			tokenized+=ch;
			tokenized+=' ';
		}else{
			tokenized+=ch;
		}
	}
	
	stringstream iss(tokenized);
	
	start(iss,ofs);
	
	ofs<<" int 20h "<<" \n";
	ofs<<"myread: "<<" \n";
  	ofs<<" mov  cx,0 "<<" \n";
  	ofs<<"morechar: "<<" \n";	
  	ofs<<" mov  ah,01h "<<" \n";
  	ofs<<" int  21h "<<" \n";
  	ofs<<" mov  dx,0 "<<" \n";
  	ofs<<" mov  dl,al "<<" \n";
  	ofs<<" mov  ax,cx "<<" \n";
  	ofs<<" cmp  dl,0d "<<" \n";
  	ofs<<" je   myret "<<" \n";
  	ofs<<" sub  dx,48d "<<" \n";
  	ofs<<" mov  bp,dx "<<" \n";
  	ofs<<" mov  ax,cx "<<" \n";
  	ofs<<" mov  cx,10d "<<" \n";
  	ofs<<" mul  cx "<<" \n";
  	ofs<<" add  ax,bp "<<" \n";
  	ofs<<" mov  cx,ax "<<" \n";
  	ofs<<" jmp  morechar "<<" \n";
 	ofs<<"myret: "<<" \n";
  	ofs<<" ret "<<" \n";
	ofs<<"myprint: "<<" \n";
  	ofs<<" mov  si,10d "<<" \n";
  	ofs<<" xor  dx,dx "<<" \n";
  	ofs<<" push ' ' "<<" \n";
  	ofs<<" mov  cx,1d "<<" \n";
	ofs<<"nonzero: "<<" \n";
  	ofs<<" div  si "<<" \n";
  	ofs<<" add  dx,48d "<<" \n";
  	ofs<<" push dx "<<" \n";
  	ofs<<" inc  cx "<<" \n";
  	ofs<<" xor  dx,dx "<<" \n";
  	ofs<<" cmp  ax,0h "<<" \n";
  	ofs<<" jne  nonzero "<<" \n";
	ofs<<"writeloop: "<<" \n";
  	ofs<<" pop  dx "<<" \n";
  	ofs<<" mov  ah,02h "<<" \n";
  	ofs<<" int  21h "<<" \n";
  	ofs<<" dec  cx "<<" \n";
  	ofs<<" jnz  writeloop "<<" \n";
  	ofs<<" ret "<<" \n"<<" \n";

  	for (int i = 0; i < variables.size(); ++i)
  	{
  		ofs<<"v"<<variables[i]<<"  dw  ?"<<" \n";
  	}
	ofs<<"code ends"<<" \n";
	return 0;
}

/*
Controls whether the variable is initialized before
*/
bool isInitialized(string str){
	for (int i = 0; i < variables.size(); ++i)
	{
		if(str.compare(variables[i])==0)
			return true;
	}
}


/*
Converts the number string into integer
*/
int getNum(stringstream& iss,string str){	
	
	int num = atoi(str.c_str());
	
	return num;
}

/*
Assembly is not case-sensitive that's why 
This function lowers the all characters
*/
string toLowerStr(string str){
	for (int i = 0; i < str.length(); ++i){
		str[i]=tolower(str[i]);
	}
	return str;
}

/*
	Consumes unnecessary spaces
*/
void consumeSpace(stringstream& iss){
	char c;
	while(iss.peek() == '&'){
		iss>>c;
	}
}

/*
	Multiplication
*/
void multiply(stringstream& iss, ofstream& ofs){
	factor(iss,ofs);
	ofs<<" pop cx "<<" \n";
	ofs<<" pop ax "<<" \n";
	ofs<<" mul cx "<<" \n";
	ofs<<" push ax "<<" \n";
	morefactors(iss,ofs);
}

/*
	Division
*/
void divide(stringstream& iss, ofstream& ofs){
	factor(iss, ofs);
	ofs<<" mov dx, 0 "<<" \n";
	ofs<<" pop cx "<<" \n";
	ofs<<" pop ax "<<" \n";
	ofs<<" div cx "<<" \n";
	ofs<<" push ax "<<" \n";
	morefactors(iss,ofs);
}
/*
	Modulo operation
*/
void modulo(stringstream& iss, ofstream& ofs){
	factor(iss, ofs);
	ofs<<" mov dx, 0 "<<" \n";
	ofs<<" pop cx "<<" \n";
	ofs<<" pop ax "<<" \n";
	ofs<<" div cx "<<" \n";
	ofs<<" push dx "<<" \n";
	morefactors(iss,ofs);
}
/*
	Addition operation 
*/
void addition(stringstream& iss, ofstream& ofs){
	term(iss, ofs);
	ofs<<" pop cx "<<" \n";
	ofs<<" pop ax "<<" \n";
	ofs<<" add ax,cx "<<" \n";
	ofs<<" push ax "<<" \n";
	moreterms(iss,ofs);
}
/*
	Subtraction
*/

void subtraction(stringstream& iss, ofstream& ofs){
	term(iss, ofs);
	ofs<<" pop cx "<<" \n";
	ofs<<" pop ax "<<" \n";
	ofs<<" sub ax,cx "<<" \n";
	ofs<<" push ax "<<" \n";
	moreterms(iss,ofs);
}

/*
	Determines factor type
	(expression), id or num
*/

void factor(stringstream& iss,ofstream& ofs){
	string str;
	
	iss>>str;
	
	if(str[0]=='('){		
		expr(iss,ofs);
		iss>>str;
		if(str.compare(")")==0){
			return;
		}else{
			cout<<"')' expected";
			exit(EXIT_FAILURE);
		}
	}else if((str[0]>='a' && str[0]<='z') || (str[0]>='A' && str[0]<='Z')){
		ofs<<" push v"<<toLowerStr(str)<<" w "<<" \n";
	}else if(str[0]>='0' && str[0]<='9'){
		ofs<<" push "<<getNum(iss,str);	
		if(getNum(iss,str)!=0)	
			ofs<<"d ";
		ofs<<" \n";
	}else{
		cout<<"invalid expression"<<" \n";
		exit(EXIT_FAILURE);
	}
}

/*
	Does multiplication, division and modulo operation
*/
void morefactors(stringstream& iss,ofstream& ofs){

	string str;
	streampos sp = iss.tellg(); 
	iss>>str;

	
	if(str.compare("*")==0||str.compare("mod")==0){
		multiply(iss,ofs);
	}else if(str.compare("/")==0||str.compare("div")==0){
		divide(iss,ofs);
	}else if(str.compare("%")==0){
		modulo(iss,ofs);
	}else{
		iss.seekg(sp);
	}

}
/*
	Does addition and subtraction
*/
void moreterms(stringstream& iss,ofstream& ofs){
	string str;
	streampos sp = iss.tellg(); 
	iss>>str;
	if(str.compare("+")==0){
		addition(iss,ofs);
	}else if(str.compare("-")==0){		
		subtraction(iss,ofs);
	}else{
		iss.seekg(sp);
	}
}
/*
	Term >> Factor Morefactors
*/

void term(stringstream& iss,ofstream& ofs){
	
	factor(iss,ofs);
	morefactors(iss,ofs);
	

/*
	Expression >> Term Moreterms
*/
}
void expr(stringstream& iss,ofstream& ofs){
	
	term(iss, ofs);
	moreterms(iss, ofs);

}
/*
	Converts print statement
	print expr
*/

void print_stm(stringstream& iss,ofstream& ofs){
	expr(iss,ofs);
	ofs<<" pop ax "<<" \n";
	ofs<<" call myprint "<<" \n";

}
/*
	Converts Read statements
	Read id
*/

void read_stm(stringstream& iss,ofstream& ofs){
	string varName;
	iss>>varName;
	if(!(varName[0]>='a' && varName[0]<='z') || (varName[0]>='A' && varName[0]<='Z')){
		cout<<"Illegal name for variable"<<" \n";
		exit(EXIT_FAILURE);
	}
	ofs<<" call myread "<<" \n";
	ofs<<" mov v"<<toLowerStr(varName)<<",cx "<<" \n";
	if(!(isInitialized(varName))){
		variables.push_back(varName);
	}

}
/*
	Converts if statements
	if expr then stm
*/

void if_stm(stringstream& iss,ofstream& ofs){

	int label= labelcounter;
	labelcounter++;
	string str;
	expr(iss,ofs);	
	iss>>str;
	if(str.compare("then")!=0){
		cout<<"\"Then\" Expected"<<" \n";
		exit(EXIT_FAILURE);
	}
	ofs<<" pop ax "<<" \n";
	ofs<<" IF Z jmp lbl"<<label<<" \n";
	stm(iss,ofs);
	ofs<<"lbl"<<label<<": "<<" \n";

	
	
}
/*
	While statement
	while expression then stm
*/
void while_stm(stringstream& iss,ofstream& ofs){
	int label= labelcounter;
	labelcounter+=2;
	ofs<<"lbl"<<label<<":"<<" \n";
	string str;
	expr(iss,ofs);
	iss>>str;

	if(str.compare("do")!=0){
		cout<<"\"do\"expected"<<" \n";
		exit(EXIT_FAILURE);
	}
	ofs<<" pop ax "<<" \n";
	ofs<<" cmp ax,0 "<<" \n";
	ofs<<" IF Z jmp lbl"<<label+1<<" \n";

	stm(iss,ofs);
	ofs<<" jmp lbl"<<label<<" \n";
	ofs<<"lbl"<<label+1<<":"<<" \n";
	
}

/*
	Initilization and assignment operations
	id = expr
*/
void init_stm(stringstream& iss,ofstream& ofs, string var){
	if(!isInitialized(var))
		variables.push_back(toLowerStr(var));
	string str;
	iss>>str;
	if(str.compare("=")!=0){
		cout<<" \"=\" expected "<<" \n";
		exit(EXIT_FAILURE);
	}
//	if(!isInitialized(var))
		ofs<<" push offset v"<<toLowerStr(var)<<" \n";
	
	expr(iss,ofs);
	ofs<<" pop ax"<<" \n";
	ofs<<" pop bp"<<" \n";
	ofs<<" mov [bp],ax"<<" \n";
}
/*
	Determines the type of assignment
	print, read, if, while,begin, assignment
*/
bool stm(stringstream& iss,ofstream& ofs){

	string str;
	iss>>str;	
	if(str.compare("end")==0){
		return false;
	}
	else if(str.compare("print")==0){
		print_stm(iss,ofs);
	}else if(str.compare("read")==0){
		read_stm(iss,ofs);
	}else if(str.compare("if")==0){
		if_stm(iss,ofs);
	}else if(str.compare("while")==0){
		while_stm(iss,ofs);
	}else if(str.compare("begin")==0){
		opt_stmts(iss,ofs);
	}else if((str[0]>='a' && str[0]<='z') || (str[0]>='A' && str[0]<='Z')){
		init_stm(iss,ofs, str);
	}else{
		return false;
	}
	
	streampos sp=iss.tellg();
	iss>>str;

	if(str.compare(";")==0)
		return true;
	
	iss.seekg(sp);
	return false;	

}
/*
	Statement list
	stm ; stmt_list
	stm
*/

void stmt_list(stringstream& iss,ofstream& ofs){	
	while(stm(iss,ofs)){}
	string str;
	streampos sp= iss.tellg();
	if(iss>>str){
		if(str.compare("end")==0){
			iss.seekg(sp);
		}else{
			iss.seekg(sp);
			stmt_list(iss,ofs);
		}
	}

}
/*
	Optional statements
	stmt_list
	empty
*/

void opt_stmts(stringstream& iss,ofstream& ofs){
	stmt_list(iss,ofs);
	string str;
	if(iss>>str){
		if(str.compare("end")!=0){
			cout<<"end expected"<<" \n";
			exit(EXIT_FAILURE);
		}
		streampos sp = iss.tellg();
		if(iss>>str){
			iss.seekg(sp);
			stm(iss, ofs);
		}
	}
}

/*
	Starts Conversion
*/

void start(stringstream& iss,ofstream& ofs){	
	stm(iss,ofs);	
}

