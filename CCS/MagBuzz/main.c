#include <msp430xG46x.h>
/*
 * main.c
 */

	//������� �������
	const int P1 = 27; //��� ������� 1209 ��
	const int P2 = 24; //��� ������� 1336 ��
	const int P3 = 22; //��� ������� 1477 ��
	const int P4 = 20; //��� ������� 1633 ��

	/*��� ���� ��� �� ��� 50% ������� ����*/
	const int D1 = 13;
	const int D2 = 12;
	const int D3 = 11;
	const int D4 = 10;

	//������ �������
	const int P5 = 47; //��� ������� 697 ��
	const int P6 = 42; //��� ������� 770 ��
	const int P7 = 38; //��� ������� 852 ��
	const int P8 = 34; //��� ������� 941 ��

	/*��� ���� ��� �� ��� 50% ������� ����*/
	const int D5 = 23;
	const int D6 = 21;
	const int D7 = 19;
	const int D8 = 17;


	//******************************************************************
	// Port1 Interrupt Service Rotine
	//******************************************************************
	#pragma vector=PORT1_VECTOR
	__interrupt void PORT1_ISR (void)
	{
	  if (P1IFG == 0x01)                        // SW1 interrupt
	  {
		//������������ ������� 1 ������
	  }

	  if (P1IFG == 0x02)                        // SW2 interrupt
	  {
		  //������������ ������� 2 ������
	  }

	  P1IFG = 0x00;	                             // clean all pending interrupts
	}

int main(void) {

	volatile unsigned int i;

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    FLL_CTL0 |= XCAP14PF;       // Configure load caps
	
    // Wait for xtal to stabilize
    do
    {
    	IFG1 &= ~OFIFG;      // Clear OSCFault flag
    	for (i = 0x47FF; i > 0; i--);   // Time for flag to set
    }
    while ((IFG1 & OFIFG)); // OSCFault flag still set?

      /* SW1 and SW2 ports configuration
       * ��� ������������ ������. ������������ ����������
       */
      P1SEL &= ~0x03;                                       // P1.0 and P1.1 I/O ports
      P1DIR &= ~0x03;                                       // P1.0 and P1.1 digital inputs
      P1IFG = 0x00;                                         // clear all interrupts pending
      P1IE |= 0x03;                                         // enable port interrupts

    //������������� ����
    //P1DIR |= 0x04; // P1.2 output
    //P1SEL |= 0x04; // P1.2 TA1 option


	return 0;
}
