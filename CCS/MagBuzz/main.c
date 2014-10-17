#include <msp430xG46x.h>
/*
 * main.c
 */

int count = 1;
const int MAX_COUNT = 17;
const int MCLK_freq = 1048576; // ��
//��������� ���������� ������ �� ����������
const int ((MCLK_freq/697) +1 )/2; // ������ ������� DTMF 697 ��
const int ((MCLK_freq/770) +1 )/2; // ������ ������� DTMF 770 ��
const int ((MCLK_freq/852) +1 )/2; // ������ ������� DTMF 852 ��
const int ((MCLK_freq/941) +1 )/2; // ������ ������� DTMF 941 ��

const int ((MCLK_freq/1209) +1 )/2; // ������� ������� DTMF 1209 ��
const int ((MCLK_freq/1336) +1 )/2; // ������� ������� DTMF 1336 ��
const int ((MCLK_freq/1477) +1 )/2; // ������� ������� DTMF 1477 ��
const int ((MCLK_freq/1633) +1 )/2; // ������� ������� DTMF 1633 ��

//******************************************************************
// Port1 Interrupt Service Rotine
//******************************************************************
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	if (P1IFG == 0x01)                        // SW1 interrupt
	{
		//������������ ������� 1 ������

		if (count == MAX_COUNT)
		{
			count = 0;
		}
		else
		{
			count++;
		}
	}

	if (P1IFG == 0x02)                        // SW2 interrupt
	{
		//������������ ������� 2 ������
	}

	P1IFG = 0x00;	                             // clean all pending interrupts
}

//******************************************************************
// Port1 Interrupt Service Rotine
//******************************************************************


int main(void)
{

	volatile unsigned int i;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	FLL_CTL0 |= XCAP14PF;       // Configure load caps

	// Wait for xtal to stabilize
	do
	{
		IFG1 &= ~OFIFG;      // Clear OSCFault flag
		for (i = 0x47FF; i > 0; i--)
			;   // Time for flag to set
	}
	while ((IFG1 & OFIFG)); // OSCFault flag still set?

	/*****************************************************/
	/*������������ ������*/
	/*****************************************************/
	/* SW1 and SW2 ports configuration
	 * ��� ������������ ������. ������������ ����������
	 */
	P1SEL &= ~0x03; // P1.0 and P1.1 I/O ports
	P1DIR &= ~0x03; // P1.0 and P1.1 digital inputs
	P1IFG = 0x00; // clear all interrupts pending
	P1IE |= 0x03; // enable port interrupts

	/*****************************************************/
	/*������������ ������*/
	/*****************************************************/

	/*****************************************************/
	/*������������ �������*/
	/*****************************************************/
	//������������� ����
	P1DIR |= 0x04; // P1.2 output
	P1SEL |= 0x04; // P1.2 TA1 option
	P2DIR |= 0x01; // P2.0 output
	P2SEL |= 0x01; // P2.0 TA2 option

	TACCTL1 = OUTMOD_4 + CCIE; // TACCR1 toggle, interrupt enabled
	TACCTL2 = OUTMOD_4 + CCIE; // TACCR2 toggle, iterrupt enabled
	TACTL = TASSEL_2 + MC_2 + TAIE; // MCLK, countmode, interrupt enabled
	/*****************************************************/
	/*������������ �������*/
	/*****************************************************/

	/*****************************************************/
	/*������������ ��*/
	/*****************************************************/
	OA0CTL0 = OAN_1 + OAPM_1; // Select inputs, power mode
	OACTL1 = OARRIP;
	/*****************************************************/
	/*������������ ��*/
	/*****************************************************/

	_BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/interrupt
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
  switch( TAIV )
  {
  case  2: TACCR1 += 16;                    // Add Offset to TACCR1
           break;
  case  4: TACCR2 += 100;                   // Add Offset to TACCR2
           break;
  case 10: P1OUT ^= 0x08;                   // Timer_A3 overflow
           break;
 }
}
