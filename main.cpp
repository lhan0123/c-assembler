#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>

using namespace std;

#define id 0 
#define delimiter 1 
#define reservedWord 2 
#define integerTable 3 
#define realNumTable 4

struct outTable{
	int tableNum = -1 ; //token對應table編號 
	int tableNo = -1 ;	//token對應 table的位置 
};

struct AllCmdToken{
	string cmd = "" ;
	vector<string> cmdToken ;
};


struct hashTable{
	int hashValue = -1 ; //hash value 
    string token = "" ; //token名稱 
};


struct TABLE6{
	string cmd = "" ; 
	outTable opr  ;
	outTable op1  ;
	outTable op2  ;
	outTable result  ;
};

struct TABLE5{
	int hashValue = -1 ; //hash value 
	string token = "" ;
	int subRoutine = -1 ;
	int type = -1 ;
	int pointer = -1 ;
};

 
class Compiler{
public:
	string inputFileName = "";  	
	string outFileName = "";
	ifstream inFile; // 讀檔 
    ofstream outFile; // 寫檔
    
    vector<string> table1 ; //delimiter table 
    vector<string> table2 ; //reserved word table
    hashTable table3[100] ;
  	hashTable table4[100] ;
    TABLE5 table5[100] ;
    vector<TABLE6> table6 ;
    vector<string> table7 ;
    vector<string> table0 ;
    
    vector<AllCmdToken> cmdTokenList ; //把所有指令切好的token放入這裡
	
	int line = 1 ; 
	int subroutine = -1 ;
	
    //讀檔 
    void readFile(){			
		do {
			cout << "請輸入檔名：";
			cin >> inputFileName ;
			inFile.open( inputFileName.c_str()); // 開檔 
			if ( inFile.fail() ) { 
      			cout << "NO FILE " << inputFileName << " PLEASE ENTER AGAIN!!" << endl; // 不能開 
    		} // if ( 找不到檔案 )
		}while(inFile.fail()) ;
		
		outFileName = inputFileName+"'s  hw3-output.txt" ;
		cout << inputFileName << " open success!!!" << endl;
		inFile.close() ;
	}//readFile()
	
	//產生delimiter table 
	void createTable1(){
	    table1.push_back(";") ;
	    table1.push_back("(") ;
	    table1.push_back(")") ;
	    table1.push_back("=") ;
	    table1.push_back("+") ;
	    table1.push_back("-") ; 
	    table1.push_back("*") ;
	    table1.push_back("/") ;
	    table1.push_back("↑") ;
	    table1.push_back(",") ;	     
	    table1.push_back(":") ;	    
	}//createTable1()
	
	//產生reserved word table 
	void createTable2(){
		table2.push_back("AND") ;
		table2.push_back("BOOLEAN") ;
		table2.push_back("CALL") ;
		table2.push_back("DIMENSION") ;
		table2.push_back("ELSE") ;
		table2.push_back("ENP") ;
		table2.push_back("ENS") ;
		table2.push_back("EQ") ;
		table2.push_back("GE") ;
		table2.push_back("GT") ;
		table2.push_back("GTO") ;
		table2.push_back("IF") ;
		table2.push_back("INPUT") ;
		table2.push_back("INTEGER") ;
		table2.push_back("LABEL") ;
		table2.push_back("LE") ;
		table2.push_back("LT") ;
		table2.push_back("NE") ;
		table2.push_back("OR") ;
		table2.push_back("OUTPUT") ;
		table2.push_back("PROGRAM") ;
		table2.push_back("REAL") ;
		table2.push_back("SUBROUTINE") ;
		table2.push_back("THEN") ;
		table2.push_back("VARIABLE") ;
	}//createTable2()
	
	long long countAscii(string token){
		long long num = 0 ;
	
		for ( int i = 0 ; i < token.size() ; i++){
			num = num + int(token[i]) ;			  
		}//for
		
		return num ;		
	}//countAscii()
	
	int hashValue(long long num){
		int hashValue = 0 ;			
		hashValue = num % 100 ;	
		return hashValue ;		
	}//hashValue()
	
