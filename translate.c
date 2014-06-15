/**
	@author: Michal Robaszynski 194165
*/

/*

FIXIT

1. ograniczenie na wielkość zmiennej
2. dość brzydko zrealizowane dzielenie
3. brak sprawdzenia czy wartość zmiennej została zdefiniowana
4. wiadomości o błędach...

*/

#ifndef TRANSLATE_GUARD
#define TRANSLATE_GUARD

#define DEBUG 0
#define ERR 0
#define LINECODE 0

#define str std::string
#define vec std::vector
#define longlong unsigned long long int

vec<str> outputSource;

vec<int> jumpStack; //przechowuje miejsca skaczace
vec<int> labelStack; //przechowuje miejsca do ktorych beda odbywac sie skoki


int tempLines;
vec<str> tempCode;

const str op_temp_a="0";
const str op_temp_b="1";
const str op_temp_c="2";
const str op_temp_d="3";
const int reserved_registers_number=4;

const str S_PLUS("+");
const str S_MINUS("-");
const str S_MULT("*");
const str S_DIV("/");
const str S_MOD("%");

const str S_GET(">=");
const str S_EQ("==");
const str S_NEQ("!=");
const str S_GT(">");



/** POMOCNICZE */
int isNumber(str name){
   const char* buf = name.c_str();
   while(*buf){
      if (!isdigit(*buf))
         return 0;
      else
         ++buf;
   }
   return 1;
}

class Constant{
	public:
		str name;
		str value;
		
		Constant(str n, str v){
			this->name=n;
			this->value=v;
		}
};

class ConstantManager{
	private:
		vec<Constant*> constantVector;
	public:
		int addConstant(str constName, str constValue){
			if(getItemIndex(constName)>-1){return 1;}
			
			Constant* c=new Constant(constName, constValue);
			
			constantVector.push_back(c);
			return 0; //success
		}
		
		int getItemIndex(str constName){
			for(int i=0; i<constantVector.size(); i++){
				if(constName==constantVector.at(i)->name){
					return i; //found
				}
			}
			//if(ERR)printf("*******Nie znaleziono stalej %s\n", constName.c_str());
			return -1; //not found
		}
		
		str getItemValue(str constName){
			int i=getItemIndex(constName);
			if(i==-1){
				//if(ERR)printf("*******Nie znaleziono wartosci stalej %s\n", constName.c_str());
				return NULL;
			}
			return constantVector.at(i)->value;
		}
		
		str getItemValue(int index){
			if(DEBUG)printf("A");
			if(index>constantVector.size()){
				if(ERR)printf("POZA ZAKRESEM\n");
				return NULL;
			}
			return constantVector.at(index)->value;
		}
};

ConstantManager constantManager;

class VariableManager{
	private:
		vec<str> variableVector;
	public:
		int addVariable(str varName){
			if(getItemIndex(varName)>-1){
				if(ERR)printf("*******ZMIENNA JUZ ISTNIEJE");
				return 1; // juz istnieje
			}
			
			if(constantManager.getItemIndex(varName)>-1){
				if(ERR)printf("*******ISTNIEJE JUZ STALA O TAKIEJ NAZWIE");
				return 2; // juz istnieje stala o takiej nazwie
			}
			
			variableVector.push_back(varName);
			
			return 0; // sukces
		}
		
		int getItemIndex(str varName){
			for(int i=0; i<variableVector.size(); i++){
				if(varName==variableVector.at(i)){
					return i+reserved_registers_number;
				}
			}
			//if(ERR)printf("*******nie ma takiej zmiennej");
			return -1;
		}
};

VariableManager	variableManager;

void addCodeLine(str line){
	tempCode.push_back(line);
}


