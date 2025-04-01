#include <stdio.h>
#include <stdlib.h>

main (argc, argv)
int argc;
char *argv[];

{
char c, str[100], dirname[100], timename[100], tempname[100], dataname[100];
int i, n, num;
float stime, b_temp, e_temp, avetemp;
FILE *time, *temp, *data, *s_temps, *fopen();

strcpy(dirname, argv[1]);
strcpy(timename, dirname);
strcpy(tempname, dirname);
strcpy(dataname, dirname);
strcpy(str, dirname);
strcat(timename, "/REFTIMES");
strcat(tempname, "/REFTEMPS");
strcat(dataname, "/REFDATA.txt");
strcat(str, "/ra_temps.txt");

time = fopen(timename, "rt");
temp = fopen(tempname, "rt");
data = fopen(dataname, "wt");
s_temps = fopen(str, "wt");

fscanf(time, "REFERENCE SCAN TIMES (min)\n\n");
fscanf(temp, "REFERENCE SCAN TEMPERATURES\n");
while (1==1)
	{
	if ((fscanf(time, "%d %f\n\n", &num, &stime)) != 2)
		break;
	fscanf(temp, "%f\n", &b_temp);
	fscanf(temp, "%f\n", &e_temp);
	avetemp = (b_temp + e_temp)/2;
	fprintf(data, "%d, %f, %f\n", num, stime, avetemp);
	fprintf(s_temps, "%f\n", avetemp);
	printf("%d %f %f\n", num, stime, avetemp);
	}
fclose(data);
fclose(s_temps);
return;
}
