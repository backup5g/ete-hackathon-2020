#include <msp430f2252.h>
#include "cs50.h"
#include <math.h>
#include "lcd.h"

void chooseRoute(int priority, int message, int origin, int destiny);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    InitLCD();
    
    P3OUT = P3OUT &~ BIT7;
    
    P3REN = P3REN | BIT6;
    
    while (1)
    {
      int package[15] = {P1IN&BIT1, P1IN&BIT2, P1IN&BIT3, P1IN&BIT4, P1IN&BIT5, P1IN&BIT6, P1IN&BIT7, P2IN&BIT2, P2IN&BIT5, P2IN&BIT6, P2IN&BIT7, P3IN&BIT1, P3IN&BIT2, P3IN&BIT3, P3IN&BIT4};
      int count = 0;

      if ((P3IN&BIT6) == 0)
      {
        int destiny = 0;
        int origin = 0;
        int priority = P1IN&BIT7;
        int message = 0;
        
        while ((P3IN&BIT6) == 0)
        {
          int power = 0;
          
          for (int i = 2; i >= 0; i--)
          {
              if (package[i] > 0)
              {
                  destiny += pow(2, power);
              }

              if (package[i + 3] > 0)
              {
                  origin += pow(2, power);
              }

              power++;
          }

          power = 0;

          for (int j = 14; j >= 7; j--)
          {
              if (package[j] > 0)
              {
                  message += pow(2, power);
              }

              power++;
          }

          chooseRoute(priority, message, origin, destiny);

          string locals[8] = {"LAB", "SA", "AL", "CF", "SP", "CA", "AU", "QU"};

          LCDPrintXYStr(1, 1, locals[origin]);
          LCDPrintXYStr(15, 1, locals[destiny]);

          string messages[6] = {"LOTADO", "VAZIO", "PERIGO", "CONTAMINADO", "VIGIADO", "SS"};

          LCDPrintXYStr(1, 2, messages[message / 10 - 1]);
        }
     }
      
      if ((P3IN&BIT5) > 0)
      { 
        count += 1;
      }
    }
}

void chooseRoute(int priority, int message, int origin, int destiny)
{
    if (priority == 1 || message == 30)
    {
        LCDPrintXYStr(16, 2, "3");
    }
    else if (message % 10 != 0 || message > 70)
    {
        LCDPrintXYStr(16, 2, "4");
    }
    else if (origin == destiny)
    {
        LCDPrintXYStr(16, 2, "NULL");
    }
    else if (origin % 2 == 0)
    {
        LCDPrintXYStr(16, 2, "1");
    }
    else if(origin % 2 != 0)
    {
        LCDPrintXYStr(16, 2, "2");
    }
}
