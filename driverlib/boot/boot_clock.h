#ifndef BOOT_CLOCK_H
#define BOOT_CLOCK_H


#define MCLK_FREQ                16000000            //MCLK=16Mhz

#define delay_us(x)  __delay_cycles((long)(MCLK_FREQ*(long)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(MCLK_FREQ*(long)x/1000.0))
#define delay_sec(x) __delay_cycles((long)(MCLK_FREQ*(long)x))

void CloseWatchDog(void);
void InitClock(void);
void InitSystick(void);

#endif
