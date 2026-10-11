#include<xc.h>
#include "adc.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "main.h"


unsigned char clock_reg[3];
unsigned char time[9];


const char org_password[PASSWORD_LENGTH] = {'0','0','1','1'};
char usr_password[PASSWORD_LENGTH];


void delay_ms(unsigned int ms)
{
    while(ms--)
        __delay_ms(1);
}


// Simple string compare
unsigned char my_strcmp(const char* a, const char* b, unsigned char len)
{
    for(unsigned char i=0; i<len; i++)
        if(a[i] != b[i])
            return 0;
    return 1;
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void display(void)
{
	clcd_print(time, LINE2(0));

	/*if (clock_reg[0] & 0x40)
	{
		if (clock_reg[0] & 0x20)
		{
			clcd_print("PM", LINE2(12));
		}
		else
		{
			clcd_print("AM", LINE2(12));
		}
	}*/
}

void get_speed(void)
{
    unsigned int speed;

    speed = read_adc(CHANNEL4) / 10.33;
    clcd_putch((speed/10 % 10) + 48, LINE2(14));
    clcd_putch((speed % 10) + 48, LINE2(15));
}

void get_event(void)
{
    unsigned char event[9][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C_"};
    static unsigned int index = 0, key, flag = 0;
    key = read_switches(STATE_CHANGE);
    
    if(flag == 0)
    {
        
        if(key == MK_SW1)
        {
            if(index < 7)
            {
                index++;
            }
        }
        else if(key == MK_SW2)
        {
            if(index > 1)
            {
                index--;
            }
        }
        else if(key == MK_SW3)
        {
            index = 8;
            flag = 1;
        }
    }
    else if(flag == 1)
    {
        if(key == MK_SW1 || key == MK_SW2)
        {
            index = 1;
            flag = 0;
        }
    }
    clcd_print(event[index], LINE2(9));
}
unsigned int flag = 0;
void blackbox(void)
{
    unsigned char key;
    if(flag == 0)
    {
        dashboard();
    }
    else if(flag == 1)
    {
        password();
    }
    else if(flag == 2)
    {
        mainmenu();
    }
    key = read_switches(STATE_CHANGE);
    if(key == MK_SW11)
    {
        flag = 1;
    }      
}
void  dashboard(void)
{
    
    clcd_print("  TIME   EVT SPED", LINE1(0));
    get_time();
    get_event();
    get_speed();
    display();
    
    
}


void password(void)
{
   /* unsigned char attempt;
    clcd_print("                ", LINE1(0));
    for(attempt=0; attempt<MAX_ATTEMPTS; attempt++)
    {
        unsigned char index = 0;
                
        clcd_print((const unsigned char*)"ENTER PASSWORD:", LINE1(0));
        
        // Clear second line
            clcd_print("                ", LINE2(0));


        unsigned char blink_state = 1; // 1=show '_', 0=hide
        unsigned int blink_counter = 0;

        // Read 8 key presses
        while(index < PASSWORD_LENGTH)
        {
            // Blink underscore every 500ms
            if(blink_counter++ >= 25) // 25*20ms ~ 500ms
            {
                blink_counter = 0;
                if(blink_state)
                    clcd_putch(' ', LINE2(index));
                else
                    clcd_putch('_', LINE2(index));
                blink_state = !blink_state;
            }

            unsigned char key = read_switches(STATE_CHANGE);
            if(key == MK_SW1)
            {
                usr_password[index] = '0';
                clcd_putch('*', LINE2(index));
                index++;
                blink_state = 1; // reset underscore
            }
            else if(key == MK_SW2)
            {
                usr_password[index] = '1';
                clcd_putch('*', LINE2(index));
                index++;
                blink_state = 1; // reset underscore
            }

            __delay_ms(20); // small delay for debouncing & blinking
        }

        // Check password
        if(my_strcmp(org_password, usr_password, PASSWORD_LENGTH))
        {
            clcd_print((const unsigned char*)"ACCESS GRANTED", LINE1(0));
            //call main menu
            mainmenu();
            break; // stop attempts
        }
        else
        {
            // Password wrong
            clcd_print((const unsigned char*)"WRONG PASSWORD", LINE1(0));

            // Show remaining attempts
            char attempt_char[2];
            attempt_char[0] = (MAX_ATTEMPTS - (attempt + 1)) + '0';
            attempt_char[1] = '\0';

            clcd_print((const unsigned char*)"ATTEMPTS LEFT:", LINE2(0));
            clcd_print((const unsigned char*)attempt_char, LINE2(14));

            delay_ms(1000);
        }
    }
    // If all attempts used
    if(attempt == MAX_ATTEMPTS)
    {
        clcd_print((const unsigned char*)"ACCESS DENIED!", LINE1(0));
        clcd_print((const unsigned char*)"BLOCKED FOR 10 SECOND", LINE2(0));
    }*/

    unsigned char attempt;

    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));

    for (attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
    {
        unsigned char index = 0;
        unsigned int blink_counter = 0;
        unsigned char blink_state = 1; // 1=show '_', 0=hide

        clcd_print("ENTER PASSWORD:", LINE1(0));
        clcd_print("                ", LINE2(0));

        // Read PASSWORD_LENGTH key presses
        while (index < PASSWORD_LENGTH-1)
        {
            // Blink underscore every ~500ms
            if (blink_counter++ >= 25)
            {
                blink_counter = 0;
                clcd_putch(blink_state ? ' ' : '_', LINE2(index));
                blink_state = !blink_state;
            }

            unsigned char key = read_switches(STATE_CHANGE);

            if (key == MK_SW1)
            {
                usr_password[index] = '0';
                clcd_putch('*', LINE2(index));
                index++;
                blink_state = 1;
            }
            else if (key == MK_SW2)
            {
                usr_password[index] = '1';
                clcd_putch('*', LINE2(index));
                index++;
                blink_state = 1;
            }

            __delay_ms(20); // debounce and blink timing
        }

        usr_password[PASSWORD_LENGTH] = '\0'; // null terminate

        // Check password
        if (my_strcmp(org_password, usr_password, PASSWORD_LENGTH))
        {
            clcd_print("ACCESS GRANTED ", LINE1(0));
            clcd_print("                ", LINE2(0));
            delay_ms(1000);
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
           // mainmenu(); // call main menu after success
            flag = 2;
            return;
        }
        else
        {
            // Wrong password
            clcd_print("WRONG PASSWORD ", LINE1(0));
            clcd_print("ATTEMPTS LEFT: ", LINE2(0));

           char attempt_char[2];
            attempt_char[0] = (MAX_ATTEMPTS - (attempt + 1)) + '0'; // Convert number to ASCII
            attempt_char[1] = '\0';
            clcd_print(attempt_char, LINE2(14));


            delay_ms(1000);
        }
    }

    // If all attempts used
    clcd_print("ACCESS DENIED! ", LINE1(0));
    clcd_print("BLOCKED 10 SEC ", LINE2(0));
    unsigned int j = 9;
    for (unsigned int i = 0; i < 10000; i++) // simple blocking delay (~10 sec)
    {
            clcd_print("BLOCKED   SEC ", LINE2(0));
            clcd_putch(j + 48, LINE2(8));
            j--;
        __delay_ms(1);
    }

}

unsigned int ind = 0, flag1 = 0;

void mainmenu(void)
{
    const unsigned char menu[5][15] = {
        "VIEW LOG",
        "CLEAR LOG",
        "DOWNLOAD LOG",
        "SET TIME",
        "CHANGE PASSWORD"
    };

    unsigned char key;

    while (1)
{
    // Clear LCD
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));

    // Print two consecutive menu items
    clcd_print(menu[ind], LINE1(3));
    clcd_print(menu[ind + 1], LINE2(3));

    // Show arrow based on flag1 (0 = top line selected, 1 = bottom line selected)
    clcd_putch(' ', LINE1(0));
    clcd_putch(' ', LINE2(0));
    if (flag1 == 0)
        clcd_putch('>', LINE1(0));
    else
        clcd_putch('>', LINE2(0));

    // Read key
    key = read_switches(STATE_CHANGE);

    if (key == MK_SW1) // Scroll Down
    {
        if (flag1 == 0)           // If arrow is at top line
        {
            flag1 = 1;            // Move arrow to second line
        }
        else                      // Arrow already at second line
        {
            if (ind < 3)          // Scroll down (till second-last item)
                ind++;
        }
    }
    else if (key == MK_SW2) // Scroll Up
    {
        if (flag1 == 1)           // If arrow is at bottom line
        {
            flag1 = 0;            // Move arrow up
        }
        else                      // Arrow already at top line
        {
            if (ind > 0)          // Scroll up (till first item)
                ind--;
        }
    }


      /*  else if (key == MK_SW3) // Select current option
        {
            // Call functions based on selected menu item
            switch (ind)
            {
                case 0:
                    view_log();
                    break;
                case 1:
                    clear_log();
                    break;
                case 2:
                    download_log();
                    break;
                case 3:
                    set_time();
                    break;
                case 4:
                    change_password();
                    break;
            }
        }*/

        __delay_ms(150); // debounce + LCD refresh delay
    }
}
