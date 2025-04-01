// Read current temp (process value) from Eurotherm TC
// Print value to screen & to file passed via argc

#include <bios.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COM1	0
#define COM2	1
#define DATA_READY	0x100
#define STX	0x02
#define ETX	0x03
#define EOT	0x04
#define ENQ	0x05
#define ACK	0x06
#define NAK	0x15
#define TRUE	1
#define FALSE	0

#define SETTINGS	(0xE0 | 0x02 | 0x18 | 0x00)

int status, n_retry = 0;
char filename[100];
float timer(void);

main (argc, argv)
int argc;
char *argv[];

{
char *r_str, rd_pv[100], *send_str();
FILE *fp, *fopen();

strcpy(filename, argv[1]);
fp = fopen(filename, "a+");
status = bioscom(0, SETTINGS, COM2);	/* set serial port parameters */
sprintf(rd_pv, "%c0000PV%c", EOT, ENQ);
r_str = send_str(rd_pv);
printf("%s\n", r_str);
fprintf(fp,"%s\n", r_str);
fclose(fp);
return 0;
}

char *send_str(char *code)
{
char c, in_mesg[100];
int i, j, in, out, status, DONE, SAVE;
int count, reset;
float start_time, cur_time;

count = 0;
reset = 0;

com_loop: 
    if(count > 5 && reset < 2)
	{
	reset++;
	count = 0;
	printf ("\nCount > 10. Resetting serial port parameters.");
	status = bioscom(0, SETTINGS, COM2);	/* set serial port parameters */
	}

    if(count > 11)
	{
	printf ("\nTIMEOUT\n");
	printf ("\nstatus = %x", status);
	printf ("\nsent string = %s", code);
	printf ("\nsent string = ");
	i = 0;
	while((c = code[i++]) != '\0')
		printf("%c", code[i]);
	leave();
	}

status = bioscom(3, 0, COM2);
while ((status & DATA_READY) != 0)	// clear out receive buffer
	{
	status = bioscom(2, 0, COM2);
	}

i = 0;
while((c = code[i++]) != '\0')
	{
      status = bioscom(1, c, COM2);
	}

status = 0;
j = 0;
DONE = FALSE;
SAVE = FALSE;

start_time = timer(); // used to determine timeout condition

while((status & DATA_READY) == 0) // check for input data
	{
	status = bioscom(3, 0, COM2);
	cur_time = timer();
	if ((cur_time - start_time) > 10)
		{
		count++;
		n_retry++;
		
		printf ("\nCP4	message sent to TC: %s", code);
		printf ("\nCount: %d. Retry No. %d. Status: %d", count, n_retry, status);
		goto com_loop;	// timeout - resend command
		}
	}	

while(!DONE)
	{
	status = bioscom(3, 0, COM2);
	if(status & DATA_READY)
	if((out = bioscom(2, 0, COM2) & 0x7f) != 0)
			{
			if((out & 0x7f) == ETX)
				{
				DONE = TRUE;
				out = bioscom(2, 0, COM2);
				break;
				}
			if(SAVE == TRUE)
				in_mesg[j++] = out & 0x7f;
			if((out & 0x7f) == 'V')
				SAVE = TRUE;
			if((out & 0x7f) == ACK)
				{
				DONE = TRUE;
				}
			}
	if ((status & DATA_READY) == 0)
		{
		start_time = timer(); // used to determine timeout condition
		while((status & DATA_READY) == 0) 	// check for input data
			{
			status = bioscom(3, 0, COM2);
			cur_time = timer();
			if ((cur_time - start_time) > 10)
				{
				count++;
				n_retry++;
				printf ("\nCP6	no response from TC for more than 10sec");
				printf ("\nCount: %d. Retry No. %d. Status: %d", count, n_retry, status);
				goto com_loop;	// timeout - resend command
				}
			}
		}
	}

in_mesg[j] = '\0';
return(in_mesg);

}

float timer()
{
    struct time t;
    float time_sec;

    gettime(&t);
    time_sec = (float) t.ti_hour * 3600 + (float) t.ti_min * 60 + (float) t.ti_sec + (float) t.ti_hund / 100;
    return(time_sec);
}

int leave (void)	// Exit the program
{
printf ("\nNumber of COM Retries = %d\n", n_retry);
printf ("\npress any key to exit");
getch();
exit(1);	// exit program
return;
}