	void Hash_table(int hashNum, int subroutine, string token, int &index){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(table5[hashNum].hashValue == -1){
			table5[hashNum].hashValue = hashNum ;
			table5[hashNum].token = copyString(token) ;
			table5[hashNum].subRoutine = subroutine ;
			index = hashNum ;
			
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(table5[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			table5[curIndex].hashValue = hashNum ;
			table5[curIndex].token = copyString(token) ;
			table5[curIndex].subRoutine = subroutine ;
			index = curIndex ;
		}//else
	}//createLinearHash()
	
	void Hash_table3(int hashNum, string token, int &index){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(table3[hashNum].hashValue == -1){
			table3[hashNum].hashValue = hashNum ;
			table3[hashNum].token = copyString(token) ;
			index = hashNum ;
			
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(table3[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			table3[curIndex].hashValue = hashNum ;
			table3[curIndex].token = copyString(token) ;
			index = curIndex ;
		}//else
	}//createLinearHash()
	
	void Hash_table4(int hashNum, string token, int &index){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(table4[hashNum].hashValue == -1){
			table4[hashNum].hashValue = hashNum ;
			table4[hashNum].token = copyString(token) ;
			index = hashNum ;
			
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(table4[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			table4[curIndex].hashValue = hashNum ;
			table4[curIndex].token = copyString(token) ;
			index = curIndex ;
		}//else
	}//createLinearHash()
	
	//判斷是否為delimiter 
	bool isDelimiter(char ch){
		string str = "" ;
		str = str + ch ;
		for ( int i = 0 ; i < table1.size() ; i++){		
			if(str.compare(table1[i]) == 0) {
				return true ;
			}//if				
		}//for		 
		return false ;
	}//isDelimiter()
	
	
	int dataType(string str){
		if(str.compare("ARRAY") == 0) return 1 ;
		else if(str.compare("BOOLEAN") == 0) return 2 ;
		else if(str.compare("CHARACTER") == 0) return 3 ;
		else if(str.compare("INTEGER") == 0) return 4 ;
		else if(str.compare("LABEL") == 0) return 5 ;
		else if(str.compare("REAL") == 0) return 6 ;
        return -1 ;
	}//dataType()
	
    bool isInteger(string token){
		for ( int i = 0 ; i < token.size() ; i++){
			if( token[i] < '0' || token[i] >'9')
			   return false ;
		}//for		
		return true ;
	}//isInteger()
	
	bool isFloat(string token){
		int dot_count = 0 ;
		for ( int i = 0 ; i < token.size() ; i++){
			if (token[i] == '.')
				dot_count ++ ;
		}//for
		
		
		if (dot_count == 1){
			if (token[0] != '.'){
				for (int i = 0 ; i < token.size() ; i++ ){
					if (token[i] != '.'){
						if (token[i] < '0' || token[i] >'9')
							return false ;
					}//if
				}//for
				
				return true ;
			}//if
			
			else
				return false ;
		}//if
		
		else
			return false ;
		
	}//isFloat
	
	//判斷是否為delimiter 
	bool isReserverWord(string str){
		for ( int i = 0 ; i < table2.size() ; i++){		
			if(str.compare(table2[i]) == 0) {
				return true ;
			}//if				
		}//for		 
		return false ;
	}//isDelimiter()
	//複製字串 
	string copyString(string token){
		string str = "" ;
		for ( int i = 0 ; i < token.size() ; i++){
		   	str = str + token[i] ;
		}//for
		return str ;
	}//copyString()
	
	string intToString(int num){
		stringstream ss;
        ss << num;
        return ss.str() ;
	}//intToString(int num)
	

	
	//切token 
	void LexicalAnalysis(){
		string cmdLine = "" ;
		char ch = '\0' ;				
		string token = "" ;
		AllCmdToken tokenList ;
		bool has_error = false ;
		
		inFile.open(inputFileName.c_str()) ; //開檔 
		
		inFile.get(ch) ;
		while(!inFile.eof()){
			while ( ch != '\n'){			
				cmdLine = cmdLine + ch ;
				inFile.get(ch) ;
				if(inFile.eof()) break ;
			}//while
			
			cout << endl ;					
			for ( int i = 0 ; i < cmdLine.size() ; i++){
				
				if ((int)cmdLine[i] == -95){ //讀到"↑" (字串) (2 bytes) 
					if(!token.empty()){	
					
					    tokenList.cmdToken.push_back(token)	;				
						token = "" ;
					}//if
					
					token =  token +cmdLine[i] ;
					i++ ;
					token =  token +cmdLine[i] ;
					tokenList.cmdToken.push_back(token)	;			
					token = "" ;
				}//if
				
				else if (cmdLine[i] == ' ' || cmdLine[i] == '\t' ){ //判斷是否為空格字元 
					if(!token.empty()){	
					    tokenList.cmdToken.push_back(token)	;								
						token = "" ;
					}//if
				}//if
				
				else if(isDelimiter(cmdLine[i])){ //判斷是否為delimiter  
								  
					if(!token.empty()){
					    tokenList.cmdToken.push_back(token)	;						
						token = "" ;
					}//if
					
					token = token + cmdLine[i] ;
					tokenList.cmdToken.push_back(token)	;
					token = "" ;					
				}//if
				
				else{
					token = token + cmdLine[i] ;
				}//else	
					
			}//for
			
			if(!token.empty()){
			    tokenList.cmdToken.push_back(token)	;			
				token = "" ;
			}//if
			
			
			tokenList.cmd = copyString(cmdLine) ;
			cmdTokenList.push_back(tokenList) ;						
			cmdLine = "" ;
			tokenList.cmdToken.clear() ;
			inFile.get(ch) ;
			
		}//while
		
		inFile.close() ;	
		
		for ( int i = 0 ; i < cmdTokenList.size() ; i++){
			if (cmdTokenList[i].cmd.empty()) cout << "empty" << endl ;
			else cout << cmdTokenList[i].cmd << endl ;
			for ( int j = 0 ; j < cmdTokenList[i].cmdToken.size() ; j++){
				cout << "token: " << cmdTokenList[i].cmdToken[j] << endl ;
			}//for
			
			cout << endl ;
		}//for
	}//LexicalAnalysis()
	
	
	bool isIdentifier(string str){
		
		if(isReserverWord(str)) return false ;
		if((str.at(0) >= 'A' && str.at(0) <= 'Z') || (str.at(0) >= 'a' && str.at(0) <= 'z')){
			if(str.size() > 1){
				for ( int i = 1 ;  i < str.size() ; i++){
					if(!(str.at(0) >= 'A' && str.at(0) <= 'Z') && !(str.at(0) >= 'a' && str.at(0) <= 'z') && !(str.at(0) >= '0' && str.at(0) <= '9') ){
						return false ;
					}
				}//for
				return true ;
			}//if
			return true ;
		}//if
		
		return false ;
	}//check_identifier()
	
	bool is_program_heading(AllCmdToken cmd){
		outFile.open(outFileName.c_str(), ios::out) ; //開檔 
		outFile.close() ;
		if(cmd.cmdToken.size() == 3 ) {
			if(isIdentifier(cmd.cmdToken[1]) && cmd.cmdToken[2].compare(";") == 0)
			   return true ;
		}//if
		
		return false ;
	}//check_program_heading()
	
	int is_type(string str){
		if (str.compare("INTEGER") == 0) return 4 ;
		else if (str.compare("REAL") == 0) return 6 ;
		else if (str.compare("BOOLEAN") == 0) return 2 ;
		return -1 ;
	}//is_type()
	
	bool isVariable(AllCmdToken tmp){
		int type = -1 ;
		
		if(tmp.cmdToken.size() <= 3) return false ;
		
		type = is_type(tmp.cmdToken[1]) ; 
		//判斷是否為type 
		if(type != -1){
			//判斷是否為:
			if(tmp.cmdToken[2].compare(":") == 0){	
			    //:後有東西	
			    if (tmp.cmdToken.size() > 3){
			    	for ( int i = 3 ; i < tmp.cmdToken.size() ; i++){
						//判斷是否為id 
						if (i % 2 != 0){
							if(!isIdentifier(tmp.cmdToken[i])){
								return false ;
							}//if
							
							if( i == tmp.cmdToken.size() -1) return false ;							   
						}//if
						
						//判斷最後一個是否為; 
						else if(i == tmp.cmdToken.size() -1){
							if(tmp.cmdToken[i].compare(";") != 0)
							   return false ;
						}//if
						
						//判斷使否為, 
						else {
							if(tmp.cmdToken[i].compare(",") != 0)
							   return false ;
						}//else
					}//for
				}//if				
				else{
					return false ;
				}//else
				
			}//if
			else{
				return false ;
			}//else
		}//if
		
		else{
			return false ;
		}//else	
		
		return true ;	
	}//isVariable()
	
	//VARIABLE轉成中間碼 
	void translate_variable_intermediate(AllCmdToken tmp){
		vector<string> idList ;
		vector<int> index_list ;
		vector<TABLE6> tmp_table6 ;
		
		int hash_index = -1 ;
		int type = dataType(tmp.cmdToken[1]) ;
		TABLE6 temp6 ;
		bool error = false ;
		
		//取出id 
		for ( int i = 3 ; i < tmp.cmdToken.size() ; i++ ){
			if (isIdentifier(tmp.cmdToken[i]))
			{   cout << "id: " << tmp.cmdToken[i] << endl ;
				idList.push_back(tmp.cmdToken[i]) ;
			}//if			   
		}//for
		
		outFile.open(outFileName.c_str(), ios::out | ios::app ) ; //開檔 
		
		//轉成中間碼  
		for ( int i = 0 ; i < idList.size() ; i++){
			if(return_table_index(idList[i], subroutine, type) == -1){
				
				Hash_table(hashValue(countAscii(idList[i])), subroutine, idList[i], hash_index) ; // 把PROGRAM的IDENTIFIER HASH 至TABLE 
				table5[hash_index].type = type ;
				index_list.push_back(hash_index) ;
				
				temp6.opr.tableNum = 5 ;
				temp6.opr.tableNo = hash_index ;
				tmp_table6.push_back(temp6) ; //將中間碼放入table6中
			}//if
			
			else{
				error = true ;
				break ;
			}//else
										
	    }//for	
	    
	    if(!error){
	    	for ( int i = 0 ; i < tmp_table6.size() ; i++){
	    		table6.push_back(tmp_table6[i]) ; //將中間碼放入table6中
	    		outFile << table6.size() << ".\t((" << tmp_table6[i].opr.tableNum << ", " << tmp_table6[i].opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
				cout << "((" << tmp_table6[i].opr.tableNum << "," << tmp_table6[i].opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
			}//for
		}//if
		
		else{
			cout << "varabie already exist!!" << endl ;
			outFile << "line" << line << "\t"<< tmp.cmd << "\t" << "varabie already exist!!" << endl ;
			for ( int i = 0 ; i < index_list.size() ; i++){
				table5[index_list[i]].token = "" ;
				table5[index_list[i]].hashValue = -1 ;
				table5[index_list[i]].subRoutine = -1 ;
				table5[index_list[i]].pointer = -1 ;
				table5[index_list[i]].type = -1 ;
			}//for	
		}//else
		
		outFile.close() ; 		
	}//translate_intermediate()
	
	
	//判斷label語法是否正確 
	bool isLabel(AllCmdToken tmp){
		if(tmp.cmdToken.size() <= 1) return false ;
		
		for ( int i = 1 ; i < tmp.cmdToken.size() ; i++){
			if ( i % 2 == 1){
				//判斷是否為identifier 
				if(!isIdentifier(tmp.cmdToken[i])){
						return false ;
				}//if
				if( i == tmp.cmdToken.size() -1) return false ;	
			}//if
			
			//判斷最後一個是否為; 
			else if(i == tmp.cmdToken.size() -1){
				if(tmp.cmdToken[i].compare(";") != 0)
				   return false ;
			}//if
			
			//判斷是否為,
			else{
				if(tmp.cmdToken[i].compare(",") != 0)
					   return false ;
			}//else			
		}//for
		return true ;
	}//isLabel()
	
    
	
	//LABEL轉成中間碼 
	void translate_label_intermediate(AllCmdToken tmp){
		vector<string> idList ;
		int hash_index = -1 ;
		TABLE6 temp6 ;
		
		//取出id 
		for ( int i = 1 ; i < tmp.cmdToken.size() ; i++ ){
			if (isIdentifier(tmp.cmdToken[i]))
			{   cout << "id: " << tmp.cmdToken[i] << endl ;
				idList.push_back(tmp.cmdToken[i]) ;
			}//if			   
		}//for
		
		outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
		//轉成中間碼 
		for ( int i = 0 ; i < idList.size() ; i++){
			Hash_table(hashValue(countAscii(idList[i])), subroutine, idList[i], hash_index) ; // 把PROGRAM的IDENTIFIER HASH 至TABLE 
			table5[hash_index].type = 5 ;			
			temp6.opr.tableNum = 5 ;
			temp6.opr.tableNo = hash_index ;
			table6.push_back(temp6) ; //將中間碼放入table6中
			
			//輸出至檔案 
			outFile <<  table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
			cout << "((" << temp6.opr.tableNum << "," << temp6.opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
	    }//for	
		
		outFile.close() ; 		
	}//translate_label_intermediate()
	
	
	bool isSubscripted(vector<string> list, vector<string> &num_total){
		num_total.clear() ;        
		if(list.size() >= 1){
			//第一個token是否是id 
			if (isIdentifier(list[0])){
				if(list.size() >= 2){
					//第二個token是否是(
					if (list[1].compare("(") == 0){
						if(list.size() >= 3){
							for ( int i = 2 ; i < list.size() ; i++){
								if ( i % 2 == 0 ){
									if ( i == list.size()-1 ) return false ;
									if (!isInteger(list[i])) return false ;
									else num_total.push_back(list[i]) ;
								}//if
								
								else if(i == list.size()-1){
									if (list[i].compare(")") != 0) return false ;
								}//if
								
								else{
									if (list[i].compare(",") != 0) return false ;
								}//else
							}//for
							
							return true ;
						}//if						
					}//if				
				}//if			
			}//if	
		}//if
	
		return false ;		
	}//isSubscripted()
	
	
	bool isDimension(AllCmdToken tmp, vector<int> &infromation_index){
		int type = -1 ;
		vector<string> array ;
		vector<string> num_total ;
		vector<string> information_table ;
		int index = 0 ;
		
		if(tmp.cmdToken.size() <= 3) return false ;
		type = is_type(tmp.cmdToken[1]) ; 
								
		//判斷是否為type 
		if(type != -1){
			//判斷是否為:
			if(tmp.cmdToken[2].compare(":") == 0){
				//: 後還有東西 
				if (tmp.cmdToken.size() > 3){
					for ( int i = 3 ; i < tmp.cmdToken.size() ; i++){
						while( i < tmp.cmdToken.size() && tmp.cmdToken[i].compare(")") != 0){
							array.push_back(tmp.cmdToken[i]) ;
							i++ ;
						}//while
						
						if( i < tmp.cmdToken.size())
							array.push_back(tmp.cmdToken[i]) ;
						
						
						if(!isSubscripted(array, num_total)){
							return false ;
						}//if
						
						else{
							infromation_index.push_back(index) ;
							information_table.push_back(intToString(type)) ;
							index++ ;
							information_table.push_back(intToString(num_total.size())) ;
							index++ ;
							for ( int i = 0 ; i < num_total.size() ; i++ ){
								information_table.push_back(num_total[i]) ;
								index++ ;
							}//for									
						}//else
						
						if(i != tmp.cmdToken.size()-1){
							i++ ;
							if( i == tmp.cmdToken.size()-1){
								if(tmp.cmdToken[i].compare(";") != 0)
					   				return false ;
							}//if
							
							else{
								if(tmp.cmdToken[i].compare(",") != 0)
						   			return false ;
							}//else
						}//if
				         
				        else
				           return false ;
												
						array.clear() ;
					}//for
					
					for ( int i = 0 ; i < information_table.size() ; i++ ){
						table7.push_back(information_table[i]) ;
					}//for
					
					return true ;
				}//if				
			}//if			
		}//if
	
		return false ;				
	}//isDimension()
	
	
	void translate_dimension_intermediate(AllCmdToken tmp, vector<int> infromation_index){
		vector<string> idList ;
		int hash_index = -1 ;
		TABLE6 temp6 ;
		int n = 0 ;
		
		//取出id 
		for ( int i = 3 ; i < tmp.cmdToken.size() ; i++ ){
			if (isIdentifier(tmp.cmdToken[i]))
			{   cout << "id: " << tmp.cmdToken[i] << endl ;
				idList.push_back(tmp.cmdToken[i]) ;
			}//if			   
		}//for
		
		outFile.open(outFileName.c_str(), ios::out | ios::app ) ; //開檔 
		//轉成中間碼 
		for ( int i = 0 ; i < idList.size() ; i++){
			Hash_table(hashValue(countAscii(idList[i])), subroutine, idList[i], hash_index) ; // 把PROGRAM的IDENTIFIER HASH 至TABLE 
			table5[hash_index].type = 1 ;
			table5[hash_index].pointer = infromation_index[n] ;
			n++ ;
			temp6.opr.tableNum = 5 ;
			temp6.opr.tableNo = hash_index ;
			table6.push_back(temp6) ; //將中間碼放入table6中
			
			outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
			cout << "((" << temp6.opr.tableNum << "," << temp6.opr.tableNo << "),\t,\t,\t)\t" << idList[i] << endl ; 
	    }//for			
	    
	    cout << "now size: " << table6.size() << endl ;
		outFile.close() ; 				
	}//translate_dimension_intermediate()
	
	bool is_adding_operator(string str){
		if(str.compare("+") != 0 && str.compare("-") != 0 && str.compare("OR") != 0) return false ;
		return true ;
	}//is_adding_operator()
	
	bool has_adding_operator(vector<string> list){
		for ( int i = 0 ; i < list.size() ; i++){
			if(is_adding_operator(list[i])) return true ;
		}//for
		
		return false ;
	}//has_adding_operator()
	
	bool is_multiplying_operator(string str){
		if(str.compare("*") != 0 && str.compare("/") != 0 && str.compare("AND") != 0 && str.compare("↑") != 0) 
			return false ;
		return true ;
	}//is_multiplying_operator()
	
	bool has_multiplying_operator(vector<string> list){
		for ( int i = 0 ; i < list.size() ; i++){
			if(is_multiplying_operator(list[i])) 
				return true ;
		}//for
		
		return false ;
	}//has_multiplying_operator()
	
	bool is_relational_operator(string str){
		if(str.compare("EQ") != 0 && str.compare("NE") != 0 && str.compare("GT") != 0 && str.compare("GE") != 0
		        && str.compare("LT") != 0 && str.compare("LE") != 0) return false ;
		return true ;
	}//is_relational_operator()
	
	bool has_relational_operator(vector<string> list){
		for ( int i = 0 ; i < list.size() ; i++){
			if(is_relational_operator(list[i])) return true ;
		}//for
		
		return false ;
	}//has_relational_operator()
	
	bool is_sign(string str){
		if(str.compare("+") != 0 && str.compare("-") != 0) return false ;
		else return true ;
	}//is_sign(string str)
	
	bool find_equal(AllCmdToken tmp){
		for ( int i = 0 ; i < tmp.cmdToken.size() ; i++){
			if(tmp.cmdToken[i].compare("=") == 0) return true ;
		}//for
		
		return false ;
	}//find_equal()
	
	bool is_variable(vector<string> variable_list){
		if(variable_list.size() == 1){
			if (isIdentifier(variable_list[0])) return true ;
		}//if
		
		else{
			//第一個token為id 
			if(isIdentifier(variable_list[0])){
				if(variable_list.size() > 1){
					if(variable_list[1].compare("(") == 0){
						if(variable_list.size() > 2){
							for ( int i = 2 ; i < variable_list.size() ; i++){
								if (i % 2 == 0){
									if(!(isInteger(variable_list[i])) && !(isIdentifier(variable_list[i])))
										return false ;
									if( i == variable_list.size()-1) return false ;	
								}//if
								
								else if(i == variable_list.size() -1){
									if(variable_list[i].compare(")") != 0) return false ;
								}//if
								
								else{
									if(variable_list[i].compare(",") != 0) return false ;
								}//else
							}//for
							
							return true ;
						}//if
					}//if
				}//if
			}//if
		}//else
		
		return false ;
	}//isVariable()
	
	//;的位置是否正確 
	bool is_end_right(AllCmdToken tmp){
	    int i = 0 ;
	    while(tmp.cmdToken[i].compare(";") != 0){
	    	i++ ;
	    	if ( i == tmp.cmdToken.size() -1)  break ;
		}//while
		
		if ( i == tmp.cmdToken.size() -1){
			if(tmp.cmdToken[i].compare(";") == 0)
				return true ;
		}//if
		
	     return false ;
	}//is_end_right
	
	
	bool is_expression(vector<string> list){
		vector<string> simple_expression_list1 ;
		vector<string> simple_expression_list2 ;
		int index = 0 ;
		
		index = find_relation_opr_index(list) ;
		if ( index > 0 && index != list.size()-1){
			for ( int i = 0 ; i < index ; i ++){
				simple_expression_list1.push_back(list[i]) ;
			}//for
			
			index++ ;
			
			for ( int i = index ; i < list.size() ; i++){
				simple_expression_list2.push_back(list[i]) ;
			}//for	
			
			if (is_simple_expression(simple_expression_list1) && is_simple_expression(simple_expression_list2))
				return true ;	
		}//if
		
		else{			
			if (is_simple_expression(list)){
				return true ;
			}//if
		}//else
		
		return false ;
	}//is_expression()
	
	bool is_simple_expression(vector<string> list){
		vector<string> term_list ;
		vector<string> factor_list ;
		vector<string> simple_expression_list ;
		
		int index = 0 ;	
		
					
		if (is_term(list)){
			//if (list.size() == 1) cout << "list: " << list[0] << endl ;
			return true ;
		}//if
		
		else if(is_sign(list[0])){
			//sign後面還有東西 
			if(list.size() > 1){
				for ( int i = 1 ; i < list.size() ; i++)
					term_list.push_back(list[i]) ;
				if(is_term(term_list)) return true ;
			}//if
		}//if
		
		//判斷括號是否對秤 
		else if(is_br_right(list)){
			index = find_opr_index(list) ;
			//adding operator前後有東西 
			if (index > 0 && index != list.size() -1){
				//分simple expression 
				for ( int i = 0 ; i < index ; i++){
					simple_expression_list.push_back(list[i]) ;
				}//for
				
				index++ ;
				
				//分term
				for ( int i = index ; i < list.size() ; i++){
					term_list.push_back(list[i]) ;
				}//for
				
				if (is_simple_expression(simple_expression_list) && is_term(term_list))
					return true ;
			}//if
		}//if
				
		return false ;
	}//is_simple_expression()
	
	bool is_br_right(vector<string> list){
		int num = 0 ;
					
		for ( int i = 0 ; i < list.size() ; i++){		
			if (list[i].compare("(") == 0) num++ ;
			else if(list[i].compare(")") == 0){
				if(num == 0) {
					return false ;	
				}//if
				num-- ;
			}//else			
		}//for
	    
		if (num == 0) return true ;
		return false ;
	}//is_br_right()
	
	int find_opr_index(vector<string> list){
		int index = -1 ;
		int num_br = 0 ;
		
		for ( int i = 0 ; i < list.size() ; i++){
			if (list[i].compare("(") == 0) num_br++ ;
			else if(list[i].compare(")") == 0) num_br-- ;
			if (is_adding_operator(list[i])){
				if(num_br == 0)
					index = i ;
			}//if
		}//for
		
		return index ;
	}//find_opr_index()
	
	int find_mul_opr_index(vector<string> list){
		int index = -1 ;
		int num_br = 0 ;
		
		
		for ( int i = 0 ; i < list.size() ; i++){
			if (list[i].compare("(") == 0) 
				num_br++ ;
			else if(list[i].compare(")") == 0) 
				num_br-- ;
				
			else if (is_multiplying_operator(list[i])){
				if(num_br == 0)
					index = i ;
			}//if
		}//for
		
		return index ;
	}//find_opr_index()
	
	int find_relation_opr_index(vector<string> list){
		int index = -1 ;
		int num_br = 0 ;
				
		for ( int i = 0 ; i < list.size() ; i++){
			if (list[i].compare("(") == 0) 
				num_br++ ;
			else if(list[i].compare(")") == 0) 
				num_br-- ;
				
			else if (is_relational_operator(list[i])){
				if(num_br == 0)
					index = i ;
			}//if
		}//for		
		return index ;
	}//find_opr_index()
	
	int find_relation_opr_index_2(vector<string> list){
		int index = -1 ;
		int num_br = 0 ;
				
		for ( int i = 0 ; i < list.size() ; i++){
			if (list[i].compare("(") == 0) 
				num_br++ ;
			else if(list[i].compare(")") == 0) 
				num_br-- ;
				
			else if (is_relation(list[i])){
				if(num_br == 0)
					index = i ;
			}//if
		}//for		
		return index ;
	}//find_opr_index_2()
	
	
	bool is_term(vector<string> list){
		vector<string> term_list;
		vector<string> factor_list ;
		int index = 0 ;
		
				
		//判斷是否為factor 
		if (is_factor(list)){
			return true ;
		}//if
		
		
		//判斷括號是否對秤 
		else if(is_br_right(list)){
			index = find_mul_opr_index(list) ;
			
			//multiplying operator前後有東西 
			if (index > 0 && index != list.size() -1){
				
				//分term 
				for ( int i = 0 ; i < index ; i++){
					term_list.push_back(list[i]) ;
				}//for
				index++ ;
				
				//分factor
				for ( int i = index ; i < list.size() ; i++){
					factor_list.push_back(list[i]) ;
				}//for
				
				if (is_term(term_list) && is_factor(factor_list))
					return true ;
			}//if
		}//if
	
			
		return false ;
	}//is_term()
	
	bool is_factor(vector<string> list){
		vector<string> expression_list ;		
		int index = 1 ;
				
		
	    if (is_variable(list)){
	    	return true ;
		}//if
	    
	    else if (is_unsigned_constant(list)){
	    	return true ;
		}//if
		
	    else{	  
		    if (list[0].compare("(") == 0){
		    	if(is_br_right(list)){  
	                for ( int i = 1 ; i < list.size()-1 ; i++){
	                	expression_list.push_back(list[i]) ;
					}//for
						
					if(is_expression(expression_list)){
						return true ;
					}//if				   
				}//if
			}//if	    	   
  		
		}//else
	    return false ;
	}//is_factor()
	
	
	bool is_unsigned_constant(vector<string> list){
		if (is_unsigned_number(list)) return true ;
		if (list.size() == 1){
			if(isIdentifier(list[0])) return true ;
		}//if
		
		return false ;
	}//is_unsigned_constant()
	
	//判斷是否為整數或小數點 
	bool is_unsigned_number(vector<string> list){
		
		if (list.size() == 1){
			if (is_unsigned_real(list[0]) || isInteger(list[0]) )
			   return true ;
		}//if
		
		return false ;
	}//is_unsigned_number()
	
	//判斷是否為小數點 
	bool is_unsigned_real(string str){
		if (isFloat(str)) return true ;
		return false ;
	}//is_unsigned_real()
	
	bool is_assign_statement(AllCmdToken tmp){
		vector<string> variable_list ;
		vector<string> expression_list ;
		int index = 0 ; 
								
		//有找到'=' 
		if(find_equal(tmp)){
		
			while(tmp.cmdToken[index].compare("=") != 0){
				variable_list.push_back(tmp.cmdToken[index]) ;
				index++ ;
			}//while
			
			for ( int i = 0 ; i < variable_list.size() ; i++){
				cout << "var: " << variable_list[i] << endl ;
			}//for
			
			//判斷variable是否正確 
			if(is_variable(variable_list)) {	
			    //判斷;是否正確 
			    if(is_end_right(tmp)){
			    	
			    	index++ ;
			    	//截出expression 
			    	while(tmp.cmdToken[index].compare(";") != 0){
						expression_list.push_back(tmp.cmdToken[index]) ;
						index++ ;
					}//while
					
				    
					//判斷expression 
					if (expression_list.size() != 0){
						
						if ( is_expression(expression_list)) {
							return true ;
						}//if
					}//if						
				}//if							
			}//if
		}//if				
		return false ;
		
	}//is_assign_statement()
	
	
	bool isTable0(string str, int &tableNo){
		for ( int i = 0 ; i < table0.size() ; i++){
			if(str.compare(table0[i]) == 0){
				tableNo = i ;
				return true ;
			}//if
		}//for
		
		return false ;
	}//isTable0()
	
	bool isTable1(string str, int &tableNo){
		for ( int i = 0 ; i < table1.size() ; i++){
			if(str.compare(table1[i]) == 0){
				tableNo = i ;
				return true ;
			}//if
		}//for
		
		return false ;
		
	}//isTable1()
	
	bool isTable3(string str, int &tableNo){
		for ( int i = 0 ; i < 100 ; i++){
			if(str.compare(table3[i].token) == 0){
				tableNo = i ;
				return true ;
			}//if
		}//for
		
		return false ;		
	}//isTable3()
	
	bool isTable4(string str, int &tableNo){
		for ( int i = 0 ; i < 100 ; i++){
			if(str.compare(table4[i].token) == 0){
				tableNo = i ;
				return true ;
			}//if
		}//for
		
		return false ;		
	}//isTable4()
	
	bool isTable5(string str, int subroutine, int &tableNo){		
		for ( int i = 0 ; i < 100 ; i++){
			if(str.compare(table5[i].token) == 0 && table5[i].subRoutine == subroutine){
				tableNo = i ;
				return true ;
			}//if
		}//for
		
		return false ;			
	}//isTable3()
	
	int find_opr(vector<string> list, int &tableNo){
		for ( int i = 0 ; i < list.size() ; i++ ){
			if (isTable1(list[i], tableNo)){
				return i ;
				break ;
			}//if
		}//for
		
		return -1 ;
	}//find_opr()
	
	bool check_op(string str, int &tableNum, int &tableNo){
		int hash_index = -1 ;
		if(!isTable5(str, subroutine, tableNo)){ //判斷table5 
			if (!isTable0(str, tableNo)){ //判斷table0 
				if (!isInteger(str)){ // 判斷table3
					if (!isFloat(str)){ // 判斷table4
						return false ;
					}//if
					
					else {
						if (!isTable4(str,tableNo)){
							Hash_table4(hashValue(countAscii(str)), str, hash_index) ;
							tableNo = hash_index ;
						}//if
						
						tableNum = 4 ;
					}//else					
				}//if
				
				else{
					if (!isTable3(str,tableNo)){
						Hash_table3(hashValue(countAscii(str)), str, hash_index) ;
						tableNo = hash_index ;
					}//if
					
					tableNum = 3 ;
				}//else
			}//if
			
			else{
				tableNum = 0 ;
			}//else
		}//if
		
		else{
			tableNum = 5 ;
		}//else
	}//check_op()
	
	int count_array(AllCmdToken tmp){
		int count = 0 ;
		for ( int i = 0 ; i < tmp.cmdToken.size()-1 ; i++){
			if (isIdentifier(tmp.cmdToken[i]) && tmp.cmdToken[i+1].compare("(") == 0)
				count ++ ;
		}//for
		
		return count ;
	}//count_array()
	
   
	void translate_assign_statement(vector<string> list){		
	    int op1_tableNum = -1, op1_tableNo = -1 ;
	    int op2_tableNum = -1, op2_tableNo = -1 ;
	    int opr_tableNum = 4, opr_tableNo = -1 ;
	    int res_tableNum = -1, res_tableNo = -1 ;
	    
	    int opr = -1 ; //opr在list的位置
	    int op1 = -1 ; //op1在list的位置
	    int op2 = -1 ; //op2在list的位置
	    int res = -1 ; //res在list的位置
	    
	    TABLE6 temp ;
	    vector<TABLE6> temp_list ; 
	    string t_num = "T" ;
	    bool error = false ;
		
		while (list.size() != 3) {
			opr = find_opr(list, opr_tableNo) ;
			op2 = opr - 1 ;
			op1 = opr - 2 ;
			string token = "" ;
			
			
			cout << "opr: " << opr << endl ;
			cout << "op1: " << op1 << endl ;
			cout << "op2: " << op2 << endl ;
			
			if(check_op(list[op1], op1_tableNum, op1_tableNo) && check_op(list[op2], op2_tableNum, op2_tableNo)){
				
				temp.opr.tableNum = 1 ;
				temp.opr.tableNo = opr_tableNo+1 ;
				temp.op1.tableNum = op1_tableNum;
				temp.op1.tableNo = op1_tableNo ;
				temp.op2.tableNum = op2_tableNum;
				temp.op2.tableNo = op2_tableNo ;
				temp.result.tableNum = 0 ;
				temp.result.tableNo = table0.size() ;
				table0.push_back(t_num+ intToString(table0.size())) ;
				temp_list.push_back(temp) ;
				
				
				list.erase(list.begin() + op1) ;
				list.erase(list.begin() + op1) ;
				list.erase(list.begin() + op1) ;
				list.insert(list.begin() + op1, t_num+ intToString(table0.size()-1)) ;
			    
			    cout << "list: " ;
			    for ( int i = 0 ; i < list.size() ; i++)
			    	cout << list[i] << "," ;
			    cout << endl ;
			}//if
			
			else{
				error = true ;
				break ;
			}		
		} //while
		
		if ( !error){
			opr = find_opr(list, opr_tableNo) ;
			op1 = opr - 1 ;
			res = opr - 2 ;
						
			if(check_op(list[op1], op1_tableNum, op1_tableNo) && check_op(list[res], res_tableNum, res_tableNo)){
				
				temp.opr.tableNum = 1 ;
				temp.opr.tableNo = opr_tableNo+1 ;
				temp.op1.tableNum = op1_tableNum;
				temp.op1.tableNo = op1_tableNo ;
				temp.result.tableNum = res_tableNum ;
				temp.result.tableNo = res_tableNo ;
				temp_list.push_back(temp) ;
			}//if
			
			else{
				error = true ;
			}//else
			
		    if(!error){
		    	for ( int i = 0 ; i < temp_list.size()-1 ; i++){
		    		table6.push_back(temp_list[i]) ;  
		    		outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
		    		outFile << table6.size() << ".\t((" << temp_list[i].opr.tableNum << "," << temp_list[i].opr.tableNo << "),\t" ;
		    		outFile << "(" << temp_list[i].op1.tableNum << "," << temp_list[i].op1.tableNo << "),\t" ;
					outFile << "(" << temp_list[i].op2.tableNum << "," << temp_list[i].op2.tableNo << "),\t" ;
		    		outFile << "(" << temp_list[i].result.tableNum << "," << temp_list[i].result.tableNo << "))\t" ;
		    		outFile << print_token(temp_list[i].result.tableNum , temp_list[i].result.tableNo) ;
		    		outFile << "=" ;		    		
		    		outFile << print_token(temp_list[i].op1.tableNum , temp_list[i].op1.tableNo) ;	
					outFile << print_token(temp_list[i].opr.tableNum , temp_list[i].opr.tableNo) ; 
					outFile << print_token(temp_list[i].op2.tableNum , temp_list[i].op2.tableNo) ;	 
							
		    		outFile << endl ;
		    		outFile.close() ;
				}//for
				
				table6.push_back(temp_list[temp_list.size()-1]) ;  
				outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔
				outFile << table6.size() << ".\t((" << temp_list[temp_list.size()-1].opr.tableNum << "," << temp_list[temp_list.size()-1].opr.tableNo << "),\t" ;
	    		outFile << "(" << temp_list[temp_list.size()-1].op1.tableNum << "," << temp_list[temp_list.size()-1].op1.tableNo << "),\t" ;
				outFile << ",\t" ;
	    		outFile << "(" << temp_list[temp_list.size()-1].result.tableNum << "," << temp_list[temp_list.size()-1].result.tableNo << "))\t" ;
	    		outFile << print_token(temp_list[temp_list.size()-1].result.tableNum , temp_list[temp_list.size()-1].result.tableNo) ;
	    		outFile << print_token(temp_list[temp_list.size()-1].opr.tableNum , temp_list[temp_list.size()-1].opr.tableNo) ;
	    		outFile << print_token(temp_list[temp_list.size()-1].op1.tableNum , temp_list[temp_list.size()-1].op1.tableNo) ;	 				   		
	    		outFile << endl ;
	    		outFile.close() ;
			}//if
			
			else{
				cout << "undefined symbol !!! " << endl ;
				outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
				outFile << "line" << line << "\tundefined symbol" << endl ;
				outFile.close() ;
			}//else								
		}//if
		
		else{
			cout << "undefined symbol !!! " << endl  ;
			outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
			outFile << "line" << line << "\tundefined symbol" << endl ;
			outFile.close() ;
		}//else
				
	}//translate_assign_statement()
	
	void translate_array_statement(vector<string> list, string token_one){		
	    int op1_tableNum = -1, op1_tableNo = -1 ;
	    int op2_tableNum = -1, op2_tableNo = -1 ;
	    int opr_tableNum = 4, opr_tableNo = -1 ;
	    int res_tableNum = -1, res_tableNo = -1 ;
	    
	    int opr = -1 ; //opr在list的位置
	    int op1 = -1 ; //op1在list的位置
	    int op2 = -1 ; //op2在list的位置
	    int res = -1 ; //res在list的位置
	    
	    TABLE6 temp ;
	    vector<TABLE6> temp_list ; 
	    string t_num = "T" ;
	    bool error = false ;
		
		while (list.size() != 3) {
			opr = find_opr(list, opr_tableNo) ;
			op2 = opr - 1 ;
			op1 = opr - 2 ;
			string token = "" ;
			
			
			cout << "opr: " << opr << endl ;
			cout << "op1: " << op1 << endl ;
			cout << "op2: " << op2 << endl ;
			
			if(check_op(list[op1], op1_tableNum, op1_tableNo) && check_op(list[op2], op2_tableNum, op2_tableNo)){
				
				temp.opr.tableNum = 1 ;
				temp.opr.tableNo = opr_tableNo+1 ;
				temp.op1.tableNum = op1_tableNum;
				temp.op1.tableNo = op1_tableNo ;
				temp.op2.tableNum = op2_tableNum;
				temp.op2.tableNo = op2_tableNo ;
				temp.result.tableNum = 0 ;
				temp.result.tableNo = table0.size() ;
				table0.push_back(t_num+ intToString(table0.size())) ;
				temp_list.push_back(temp) ;
				
				
				list.erase(list.begin() + op1) ;
				list.erase(list.begin() + op1) ;
				list.erase(list.begin() + op1) ;
				list.insert(list.begin() + op1, t_num+ intToString(table0.size()-1)) ;
			    
			    cout << "list: " ;
			    for ( int i = 0 ; i < list.size() ; i++)
			    	cout << list[i] << "," ;
			    cout << endl ;
			}//if
			
			else{
				error = true ;
				break ;
			}		
		} //while
		
		if ( !error){
			opr = find_opr(list, opr_tableNo) ;
			op1 = opr - 1 ;
			res = opr - 2 ;
						
			if(check_op(list[op1], op1_tableNum, op1_tableNo) && check_op(list[res], res_tableNum, res_tableNo)){
				
				temp.opr.tableNum = 1 ;
				temp.opr.tableNo = opr_tableNo+1 ;
				temp.op1.tableNum = op1_tableNum;
				temp.op1.tableNo = op1_tableNo ;
				temp.result.tableNum = res_tableNum ;
				temp.result.tableNo = res_tableNo ;
				temp_list.push_back(temp) ;
			}//if
			
			else{
				error = true ;
			}//else
			
		    if(!error){
		    	for ( int i = 0 ; i < temp_list.size()-1 ; i++){
		    		table6.push_back(temp_list[i]) ;  
		    		outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
		    		outFile << table6.size() << ".\t((" << temp_list[i].opr.tableNum << "," << temp_list[i].opr.tableNo << "),\t" ;
		    		outFile << "(" << temp_list[i].op1.tableNum << "," << temp_list[i].op1.tableNo << "),\t" ;
					outFile << "(" << temp_list[i].op2.tableNum << "," << temp_list[i].op2.tableNo << "),\t" ;
		    		outFile << "(" << temp_list[i].result.tableNum << "," << temp_list[i].result.tableNo << "))\t" ;
		    		outFile << print_token(temp_list[i].result.tableNum , temp_list[i].result.tableNo) ;
		    		outFile << "=" ;		    		
		    		outFile << print_token(temp_list[i].op1.tableNum , temp_list[i].op1.tableNo) ;	
					outFile << print_token(temp_list[i].opr.tableNum , temp_list[i].opr.tableNo) ; 
					outFile << print_token(temp_list[i].op2.tableNum , temp_list[i].op2.tableNo) ;	 
							
		    		outFile << endl ;
		    		outFile.close() ;
				}//for
				
				table6.push_back(temp_list[temp_list.size()-1]) ;  
				outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔
				outFile << table6.size() << ".\t((" << temp_list[temp_list.size()-1].opr.tableNum << "," << temp_list[temp_list.size()-1].opr.tableNo << "),\t" ;
	    		outFile << "(" << temp_list[temp_list.size()-1].op1.tableNum << "," << temp_list[temp_list.size()-1].op1.tableNo << "),\t" ;
				outFile << "(5," << return_table_index(token_one, subroutine,1) << "),\t"  ;
	    		outFile << "(" << temp_list[temp_list.size()-1].result.tableNum << "," << temp_list[temp_list.size()-1].result.tableNo << "))\t" ;
	    		outFile << token_one << "(" << print_token(temp_list[temp_list.size()-1].result.tableNum , temp_list[temp_list.size()-1].result.tableNo) << ")";
	    		outFile << print_token(temp_list[temp_list.size()-1].opr.tableNum , temp_list[temp_list.size()-1].opr.tableNo) ;
	    		outFile << print_token(temp_list[temp_list.size()-1].op1.tableNum , temp_list[temp_list.size()-1].op1.tableNo) ;	 				   		
	    		outFile << endl ;
	    		outFile.close() ;
			}//if
			
			else{
				cout << "undefined symbol !!! " << endl ;
				outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
				outFile << "line" << line << "\tundefined symbol" << endl ;
				outFile.close() ;
			}//else								
		}//if
		
		else{
			cout << "undefined symbol !!! " << endl  ;
			outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
			outFile << "line" << line << "\tundefined symbol" << endl ;
			outFile.close() ;
		}//else
				
	}//translate_assign_statement()
	
	string print_token(int tableNum, int tableNo){
		if (tableNum == 0){
			return table0[tableNo] ;
		}//if
		else if (tableNum == 1){
			return table1[tableNo-1] ;
		}//if
		else if (tableNum == 3){
			return table3[tableNo].token ;
		}//if
		else if (tableNum == 4){
			return  table4[tableNo].token ;
		}//if
		else if (tableNum == 5){
			return table5[tableNo].token ;
		}//if
	}//print_token() 
	
	//判斷運算子的優先順序情況 
   int Priority( string op, string top_Item){
	if ( op.compare("+") == 0 || op.compare("-") == 0 ) {
		if (  top_Item.compare("+") == 0 ||  top_Item.compare("-") == 0 )
			return 1 ;   //情況1:讀到運算子為+、-，運算子堆疊的最上層資料為 +、-
		else if (  top_Item.compare("*") == 0 ||  top_Item.compare("/") == 0 || top_Item.compare("↑") == 0 )
		    return 2 ;   //情況2:讀到運算子為+、-，運算子堆疊的最上層資料為 *、/、 ↑
		else if (  top_Item.compare("(") == 0 || top_Item.compare("=") == 0 )
		    return 3 ;	 //情況3:讀到運算子為+、-，運算子堆疊的最上層資料為 '('	
	} // if
	
	else if ( op.compare("*") == 0 || op.compare("/") == 0 ) {
		if (  top_Item.compare("+") == 0 ||  top_Item.compare("-") == 0 || top_Item.compare("(") == 0 || top_Item.compare("=") == 0)
			return 4 ;  //情況4:讀到運算子為*、/，運算子堆疊的最上層資料為 +、-
		else if (  top_Item.compare("*") == 0 ||  top_Item.compare("/") == 0 )
		    return 5 ;  //情況5:讀到運算子為*、/，運算子堆疊的最上層資料為 *、/
		else if (  top_Item.compare("↑") == 0 )
		    return 6 ;			
	}//if	
	
	else if (op.compare("↑") == 0 ) {
		if (  top_Item.compare("+") == 0 ||  top_Item.compare("-") == 0 ||  top_Item.compare("*") == 0 ||  top_Item.compare("/") == 0 ||  top_Item.compare("(") == 0 || top_Item.compare("=") == 0)
			return 7 ;  //情況4:讀到運算子為*、/，運算子堆疊的最上層資料為 +、-
		else if (  top_Item.compare("↑") == 0 )
		    return 8 ;		
	}//if	
}//Priority()

bool return_topItem(string &topItem, vector<string> operand_list){
     if ( operand_list.size() != 0 ) {
     	topItem = operand_list[operand_list.size()-1] ;
     	return true ;    	
	 }//if
	 
	 return false ;
}//return_topItem()

   void InfixToPostfix(vector<string>& postfix_list, vector<string> list){
   	    vector<string> operand_list ;
		string topItem = "" ; //回傳堆疊operandList的最上層資料 
		int priorityCase = 0 ; //回傳運算子的優先順序情況
								
		for ( int i = 0 ; i < list.size() ; i++ ) {
				
			if ( isInteger(list[i]) || isFloat(list[i]) || isIdentifier(list[i])) { //讀到數字 
				postfix_list.push_back(list[i]) ;
			}//if
			
			else {
				
			    // 當op為+、-、*、/ 
				if ( list[i].compare("+") == 0 || list[i].compare("-") == 0 || list[i].compare("*") == 0 
				     || list[i].compare("/") == 0 || list[i].compare("↑") == 0 || list[i].compare("=") == 0) {
				     
					 
				    if(operand_list.size() == 0) //若存運算子和括號的堆疊無資料，直接把op存入堆疊的最上層 
				        operand_list.push_back(list[i]) ;
				        
				    else {
				    	
				    	return_topItem(topItem, operand_list) ;
				    	priorityCase = Priority( list[i], topItem) ; //判斷運算子的優先順序情況
				    	
				    	switch( priorityCase ) {
				    		case 1 : //情況1:讀到運算子為+、-，運算子堆疊的最上層資料為 +、-
		 				       	postfix_list.push_back(topItem) ; 
								operand_list.erase(operand_list.end()) ;
								operand_list.push_back( list[i] ) ;
								break ;
							
							case 2 : //情況2:讀到運算子為+、-，運算子堆疊的最上層資料為 *、/
							    while ( return_topItem(topItem, operand_list) && topItem.compare("(") != 0 ) {								    
								    postfix_list.push_back(topItem) ;
								    operand_list.erase(operand_list.end()) ;   	
								}//while
								
				    		    operand_list.push_back( list[i] ) ;
				    		    break ;
				    		    
				    		case 3 : //情況3:讀到運算子為+、-、*、/，運算子堆疊的最上層資料為 '('
				    			operand_list.push_back( list[i] ) ;
				    			break ;
				    			
				    		case 4 :  //情況4:讀到運算子為*、/，運算子堆疊的最上層資料為 +、-
				    			operand_list.push_back( list[i] ) ;
				    			break ;
				    		
				    		case 5 :  //情況5:讀到運算子為*、/，運算子堆疊的最上層資料為 *、/
				    		   postfix_list.push_back(topItem) ;
							   operand_list.erase(operand_list.end()) ;
							   operand_list.push_back( list[i] ) ;
							   break ;	
							   
							case 6 :  
				    		   while ( return_topItem(topItem, operand_list) && (topItem.compare("↑") == 0 || topItem.compare("*") == 0  || topItem.compare("/") == 0 )) {								    
								    postfix_list.push_back(topItem) ;
								    operand_list.erase(operand_list.end()) ;   	
								}//while
				    		    
				    		    operand_list.push_back( list[i] ) ;
				    		    break ;
							   
							case 7 :  
				    		   operand_list.push_back( list[i] ) ;
							   break ;	  
							   
							case 8 :  
				    		  	operand_list.push_back( list[i] ) ;
							   break ;	   						    						    			
						}//end switch			    	
					}//else
				     	
				}//if
				
				else if ( list[i].compare("(") == 0 ) //讀到'('，執行以下動作 
				    operand_list.push_back( list[i] ) ;
				    
			    else if ( list[i].compare(")") == 0 ) {  //讀到')'，執行以下動作 
			    	while ( return_topItem(topItem, operand_list) && topItem.compare("(") != 0 ) {												    
						postfix_list.push_back(topItem) ;
						operand_list.erase(operand_list.end()) ;  	
					}//while
			    	
			    	if(operand_list.size() != 0 && topItem.compare("(") == 0  )	
				    	operand_list.erase(operand_list.end()) ;			    	 
				}//if
			}//else	
			
			
	        topItem.clear() ; //字串清空
		}//for
		
		//若堆疊(operandStack)尚有資料
		while ( return_topItem(topItem, operand_list)) {
			postfix_list.push_back(topItem) ;  //把最上層資料新增至堆疊(postfix)的最上層
			operand_list.erase(operand_list.end()) ;
		}//while

   }//InfixToPostfix()
	
	
	bool is_relation(string str){
		if (is_relational_operator(str) || str.compare("OR") == 0  || str.compare("AND") == 0 ) return true ;
		return false ;
	}//is_relation()
	
	bool is_constant(vector<string> list){
		if (is_sign(list[0])){
			if (list.size() == 2){
				if (isInteger(list[1]) || isFloat(list[1]) || isIdentifier(list[1])) 
					return true ;
			}//if
			
		}//if
		
		else{
			if (list.size() == 1){
				if (isInteger(list[0]) || isFloat(list[0]) || isIdentifier(list[0]) ) 
					return true ;
			}//if
		}//else
		
		return false ;
	}//is_constant()
	
	bool is_condition_variable(vector<string> list){
		if (is_variable(list) || is_constant(list)) 
			return true ;
		return false ;
	}//is_condition_variable()
	
	bool is_condition(vector<string> list){
		int index = -1 ;
		vector<string> condition_variable_list1 ;
		vector<string> condition_variable_list2 ;
		
		index = find_relation_opr_index_2(list) ;
		
		//relation前後有東西 
		if (index > 0 && index != list.size() -1){
			for ( int i = 0 ; i < index ; i++){
				condition_variable_list1.push_back(list[i]) ;
			}//for
			
			index++ ;
			for ( int i = index ; i < list.size() ; i++){
				condition_variable_list2.push_back(list[i]) ;
			}//for
			
			/*
			for ( int i = 0 ; i < condition_variable_list1.size() ; i++)
				cout << "s1: " << condition_variable_list1[i] << endl ;
			for ( int i = 0 ; i < condition_variable_list2.size() ; i++)
				cout << "s2: " << condition_variable_list2[i] << endl ;*/
			
			if (is_condition_variable(condition_variable_list1) && is_condition_variable(condition_variable_list2)){
				//cout << "condition right!!!" << endl ;
				return true ;
			}//if			
		}//if
		
		return false ;
	}//is_condition()
	
	
	bool is_if_statement(AllCmdToken tmp){
		
		int if_count = 0, if_index = -1  ;
		int then_count = 0, then_index = -1  ;
		int else_count = 0, else_index = -1 ;
		
		vector<string> condition ;
		vector<string> statement1 ;
		vector<string> statement2 ;
		
		//判斷 ;是否正確 
		if (is_end_right(tmp)){
			for ( int i = 0 ; i < tmp.cmdToken.size() ; i++){
				if (tmp.cmdToken[i].compare("IF") == 0) {
					if_count++ ;
					if_index = i ;
				}//if
				
				else if (tmp.cmdToken[i].compare("THEN") == 0) {
					then_count++ ;
					then_index = i ;
				}//if
				
				else if (tmp.cmdToken[i].compare("ELSE") == 0) {
					else_count++ ;
					else_index = i ;
				}//if			
			}//for
		
		
			if (if_count==1 && then_count == 1 && else_count <=1 ){						
				
	            if (if_index == 0 && (then_index != 0 && then_index != tmp.cmdToken.size()-1 ) ){
	            	
	            	//分condition 
					for ( int i = 1 ; i < then_index ; i++){
						condition.push_back(tmp.cmdToken[i]) ;
					}//for
	            	
	            	//沒有else 
	            	if (else_count == 0){
	            		if ((then_index+1) != tmp.cmdToken.size()-1){
	            			for ( int i = then_index+1 ; i < tmp.cmdToken.size()-1 ; i++ ){
	            				statement1.push_back(tmp.cmdToken[i]) ;
							}//for
						}//if
						
						if (is_condition(condition) && is_statement(statement1)) 
							return true ;	            		
					}//if
					
					//有else 
	            	else if (else_count == 1 && (else_index != 0 && else_index != tmp.cmdToken.size()-1)) {
	            		for ( int i = then_index+1 ; i < else_index ; i++ ){
	            			statement1.push_back(tmp.cmdToken[i]) ;
						}//for
						
						if ((else_index+1) != tmp.cmdToken.size()-1){
	            			for ( int i = else_index+1 ; i < tmp.cmdToken.size()-1 ; i++ ){
	            				statement2.push_back(tmp.cmdToken[i]) ;
							}//for
						}//if
						
						/*
						for ( int i = 0 ; i < condition.size() ; i++)
							cout << "con: " << condition[i] << endl ;
						for ( int i = 0 ; i < statement1.size() ; i++)
							cout << "st1: " << statement1[i] << endl ;	
						for ( int i = 0 ; i < statement2.size() ; i++)
							cout << "st2: " << statement2[i] << endl ;
						*/			
						
						if (is_condition(condition) && is_statement(statement1) && is_statement(statement2)) 
							return true ;	            								
					}//if										
				}//if
			}//if
		}//if
				
		return false ;
	}//is_if_statement
	
	
	bool is_statement(vector<string> list){
	    AllCmdToken temp ;
		int dim = 0 ;
		vector<string> infix_list ;
		vector<string> postfix_list ;
		
		for ( int i = 0 ; i < list.size() ; i++){
			temp.cmdToken.push_back(list[i]) ;
		}//for
		 
		temp.cmdToken.push_back(";") ;
	    
	    //IF statement
		if (list[0].compare("IF") == 0) {
			if(is_if_statement(temp)) 
				return true ;
		}//if
		
		//call
		else if(list[0].compare("CALL") == 0) {
			if(is_call(temp))
				return true ;
		}//if
		
		//input
		else if(list[0].compare("INPUT") == 0 || list[0].compare("OUTPUT") == 0){
			if(is_input_output(temp))
				return true ;
		}//if
				
		//gto
		else if(list[0].compare("GTO") == 0) {	
		    if(is_GTO(temp)){
		    	return true ;
			}//if
				
		}//if
		
		//assignment
		else if(isIdentifier(list[0])){
			if(is_assign_statement(temp)){	
				if (count_array(temp) == 1){								
					if (return_table_index(temp.cmdToken[0], subroutine, 1) != -1){
						if (table5[return_table_index(temp.cmdToken[0], subroutine, 1)].type == 1){
							dim = table5[return_table_index(temp.cmdToken[0], subroutine, 1)].pointer + 1 ;
							if (dim == 1){
								for ( int j = 1 ; j < temp.cmdToken.size()-1 ; j++){	
								    if (temp.cmdToken[j].compare("(") != 0 && temp.cmdToken[j].compare(")") != 0)												    												
										infix_list.push_back(temp.cmdToken[j]) ;																																																	
								}//for	
								
								InfixToPostfix(postfix_list, infix_list) ;
								for ( int i = 0 ; i < postfix_list.size() ; i++)
									cout << postfix_list[i] << ","  ;
								cout << endl ;
							
							
								translate_array_statement(postfix_list,temp.cmdToken[0] ) ;
								postfix_list.clear() ;
								infix_list.clear() ;												
							}//if
						}//if																																												
					}//if																														
				}//if
				
				else if (count_array(temp) == 0){
					for ( int j = 0 ; j < temp.cmdToken.size()-1 ; j++){	
						 infix_list.push_back(temp.cmdToken[j]) ;											    																																																																									
					}//for	
														
					InfixToPostfix(postfix_list, infix_list) ;
					for ( int i = 0 ; i < postfix_list.size() ; i++)
						cout << postfix_list[i] << ","  ;
					cout << endl ;
				
				
					translate_assign_statement(postfix_list) ;
					postfix_list.clear() ;
					infix_list.clear() ;																			
				}//if			
				return true;
			}//if		    
		}//else
	
	    return false ;
	}//is_statement()
	
	bool is_GTO(AllCmdToken tmp){
		
		//判斷 ;是否正確 
		if (is_end_right(tmp)){
			if (tmp.cmdToken.size() ==3){
				if (isIdentifier(tmp.cmdToken[1]))
					return true ;
			}//if
		}//if
		
		return false ;
	}//is_GTO()
	
	void translate_GTO(AllCmdToken tmp){
		TABLE6 temp6 ;
		temp6.opr.tableNum = 2 ;
		temp6.opr.tableNo = 11 ;
		
		outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 									
		table6.push_back(temp6) ; //將中間碼放入table6中
	    outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << tmp.cmd<< endl ; 
	    outFile.close() ;
	}//translate_GTO()
	
	bool is_input_output(AllCmdToken tmp){
		vector<string> variable_list ;
		
		//判斷 ;是否正確 
		if (is_end_right(tmp)){
			for ( int i = 1 ; i < tmp.cmdToken.size() -1 ; i++){
				variable_list.push_back(tmp.cmdToken[i]) ;
			}//for
			
			if (is_variable(variable_list) )
				return true ;
		}//if
		
		return false ;
	}//is_input_output()
	
	bool is_call(AllCmdToken tmp){
		vector<string> argument_list ;
		int count_left = 0 ;
		int count_right = 0 ;
		
		
		if (is_end_right(tmp)){
			for ( int i = 0 ; i < tmp.cmdToken.size() ; i++){
				if (tmp.cmdToken[i].compare("(") == 0)
					count_left++ ;
				else if(tmp.cmdToken[i].compare(")") == 0)
					count_right++ ;
			}//for
			
			if ( count_left == 1 && count_right == 1 ){
				if ( isIdentifier(tmp.cmdToken[1])){
					if (tmp.cmdToken[2].compare("(") == 0 && tmp.cmdToken[tmp.cmdToken.size()-2].compare(")") == 0){
						for ( int i = 3 ; i < tmp.cmdToken.size()-1 ; i++){
							while ( tmp.cmdToken[i].compare(",") != 0 && tmp.cmdToken[i].compare(")") != 0){
								argument_list.push_back(tmp.cmdToken[i]) ;
								i++ ;
							}//while
							
							if (argument_list.size() != 0 ){
								if (!is_argument(argument_list))
								return false ;
							}//if
							
							else
								return false ;
																							
						    argument_list.clear() ;
						}//for
						
						return true ;
					}//if
				}//if
			}//if		
		}//if
		
		return false ;
	}//is_call()
	
	void translate_call(AllCmdToken tmp){
		vector<string> infromation_table ;
		string str = "" ;
        int index = 0 ;
        TABLE6 temp6 ;
		 
		for ( int i = 3 ; i < tmp.cmdToken.size() -2 ; i++){
		 	while (tmp.cmdToken[i].compare(",") != 0){
		 		str = str + tmp.cmdToken[i] ;
				i++ ;
				if (i == tmp.cmdToken.size() -2)	
					break ;
			}//while
			
			infromation_table.push_back("5") ;
			infromation_table.push_back(str) ;
			str = "" ;
		}//for
		
		index = table7.size() ;
		
		table7.push_back(intToString(infromation_table.size()/2)) ;
		for ( int i = 0 ; i < infromation_table.size() ; i++){
			table7.push_back(infromation_table[0]) ;
		}//for
		
		temp6.opr.tableNum = 2 ;
		temp6.opr.tableNo = 3 ;
		table6.push_back(temp6) ;
		outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
		outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << tmp.cmd<< endl ; 
		outFile.close() ;
	}//translate_call()
	
    bool is_argument(vector<string> list){
    	if (list.size() == 1){
    		if (isIdentifier(list[0]))
    			return true ;
		}//if
		
		else if(is_constant(list))
			return true ;
		
		return false ;
	}//is_argument()
	
	void translate_subroutine(AllCmdToken tmp){
		vector<string> idList ;
		vector<string> temp_list ;
		vector<string> variable_list ;
		vector<int> index_list ;
		TABLE6 temp6 ;
		vector<TABLE6> tmp_table6 ;
		int hash_index = -1 ;
		int type = -1 ;
		bool error = false ;
		
		for ( int i = 3 ; i < tmp.cmdToken.size()-2 ; i++)
			idList.push_back(tmp.cmdToken[i]) ;
			
	    for ( int i = 0 ; i < idList.size() ; i++){
	    	type = dataType(idList[i]) ;
	    	i++ ;
	    	while(dataType(idList[i]) < 0){
	    		if (isIdentifier(idList[i])){
	    			variable_list.push_back(idList[i]) ;
				}//if
				i++ ;
				if (i == idList.size())
					break ;
			}//while
			
			i--;
			
			for ( int j = 0 ; j < variable_list.size() ; j++){
				if(return_table_index(variable_list[j], subroutine, type) == -1){					
					Hash_table(hashValue(countAscii(variable_list[j])), subroutine, variable_list[j], hash_index) ; // 把PROGRAM的IDENTIFIER HASH 至TABLE 
					table5[hash_index].type = type ;
					index_list.push_back(hash_index) ;
					
					temp6.opr.tableNum = 5 ;
					temp6.opr.tableNo = hash_index ;
					tmp_table6.push_back(temp6) ; //將中間碼放入table6中
				}//if
				
				else{
					error = true ;
					break ;
				}//else															
	    	}//for	
			variable_list.clear() ;	    	
		}//for
		
		outFile.open(outFileName.c_str(), ios::out | ios::app ) ;
		
		if(!error){
	    	for ( int i = 0 ; i < tmp_table6.size() ; i++){
	    		table6.push_back(tmp_table6[i]) ; //將中間碼放入table6中
	    		outFile << table6.size() << ".\t((" << tmp_table6[i].opr.tableNum << ", " << tmp_table6[i].opr.tableNo << "),\t,\t,\t)\t" << table5[index_list[i]].token<< endl ; 
				cout << "((" << tmp_table6[i].opr.tableNum << "," << tmp_table6[i].opr.tableNo << "),\t,\t,\t)\t" << table5[index_list[i]].token<< endl ; 
			}//for
		}//if
		
		else{
			cout << "varabie already exist!!" << endl ;
			outFile << "line" << line << "\t"<< tmp.cmd << "\t" << "varabie already exist!!" << endl ;
			for ( int i = 0 ; i < index_list.size() ; i++){
				table5[index_list[i]].token = "" ;
				table5[index_list[i]].hashValue = -1 ;
				table5[index_list[i]].subRoutine = -1 ;
				table5[index_list[i]].pointer = -1 ;
				table5[index_list[i]].type = -1 ;
			}//for	
		}//else
		
		outFile.close() ; 				
	}//translate_subroutine()
	
	bool is_subroutine_heading(AllCmdToken tmp){
		vector<string> list ;
		vector<string> parameter_group_list ;
		int count_left = 0 ;
		int count_right = 0 ;
		int index = 0 ;
										  
		 if (is_end_right(tmp)){
		 	for ( int i = 0 ; i < tmp.cmdToken.size() ; i++){
				if (tmp.cmdToken[i].compare("(") == 0)
					count_left++ ;
				else if(tmp.cmdToken[i].compare(")") == 0)
					count_right++ ;
			}//for
			
			
			if ( is_br_right(tmp.cmdToken) ){				
				if (isIdentifier(tmp.cmdToken[1])){					
					if (tmp.cmdToken[2].compare("(") == 0 && tmp.cmdToken[tmp.cmdToken.size()-2].compare(")") == 0){
						for ( int i = 3 ; i < tmp.cmdToken.size()-2 ; i++){
							list.push_back(tmp.cmdToken[i]) ;
						}//for
						
						if (list.size() != 0){
							parameter_group_list.push_back(list[0]) ;
														
							for ( int i = 1 ; i < list.size() ; i++){
								while (is_type(list[i]) == -1){
									parameter_group_list.push_back(list[i]) ;
									i++ ;
								
									if( i == list.size())
										break ;
								}//while
							    
							    	
								if ( i != list.size()){
									i-- ;
									if (list[i].compare(",") != 0)
										return false ;
								    
								    parameter_group_list.erase(parameter_group_list.end()) ;
								}//if
								
								if(!is_parameter_group(parameter_group_list))
									return false ;
								
								parameter_group_list.clear() ;
								if ( i < list.size()){
									i++ ;
									parameter_group_list.push_back(list[i]) ;
								}//if
							}//for
							
							return true ;
						}//if
						
						/*					
						if (is_type(list[0]) != -1){
							parameter_group_list.push_back(list[0]) ;
							
							for ( int i = 1 ; i < list.size() ; i++){
								while(is_type(list[i]) == -1){
									parameter_group_list.push_back(list[i]) ;
									i++ ;
									
									if( i == list.size())
										break ;
								}//while
								
								if ( i != list.size()){
									i-- ;
									if (list[i].compare(",") != 0)
										return false ;
								}//if
								
								if(!is_parameter_group(parameter_group_list))
									return false ;
							}//for
							
							return true ;
						}//if*/
					}//if
				}//if
			}//if
		 }//if
		 
		 return false ;
	}//is_subroutine_heading()
	
	bool is_parameter_group(vector<string> list){
	    vector<string> parameter_list ;
	    
	    /*
	    for ( int i = 0 ; i < list.size() ; i++)
	    	cout << "list: " << list[i] << endl ;
	    cout << endl ;*/
	    
		if(list.size() > 3){
			if(is_type(list[0]) != -1){
				if (list[1].compare(":") == 0){
		            for ( int i = 2 ; i < list.size() ; i++){
		            	while( list[i].compare(",") != 0){
		            		
		            		if ( list[i].compare("(") == 0){
		            			while (list[i].compare(")") != 0){
		            				parameter_list.push_back(list[i]) ;
		            				i++ ;
								}//while
								parameter_list.push_back(list[i]) ;
								i++ ;		            				
							}//if
							
							else{
								parameter_list.push_back(list[i]) ;
			            		i++ ;	
							}//else
		            		
		            		
		            		if ( i == list.size())
	                           break ;
						}//while
						
						/*
						for ( int i = 0 ; i < parameter_list.size() ; i++)
							cout << "pr: " << parameter_list[i] << endl ;*/
							
						if (!is_parameter(parameter_list))
							return false ;
							
						parameter_list.clear() ;
					}//for		
					
					return true ;
				}//if
			}//if
		}//if
		
		return false ;
	}//is_parameter_group()
	
	bool is_parameter(vector<string> list) {
		if (list.size() == 1){
			if (isIdentifier(list[0]))
				return true ;
		}//if
		
		else{
			if (is_array(list))
				return true ;
		}//else
		
		return false ;
	}//is_parameter()
	
	
	bool is_array(vector<string> list){
		
		if(list.size() == 3){
			if (isIdentifier(list[0]) && list[1].compare("(") == 0 && list[2].compare(")") == 0 )
				return true ;
		}//if
		
		return false ;
		
		/*
		if (list.size() > 4){
			if (isIdentifier(list[0])){
				if (list[1].compare("(") == 0 && list[list.size()-1].compare(")") == 0){
					for ( int i = 2 ; i < list.size()-1 ; i++){
						if (i % 2 == 0){
							if (!isInteger(list[i]))
								return false ;
						}//if
						
						else{
							if(list[i].compare(",") != 0)
								return false ;
						}//else
					}//for
					
					return true ;
				}//if
			}//if
		}//if
		
		return false ;*/
	}//is_array()
				
	int return_type(string str){
		for ( int i = 0 ; i < 100 ; i++){
			if(str.compare(table5[i].token) == 0){
				return table5[i].type ;
			}//if
		}//for
		
		return -1 ;
	}//return_type()
		
	int return_table_index(string str, int subroutine, int type){
		for ( int i = 0 ; i < 100 ; i++){
			if (str.compare(table5[i].token) == 0 && (table5[i].subRoutine == subroutine) && (table5[i].type == type)){
				return  i ;
			}//if
		}//for
		
		return -1 ;
	}//return_lable_index()
	 
	
	void SyntaxAnalysis(){
		int hash_index = -1 ;
		int type = -1 ;
		
		int index = 0 ;
		int index_2 = 0 ;
		int dim = 0 ;
		int n = 0 ;
		AllCmdToken tempCmdToken ;
		bool isENP = false ;
		bool isSUB = false ;
		vector<string> infix_list ;
		vector<string> postfix_list ;
		vector<int> information_index ;
		bool error = false ;
		TABLE6 temp6 ;
		
		while(cmdTokenList[index].cmdToken.size() == 0) index++ ;
	
		for ( int i = index ; i < cmdTokenList.size() ; i++){
			
			if(i == index){
				if(is_program_heading(cmdTokenList[index])) {
					Hash_table(hashValue(countAscii(cmdTokenList[index].cmdToken[1])), subroutine, cmdTokenList[index].cmdToken[1],  hash_index) ;
					subroutine = hash_index ;
				}//if					
				else{
					cout << "syntax error!!" << endl ;
					outFile.open(outFileName.c_str(), ios::out| ios::app) ; //開檔 
					outFile << "line" << line << "\t"<< cmdTokenList[index].cmd << "\t" << "syntax error!!" << endl ;
					outFile.close() ;					
				}//else					
			}//if
			
			else{	
				if(cmdTokenList[i].cmdToken.size() != 0 ){
					
					if (cmdTokenList[i].cmdToken[0].compare("ENP") == 0){
						
						if ( !isENP && (cmdTokenList[i].cmdToken.size() == 2 && cmdTokenList[i].cmdToken[1].compare(";")) == 0){
							cout << "is ENP right!!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							temp6.opr.tableNum = 2 ;
							temp6.opr.tableNo = 6 ;
							table6.push_back(temp6) ; //將中間碼放入table6中
							outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << cmdTokenList[i].cmd << endl ; 
							outFile.close() ;
							isENP = true ;
						}//if
						
						else{
							isENP = true ;
							cout << "syntax error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else	
					}//if
					
					else if (cmdTokenList[i].cmdToken[0].compare("ENS") == 0){
						
						if ( isSUB && cmdTokenList[i].cmdToken.size() == 2) {
							if (cmdTokenList[i].cmdToken[1].compare(";") == 0){
								isSUB = false ;
								cout << "is ENS right!!!" << endl ;
								outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
								temp6.opr.tableNum = 2 ;
								temp6.opr.tableNo = 7 ;
								table6.push_back(temp6) ; //將中間碼放入table6中
								outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << cmdTokenList[i].cmd << endl ; 
								outFile.close() ;
							}//if
							
							else {
								cout << "syntax error!!" << endl ;
								outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
								outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
								outFile.close() ;								
							}//else
						}//if
						
						else{
					
							cout << "syntax error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else	
					}//if
					
					else if (cmdTokenList[i].cmdToken[0].compare("SUBROUTINE") == 0){
						
						if ( isENP && is_subroutine_heading(cmdTokenList[i])){
							Hash_table(hashValue(countAscii(cmdTokenList[i].cmdToken[1])), subroutine, cmdTokenList[i].cmdToken[1],  hash_index) ;
							table5[hash_index].subRoutine = -1 ;
							subroutine = hash_index ;
							
							isSUB = true ;
							cout << "subroutine right !!" << endl ;
							translate_subroutine(cmdTokenList[i]) ;
						}//if					
						
						else{
							cout << "subroutine error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else	
					}//if
					
					else if (cmdTokenList[i].cmdToken[0].compare("DIMENSION") == 0){
						if(isDimension(cmdTokenList[i], information_index)){
							translate_dimension_intermediate(cmdTokenList[i], information_index) ;
						}//if	
						
						else{
							cout << "syntax error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else			 				
					}//if
					
					else if(cmdTokenList[i].cmdToken[0].compare("VARIABLE") == 0){
						
					    if(isVariable(cmdTokenList[i])){
					    	translate_variable_intermediate(cmdTokenList[i]) ;
						}//if
						else{
							cout <<  "syntax error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else
					}//if
					
					else if(cmdTokenList[i].cmdToken[0].compare("LABEL") == 0){
						if(isLabel(cmdTokenList[i])){
							translate_label_intermediate(cmdTokenList[i]) ;
						}//if	
						else{
							cout <<  "syntax error!!" << endl ;
							outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
							outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
							outFile.close() ;
						}//else			
					}//if
					
					else{			
					    			
						//第一個token為id
						if (return_type(cmdTokenList[i].cmdToken[0]) == 5){
							
							if(cmdTokenList[i].cmdToken.size() > 1){						
								//把label先拿掉 
								tempCmdToken.cmd = copyString(cmdTokenList[i].cmd) ;
								for ( int j = 1 ; j < cmdTokenList[i].cmdToken.size() ; j++){
									tempCmdToken.cmdToken.push_back(cmdTokenList[i].cmdToken[j]) ;
								}//for
								
								
								//ENP
								if (cmdTokenList[i].cmdToken[1].compare("ENP") == 0){				
									if ( !isENP && cmdTokenList[i].cmdToken.size() == 3 && cmdTokenList[i].cmdToken[2].compare(";") == 0) {
										cout << "is ENP right!!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										temp6.opr.tableNum = 2 ;
										temp6.opr.tableNo = 6 ;
										table6.push_back(temp6) ; //將中間碼放入table6中
										outFile << table6.size() << ".\t((" << temp6.opr.tableNum << ", " << temp6.opr.tableNo << "),\t,\t,\t)\t" << cmdTokenList[i].cmd << endl ; 
										outFile.close() ;
										isENP = true ;
									}//if
									
									else{
										isENP = true ;
										cout << "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else	
								}//if
																
								//IF STATEMENT
								else if (cmdTokenList[i].cmdToken[1].compare("IF") == 0){									
									if(is_if_statement(tempCmdToken)) {
										// label 放入中間碼
										//轉成中間碼 
										cout << "rightright!!!" << endl ;
									}
									else{
										cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else
								}//if
								
								//CALL STATEMENT
								else if(cmdTokenList[i].cmdToken[1].compare("CALL") == 0){
									if(is_call(tempCmdToken)){
										cout << "rightright!!!" << endl ;
										translate_call(tempCmdToken) ;
									}//if
									else{
										cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else
								}//if 
								
								//INPUT OUTPUT STATEMENT
								else if(cmdTokenList[i].cmdToken[1].compare("INPUT") == 0 || cmdTokenList[i].cmdToken[1].compare("OUTPUT") == 0){
									if(is_input_output(tempCmdToken)) cout << "rightright!!!" << endl ;
									else{
										cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else
								}//if
								
								//GTOSTATEMENT
								else if(cmdTokenList[i].cmdToken[1].compare("GTO") == 0) {
									cout << "GTO right!!!" << endl ;
									if(is_GTO(tempCmdToken)) {
										translate_GTO(tempCmdToken) ;
										cout << "GTO right!!!" << endl ;
									}//if
									else{
										cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else
								}//if
								
								//assignmetn statement
								else if(isIdentifier(cmdTokenList[i].cmdToken[1])) {
							    	if(is_assign_statement(tempCmdToken)) {
							    		cout << "rightright!!!" << endl ;
							    		cout << tempCmdToken.cmd << endl ;
		
										if (count_array(tempCmdToken) == 1){								
											if (return_table_index(tempCmdToken.cmdToken[0], subroutine, 1) != -1){
												if (table5[return_table_index(tempCmdToken.cmdToken[0], subroutine, 1)].type == 1){
													dim = table5[return_table_index(tempCmdToken.cmdToken[0], subroutine, 1)].pointer + 1 ;
													if (dim == 1){
														for ( int j = 1 ; j < tempCmdToken.cmdToken.size()-1 ; j++){	
														    if (tempCmdToken.cmdToken[j].compare("(") != 0 && tempCmdToken.cmdToken[j].compare(")") != 0)												    												
																infix_list.push_back(tempCmdToken.cmdToken[j]) ;																																																	
														}//for	
														
														InfixToPostfix(postfix_list, infix_list) ;
														for ( int i = 0 ; i < postfix_list.size() ; i++)
															cout << postfix_list[i] << ","  ;
														cout << endl ;
													
													
														translate_array_statement(postfix_list,tempCmdToken.cmdToken[0] ) ;
														postfix_list.clear() ;
														infix_list.clear() ;												
													}//if
												}//if																																												
											}//if																														
										}//if
										
										else if (count_array(tempCmdToken) == 0){
											for ( int j = 0 ; j < tempCmdToken.cmdToken.size()-1 ; j++){	
												 infix_list.push_back(tempCmdToken.cmdToken[j]) ;											    																																																																									
											}//for	
																				
											InfixToPostfix(postfix_list, infix_list) ;
											for ( int i = 0 ; i < postfix_list.size() ; i++)
												cout << postfix_list[i] << ","  ;
											cout << endl ;
										
										
											translate_assign_statement(postfix_list) ;
											postfix_list.clear() ;
											infix_list.clear() ;																			
										}//if
																																														 
									}//if
								    else{
							    		cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< tempCmdToken.cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
									}//else
								}//if
								
								//若以上情況皆沒有，syntax error
								else{
										cout <<  "syntax error!!" << endl ;
										outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
										outFile << "line" << line << "\t"<< tempCmdToken.cmd << "\t" << "syntax error!!" << endl ;
										outFile.close() ;
								}//else
							}//if
							
							//若以上情況皆沒有，syntax error
							else{
								cout <<  "syntax error!!" << endl ;
								outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
								outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
								outFile.close() ;
							}//else																					
						}//if
						
						//第一個token不為id
						else{
							//IF STATEMENT
							if (cmdTokenList[i].cmdToken[0].compare("IF") == 0) {
								if(is_if_statement(cmdTokenList[i]))  cout << "rightright!!!" << endl ;
								else cout <<  "syntax error!!" << endl ;
							}//if
							
							//CALL STATEMENT
							else if(cmdTokenList[i].cmdToken[0].compare("CALL") == 0) {
								if(is_call(cmdTokenList[i])){
									cout << "rightright!!!" << endl ;
									translate_call(cmdTokenList[i]) ;
								}//if
								else{
									cout <<  "syntax error!!" << endl ;
									outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
									outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
									outFile.close() ;
								}//else
							}//if
							
							//INPUT OUTPUT STATEMENT
							else if(cmdTokenList[i].cmdToken[0].compare("INPUT") == 0 || cmdTokenList[i].cmdToken[0].compare("OUTPUT") == 0){
								
								if(is_input_output(cmdTokenList[i])) cout << "rightright!!!" << endl ;
								else{
									cout <<  "syntax error!!" << endl ;
									outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
									outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
									outFile.close() ;
								}//else
							}//if
							
							//GTO STATEMENT
							else if(cmdTokenList[i].cmdToken[0].compare("GTO") == 0) {
								if(is_GTO(cmdTokenList[i])) {
									translate_GTO(cmdTokenList[i]) ;
									cout << "GTO right!!!" << endl ;
								}//if
								else{
									cout <<  "syntax error!!" << endl ;
									outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
									outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
									outFile.close() ;
								}//else
							}//if
							
							//ASSIGNMETN STATEMENT
							else if(isIdentifier(cmdTokenList[i].cmdToken[0])){
								if(is_assign_statement(cmdTokenList[i]))  {
									cout << "rightright!!!" << endl ;
									cout << cmdTokenList[i].cmd << endl ;
									
									if (count_array(cmdTokenList[i]) == 1){								
										if (return_table_index(cmdTokenList[i].cmdToken[0], subroutine, 1) != -1){
											if (table5[return_table_index(cmdTokenList[i].cmdToken[0], subroutine, 1)].type == 1){
												dim = table5[return_table_index(cmdTokenList[i].cmdToken[0], subroutine, 1)].pointer + 1 ;
												if (dim == 1){
													for ( int j = 1 ; j < cmdTokenList[i].cmdToken.size()-1 ; j++){	
													    if (cmdTokenList[i].cmdToken[j].compare("(") != 0 && cmdTokenList[i].cmdToken[j].compare(")") != 0)												    												
															infix_list.push_back(cmdTokenList[i].cmdToken[j]) ;																																																	
													}//for	
													
													InfixToPostfix(postfix_list, infix_list) ;
													for ( int i = 0 ; i < postfix_list.size() ; i++)
														cout << postfix_list[i] << ","  ;
													cout << endl ;
												
												
													translate_array_statement(postfix_list,cmdTokenList[i].cmdToken[0] ) ;
													postfix_list.clear() ;
													infix_list.clear() ;												
												}//if
											}//if																																												
										}//if																														
									}//if
									
									else if (count_array(cmdTokenList[i]) == 0){
										for ( int j = 0 ; j < cmdTokenList[i].cmdToken.size()-1 ; j++){	
											 infix_list.push_back(cmdTokenList[i].cmdToken[j]) ;											    																																																																									
										}//for	
																			
										InfixToPostfix(postfix_list, infix_list) ;
										for ( int i = 0 ; i < postfix_list.size() ; i++)
											cout << postfix_list[i] << ","  ;
										cout << endl ;
									
									
										translate_assign_statement(postfix_list) ;
										postfix_list.clear() ;
										infix_list.clear() ;																			
									}//if
																																													 
								}//if
							    else{
						    		cout <<  "syntax error!!" << endl ;
									outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
									outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
									outFile.close() ;
								}//else
							}//else
							
							//若以上情況皆沒有，syntax error 
							else{
									cout <<  "syntax error!!" << endl ;
									outFile.open(outFileName.c_str(), ios::out | ios::app) ; //開檔 
									outFile << "line" << line << "\t"<< cmdTokenList[i].cmd << "\t" << "syntax error!!" << endl ;
									outFile.close() ;
							}//else
						}//else						 						
					}//else
				}//if				
			}//else
			
			line++ ;
		}//for
				
	}//SyntaxAnalysis()
	
	
	void printTable5(){
		for ( int i = 0 ; i < 100 ; i++){
			cout << i << "\t" << table5[i].token << "\t" << table5[i].subRoutine << "\t" << table5[i].type << "\t" << table5[i].pointer << endl ;
		}//for
	}//printTable5()
	
	void printTable7(){
		cout << "table7:" << endl ;
		for ( int i = 0 ; i < table7.size() ; i++){
			cout << i << "\t" << table7[i] << endl ;
		}//for
	}//printTable7()
	

	void printTable3(){
		cout << "table3:" << endl ;
		for ( int i = 0 ; i < 100 ; i++){
			cout << i << "\t" << table3[i].token << endl ;
		}//for
	}//printTable5()
	
	void printTable4(){
		cout << "table4:" << endl ;
		for ( int i = 0 ; i < 100 ; i++){
			cout << i << "\t" << table4[i].token << endl ;
		}//for
	}//printTable5()
};

int main(int argc, char** argv) {
	Compiler compiler ;
	compiler.readFile() ; //讀檔 
	compiler.createTable1(); //建立delimiter table 
	compiler.createTable2(); //建立delimiter table 
	compiler.LexicalAnalysis() ; //切token 
	cout << "systas analysis: " << endl ;
	compiler.SyntaxAnalysis() ;
	cout << "success !!!!" << endl ;
	compiler.printTable5() ;
	//compiler.printTable7() ;
	//compiler.printTable3() ;
	//compiler.printTable4() ;
	return 0;
}
