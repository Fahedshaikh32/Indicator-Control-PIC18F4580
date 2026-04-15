/*
 * File:   newmain.c
 * Author: Fahed
 *
 * Created on 15 April, 2026, 11:43 PM
 */

#include <xc.h>

// --- Configuration ---
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 20000000

// --- Indicator States ---
#define OFF     0
#define LEFT    1
#define RIGHT   2
#define HAZARD  3

// --- LED Pins ---
#define LEFT_LED1   PORTBbits.RB0
#define LEFT_LED2   PORTBbits.RB1
#define RIGHT_LED1  PORTBbits.RB6
#define RIGHT_LED2  PORTBbits.RB7

// --- Function to read keypad ---
char read_key()
{
    return PORTC & 0x0F;
}

void main()
{
    // Setup: LEDs as output, keypad as input
    TRISB = 0x00;
    TRISC = TRISC | 0x0F;

    // Variables
    int state = OFF;
    int blink = 0;

    int left_cnt  = 0;
    int right_cnt = 0;

    int left_released  = 0;
    int right_released = 0;
    int hazard_timer   = 0;

    char prev_key = 0x0F;
    char key      = 0;

    while(1)
    {
        // Step 1: Read current key
        key = read_key();

        // Step 2: Count how long each button is held
        if(key == 0x0E)
            left_cnt++;
        else
            left_cnt = 0;

        if(key == 0x0D)
            right_cnt++;
        else
            right_cnt = 0;

        // Step 3: Check if left button was just released
        if(prev_key == 0x0E && key != 0x0E)
        {
            left_released = 1;
            hazard_timer  = 0;
        }

        // Step 4: Check if right button was just released
        if(prev_key == 0x0D && key != 0x0D)
        {
            right_released = 1;
            hazard_timer   = 0;
        }

        // Step 5: Save current key for next loop
        prev_key = key;

        // Step 6: Run hazard timer if any button was released
        if(left_released == 1 || right_released == 1)
        {
            hazard_timer++;
        }

        // Step 7: If 1.5 seconds passed with no second button, reset hazard tracking
        if(hazard_timer > 15)
        {
            left_released  = 0;
            right_released = 0;
            hazard_timer   = 0;
        }

        // Step 8: State machine - decide what to do

        // Case 1: Both buttons were pressed one after another = HAZARD
        if(left_released == 1 && right_released == 1)
        {
            if(state == HAZARD)
                state = OFF;
            else
                state = HAZARD;

            // Reset everything
            left_released  = 0;
            right_released = 0;
            hazard_timer   = 0;
            left_cnt       = 0;
            right_cnt      = 0;
        }

        // Case 2: Left button held for 1 second = LEFT indicator
        else if(left_cnt >= 10 && right_released == 0)
        {
            if(state == LEFT)
                state = OFF;
            else
                state = LEFT;

            left_cnt      = 0;
            left_released = 0;
        }

        // Case 3: Right button held for 1 second = RIGHT indicator
        else if(right_cnt >= 10 && left_released == 0)
        {
            if(state == RIGHT)
                state = OFF;
            else
                state = RIGHT;

            right_cnt      = 0;
            right_released = 0;
        }

        // Step 9: Blink toggle (every 100ms this flips 0->1 or 1->0)
        blink = !blink;

        // Step 10: Control LEDs based on current state
        switch(state)
        {
            case OFF:
                LEFT_LED1  = 0;
                LEFT_LED2  = 0;
                RIGHT_LED1 = 0;
                RIGHT_LED2 = 0;
                break;

            case LEFT:
                LEFT_LED1  = blink;
                LEFT_LED2  = blink;
                RIGHT_LED1 = 0;
                RIGHT_LED2 = 0;
                break;

            case RIGHT:
                RIGHT_LED1 = blink;
                RIGHT_LED2 = blink;
                LEFT_LED1  = 0;
                LEFT_LED2  = 0;
                break;

            case HAZARD:
                LEFT_LED1  = blink;
                LEFT_LED2  = blink;
                RIGHT_LED1 = blink;
                RIGHT_LED2 = blink;
                break;
        }

        // Step 11: Wait 100ms before next loop (this makes each count = 100ms)
        __delay_ms(100);
    }
}