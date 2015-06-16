#include <stdio.h>
#include <string.h>

int layer;

int layer1;
int flag;


char* trim(char *str)
{
	char *p1, *p2;
	
	if(!str || !str[0]) return str;
	p1=str;
	while(*p1==' ' || *p1=='\t' || *p1=='\r' || *p1=='\n') p1++;
	
	p2=str;
	while(*p2) p2++;
	p2--;
	while( *p2==' ' || *p2=='\t' || *p2=='\r' || *p2=='\n' ) p2--;
	p2++;
	if(*p2) *p2='\0';
	
	strcpy(str, p1);
	return str;
}


int process_line1(char *line)
{
	char *p;
	int len;
	
	if( strstr(line, "//") == line) return 0;
	
	
	if( line[0] == '/' && line[1] == '*'  && flag==0)
	{
		flag = 1;
		return 0;
	}
	
	if( flag == 1)
	{
		
		
		len = 0;
		for(p=line; *p; p++, len++);
		if( len < 2 ) return 0;
		if(*(p-2) == '*' && *(p-1) == '/' ) flag = 0;
		return 0;
	}
	
	for(p=line; *p; p++)
	{
		if(*p == '{') layer1++;
		if(*p == '}') layer1--;
	}
	return 0;
}


int main(int argc, char **argv)
{
	FILE *fp, *fp1;
	char filename[30], filename1[30];
	char line[1000], command[200];
	int i;
	
	
	if(argc<2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}
	
	
	sprintf(filename, argv[1]);
	sprintf(filename1, "%s.tmp", filename);
	
	
	if( (fp=fopen(filename, "r"))==NULL)
	{
		printf("Error open %s\n", argv[1]);
		return -1;
	}
	
	if( (fp1=fopen(filename1, "w+"))==NULL)
	{
		printf("Error open %s\n", argv[1]);
		return -1;
	}
	
	
	
	layer = 0;
	layer1 = 0;
	flag =0;
	
	while(!feof(fp))
	{
		memset(line, '\0', 1000);
		fgets(line, 1000, fp);
		if(feof(fp)) break;
		trim(line);
		
		process_line1(line);
		
		if(layer1 >= layer)
		{
			for(i=0; i<layer; i++) fprintf(fp1, "\t");
			fprintf(fp1, "%s\n", line);
			layer = layer1;
			//        process_line(line);
		}
		else
		{
			
			//       process_line(line);
			layer = layer1;
			for(i=0; i<layer; i++) fprintf(fp1, "\t");
			fprintf(fp1, "%s\n", line);
			
		}
	}
	
	fclose(fp);
	fclose(fp1);
	
	sprintf(command, "mv %s %s", filename1, filename);
	system(command);
}





