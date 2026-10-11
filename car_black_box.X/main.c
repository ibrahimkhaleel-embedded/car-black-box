/*
 * File:   main.c
 * Author: USER
 *
 * Created on 11 November, 2025, 7:34 AM
 */


#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "ds1307.h"
#include "at24c04.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "main.h"


void init_config(void)
{
    init_matrix_keypad();
    init_i2c();
    init_ds1307();
    init_clcd();
    init_adc();
}

void main(void) 
{
    init_config();
    while(1)
    {
      // dashboard();
        blackbox();
    }
    return;
}
