/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lasablon <lasablon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:57:16 by lasablon          #+#    #+#             */
/*   Updated: 2025/03/15 13:46:17 by lasablon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
#define UART_H
#include <avr/io.h>

char uart_rx(void);
void uart_tx(char c);
void uart_printstr(const char *str);
void uart_printstr_endl(const char *str);
void uart_init(void);
void ft_putnbr(int16_t nbr);
void print_hex_value(char c);
#endif
