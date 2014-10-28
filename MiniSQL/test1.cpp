#include <cstdio>
#include <iostream>
#include <string>
 int main(){
 	 FILE *fout = fopen("out.txt", "r+");
	 fseek(fout, 5, 0);
	 fwrite("test", 4, 1, fout);
	 fclose(fout);
}

