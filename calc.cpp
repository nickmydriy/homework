//program calculator
//NB: All numbers, symbols and brakets are isolated by spaces. After the last number (bracket) follows a newline ('\ n').
//PS: Result and the value shall not exceed the type int.
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<pair<int, char>>> st;             // st - stack containing only numbers and sign, without brakets;
int br_checker;

bool is_number(string s)                   
{
	for (int i = 0; i < s.length(); i++)
	{
		int a = (int)s[i] - (int)'0';
	    if (!( a < 9 && a > 0) && !(s[i] == '-' && i == 0))
			return 1;
	}
	return 0;
}

void error_in(string s, char c)                 // processing error
{
	if (c == 'n' && is_number(s) == 1 && s != "0")
	{
		cout << "Error! Invalid number format! (Or missing space between characters, brackets, numbers)";
		exit(0);
	}
	if (c == 's' && s != "+" && s!= "-" && s != "*" && s != "/" && s != "%" && s != ")")
    {
		cout << "Error! Invalid sign format!";
		exit(0);
	}
			
}

void error_brakets(int i)                        // processing error
{
	if (i < 0) 
	{
		cout << "Error! Incorrect bracket sequence!";     //Error!!
		exit(0);
	}
}


long calc_st(int level)     // function that calculate expression in the brakets
{
	int i = 0, sum = 0;
	while(1)                              // first multiplication, etc.
	{
		if (st[level][i].second == '*')
		{
			st[level][i].first *= st[level][i+1].first;
			st[level][i].second = st[level][i+1].second;
			st[level].erase(st[level].begin() + i + 1);
		}
		else
			if (st[level][i].second == '/')
			{   
				if (st[level][i+1].first == 0)       // processing error
				{
					cout << "Error! Division by zero!";
					exit(0);
				}
				st[level][i].first /= st[level][i+1].first;
				st[level][i].second = st[level][i+1].second;
				st[level].erase(st[level].begin() + i + 1);
			}
			else
				if (st[level][i].second == '%')
				{
					if (st[level][i+1].first == 0)       // processing error
				        {
				        	cout << "Error! Division by zero!";
				        	exit(0);
				        }
					st[level][i].first %= st[level][i+1].first;
					st[level][i].second = st[level][i+1].second;
					st[level].erase(st[level].begin() + i + 1);
				}
				else
					if (st[level][i].second == '!')    // '!' means the end of the bracket (expression)
						break;
					else
						i++;
	}
	sum += st[level][0].first;                      // then summing 
	for( i = 0; i < st[level].size() - 1; i++)    
		if (st[level][i].second == '+')
			sum += st[level][i+1].first;
		else
			sum -= st[level][i+1].first;
	st.pop_back();                                      //  clean stack
	return sum;                                         // return value of expression in brakets
}

long build_st(int level)                                  // build stack
{
	int number = 0, j = 0;                                
	string value;                                         // temporary variable
	char sign;
	st.push_back(vector<pair<int,char>>());
	while(1)
	{
		cin >> value;                                         

		if (value != "(")                                  
		{
			if (getchar() == (int)'\n')                        // if we found '\n' then we must numerate general expression
			{
				error_brakets(br_checker);
				error_in(value , 'n');                  // processing error
				st[level].push_back(pair<int, char>());
				number = stoi(value);
				st[level][j].first = number;
				st[level][j].second = '!';                       //  '!' means the end of the bracket (expression)
				return calc_st(level);                                     
			}
			else
			{
				cin >> sign;
				if (sign == ')')                                  // if we found ')' then we must numerate expression in brakets
				{
					br_checker--;
					error_brakets(br_checker);
					error_in(value, 'n');
					number = stoi(value);
					st[level].push_back(pair<int, char>());
					st[level][j].first = number;
					st[level][j].second = '!';
					return calc_st(level);
				}
				else                                             // if dont found anythyng then we read sign  
				{                                                //(because after number can follows only '(', '\n' or sign;
					error_in(value, 'n');
					string s = "";
					s += (char)sign;
					error_in(s, 's');
					number = stoi(value);
					st[level].push_back(pair<int, char>());
					st[level][j].first = number;
					st[level][j].second = sign;
					j++;
				}
			}
		}
		else                                                      //if we found '(' then we must numerate exprisson in this brakets  
		{                                                         //and only then numerate this expression
			br_checker++;                                         //because stack can contain only numbers and signs
			error_brakets(br_checker);
			st[level].push_back(pair<int,char>());     
			st[level][j].first = build_st(level + 1);             //numerate expression in brakets (recursion)
			sign = getchar();
			if (sign == (int)'\n')                                //check on the end of exprission
			{				
				st[level][j].second = '!';                       
				return calc_st(level);
			}
			cin >> sign;
			string s = "";
			s += sign;
			error_in(s, 's');
			if (sign == (int)')')                                  //check on the braket after braket ( ex: 1 + ( 1 + ( 1 + 1 ) ) = ? )
			{	
				br_checker--;
				error_brakets(br_checker);
				st[level][j].second = '!';
				return calc_st(level);
			}
			st[level][j].second = sign;
			j++;
		}
	}
}

int main()
{
	cout << "Please, enter your expression;" << "\n";
	cout << "All numbers, symbols and brakets are isolated by spaces. After the last number (bracket) follows a newline." << "\n";
	int ans = build_st(0);                                       // run function
	cout << ans;                                              // output
	
}
