// Part of Samcol_2 macro, works with
// Eurotherm Temperature Controller
//
// Program function: 
// Adjusts PID parameters to the average good values for the range from 25C to 500C
// (Pb = 2.6, ti = 0.6, td = 0.1)
//
// 9/7/12 DKM

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

float timer(void);
char mk_BCC_checksum(char *c_code);

char *r_str;
int status, n_retry = 0;

int main (void)
{
char *r_str, *send_str(), xx_strng[5], wt_xx[100], BCC;
float xx_val;

status = bioscom(0, SETTINGS, COM2);	/* set serial port parameters */

// Set and send Proportional band Pb
xx_val = 2.6;
sprintf (xx_strng, "%1.3f", xx_val);
sprintf (wt_xx, "XP%s%c", xx_strng, ETX);
BCC = mk_BCC_checksum(wt_xx);
sprintf (wt_xx, "%c0000%cXP%s%c%c", EOT, STX, xx_strng, ETX, BCC);
r_str = send_str(wt_xx);
printf ("\n Pb = %s", xx_strng);

// Set and send Integral time ti
xx_val = 0.6;
sprintf (xx_strng, "%1.3f", xx_val);
sprintf(wt_xx, "TI%s%c", xx_strng, ETX);
BCC = mk_BCC_checksum(wt_xx);
sprintf (wt_xx, "%c0000%cTI%s%c%c", EOT, STX, xx_strng, ETX, BCC);
r_str = send_str(wt_xx);
printf (" ti = %s\n", xx_strng);

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

char mk_BCC_checksum(char *c_code)
{
int i;
char BCC;

    BCC = (c_code[0] ^ c_code[1]);
    for (i = 2; c_code[i] != '\0'; i++)
    {
        BCC = (BCC ^ c_code[i]);
    }
	
return(BCC);
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