void generateDivision(){
	char temp[50];
		int mod=0;
		//c=0, d=1
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("STORE " + op_temp_c);
		//jezeli b=0 to skaczemy 
		//addCodeLine("JZ " + tempCode.size()+66);
		sprintf(temp, "JZ %d", tempCode.size()+mod+66);addCodeLine(temp);
		addCodeLine("ZERO");
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("INC");
		addCodeLine("STORE " + op_temp_d);
		
		//jesli b parzyste -> skacz (czyt. jezeli b+1 nieparzyste -> skacz)
		// ********** 0 *********
		int label[7];
		label[0] = tempCode.size();
		addCodeLine("LOAD " + op_temp_b);
		//addCodeLine("INC");
		// dopokia b+1 jest nieparzyste (b jest parzyste)
		sprintf(temp, "JODD %d", tempCode.size()+mod+17);addCodeLine(temp);
		addCodeLine("LOAD " + op_temp_d);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_d);
		
		//jezeli a+1 nieparzyste -> 2
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("INC");
		sprintf(temp, "JODD %d", tempCode.size()+mod+4);addCodeLine(temp);
		addCodeLine("LOAD " + op_temp_d);
		addCodeLine("INC");
		addCodeLine("STORE " + op_temp_d);
		// ************ 2 **********
		
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_a);
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_b);
		sprintf(temp, "JUMP %d", label[0]);addCodeLine(temp);
		

		// **************** 1
		label[1]=tempCode.size();
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SUB " + op_temp_a);
		sprintf(temp, "JG %d", tempCode.size()+mod+5);addCodeLine(temp); //3
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_b);
		sprintf(temp, "JUMP %d", label[1]);addCodeLine(temp); //1
		
		// ************* 3
		label[3]=tempCode.size();
		addCodeLine("LOAD " + op_temp_b);
		sprintf(temp, "JODD %d", tempCode.size()+mod+16);addCodeLine(temp); //4
		addCodeLine("LOAD " + op_temp_c);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_b);
		addCodeLine("SUB " + op_temp_a);
		sprintf(temp, "JG %d", tempCode.size()+mod+7);addCodeLine(temp); // -> 5
		addCodeLine("LOAD " + op_temp_c);
		addCodeLine("INC");
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SUB " + op_temp_b);
		addCodeLine("STORE " + op_temp_a);
		// ****************** 5
		sprintf(temp, "JUMP %d", label[3]);addCodeLine(temp); // ->3
		// jezeli d>0 (r>0) to rob; jesli rowna- skacz
		// ***************** 4
		label[4]=tempCode.size();
		addCodeLine("LOAD " + op_temp_d);
		sprintf(temp, "JZ %d", tempCode.size()+mod+14);addCodeLine(temp); // -> 6
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_a);
		addCodeLine("LOAD " + op_temp_d);
		addCodeLine("INC");
		sprintf(temp, "JODD %d", tempCode.size()+mod+4);addCodeLine(temp); // -> 7
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("INC");
		addCodeLine("STORE " + op_temp_a);
		// ************* 7
		addCodeLine("LOAD " + op_temp_d);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_d);
		sprintf(temp, "JUMP %d", label[4]);addCodeLine(temp); // -> 4
		// **************** 6
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_a);
		addCodeLine("STORE " + op_temp_b); //reszta w b
		addCodeLine("LOAD " + op_temp_c); // przepisac wynik z c do a 
		addCodeLine("STORE " + op_temp_a);
}

/**
 * ZARZADZANIE PAMIECIA
 */

/** generuje wartosc w rejestrze a **/
/* Przestarzala: ograniczona dlugosc
void generateValue(unsigned long long int a){
	addCodeLine("ZERO");
	vec<str> buff;
	while(a>0){
		if(a%2){ //jesli a nieparzyste
			buff.push_back("INC");
			a--;
		}else{ //jesli a parzyste
			buff.push_back("SHL");
			a/=2;
		}
	}

	for(int i=buff.size()-1; i>=0; i--){
        addCodeLine(buff.at(i));
	}
}

void generateValue(str a){
	unsigned long long int j=atoll(a.c_str());
	generateValue(j);
}*/


void binaryNumberToCode(str bin){
    vec<str> v;
    addCodeLine("ZERO");
    //printf("ZMIANA\n");
    while(bin!="0"){
        //printf("BIN %s\n", bin.c_str());
        char l=bin.at(bin.size()-1);
        if(l=='1'){
            v.push_back("INC");
            bin.at(bin.size()-1)='0';
        }else if(l=='0'){
            v.push_back("SHL");
            bin.erase(bin.size()-1, 1);
        }
    }
    for(int i=v.size()-1; i>=0; i--){
        addCodeLine(v.at(i));
	}
}

