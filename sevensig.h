#ifndef sevensig_h
#define sevensig_h

#include <avr/io.h>


// 0 ������ �����
// 1 ������
// 2 �����
// 3 ������ ������
// 4 �����������
// 5 ������� �����
// 6 �������
// 7 ������� ������

#define WDP_mask 0b110

#define _DIODE_2H_ON {PORTC &=B11001110; PORTC |=B00010000;} // 40\60
#define _DIODE_AUTO_ON {PORTC &=B11001110; PORTC |=B00000001;} //4H
#define _DIODE_4H_ON {PORTC &=B11001110; PORTC |=B00100000;} //2H

#define DIODE_ALL_OFF {PORTC &=B11001110; } 
#define DIODE_ALL_ON {PORTC |=B00110001;}

#define _4H_ON PORTB |=B00100000 
#define _4H_OFF PORTB &= B11011111 
#define ActivatePin 10

#define SEG_1 PORTB &= B11110111 //3 seg
#define SEG_2 PORTB &= B11111101 //2 seg
#define SEG_3 PORTB &= B11111110 //4 seg
#define SEG_4 PORTB &= B10111111 //1 seg
#define SEG_OFF {PORTB |= B01001011; PORTD &= B00000011; PORTC &= B11110011;}// �������� ��� ��������

#define L_A B11111000
#define L_A_c B00001000
#define L_u B00001000
#define L_u_c B00001100
#define L_t B00110000
#define L_t_c B00001100
#define L_o B00011000
#define L_o_c B00001100

#define L_S B01111000
#define L_S_c B00000100
#define L_P B11110000
#define L_P_c B00001000

#define L_C B01100000
#define L_C_c B00001100

#define L_L B00100000
#define L_L_c B00001100

#define L_c B00010000
#define L_c_c B00001100


#define L_empty B00000000
#define L_empty_c B00000000
#define L_defis B00010000
#define L_defis_c B00000000
#define L_defis_point B00010100
#define L_defis_point_c B00000000
#define L_O B11101000
#define L_O_c B00001100
#define L_O_point B11101100
#define L_O_point_c B00001100
#define L_1 B10001000
#define L_1_c B00000000
#define L_1_point B10001100
#define L_1_point_c B00000000
#define L_2 B11010000
#define L_2_c B00001100
#define L_2_point B11010100
#define L_2_point_c B00001100
#define L_3 B11011000
#define L_3_c B00000100
#define L_3_point B11011100
#define L_3_point_c B00000100
#define L_4 B10111000
#define L_4_c B00000000
#define L_4_point B10111100
#define L_4_point_c B00000000
#define L_5 B01111000
#define L_5_c B00000100
#define L_5_point B01111100
#define L_5_point_c B00000100
#define L_6 B01111000
#define L_6_c B00001100
#define L_6_point B01111100
#define L_6_point_c B00001100
#define L_7 B11001000
#define L_7_c B00000000
#define L_7_point B11001100
#define L_7_point_c B00000000
#define L_8 B11111000
#define L_8_c B00001100
#define L_8_point B11111100
#define L_8_point_c B00001100
#define L_9 B11111000
#define L_9_c B00000100
#define L_9_point B11111100
#define L_9_point_c B00000100
#define L_H B10111000
#define L_H_c B00001000

#endif
