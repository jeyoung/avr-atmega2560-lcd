#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LCD PORTB
#define CTL PORTH

#define RS PH5
#define EN PH6

void delay_ms(uint16_t);

void send_cmd(unsigned char cmd)
{
    CTL &= ~(1 << RS);

    LCD = (LCD & 0x0F) | (cmd & 0xF0);
    CTL |= (1 << EN);
    _delay_us(2);
    CTL &= ~(1 << EN);

    LCD = (LCD & 0x0F) | (cmd << 4);
    CTL |= (1 << EN);
    _delay_us(2);
    CTL &= ~(1 << EN);
}

void send_byte(unsigned char byte)
{
    CTL |= (1 << RS);

    LCD = (LCD & 0x0F) | (byte & 0xF0);
    CTL |= (1 << EN);
    _delay_us(2);
    CTL &= ~(1 << EN);

    LCD = (LCD & 0x0F) | (byte << 4);
    CTL |= (1 << EN);
    _delay_us(2);
    CTL &= ~(1 << EN);
}

void send_str(const char *s)
{
    while (*s) {
        send_byte(*s++);
    }
    _delay_us(40);
}

int main(void)
{
    // Set PB4..PB7 as output
    DDRB |= (0xFF << 4);
    // Set PH5 and PH6 as output
    DDRH |= (1 << PH6) | (1 << PH5);

    delay_ms(50);

    // Initialise LCD and set to 4-bit data mode
    send_cmd(0x30);
    delay_ms(2);

    send_cmd(0x28);
    _delay_us(40);

    send_cmd(0x0C);
    _delay_us(40);

    send_cmd(0x01);
    delay_ms(2);

    send_cmd(0x02);
    delay_ms(2);

    // send a byte
    // send_str("Hello, world!");

    int i = 0;
    char s[1024];
    while (1) {
	send_cmd(0x02);
	delay_ms(2);

        delay_ms(1000);
	sprintf(s, "Counter: %d", ++i);
	send_str(s);
    }
}

void delay_ms(uint16_t ms)
{
    while (--ms > 0)
        _delay_ms(1);
}