str divideByTwo(str dec){
    int rem=0;
    int m=dec.size();
    for(int i=0; i<m; i++){
        int a=dec.at(i)-48; //cyfra
        dec.at(i)=(rem+a)/2+48;
        if(a%2){rem=10;}else{rem=0;}
    }
    //printf("DZIEL %s\n", dec.c_str());
    return dec;
}

str strDecToStrBin(str dec){
    str bin="";
    int m=dec.size();
    int i=0;
    while(dec.size()){
        //printf("DEC %s\n", dec.c_str());
        char l=dec.at(dec.size()-1);
        if(l=='0' || l=='2' || l=='4' || l=='6' || l=='8'){ //parzysta
            bin='0'+bin;
        }else{
            bin='1'+bin;
        }
        dec=divideByTwo(dec);
        if(dec.at(0)=='0'){dec.erase(0,1);}
    }
    return bin;
}

void generateValue(str a){
    str binary=strDecToStrBin(a);
    binaryNumberToCode(binary);
}



int generateP_A(str a){
	if(isNumber(a)){
		generateValue(a);
	}else{
		int AvarIndex=variableManager.getItemIndex(a);
		if(AvarIndex==-1){ 
			// jezeli nie istnieje zmienna a
			AvarIndex=constantManager.getItemIndex(a);
			if(AvarIndex==-1){
				if(ERR)printf("*******ARITH OP NIE ZNALEZIONO");
				return -1;
			}else{
				generateValue(constantManager.getItemValue(AvarIndex));
				// generuj wartosc stalej
			}
		}else{ // jezeli istnieje zmienna
			char temp[50];
			sprintf(temp, "LOAD %d", AvarIndex);
			addCodeLine(temp);
		}
	}
	return 0;
}
 
 /** Funkcja ktora
	1. Sprawdza czy VALUE jest liczba- jesli tak- generuj w rejestrze a
	2. Sprawdza, czy VALUE jest stala- laduj stala
	3. Sprawdza, czy VALUE jest zmienna - laduj zmienna
	Po wykonaniu tej funkcji rejestr pomocniczy op_temp_a zawiera a, op_temp_b zawiera b
	*/
int generateP_AB(str a, str b){
	int ret=generateP_A(a);
	
	addCodeLine("STORE " + op_temp_a);
	
	if(ret!=0)return ret;
	
	ret=generateP_A(b);
	
	if(ret!=0)return ret;
	
	addCodeLine("STORE " + op_temp_b);
	return 0;
}
 
int declareConstant(str constName, str constValue){
	if(DEBUG)printf("\tDeklaracja stalej >%s< o wartosci >%s<\n", constName.c_str(), constValue.c_str());
	return constantManager.addConstant(constName, constValue);
}

int declareVariable(str varName){
	if(DEBUG)printf("\tDeklaracja zmiennej <%s>\n", varName.c_str());
	return variableManager.addVariable(varName);
}

int generateVariableAssign(str varName, str varVal){
	if(DEBUG)printf("\tPrzypisuje zmiennej <%s> wartosc <%s>\n", varName.c_str(), varVal.c_str());
	int varIndex=variableManager.getItemIndex(varName);
	if(varIndex==-1){
		int cVarIndex=constantManager.getItemIndex(varName);
		if(cVarIndex==-1){ // jezeli nie istnieje ani stala ani zmienna
			return 1;
		}else{ // jezeli istnieje stala
			return 2; 
		}
	}
	// istnieje indeks zmiennej
	if(DEBUG)printf("Indeks zmiennej to %d\n", varIndex);
	char temp[50];
	sprintf(temp, "STORE %d", varIndex);
	addCodeLine(temp);
	return 0;
}

/** generuje wartosc w rejestra a **/
/* void generateValue1(unsigned long long int a){
	if(DEBUG)printf("Generuje wartosc %llu\n", a);
	addCodeLine("ZERO");
	for(int i=0; i<a; i++){
		addCodeLine("INC");
	}
}*/


void jumper(){
	if(DEBUG)printf("TEST JUMPERA");
	int line=tempCode.size()-1;
	jumpStack.push_back(line);
	if(DEBUG)printf("Tymczasowy jumper w linii %d\n", line);
}

