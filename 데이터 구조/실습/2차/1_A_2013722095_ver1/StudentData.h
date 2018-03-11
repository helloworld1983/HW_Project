#ifndef _STUDENTDATA_H_
#define _STUDENTDATA_H_

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

#pragma warning(disable : 4996)
using namespace std;


class StudentData{
private:
	int			StudentID;		// 학번
	char		Name[10];		// 이름
	int			Year;				// 학년
	double	AvgGrade;		// 평점

public:
	StudentData(){
		StudentID = 0;
		memset(Name, 0, 10);
		Year = 0;
		AvgGrade = 0;
	};
	~StudentData(){};

	void	setStudentID(int id)			{ StudentID = id; }
	void	setName(char* name)		{ strcpy(Name, name); }
	void	setYear(int year)				{ Year = year; }
	void	setAvgGrade(char *a, char *b, char *c){
		setGrade(a,b,c);
		AvgGrade = getGrade(a) + getGrade(b) + getGrade(c);
		AvgGrade /= 3;
		int tmp = AvgGrade * 1000;
		int tmp2 = tmp / 10;
		if (tmp % 10 > 4){
			AvgGrade = (tmp2 + 1) / 100.0;
		}
		else{
			AvgGrade = tmp2 / 100.0;
		}
	}

	int			getYear()				{ return Year; }
	int			getStudentID()		{ return StudentID; }
	char*		getName()				{ return Name; }
	double	getAvgGrade()			{ return AvgGrade; }

	double	getGrade(char *ch){

		if (strcmp(ch, "A+") == 0){
			return 4.5;
		}
		else if (strcmp(ch, "A0") == 0){
			return 4.0;
		}
		else if (strcmp(ch, "B+") == 0){
			return 3.5;
		}
		else if (strcmp(ch, "B0") == 0){
			return 3.0;
		}
		else if (strcmp(ch, "C+") == 0){
			return 2.5;
		}
		else if (strcmp(ch, "C0") == 0){
			return 2;
		}
		else if (strcmp(ch, "D+") == 0){
			return 1.5;
		}
		else if (strcmp(ch, "D0") == 0){
			return 1.0;
		}
		else{
			return 0;
		}
		return 0;
	}
	virtual void printGrade()=0;
	virtual void printGrade(ofstream * fout)=0;
	virtual void setGrade(char* a, char* b, char *c) = 0;
};

bool compare(pair< pair< double, int>, StudentData* > a, pair< pair< double, int>, StudentData* > b);

#endif