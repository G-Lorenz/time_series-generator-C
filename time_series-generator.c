#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PATH_DATE "/date.txt"
#define PATH_TO_TS "../covid19-time_series/"
#define STR_DIM 20
#define COL 14
#define DAYS 600
#define LEN_TITLE 50
#define DATE_SIZE 10

#define fips 0 //int
#define Admin2 1
#define Province_State 2
#define Country_Region 3
#define Last_Update 4
#define lat 5
#define long_ 6 //double
#define confirmed 7
#define deaths 8
#define recovered 9
#define active 10 //int
#define Combined_key 11
#define incident_rate 12
#define case_fatality_ratio 13 //double

void token_generator(char *temp, int *i, char *token){
	int j=0;
	token[0]='\0';
	fprintf(stderr, "token allocato = %s\n", token);
	fprintf(stderr, "temp = %s\n", &temp[*i]);
	fprintf(stderr, "temp[%d] = %c\n", *i, temp[*i]);
	while (temp[*i]!=',' && temp[*i]!='\0'){
		token[j] = temp[*i];
		(*i)++;
		j++;
	}
	(*i)++;
	token[j]='\0';
	fprintf(stderr, "token creato = %s\n", token);
	return;
}

int main (void){
	FILE* fin;	//input
	FILE* fts;	//old time_series file
	FILE* fout;	//new time_series file
	FILE* fdate;	//date file
	char **row;	//input row (splitted in subseries: see constants above)
	int i, s, j;	//variables for cycles
	char *token, *temp; //token: output of strtok, temp: line read in fin
	const char comma[2] = ",";
	const char hyphen[2]= "-";
	char *series=["confirmed", "deaths", "recovered", "active"];
	char *title;	//title of fts/fout
	char **row_out;	// output row (one for series)
	char *month, *day, *year;

	row=malloc(COL*sizeof(char*));	//allocation of row
	for(i=0; i<COL; i++){
		row[i]=malloc(STR_DIM*sizeof(char));
	}

	temp=malloc(COL*STR_DIM*sizeof(char));
	title = malloc(4*sizeof(char));
	row_out=malloc(4*sizeof(char*));//4: number of elements of series
	for(i=0; i<4; i++){		//idem
		row_out[i]=malloc(DAYS*STR_DIM*sizeof(char));
		title[i]=malloc(LEN_TITLE*sizeof(char));
		strcpy(title[i], PATH_TO_TS"time_series_covid19_"); //creation of standard title
		strcat(title[i], series[i]);
	}
	fdate = fopen(PATH_DATE, "w+"); //open date file with w+ in order to rewrite
	fgets(date, DATE_SIZE, fdate);
//	month = strtok(date, hyphen);
//	day = strtok(NULL, hyphen);
//	year = strtok(NULL, hyphen);

//remember to add a cycle upon all input files	
	system("wget https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_daily_reports/11-09-2020.csv -O a_tmpfile");
	fin = fopen("a_tmpfile", "r");
	fprintf(stderr, "fp=%p\n",(void*) fin);
	fgets(temp, STR_DIM*COL, fin);
	fprintf(stderr, "temp=%s\n", temp);
	token = strtok(temp, comma);
	for (i=0; i<14; i++) {
		row[i]=token;
		token = strtok(NULL, comma);
		fprintf(stderr, "%s\n", row[i]);
	}
		fprintf(stderr, "%s\n", row[recovered]);
	free(token);
	token=malloc(STR_DIM*sizeof(char));
	while (!feof(fp)){
		i=0;
		fgets(temp, STR_DIM*COL, fin);
		fprintf(stderr, "temp=%s\n", temp);
		for (j=0; j<14; j++){
			token_generator(temp, &i, row[j]);
		}
		printf("row[recovered] = %s\n", row[recovered]);
		for (s=0; s<4; s++){//full title
			strcat(title[s], row[Country_Region]);
			strcat(title[s], ".csv");
			fts[s] = fopen(title[s], "r");
		}
		if (fts[0] != NULL){//fout[0]	exists <=> fout[s] exists
			for (s=0; s<4; s++){//add element to each series
				fgets(row_out[s], STR_DIM*COL, fts[s]);
				fprintf(fout[s], "%s,%s\n",row_out[s], row[series[s]]);
			}
		}
		else{
			if (!strcmp(row[Admin2],"") || !strcmp(row[Province_State],"")){
				for (s=0; s<4; s++){//initialize series
					strcat(row_out[s], row[Admin2]);
					strcat(row_out[s], row[Province_State]);
					strcat(row_out[s], row[Country_Region]);
					strcat(row_out[s], row[series[s]]);
				}
			}
		}
	}
	printf("%s\n", row[confirmed]);
	system("rm a_tmpfile");
	return 0;
}