void labeler(){
	if(DEBUG)addCodeLine("---ETYKIETA---");
	int line=tempCode.size()-1;
	labelStack.push_back(line);
	if(DEBUG)printf("Tymczasowy label w linii %d\n", line);
}

/*
	Zaczynajac od ostatnio dodanych etykiet i jumperow dopasowujemy je
*/


int generateArithOp(str op, str a, str b){
	generateP_AB(a, b);
	
	if(op==S_PLUS){
		addCodeLine("ADD " +op_temp_a);
	}else if(op==S_MINUS){
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SUB " + op_temp_b);
	}else if(op==S_MULT){
		char temp[50];
		if(DEBUG)addCodeLine("-----------POCZATEK MNOZENIA");
		addCodeLine("ZERO");
		addCodeLine("STORE " + op_temp_c);
		
		int startLineNumber=tempCode.size(); 
		// ETYKIETA START
		
		addCodeLine("LOAD " + op_temp_b);
		
		sprintf(temp, "JZ %d", tempCode.size()+19);
		addCodeLine(temp);
		sprintf(temp, "JODD %d", tempCode.size()+8);
		addCodeLine(temp);
		
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_a);
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_b);
	
		sprintf(temp, "JUMP %d", startLineNumber);
		addCodeLine(temp);
		
		//KONIEC DLA PARZYSTEGO
		
		addCodeLine("LOAD " + op_temp_c);
		addCodeLine("ADD " + op_temp_a);
		addCodeLine("STORE " + op_temp_c);
		
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SHL");
		addCodeLine("STORE " + op_temp_a);
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("SHR");
		addCodeLine("STORE " + op_temp_b);
		sprintf(temp, "JUMP %d", startLineNumber);
		addCodeLine(temp);
		
		addCodeLine("LOAD " + op_temp_c);
		//addCodeLine("DEC");
		addCodeLine("STORE " + op_temp_a); // reg_a=a
		if(DEBUG)addCodeLine("-----------KONIEC MNOZENIA");
		
	}else if(op==S_DIV){
		generateDivision();
	}else if(op==S_MOD){
		generateDivision();
		addCodeLine("LOAD " + op_temp_b);
		addCodeLine("STORE " + op_temp_a);
	}
	else{
		if(ERR)printf("Nieznany operator %s\n", op.c_str());
	}
	return 0;
}

int generateBoolOp(str op, str a, str b){
	generateP_AB(a, b);
	
	//w tym miejscu w REJ_A jest 'b', pierwszy w p_666
	
	if(op==S_GET){
		addCodeLine("SUB " + op_temp_a); // REJ_A=b-a
		addCodeLine("JG"); // if b-a > 0 <=> b > a -> skacz,
	}else if(op==S_EQ){
		addCodeLine("STORE " + op_temp_b);
		addCodeLine("SUB " + op_temp_a); // REJ_A=b-a
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SUB " + op_temp_b); // REJ_A=a-b
		addCodeLine("ADD " + op_temp_c); // w Z by sie to skrocilo; dla N wynik > 0 swiadczy o niezerowosci ktoregos ze skladnikow
		addCodeLine("JG"); //skaczemy gry (b-a) + (a-b) > 0, wtedy sa nierowne
	}else if(op==S_NEQ){
		addCodeLine("STORE " + op_temp_b);
		addCodeLine("SUB " + op_temp_a); // REJ_A=b-a
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("LOAD " + op_temp_a);
		addCodeLine("SUB " + op_temp_b); // REJ_A=a-b
		addCodeLine("ADD " + op_temp_c); // w Z by sie to skrocilo; dla N wynik > 0 swiadczy o niezerowosci ktoregos ze 
		addCodeLine("JZ");
	}else if(op==S_GT){
		addCodeLine("STORE " + op_temp_b);
		addCodeLine("SUB " + op_temp_a); // REJ_A = b-a
		addCodeLine("STORE " + op_temp_c);
		addCodeLine("LOAD " + op_temp_a); 
		addCodeLine("SUB " + op_temp_b); // REJ_A = a-b
		addCodeLine("SUB " + op_temp_c);
		addCodeLine("JZ");
	}else{
		return -1;
		if(ERR)printf("Nieznany operator %s", op.c_str());
	}
	jumper();
	return 0;
}

