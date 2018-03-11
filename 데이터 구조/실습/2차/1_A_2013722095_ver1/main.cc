#include "Manager.h"



int main(void){
	cout.setf(ios::fixed);
	cout.precision(2);

	int bpOrder = 3;

	Manager ds(bpOrder);
	ds.run("command.txt");

	return 0;
}

bool compare(pair< pair< double, int>, StudentData* > a, pair< pair< double, int>, StudentData* > b){
	/* 아래 작성된 코드는 이용 또는 삭제, 변경이 가능함 -- 과제 제출 시 본 주석 삭제 */

	if (a.first.first == b.first.first){
		return a.first.second > b.first.second;
	}
	else{
		return a.first.first < b.first.first;
	}
}