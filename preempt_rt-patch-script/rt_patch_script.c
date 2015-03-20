/*	
 *	Preempt_rt patch automated script
 *
 *	A) Analyze patches/series text file for patch file names,
 *	B) Generate shell script file to do patches,
 *	C) Do patches,
 *	D) Report in log file that patch is succeeded, hunked, failed.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATCHSERIES "patches/series"
#define SHELLSCRIPTNAME "patch_series.sh"
#define LOGFILE "rt_patch_log.txt"

int main (int argc, char *argv[]) {
	short int G = 0, P = 0, L = 0;
	
	/* check to see if user entered valid option arguments */
    if (argc < 2)
    {
USAGE:  printf("Usage: %s [OPTION]\n", argv[0]);
        printf("Try '%s -h' for more information.\n", argv[0]);
        return 0;
    }
    /* manual for usage options */
    if (!strcmp(argv[1], "-h")) {
		printf("\n\t-p\tDo patches without logging\n\n");
		printf("\t-l\tReport results in a log file: %s after patching\n\n", LOGFILE);
		printf("\t-g\tGenerate shell script file: %s\n\n", SHELLSCRIPTNAME);
		printf("example usage:\t%s -gl\n", argv[0]);
        return 0;
    } else {
    	if (argv[1][0] == '-') {
    		char *g = strchr(argv[1], 'g');		if (g != NULL) G = 1;
			char *p = strchr(argv[1], 'p');   	if (p != NULL) P = 1;
			char *l = strchr(argv[1], 'l');   	if (l != NULL) L = 1;
			if (	(!G && !P && !L) || /* No parameter given */
					(P && L) )			/* both p,l can not be set */ 
				goto USAGE;
    	} else goto USAGE;
    }

	static const char prefix[] = "cat patches/";
	static const char suffix[] = " | patch -p1";
	static const char tempout[] = "temp.out";
	
	char line[128], resultline[128];
	
	FILE *infile = fopen(PATCHSERIES, "r");
	FILE *outfile;
	FILE *logfile;
	if (infile == NULL) {
		perror (PATCHSERIES);
		exit(1);
	}
	
	if (G) {
		outfile = fopen(SHELLSCRIPTNAME, "w");
		if (outfile == NULL) {
			perror (SHELLSCRIPTNAME);
			exit(1);
		}
	}
		
	if (L) {
		logfile = fopen(LOGFILE, "w");
		if (logfile == NULL) {
			perror (LOGFILE);
			exit(1);
		}
	}	
	
	while (fgets(line, sizeof(line), infile) != NULL) {
		/*	A) Analyzing patch file names */
		/*	remove comments from 'patch filename list'	*/
		if (line[0] == '#' || line[0] == '\n') { 
			continue;
		}
				
		/* 	remove newline char from 'patch filename list' */
		size_t ln = strlen(line) - 1;
		if (line[ln] == '\n')
			line[ln] = '\0';
		
		strcpy(resultline, "");
		strcat (resultline, prefix);
		strcat (resultline, line);
		strcat (resultline, suffix);
		
		/*	B) Generating shell script file	*/
		if (G)	fprintf(outfile, "%s\n", resultline);

		/*	C) Do patches w/o log	*/
		if (P)	system(resultline);
		
		/*	D) Report in a log file	*/
		if (L) {
			strcat (resultline, " > temp.out");
			system(resultline);
			
			FILE *handle = fopen(tempout, "r");
			char templine[256];
			if (handle == NULL) {
				perror (tempout);
				exit(1);
			}
			
			fprintf(logfile, "PATCH: %s\n", line);
			
			while (fgets(templine, sizeof(templine), handle) != NULL) {
				if (templine[0] == 'p') {
					char patching[10], file[5], path[256];
					sscanf(templine, "%s %s %s", patching, file, path);
					fprintf(logfile, "\tFILE: %s\n", path);
				} else {
					fprintf(logfile, "\t\t%s", templine);
				}
				
			}
			
			fclose (handle);
		}
	}
	
	fclose (infile);
	if (G)	fclose (outfile);
	if (L) {
		fclose (logfile);
		if (remove(tempout))	perror(tempout);		
	}	
	
	return 0;
}
