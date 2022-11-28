/*
 * helper.h
 *
 *  Created on: 27 Oct 2022
 *      Author: jiahao
 */

#ifndef HELPER_H_
#define HELPER_H_

// ================================================================================
// Helper macros and helper function forward declarations
// ================================================================================

#define RED     (0x01)
#define GREEN   (0x02)
#define BLUE    (0x04)

#define LOW     (uint8_t) 0x00
#define HIGH    (uint8_t) 0x01

void Switch1_init(void);
bool is_Switch1_Pressed(void);
void Switch1_interruptEnable(uint8_t transition);

void Switch2_init(void);
void Switch2_interruptEnable(uint8_t transition);
bool is_Switch2_Pressed(void);

void LED1_init(void);
void LED1_on(void);
void LED1_off(void);
void LED1_toggle(void);

void LED2_init(uint8_t colour);
void LED2_on(uint8_t colour);
void LED2_off(uint8_t colour);
void LED2_toggle(uint8_t colour);

#endif /* HELPER_H_ */