void fixIfLabels(){
	if(DEBUG)printf("Fixin'\n");
	
	for(int i=0; i<2; i++){
		int jumperLine=jumpStack.back();
		if(DEBUG)printf("Skok z %d -> ", jumperLine);
		str jumper=tempCode.at(jumperLine);
		
		if(labelStack.size()==0){
			if(DEBUG)printf("NO LABELS LEFT\n");
			return;
		}
		jumpStack.pop_back();
		
		int labelLine=labelStack.back();
		
		labelStack.pop_back();
		
		char op[10];
		char direction[50];
		char lab[50];
		sscanf(jumper.c_str(), "%s", op);
		if(DEBUG)printf(" ---> %d\n", labelLine+1);
		sprintf(lab, "%s %d", op, labelLine+1);
		str t=lab;
		tempCode.at(jumperLine)=t;	
	}
}

void generateIf(){
	int pl=tempCode.size();
	if(DEBUG)printf("Generuje if w linii %d\n", pl);
}

void generateThen(){
	int pl=tempCode.size();
	if(DEBUG)printf("Generuje then w linii %d\n", pl);
	addCodeLine("JUMP");
	jumper();
	labeler();
}
void generateElse(){
	int pl=tempCode.size();
	if(DEBUG)printf("Generuje else w linii %d\n", pl);
	labeler();
	
	//char temp[30];
	//sprintf(temp, "IKSDE %d", labelStack.size());
	//addCodeLine(temp);
	fixIfLabels();
}

void fixWhileLabels(){
	int label1=labelStack.back();
	labelStack.pop_back();
	int label2=labelStack.back();
	labelStack.pop_back();
	labelStack.push_back(label1);
	labelStack.push_back(label2);
	
	if(DEBUG)printf("Zamieniam etykiety %d i %d\n", label1, label2);
	
	for(int i=0; i<2; i++){
		int jumperLine=jumpStack.back();
		if(DEBUG)printf("Skok z %d -> ", jumperLine);
		str jumper=tempCode.at(jumperLine);
		
		if(labelStack.size()==0){
			if(DEBUG)printf("NO LABELS LEFT\n");
			return;
		}
		jumpStack.pop_back();
		
		int labelLine=labelStack.back();
		labelStack.pop_back();
		
		char op[10];
		char direction[50];
		char lab[50];
		sscanf(jumper.c_str(), "%s", op);
		if(DEBUG)printf(" ---> %d\n", labelLine+1);
		sprintf(lab, "%s %d", op, labelLine+1);
		str t=lab;
		tempCode.at(jumperLine)=t;	
	}
}

void generateWhile(){
	int pl=tempCode.size();
	if(DEBUG)printf("Generuej while w linii %d\n", pl);	
	labeler();
}

void generateDo(){
	int pl=tempCode.size();
	if(DEBUG)printf("Generuje else w linii %d\n", pl);
	addCodeLine("JUMP");
	jumper();
	labeler();
	fixWhileLabels();
}

int generateWrite(str a){
	if(DEBUG)printf("Generujemy write\n");
	int ret=generateP_A(a);
	if(ret!=0)return ret;
	addCodeLine("STORE " + op_temp_a);
	addCodeLine("PRINT " + op_temp_a);
	return 0;
}

int generateRead(str a){
	if(DEBUG)printf("Generujemy read\n");
	if(isNumber(a)){
		return -2;
	}else{
		int AvarIndex=variableManager.getItemIndex(a);
		if(AvarIndex==-1){ 
			// jezeli nie istnieje zmienna a
			AvarIndex=constantManager.getItemIndex(a);
			if(AvarIndex==-1){
				if(ERR)printf("*******WRITERR");
				return -1;
			}else{
				return -3;
			}
		}else{ // jezeli istnieje zmienna
			char temp[50];
			sprintf(temp, "SCAN %d", AvarIndex);
			addCodeLine(temp);
		}
	}
	return 0;
}


void endOfProgram(){
	if(DEBUG)printf("Koniec programu, zwracam HALT\nKOD:\n");
	addCodeLine("HALT");
	for(int i=0; i<tempCode.size(); i++){
		if(DEBUG || LINECODE)printf("%d. ", i);
		printf("%s\n", tempCode.at(i).c_str());
	}
}

#endif