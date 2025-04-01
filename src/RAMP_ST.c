// Eurotherm Temperature Controller
//
// Program function: Start the heating ramp program
//
// DKM 05/12

#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

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

int mk_BCC_checksum(char *c_code);
int BCC;

int main(void)
{
char *r_str, rd_sl[100], wt_sl[100], wt_sl_BCC[100], *send_str(), sl_strng[5];
int status, sl_val;
char BCC_ch;

status = bioscom(0, SETTINGS, COM2);	/* set serial port parameters */

/*
// Set the setpoint
    printf ("\nSpecify the Setpoint Value: ");
    scanf ("%d", &sl_val);
    sprintf (sl_strng, "%d", sl_val);
    fflush (stdin);
*/

    sprintf (sl_strng, "0002");
    fflush (stdin);

// Calculate checksum (BCC)
    sprintf(wt_sl_BCC, "OS>%s%c", sl_strng, ETX);
    mk_BCC_checksum(wt_sl_BCC);
    BCC_ch = toascii(BCC);

// Send command to set the setpoint
    sprintf(wt_sl, "%c0000%cOS>%s%c%c", EOT, STX, sl_strng, ETX, BCC_ch);
    r_str = send_str(wt_sl);
    printf("\n%s", r_str);
/*
// Read current setpoint
    sprintf(rd_sl, "%c0000SL%c", EOT, ENQ);
    r_str = send_str(rd_sl);
    printf("\n%s", r_str);
*/

return 0;
}

char *send_str(char *code)
{
char c, in_mesg[100];
int i, j, in, out, status, DONE, SAVE;

i = 0;
while((c = code[i++]) != '\0')
	{
      status = bioscom(1, c, COM2);
	 }

j = 0;
DONE = FALSE;
SAVE = FALSE;
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
			if((out & 0x7f) == 'L')
				SAVE = TRUE;
			if((out & 0x7f) == ACK)
				{
				DONE = TRUE;
				}
			}
	}
in_mesg[j] = '\0';
return(in_mesg);

}

int mk_BCC_checksum(char *c_code)
{
int i;

    BCC = (c_code[0] ^ c_code[1]);
    for (i = 2; c_code[i] != '\0'; i++)
    {
        BCC = (BCC ^ c_code[i]);
    }

return 0;
}