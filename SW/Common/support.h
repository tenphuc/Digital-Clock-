/*
 * support.h
 *
 *  Created on: Nov 20, 2025
 *      Author: Windows
 */

#ifndef SRC_SUPPORT_H_
#define SRC_SUPPORT_H_


void str_empty(char str[]);
int len_str(char str[]);
int find_str(char str1[], char str2[]);
int find_strL(char str1[], char str2[]);
void Concatstr(char str1[], char str2[]);
void int2char(int num, char str[]);
int char2int(char str[]);
void ftoa_1(float value, char *buf);
#endif /* SRC_SUPPORT_H_ */
