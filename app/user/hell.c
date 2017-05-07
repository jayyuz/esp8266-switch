///*************************************************************************************
// �ֵ����
// ʹ��PIC��Ƭ��PIC16F1824,�ڲ�1024bit FLSAH  64bit SRAM 128bit EEPROM 6��I/O
// GP3��������λ��GP5�������LED�Ƿ���룬GP2�����������������飬GP0���PWM����
// GP4����״ָ̬ʾ��GP1���ڿ���ICʹ�ܶ�
// �ڿ���״̬�µ���S1��������λΪ4��������˸ģʽ�µ�������λ��3��
// ����״̬5�����ֱ��Ǽ�����2500MA),���ܵ���3����ᣩ��������150MA)������(450MA)��������1500MA),����Ĭ��Ϊ��
// ��˸ģʽ��Ƶ������15HZ/6HZ 2S,2.5A)
// �����⵵����0.S�����õ�λ,ֻ��������ģʽ�ĵ�λ
// ��·�����ѹ��Χ3.0-4.2V,������ѹ����
// *************************************************************************************/
///*************************************************************************************
// Project:                      pd40
// Chip type:                     PIC16F1824
// Clock frequency:               INTERNATE 4MHZ
// data;                          20160413
// Change data:                   20160413
// recode :      cr40.genggaiweiPD40YONG
// recode :      //���ĺò�����ʽ��˫��������ʾ��20150828��0429����AD���ʱ��
// *************************************************************************************/
////����ͷ�ļ�*************************************************************************
//#include <pic.h>
//
////����оƬ����������λ
//__CONFIG(0x2A04);//(0x2C04)
//__CONFIG(0x14FF);//(0x1EFF)�ڲ�RC������WDTE =0,�ϵ���ʱ��ʱ��ʹ��;MCLR����ͨI/O;Ƿѹ���ʹ��
//
////��λ����**********************************************************************
//
//#define	  		ADV					RA0				//AD������
//#define			AD_SW				RC4				//AD MOS ���ƿ�
//#define 	    LED_ON1				RC3				//LED1ʹ�ܿ��ƶ�
//#define			SW_CH				RA2				//sw2
//#define			SW_OF				RA1				//SW1
//#define			OTP					RA3				//�¶ȼ������
//#define			DIS2				RC1				//������ʾһ rled
//#define			DIS1				RC2				//������ʾ�� gled
//#define			PWM_OUT				RC5				//PWM����˿�
//#define       CHARGE_STATE 			RC0             //**���״̬??????????
//#define       CHARGE_INPUT			RA5             //����������???????????????
//
//#define			Jie  	   2				//���ܵ�30MA
//#define			Ji	      216		   //4.8A
//#define			Gao	   		66				//1500mA 69
//#define			Zhong	   16	         //420mA*************?
//#define			Di   	   6       		//150mA			������
//#define       unchar           			 unsigned char
//#define       unint            			 unsigned int
//#define      key_mask           0b00000110			//Ra1 Ra2Ϊ��������� ��������IO*****0b00011000
//#define      key_mask           0b00000110			//Ra1 Ra2Ϊ��������� ��������IO*****0b00011000
//#define      key_state_0            0                        //������ʼ̬
//#define      key_state_1            1                        //������һ�ΰ���ȷ��̬
//#define      key_state_2            2                        //����̧��ȷ��̬
//#define      key_state_3            3                        //����̧��ȷ��̬
//#define      key_state_4            4                        //����̧��ȷ��̬
//#define      key_state_5            5                        //����̧��ȷ��̬
//#define      key_state_6            6                        //����̧��ȷ��̬
//#define      key_state_7            7                        //����̧��ȷ��̬
//#define      key_state_8            8                        //����̧��ȷ��̬
//#define      key_state_9            9                        //����̧��ȷ��̬
//static char key_state = 0;
//unchar stime = 0;                        //2ms��ʱ������żĴ���
//unchar POWER_ON_FLAG = 0;                        //��Ƭ������״̬�Ĵ���
//unchar BAT = 0;				   						       //������ʾ�Ĵ���
//unint count_temp1 = 0;                        //  ������ʱ������Ĵ���
//unint count_temp2 = 0;                        //  ������ʱ�����Ĵ���
//unint count_temp3 = 0;                        //  ��������ͬʱ���£�ʱ�����Ĵ���
//unchar bright_time = 0;   					        //  ������ʱ
//unint address1 = 0x0010;                  //��λ�����ַ
//unint address2 = 0x0020;                  //��λ�����ַ
//unchar temp1 = 0;                        //������λ������ʱ��żĴ���
//unchar temp2 = 0;                        //������λ������ʱ��żĴ���
//unchar DELAY_02S = 0;							       //0.2ms delay
//unchar key_number = 0;                        //����ֵ��żĴ���
//unchar data_temp = 8;                        //PWM�����洢�Ĵ���
//unint dalay_time_10ms = 0;                        //10mS��ɴ����Ĵ���
//unint lock_display_time = 0;							//��������ʾ
//unchar sleep_flag = 0;
//unchar data1 = 0;
//unchar data2 = 0;
//unint adresult = 0;
//unchar VOL_FLAG = 0;
//unchar Double_flag = 0;
//bit LOCK = 0;				//Ĭ�ϲ�������ÿ�������ϵ��ʾ������������ʱ��LOCK==1
//unchar number1 = 0;		    //
//unchar PWM_DATA = 0;         //pwmռ�ձȳ�ֵ
//unint AUTO_TEMP_TIME = 0;   //���ܽ���ʱ��
//unchar SOS_H[] = { 34, 34, 34, 100, 100, 100, 34, 34, 34 }; //SOS�ߵ�ƽʱ��(ms)   |--340-340-340-1000-1000-1000-340-340-340--|
//unchar SOS_L[] = { 33, 33, 200, 33, 33, 200, 33, 33, 233 }; //SOS�͵�ƽʱ��(ms)   |--330-330-2000-330-330-2000-330-330-2330--|
//unchar SOS1 = 0;
//unchar SOS2 = 0;
//unchar SOS_DATA = 0;                        //SOS�ڲ�����Ĵ���
//unchar FC_FALSH_DATA = 0;
//unchar data_temp1 = 1;                        //��˸��λPWM������ʱ�洢�Ĵ���
//unchar data_temp2 = 0;                        //��˸��λPWM������ʱ�洢�Ĵ���
//unchar temp3 = 0;                        //˫����ɱ�ʾ��ʱ��żĴ���
//unint dalay_time_1ms = 0;                        //1mS��ɴ����Ĵ���
//unchar BAT_VO_CH = 0;
//unchar FC_FALSH_FLAG = 0;                        //��������״̬�Ĵ���
//unchar Input_voltage_low = 0;
//unchar temp5 = 254;                        //��λ��λ��ʱ��żĴ���
//unchar stop_run_flash = 0;				//�ر�LEDʱ��ֹͣ���б���
//unchar MIRCOR = 0;                       //��������
//unchar STOP_RUN_FLASH = 0;			//�ر�LEDʱ��ֹͣ���б���
//unchar CHARGE_PUTIN_FLAG = 0;			// //��������������
//unchar CHARGE_DONE_FLAG = 0;			//�����������״̬���
//unchar DISPLAY_ELE = 0;
//unint display_time = 0;
//unchar time_2s = 0;
//unint time_10s = 0;
//unint time_2min = 0;
//unint time_3min = 0;			//
//unchar time_2min_flag = 0;
//unchar time_3_min_flag = 0;
//
//unchar DOWN_FLAG = 0;				//����λ��������־λ
//unchar DOWN_ELE = 0;				//������ѹλ
//
//unchar REMENBER_data_temp1 = 0;		//
//unchar BAOSHAN_JIANDANG = 0;			//����������־
//unchar add_flag = 0;					//������ˣ��ͱ�־
//unchar RESET_LOCK = 0;				//
//unint RESET_ELE = 0;				//�ݵ�ѹ��λ��־��λ���
//unchar CHAR_B = 0;
//unchar CHAR_C = 0;
//unint persent_10_time = 0;
//
//unchar input_flag1 = 0;
////	unint  persent_10_time=0;
//unchar Ji_down = 0;			//
//unchar AD_FLAG = 0;			//
//unchar AD_FLAG1 = 0;
//unchar mode_return_flag = 0;  //����ʱ��ģʽ���ر�־
//unchar DIS_FLAG = 0;			//
//unchar DIS_TIME = 0;			//
//unchar DIS_DELAY_500ms = 0;	//
//unchar delay1s = 0;			//
//unint SLEEP_TIME = 0;		//
//
////*******************************************************************
////�������ƣ�CUP_Init();
////�����������
////�����������
////�����������˿�����
//
////********************************************************************
//void CUP_Init(void) {
//	TRISA = 0b00101111;           //��ʼ��GPIO  A1 A2���������**0b00111010;
//	TRISC = 0b00000001; 				//****00000000.RC0Ϊ���״̬����
//
//	OPTION_REG = 0b00000011;           //������ʹ���ø�,T0�ڲ�ʱ��Դ��16��Ƶ
//
//	WPUA = 0b00000110;           //RA5 00000000**1.2����������
//	WPUC = 0b00000000;             //RC ������
//
//	ANSELA = 0b00000001;             //��ra1����ȫ��Ϊ����IO��*********
//	ANSELC = 0b00000000;             //����RC1Ϊģ�������,����λEER**����IO��	**********
//
//	IOCAP = 0b00000000;             //��ֹRA0,RA2���������ص�ƽ�仯�ж�
//	IOCAN = 0b00000000;             //��ֹRA0,RA2���Ÿ����ص�ƽ�仯�ж�
//	IOCAF = 0b00000000;             //��ƽ�仯�ж�PORTA��־λ
//
//	INTCON = 0b11101000;           //GIE = 1;INTE =0;PEIE=1;T0IE=1;GPIE=0;
//	OSCCON = 0b01101010;				//4MHZ,ʹ���ڲ�ʱ��0b01101011;
//	TMR0 = 0X7C;                 //
//
//	AD_SW = 1;
//
//	BORCON = 0B100000001;
//
//	PORTA = 0b00000000;				//RA ȫ��L
//	PORTC = 0b00000000;				//RC ȫ��L
//	LATA = 0b00000000;
//	LATC = 0b00000000;
//	CM1CON0 = 0b00000000;             //ȫ����Ϊ��ͨIO�ڣ��Ƚ����ر�
//	CM2CON0 = 0b00000000;             //ȫ����Ϊ��ͨIO�ڣ��Ƚ����ر�
//}
//
////*******************************************************************
////�������ƣ�data_write();
////�����������
////�����������
////����������EEPROMд �ӳ��򣬽��ı��ĵ�λ��ʾд��EEPROM��
////�������ڣ�20150112
////********************************************************************
//void data_write(void) {
//	if ((temp1 != data1))                      //�������Ƿ��е�λ�任
//	{
//		eeprom_write(address1, data1);         //����������е�λ�任�������
//		temp1 = data1;                       //���µ�λ��żĴ���
//
//	}
//}
////*******************************************************************
////�������ƣ�data_read();
////�����������
////�����������
////������������EEPROM�ӳ���
////�������ڣ�20150112
////********************************************************************
//void data_read(void) {
//	data1 = EEPROM_READ(address1);    //��EEPROM
//	if (data1 >= 5)
//		data1 = 0;                       //���������״�Ϊ����
//	temp1 = data1;
//
//}
//
////*******************************************************************
////�������ƣ�key_scan_on
////�����������
////�������������״̬��0Ϊ�����壬3Ϊ70ms���壬4Ϊ150MS����
////��������������ɨ�����
////�������ڣ�2010.07.24
////�޸����ڣ�2010.12.30
////*******************************************************************
//
//unchar Key_scan_ON(void) {
//	unchar key_press = 0;                      //����ɨ���żĴ���
//	unchar key_return = 0;                   //��������ֵ��0Ϊ�ް�����1ΪS1������2ΪS2������3ΪS1˫��
//	key_press = PORTA & key_mask;              //��GP1��������IO��
//	switch (key_state) {
//
//	case key_state_0:
//		if ((key_press != key_mask) && (SW_OF == 0) && (SW_CH == 1))  //����������  2
//			key_state = key_state_1;
//		if ((key_press != key_mask) && (SW_CH == 0) && (SW_OF == 1))  //����������  1
//			key_state = key_state_4;
//		if ((PORTA & key_mask) == 0)    //��������ͬʱ���£���ʱ����
//				{
//			key_state = key_state_7; //��ת��ȷ��״̬��ȷ���Ƿ�������
//		} else {
//			count_temp1 = 0;
//			count_temp2 = 0;
//			count_temp3 = 0;
//		}
//		break;
//	case key_state_1:
//		count_temp1++;	//�����ж�
//		if ((PORTA & key_mask) == key_mask)  //�������������̧����
//		{
//			key_return = 1;
//			key_state = key_state_0;
//			count_temp1 = 0;
//
//		}
//		if (((PORTA & key_mask) != 0) && (SW_OF == 0))  //��ʾ����������,������������
//				{
//			if (count_temp1 > 50)       //>0.5s
//				key_state = key_state_2;   //���³���0.5s  ��ת�������ͷ�״̬�ж�
//		} else    //�����������£�����ת��˫����ģʽ
//		{
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7;  //��ת��ȷ��״̬��ȷ���Ƿ�������
//		}
//
//		break;
//	case key_state_2:
//		if (count_temp1 < 254)
//			count_temp1++;
//		if ((PORTA & key_mask) == key_mask)  //�������������̧����
//		{
//			count_temp1 = 0;
//			key_state = key_state_0;
//		}
//		if (((PORTA & key_mask) != 0) && (SW_OF == 0))  //��ʾ����������  ��û�еڶ�����������
//				{
//			if (count_temp1 > 50)       //>0.5s
//					{
//				count_temp1 = 0;
//				key_return = 2;
//				key_state = key_state_3; //���³���0.5s  ��ת
//			}
//		} else if (key_press == 0) //�����������£�����ת��˫����ģʽ
//				{
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7; //��ת��ȷ��״̬��ȷ���Ƿ�������
//		}
//		break;
//	case key_state_3:
//		if ((PORTA & key_mask) == key_mask)  //�������������̧����
//		{
//			key_return = 4;  						 //����������0.5s �ػ�������ֵΪ4������ػ�
//			key_state = key_state_0;
//			count_temp1 = 0;
//			count_temp3 = 0;
//		} else if ((PORTA & key_mask) == 0) //�����������£�����ת��˫����ģʽ
//				{
//			if (count_temp3 <= 299)
//				count_temp3++;
//			if (count_temp1 <= 299)
//				count_temp1++;
//			if (count_temp1 >= 289) {
//				count_temp1 = 0;
//				key_state = key_state_7; 					//��ת��ȷ��״̬��ȷ���Ƿ�������
//			}
//		}
//		break;
//
//	case key_state_4:
//
//		if (count_temp2 <= 254)
//			count_temp2++;
//		if (((PORTA & key_mask) == key_mask) && (count_temp2 >= 2)) //�������������̧����
//				{
//			key_return = 6;
//			key_state = key_state_0;
//			count_temp2 = 0;
//		}
//		if (((PORTA & key_mask) != 0) && (SW_CH == 0))  //��ʾfu��������,������������
//				{
//			if (count_temp2 > 50)       //>0.5s
//				key_state = key_state_5;   //���³���0.5s  ��ת�������ͷ�״̬�ж�
//		} else if ((key_mask & PORTA) == 0)   //�����������£�����ת��˫����ģʽ
//				{
//			count_temp2 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7;  //��ת��ȷ��״̬��ȷ���Ƿ�������
//		}
//		break;
//
//	case key_state_5:												//�����������жϳ���
//
//		if (count_temp2 <= 254)
//			count_temp2++;
//		if ((PORTA & key_mask) == key_mask) //������������ɿ� ��
//		{
//			if (count_temp2 < 50)
//				key_return = 6;
//			else
//				key_return = 7;
//			key_state = key_state_0;
//			count_temp2 = 0;
//		}
//		if ((SW_CH == 0) && (count_temp2 >= 50)) {
//			key_return = 7;  									//����������0.8�룬����ֵ7
//			key_state = key_state_6;
//			count_temp2 = 0;
//		} else if (key_press == 0)   //�����������£�����ת��˫����ģʽ
//				{
//			count_temp2 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7; 						//��ת��ȷ��״̬��ȷ���Ƿ�������
//		}
//		break;
//	case key_state_6:
//		if (key_press == key_mask) 							//����������ɿ����򷵻�ֵΪ5����λ
//		{
//			key_return = 9;  						//����������0.5s �ػ�������ֵΪ4������ػ�
//			key_state = key_state_0;
//			count_temp2 = 0;
//			count_temp3 = 0;
//		} else if (key_press == 0)   //�����������£�����ת��˫����ģʽ
//				{
//			if (count_temp3 <= 299)
//				count_temp3++;
//			if (count_temp2 <= 299)
//				count_temp2++;
//			if (count_temp2 >= 289) {
//				count_temp2 = 0;
//				key_state = key_state_7; 					//��ת��ȷ��״̬��ȷ���Ƿ�������
//			}
//		}
//		break;           //���������������ж�����ɿ������ص�ָ����״̬//////////////////////////////
//
//	case key_state_7:
//		if (key_press == ((PORTA & key_mask)) && (SW_OF == 0) && (SW_CH == 0)) //�����������ȶ�״̬��0000 0000����0001 1000��
//				{
//			key_state = key_state_8;								//�ȶ�״̬ȷ�ϣ���ʱ
//			count_temp1 = 0;
//			count_temp2 = 0;
//		} else
//			key_state = key_state_0;
//		break;
//	case key_state_8:
//
//		if (count_temp3 <= 299)
//			count_temp3++;
//		if (((key_press == key_mask) && (SW_OF == 1) && (SW_CH == 1))
//				|| ((SW_OF == 0) && (SW_CH == 1))
//				|| ((SW_OF == 1) && (SW_CH == 0)))       				//������̧��
//				{
//
//			key_return = 0;         			 			           //��������������
//			key_state = key_state_9;
//			temp2 = 1;									//�ָ���־���ȴ������ͷ�
//			count_temp3 = 0;
//		} else if ((count_temp3 >= 289) && ((key_mask & PORTA) == 0)) {
//			key_return = 8;  									//�� �� ����ͬʱ��������
//			//count_temp3=0;
//			key_state = key_state_9;
//		}
//		break;
//
//	case key_state_9:
//		if ((PORTA & key_mask) == key_mask)  //�������������̧����
//		{
//			count_temp2 = 0;
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_0;
//		}
//		break;
//		//-------------------------------------------------------------------------------------------------------------
//		//-------------------------------------------------------------------------------------------------------------
//	default:
//		key_state = key_state_0;
//		break;
//	}
//	return key_return;
//}
//
////**********************************************************************************************
////�������ƣ�INTERRUPT_Init ();
////�����������
////�����������
////�����������˿��жϳ�ʼ���ӳ���
////�������ڣ�2010.09.24
////**********************************************************************************************
//void INTERRUPT_Init(void) {
//	INTCON = 0b11100000;              //GIE = 1;INTE =0;PEIE=1;T0IE=1;GPIE=0;
//	TMR0 = 0X7C;             //GPPU=0,T0CS=0;PSA= 0,PS2:1=101 64��Ƶ��4M/64=62.5KHZ
//	TRISC5 = 0;
//	PR2 = 250;
//	CCPR1L = 0;
//	CCP1CON = 0b00001100;
//	PSTR1CON = 0X01;				//����RC5
//	T2CON = 0b00000001;    //��ʱ��2ʮ����Ƶ,Ԥ��Ƶ4�����Ƶ1   980hz
//	TMR2ON = 1;
//}
////*******************************************************************
////*******************************************************************
////�������ƣ�interrupt key_int();
////�����������
////�����������
////�����������жϷ������
////�������ڣ�2010.07.24
////*******************************************************************
//void interrupt key_int(void)
//{
//	if(TMR0IF)
//	{
//		TMR0IF = 0;                             //�ж������־λ����
//		TMR0 = 0X7C;//(124+1)/62.5=2ms
//		stime++;//ʱ���żĴ�����һ
//	}
//	if(stime>=5) {
//		//	BAT = 3;
//		if((BAT==3)&&(POWER_ON_FLAG!=0))//����10%
//		{
//			persent_10_time++;
//			if(persent_10_time>=50)
//			{
//				persent_10_time=0;
//				DIS2=~DIS2;
//			}
//		}
//	}
//}
//
////*******************************************************************
////�������ƣ�AD_Init(void)
////�����������
////�����������
////�������������ģó�ʼ��
////�������ڣ�2010.09.09
////*******************************************************************
//
//void AD_Init(void) {
//	TRISA0 = 1;
//	ANSELA = 0b00000001;
//	ADCON0 = 0b00000001;       //ת�������ʽ�Ҷ��룬3.0V�ο���ѹ��ģ��ͨ��AN3(RA4)������ADת����ADת��ģ���
//	ADCON1 = 0b10100011;         //
//	FVREN = 1;         //�ر��ڲ��ο���ѹ
//	FVRRDY = 1;  //��ʹ�ܲο���ѹ
//	ADFVR0 = 0;
//	ADFVR1 = 1;  //�̶��ο���ѹguanbi
//	ADIE = 0;                  	//ADת���жϽ�ֹ
//	ADIF = 0;                  	//�ⲿ�жϽ�ֹ
//	ADON = 0;                    //ADת��ģ��ر�
//	AD_SW = 1;
//}
////*******************************************************************
////�������ƣ�LED_ind_Init(void)
////�����������
////�����������
////����������LEDָʾ�Ƴ�ʼ��
////�������ڣ�2010.09.09
////*******************************************************************
//
//void ADC_Read(void) {
//	AD_SW = 1;					//
//	unchar i, j;				//
//	ADON = 1;					//
//	ADIE = 0;					//
//	adresult = 0;				//adֵ
//	for (i = 30; i > 0; i--)			//10
//			{
//		for (j = 0; j < 3; j++)
//			;		//
//		ADGO = 1;				//
//		while (ADGO)
//			;			//
//		adresult = adresult + ((ADRESH << 8) | ADRESL) / 30;    //
//	}
//	ADON = 0;			//ADת��ģ��ر�
//	//if(adresult<=875) BAT=1;		//70%
//	//if(adresult<=840) BAT=2;		//50%
//	//if(adresult<=790) BAT=3;		//10%
//	//�����ص�ѹС��12K 10K    70%;3.85V(875)   30:3.7V(840)    10%:3.5V(790)
//	if (adresult <= 716)         //3.2
//			{
//		BAT = 3;
//	} else if (adresult <= 791) {  //3.5v
//		BAT = 2;		//
//	} else if (adresult <= 817) {   //3.6��ʾ��ѹ
//		BAT = 1;		//
//	} else {
//		BAT = 0;		//
//	}
//	//   	if(adresult<=859) {data_temp1=2;DOWN_ELE=1;}  //3.85V �޸��ص�ѹ
//	//		if(adresult<=772) {data_temp1=3;DOWN_ELE=2;}  //3.4V
//	//		if(adresult<=727) {data_temp1=4;DOWN_ELE=3;}  //3.2V
//	//		if(adresult<=682) {data_temp1=5;DOWN_ELE=4;}  //3.0V
//
//	if (CHARGE_INPUT == 1) {
//		if (adresult <= 909) {
//			RESET_ELE = 1;
//		}  //4.0V909
//		if (adresult <= 886) {
//			RESET_ELE = 2;
//		}  //3.9V886
//		if (adresult <= 840) {
//			RESET_ELE = 3;
//		}  //3.7V840
//		if (adresult <= 750) {
//			RESET_ELE = 4;
//		}  //3.3V750
//	}
//
//	if ((adresult <= 800) && (data1 == 0)) {
//		DOWN_ELE = 1;
//	} //3.5=794  3.55v       data1������ѹ
//	if ((adresult <= 750) && (data1 == 4)) {
//		DOWN_ELE = 2;
//	} //3.35V757          	data1
//	if ((adresult <= 670) && (data1 == 3)) {
//		DOWN_ELE = 3;
//	}  //3.25V=735     		data1
//	if ((adresult <= 630) && (data1 == 2)) {
//		DOWN_ELE = 4;
//	}  //3.0V=678 2.9=635 	data1����
//	AD_SW = 1;					//
//	//--------------------------------------------------------------------------
//}
//
////*******************************************************************
////�������ƣ�void autotempr_control();
////�����������
////�����������
////�����������ڼ�����ʱ�������¿أ�ÿ30s���һ�Σ��¶ȸ��������¶ȣ������ȼ���10%
////����¶ȵ��������¶ȣ�����������10% ֱ��������
////�������ڣ�20150227
////********************************************************************
//
////
///*void autotempr_control(void)
// {
// if((OTP==0)&&(AUTO_TEMP_TIME>=2910))   //����¶ȸ���65���϶ȣ�����30s��ʱ�䵽�������ȼ�С
// {
// AUTO_TEMP_TIME=0;   //��ʱ�������
// PWM_DATA++;
// if(PWM_DATA>=13) PWM_DATA=13;
// }
// if((OTP==1)&&(AUTO_TEMP_TIME>=2910))   //����¶ȸ���65���϶ȣ�����30s��ʱ�䵽������������
// {
// AUTO_TEMP_TIME=0;   //��ʱ�������
// switch(data1)
// {
// case 0:
// PWM_DATA--;
// if(PWM_DATA<=0) PWM_DATA=1;
// break;
// case 4:
// PWM_DATA--;
// if(PWM_DATA<=8)PWM_DATA=8;
// break;
// default:break;
// }
// }
// switch(PWM_DATA)  //2.8A��1A  Լ�ٷ�֮5�ĵ��� �ݼ�
// {
// case 0:
// break;
// case 1:
// CCPR1L=Ji;   //175 2800mA
// break;
// case 2:
// CCPR1L=164;
// break;
// case 3:
// CCPR1L=153;
// break;
// case 4:
// CCPR1L=142;
// break;
// case 5:
// CCPR1L=131;
// break;
// case 6:
// CCPR1L=120;
// break;
// case 7:
// CCPR1L=109;
// break;
// case 8:
// CCPR1L=98;
// break;
// case 9:
// CCPR1L=87;
// break;
// case 10:
// CCPR1L=76;//1.3a 800 lux
// break;
// case 11:
// CCPR1L=65;
// break;
// case 12:
// CCPR1L=54;
// break;
// case 13:
// CCPR1L=43;//750ma
// break;
// default:
// break;
// }
// }*/
//void LCOK_prompt(void)   //����������ʾ
//{
//	switch (number1) {
//	case 0:
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		if (lock_display_time >= 50)     //0.5��
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 1:
//		LED_ON1 = 1;
//		CCPR1L = Di;
//		if (lock_display_time >= 25)     //0.5��
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 2:
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		if (lock_display_time >= 25)     //0.5��
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 3:
//		LED_ON1 = 1;
//		CCPR1L = Di;
//		if (lock_display_time >= 25)     //0.5��
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 4:
//		key_number = 0;
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		number1 = 0;
//		POWER_ON_FLAG = 0;
//		sleep_flag = 0;
//		temp2 = 0;											 //temp2=0������
//		break;
//	default:
//		break;
//	}
//	//��������������������������������������������������������������������������״̬����������2����Ӧ��
//}
////*******************************************************************
////�������ƣ�lighting_MODE();
////�����������
////�����������
////��������������ģʽ
////�������ڣ�20150227
////********************************************************************
//
//void lighting_MODE(void) {
//	switch (key_number) {
//	case 0:
//		break;
//
//	case 1:						//�����������������������������������������������������������������������������⣨����������
//		key_number = 0;         	//��������ֵ����
//		data_temp = 255;       			//ÿ���а����ı䣬��Ҫ���½��뿪�����
//		dalay_time_10ms = 0;
//		data1++;
//		if (data1 >= 5)
//			data1 = 0;
//		if (((data1 == 0) && (DOWN_FLAG == 1))
//				|| (DOWN_ELE == 1) && ((data1 == 0)))
//			data1 = 1;   //�����������������
//		if (((data1 == 4) && (DOWN_FLAG == 2))
//				|| (DOWN_ELE == 2) && ((data1 == 4)))
//			data1 = 1;   //�����������������
//		if (((data1 == 3) && (DOWN_FLAG == 3))
//				|| (DOWN_ELE == 3) && ((data1 == 3)))
//			data1 = 1;   //�����������������
//		if (((data1 == 2) && (DOWN_FLAG == 4))
//				|| (DOWN_ELE == 4) && ((data1 == 2)))
//			data1 = 1;   //�����������������
//
//		/*	if(CHARGE_INPUT==1)			//���ǿ��ת���ɵ�����
//		 {
//		 data1=2;
//		 }  */
//		break;
//	case 6:   //������������������������������������������������������������������������������
//		key_number = 0;
//		POWER_ON_FLAG = 2;
//		data_temp2 = 0;
//		dalay_time_1ms = 0;
//		dalay_time_10ms = 0;
//		FC_FALSH_DATA = 0;
//		bright_time = 0;
//		SOS_DATA = 0;
//		data2 = 0;
//		SOS1 = 0;
//		SOS2 = 0;
//		temp5 = 0;						//������������
//		stop_run_flash = 0;   //����ģʽ���뱬��ģʽ����ʼ��һ������
//		break;
//	case 7:   //�����������������������������������������������������������������������ر�LED������0.5�룩
//		key_number = 0;         //��������ֵ����
//		CCPR1L = 0;
//		LED_ON1 = 0;
//		break;
//	case 9:         //�����������������������������������������������������������������������ػ�������0.5�룩
//		key_number = 0;
//		if (temp2 == 1) //��������ֵ����				 //����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//				{
//			data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//			PWM_OUT = 0;                        //PWM����͵�ƽ
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			stop_run_flash = 0;
//			data_temp = 255;
//			temp2 = 0;
//		}
//		if (temp2 == 0) 										//�������0.5s�ػ���tem
//				{
//			temp2 = 1;
//		}
//		break;
//	case 8: 				//��������������������������������������������������������������������������״̬����������2����Ӧ��
//		key_number = 0;
//		number1 = 0;
//		data_temp = 255;
//		LOCK = !LOCK;
//		lock_display_time = 0;
//		POWER_ON_FLAG = 3;
//		break;
//	case 2: 				//�����������������������������������������������������������������������ر�LED������0.5�룩
//		key_number = 0;         //��������ֵ����
//		PWM_OUT = 0;                        //PWM����͵�ƽ
//		LED_ON1 = 0;
//		break;
//	case 4:                      //�����������������������������������������������������������������������ػ�������0.5�룩
//		key_number = 0;
//		if (temp2 == 1)   //��������ֵ����	//����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//				{
//			data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//			PWM_OUT = 0;                        //PWM����͵�ƽ
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			data_temp = 255;
//			temp2 = 0;											 //temp2=0������
//		}
//		if (temp2 == 0) 										//�������0.5s�ػ���tem
//				{
//			temp2 = 1;
//		}
//		break;
//	default:
//		key_number = 0;
//		break;   //�������������
//	}
//
//	if (CHARGE_INPUT == 1)	//******zzc������
//			{
//		data1 = 2;
//		data_write();
//		if (CHARGE_STATE == 0) {
//			DIS2 = 1;
//
//			DIS1 = 0;
//		} else {
//			DIS2 = 0;
//
//			DIS1 = 1;
//		}
//	} else if ((BAT != 3) && (CHARGE_INPUT == 0))	//888888&&(BAT!=2)
//			{
//		DIS2 = 0;
//
//		DIS1 = 0;
//	}
//	//data1=0; ����
//	//data1=4; ����
//	//data1=3; ����
//	//data1=2; ����
//	//data1=1; ����
//	switch (data1)	//��������������������ѡ�������������������//
//	{
//	case 0:
//		if (data_temp != 0)          //����Ǵ�������λ�л�����
//				{
//			time_2min = 0;
//			time_2min_flag = 0;
//			time_2s = 0;
//
//			data_temp = 0;
//			CCPR1L = Ji;   					//��������
//			LED_ON1 = 1;    		//ʹ�����
//			AUTO_TEMP_TIME = 0;			//���¸�ֵ�������¿�ʱ��
//			PWM_DATA = 1;           //ÿ�λ������»ص�������ʱ�������¿����¿�ʼ���
//		}
//		if (CCPR1L >= 80)  //
//				{
//			if (OTP == 0) {
//				if (AUTO_TEMP_TIME > 100)  			//ÿһ���½���ʱ�䣬�½��ٶ�1S
//						{
//					AUTO_TEMP_TIME = 0;
//					CCPR1L = 70;   //
//				}
//			} else if ((data1 == 0) && (time_2min > 9000)) //�����ڸ���***1200=2min 1.5MIN
//					{
//				if (AUTO_TEMP_TIME > 100)   			//ÿһ���½���ʱ�䣬�½��ٶ�1S
//						{
//					AUTO_TEMP_TIME = 0;
//					CCPR1L--;   //�����¿����ܽ������������
//					if (CCPR1L <= 70) {
//						CCPR1L = 70;
//					}
//
//				}
//			}
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))   //�����ѹ
//				{
//			DOWN_FLAG = 1;
//			dalay_time_10ms = 0;
//			data1 = 4;
//			data_temp = 255;
//		}
//		break;                     //1200���������������������㣲������mA
//	case 1:
//		if (data_temp != 1)          //����Ǵ�������λ�л�����
//				{
//			data_temp = 1;        //ִ�е���PWM
//			CCPR1L = Jie;            //��������
//			LED_ON1 = 1;						 //ʹ�����
//		}
//		break;                      //15mA
//	case 2:
//		if (data_temp != 2)          //����Ǵ�������λ�л�����
//				{
//			data_temp = 2;         //ִ������PWM
//			CCPR1L = Di;              //��������
//			LED_ON1 = 1;					  //ʹ�����
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))				//�����ѹ
//				{
//			DOWN_FLAG = 4;
//			dalay_time_10ms = 0;
//			data1 = 1;
//			data_temp = 255;
//		}
//		break;                 //150mA
//	case 3:
//		if (data_temp != 3)         //����Ǵ�������λ�л�����
//				{
//			data_temp = 3;            //ִ�и���PWM
//			CCPR1L = Zhong;					//��������
//			LED_ON1 = 1;					//ʹ�����
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))				//�����ѹ
//				{
//			DOWN_FLAG = 3;
//			dalay_time_10ms = 0;
//			data1 = 2;
//			data_temp = 255;
//		}
//		break;                    //400mA
//	case 4:
//		if (data_temp != 4)         //����Ǵ�������λ�л�����
//				{
//			data_temp = 4;        //ִ�и���PWM
//			CCPR1L = Gao;						//��������
//			LED_ON1 = 1;					//ʹ�����
//			PWM_DATA = 8;
//			AUTO_TEMP_TIME = 0;			//���¸�ֵ�������¿�ʱ��
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))			//�����ѹ
//				{
//			DOWN_FLAG = 2;
//			dalay_time_10ms = 0;
//			data1 = 3;
//			data_temp = 255;
//		}
//		break;                    //400mA
//	default:
//		break;
//	}
//}
////*******************************************************************
////�������ƣ�LED_FC_FALSH();
////�����������
////�����������
////������������Ҫ�������2.9A ��Ƶ����
////�������ڣ�2011.12.06
////*******************************************************************
//void LED_FC_FALSH(void) {
//	switch (FC_FALSH_DATA) {
//	case 0:
//		FC_FALSH_FLAG = 0;
//		if (LED_ON1 == 0) {
//			if (dalay_time_10ms >= 3) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 1;
//			}
//		} else if (LED_ON1 == 1) {
//			if (dalay_time_10ms >= 3) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 0;
//				bright_time++;
//				if (bright_time >= 30) {
//					bright_time = 0;
//					FC_FALSH_DATA = 1;
//				}
//			}
//		}
//		break;
//	case 1:
//		if (LED_ON1 == 0) {
//			if (dalay_time_10ms >= 7) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 1;
//			}
//		} else if (LED_ON1 == 1) {
//			if (dalay_time_10ms >= 7) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 0;
//				bright_time++;
//				if (bright_time >= 12) {
//					bright_time = 0;
//					FC_FALSH_DATA = 0;
//					FC_FALSH_FLAG = 1;
//				}
//			}
//		}
//		break;
//	default:
//		break;
//	}
//}
////*******************************************************************
////�������ƣ�LED_SOS_FALSH();
////�����������
////�����������
////������������Ҫ�������200mA SOS
////�������ڣ�2010.07.21
////*******************************************************************
//void LED_SOS_FALSH(void) {
//	switch (SOS_DATA) {
//	case 0:
//		LED_ON1 = 1;
//		if (dalay_time_10ms >= SOS_H[SOS1]) //unchar	SOS_H[]={34,34,34,100,100,100,34,34,34};
//				{
//			dalay_time_10ms = 0;
//			SOS_DATA++;
//			SOS1++;
//		}
//		break;
//	case 1:
//		LED_ON1 = 0;
//		if (dalay_time_10ms >= SOS_L[SOS2]) //unchar	SOS_L[] ={33,33,200,33,33,200,33,33,233};
//				{
//			dalay_time_10ms = 0;
//			SOS_DATA--;
//			SOS2++;
//		}
//		if (SOS2 == 9) {
//			SOS1 = 0;
//			SOS2 = 0;
//			dalay_time_10ms = 0;
//		}
//		break;
//	default:
//		break;
//	}
//}
////=================================================================================���� -SOS
//void FLASH_BS_SOS_MODE(void) //����/SOS
//{
//	if (CHARGE_INPUT == 1)	//******zzc������
//			{
//		data1 = 2;
//		data_write();
//		if (CHARGE_STATE == 0) {
//			DIS1 = 0;
//			DIS2 = 1;
//		} else {
//			DIS1 = 1;
//			DIS2 = 0;
//		}
//	} else if ((BAT != 3) && (CHARGE_INPUT == 0))	//888888( BAT!=2)&&
//			{
//		DIS2 = 0;
//
//		DIS1 = 0;
//	}
//	switch (key_number)                   //����˸״̬�£����ݰ�������ֵ
//	{
//	case 0:
//		break;  //�ް�������������ִ��
//	case 1:
//		POWER_ON_FLAG = 1;
//		LED_ON1 = 1;						//ʹ�����
//		data_temp = 255;				//������䵵λ
//		key_number = 0;					//�㰴��ֵ
//		data_temp2 = 0;
//		stop_run_flash = 0;     //�ָ���˸��־��Ϊ��һ�´εĽ��롣
//		break;
//	case 2:     //�����������������������������������������������������������������������ر�LED������0.5�룩
//		key_number = 0;         //��������ֵ����
//		CCPR1L = 0;                  //�ر�LED�������
//		LED_ON1 = 0;  	     		 //�ر��������
//		stop_run_flash = 1;					 //�ر���˸����
//		break;
//	case 4:					 //�����������������������������������������������������������������������ػ�������0.5�룩
//		key_number = 0;
//		if (temp2 == 1)	//��������ֵ����			//����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//				{
//			data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//			CCPR1L = 0;            //PWM����͵�ƽ
//			stop_run_flash = 0;     //�ָ���˸��־��Ϊ��һ�´εĽ��롣
//			LED_ON1 = 0; 			//�ر�����оƬʹ��
//			POWER_ON_FLAG = 0;
//			temp2 = 0;											 //temp2=0������
//		}
//		if (temp2 == 0) 										//�������0.5s�ػ���tem
//				{
//			temp2 = 1;
//		}
//		break;
//	case 6:
//		key_number = 0;           //��������ֵ����
//		data_temp2 = 0;
//		data2++;                //��˸����־λ��0
//		if (data2 >= 2)
//			data2 = 0;
//		dalay_time_1ms = 0;
//		dalay_time_10ms = 0;
//		FC_FALSH_DATA = 0;
//		bright_time = 0;
//		SOS_DATA = 0;
//		SOS1 = 0;
//		SOS2 = 0;
//		stop_run_flash = 0;     //�ָ���־��
//		break;
//	case 7:     //�����������������������������������������������������������������������ر�LED������0.5�룩
//		key_number = 0;         //��������ֵ����
//		CCPR1L = 0;
//		LED_ON1 = 0;
//		stop_run_flash = 1;     //�رա�
//		break;
//	case 8:     //��������������������������������������������������������������������������״̬����������2����Ӧ��
//
//		key_number = 0;
//		data_temp2 = 0;
//		data_temp = 255;
//		LOCK = !LOCK;
//		lock_display_time = 0;
//		number1 = 0;
//		POWER_ON_FLAG = 3;
//		stop_run_flash = 0;     //�ָ���˸��־��Ϊ��һ�´εĽ��롣
//		break;
//	case 9:     //�����������������������������������������������������������������������ػ�������0.5�룩
//		key_number = 0;
//		if (temp2 == 1) //��������ֵ����				 //����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//				{
//			data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//			PWM_OUT = 0;                        //PWM����͵�ƽ
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			temp2 = 0;											 //temp2=0������
//			stop_run_flash = 0;     //�ָ���˸��־��Ϊ��һ�´εĽ��롣
//		}
//		if (temp2 == 0) 										//�������0.5s�ػ���tem
//				{
//			temp2 = 1;
//		}
//		break;
//
//	default:
//		key_number = 0;
//		break;
//	}
//	if ((stop_run_flash != 1) && (CHARGE_INPUT != 1)) //CHARGE_STATE!=1*******ZC
//			{
//		switch (data2)                                //���ݱ���������˸Ƶ��
//		{
//		case 0:
//			if (temp5 != 5)  		//���TIMER1�ڲ���1450ma�ĳ�ֵ
//					{
//				temp5 = 5;                //ִ�е�����λ
//				TMR1ON = 1;                //��TIMER1�ж�
//				dalay_time_10ms = 0;
//				FC_FALSH_DATA = 0;
//				//	CCPR1L=Ji;
//				PWM_DATA = 1;   //��ʼ���������¿�����
//				//	if((BAOSHAN_JIANDANG==1)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==2)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==3)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==4)&&(data_temp1==1))data_temp1=2;
//			}
//			switch (data_temp1) {
//
//			case 0:
//				break;
//			case 1:
//				if (BAOSHAN_JIANDANG == 1)
//					break;
//				if (BAOSHAN_JIANDANG == 2)
//					break;
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Ji;
//				break;
//			case 2:
//				if (BAOSHAN_JIANDANG == 2)
//					break;
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Gao;
//				break;
//			case 3:
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Zhong;
//				break;
//			case 4:
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Di;
//				break;
//			case 5:
//				CCPR1L = Jie;
//				break;
//			default:
//				break;
//			}
//			if ((VOL_FLAG == 1) && (dalay_time_1ms >= 150))				//�����ѹ
//					{
//				dalay_time_1ms = 0;
//				if (data_temp1 < 5) {
//					data_temp1++;
//				}
//			}
//			LED_FC_FALSH();					//��Ƶ
//
//			break;   //15HZ����
//			//-------------------------------------------------------------------------------------
//		case 1:
//			if (temp5 != 6)                 //���TIMER1�ڲ���200ma�ĳ�ֵ
//					{
//				temp5 = 6;                   //ִ��200mA����λ
//				TMR1ON = 1;                   //��TIMER1�ж�
//				SOS_DATA = 0;
//				data_temp = 3;                    //���ó�ֵ
//				dalay_time_10ms = 0;
//				CCPR1L = Di;
//				SOS1 = 0;
//				SOS2 = 0;
//			} else {
//				switch (data_temp2) {
//				case 0:
//					break;
//
//				case 1:
//					CCPR1L = Jie;
//					break;
//				default:
//					break;
//				}
//			}
//
//			LED_SOS_FALSH();                   //��SOSʹ��LED
//			if (VOL_FLAG == 1) {
//				if (dalay_time_1ms >= 200)		//�״ο�����ʱ2s
//						{
//					dalay_time_1ms = 0;
//					Input_voltage_low = 1;
//					data_temp2++;
//					if (data_temp2 >= 1)
//						data_temp2 = 1;
//				}
//			} else {
//				dalay_time_1ms = 0;
//			}
//			break;   //SOS
//		default:
//			break;
//		}
//	}
//
//	/*if(BAT!=3){
//	 DIS1=DIS2=0;
//	 }*/
//}
////=========================================================================��ʾ����
//void DISPLAY_ELV() {
//	if (display_time >= 300)  //2��ʱ�䵽���˳�
//			{
//		display_time = 0;
//		POWER_ON_FLAG = 0;
//		sleep_flag = 0;
//	}
//	//ADC_Read();
//	if ( CHARGE_INPUT != 1) {
//		switch (BAT) {
//		case 0:
//			DIS1 = 1;
//			DIS2 = 0;
//			break;
//		case 1:  //70%
//			DIS1 = 1;
//			DIS2 = 0;
//			break;
//		case 2:	//30%
//			DIS1 = 0;
//			DIS2 = 1;
//			break;
//		case 3:  //10%
//			if (stime >= 5) {
//				{
//					persent_10_time++;
//					if (persent_10_time >= 50) {
//						persent_10_time = 0;
//						DIS2 = ~DIS2;
//					}
//				}
//			}
//			//if(DIS_DELAY_500ms>=80)
//			//  {DIS_DELAY_500ms=0;
//			//		DIS1=0;
//			//   DIS2=~DIS2;}
//			//	DIS1=0;DIS2=1;
//			break;
//		default:
//			break;
//		}
//	}
//	if (CHARGE_INPUT == 1)	//
//			{
//		if (CHARGE_STATE == 0) {
//			DIS2 = 1;
//			DIS1 = 0;
//		} else {
//			DIS2 = 0;
//			DIS1 = 1;
//		}
//	}
//
//}
////=============================================================================================
//void LOW_VOLDOWN()   //��ѹ�����Ӻ���
//{
//	//	if(adresult<=795) {DOWN_ELE=1;}  //3.5V
//	//	if(adresult<=772) {DOWN_ELE=2;} //3.4V
//	//	if(adresult<=727) {DOWN_ELE=3;}  //3.2V
//	//	if(adresult<=682) {DOWN_ELE=4;}  //3.0V
//
//	//	if(adresult<=909) {RESET_ELE=1;}  //4.0V
//	//	if(adresult<=886) {RESET_ELE=2;}  //3.9V
//	//	if(adresult<=840) {RESET_ELE=3;}  //3.7V
//	//	if(adresult<=750) {RESET_ELE=4;}  //3.3
//
//	//	if(((data1==0)&&(DOWN_FLAG==1))||(DOWN_ELE==1)&&((data1==0)))data1=1;   //�����������������
//	//	if(((data1==4)&&(DOWN_FLAG==2))||(DOWN_ELE==2)&&((data1==4)))data1=1;   //�����������������
//	//	if(((data1==3)&&(DOWN_FLAG==3))||(DOWN_ELE==3)&&((data1==3)))data1=1;   //�����������������
//	//	if(((data1==2)&&(DOWN_FLAG==4))||(DOWN_ELE==4)&&((data1==2)))data1=1;   //�����������������
//
//	if (CHARGE_INPUT == 1)	//������
//			{
//		switch (RESET_ELE)	//�ҵ�ѹ�ָ�
//		{
//		case 1:					//4.0�ָ���
//			DOWN_ELE = 0;
//			DOWN_FLAG = 0;
//			break;
//		case 2:
//			DOWN_ELE = 1;		//3.9�ָ���
//			DOWN_FLAG = 1;
//			break;
//		case 3:					//3.7�ָ���
//			DOWN_ELE = 2;
//			DOWN_FLAG = 2;
//			break;
//		case 4:					//3.3�ָ���
//			DOWN_ELE = 3;
//			DOWN_FLAG = 3;
//			break;
//		default:
//			break;
//		}
//	}
//	switch (DOWN_ELE) {
//	case 0:
//		break;
//	case 1:				//������ڼ����������½�һ��
//		if (data1 == 0) {
//			DOWN_FLAG = 1;		//��־һ
//			data1 = 4;
//		}
//		//--------------------------------------------------------------------------
//		if (data_temp1 == 1)		// ����ģʽ�� pwmռ�ձȵı�־λ
//				{
//			BAOSHAN_JIANDANG = 1;
//			data_temp1 = 2;
//		}
//
//		break;
//	case 2:				//������ڸ����������½�������
//		if ((data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 2;		//��־һ
//			data1 = 3;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 2) || (data_temp1 == 1))		// ����ģʽ�� pwmռ�ձȵı�־λ
//				{
//			BAOSHAN_JIANDANG = 2;
//			data_temp1 = 3;
//		}
//		break;
//	case 3:				//������������������½�������
//		if ((data1 == 3) || (data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 3;		//��־һ
//			data1 = 2;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 3) || (data_temp1 == 2) || (data_temp1 == 1))// ����ģʽ�� pwmռ�ձȵı�־λ
//				{
//			data_temp1 = 4;
//			BAOSHAN_JIANDANG = 3;
//		}
//		break;
//	case 4:				//������ڵ����������½�������
//		if ((data1 == 2) || (data1 == 3) || (data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 4;		//��־һ
//			data1 = 1;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 4) || (data_temp1 == 2) || (data_temp1 == 1))// ����ģʽ�� pwmռ�ձȵı�־λ
//				{
//			data_temp1 = 5;
//			BAOSHAN_JIANDANG = 4;
//		}
//		break;
//	default:
//		break;
//	}//=============================================================================================
//
//}
//
//void tsoftReset();
////*******************************************************************
////�������ƣ�main();
////�����������
////�����������
////����������������
////�������ڣ�2010.09.14
////*******************************************************************
//
//void main(void) {
//
//	unint i;
//
//	CUP_Init();				    //��λ����
//
//	INTERRUPT_Init();
//	AD_Init();
//	data_read();                         //��EEPROM�м���ĵ�λ
//
//	softReset: while (1)                                      //��ѭ��
//	{
//		if (stime >= 5)                                 //10MS��
//				{
//			stime = 0;                          //ɨ��ʱ�䵽��ʾΪ����
//			AUTO_TEMP_TIME++;
//			LOW_VOLDOWN();										//����ѹ��������־
//			if ((POWER_ON_FLAG == 1) && (data1 == 0)) {
//				time_2min++;
//				if (time_2min_flag == 1)
//					time_2s++;
//				else
//					time_2s = 0;
//			} else   //�����Եļ�ʱ����
//			{
//				time_2min = 0;
//				time_2min_flag = 0;
//				time_2s = 0;
//			}
//			// OTP=1;
//			if (CCPR1L >= 80)  //
//					{
//				if (OTP == 0) {
//					if (AUTO_TEMP_TIME > 100)  			//ÿһ���½���ʱ�䣬�½��ٶ�1S
//							{
//						AUTO_TEMP_TIME = 0;
//						CCPR1L = 70;   //
//					}
//				} else if ((data1 == 0) && (time_2min > 9000)) //�����ڸ���***1200=2min 1.5MIN
//						{
//					if (AUTO_TEMP_TIME > 100)   			//ÿһ���½���ʱ�䣬�½��ٶ�1S
//							{
//						AUTO_TEMP_TIME = 0;
//						CCPR1L--;   //�����¿����ܽ������������
//						if (CCPR1L <= 70) {
//							CCPR1L = 70;
//						}
//
//					}
//				}
//			}
//
//			//*****************************************************************8888
//			if ((CHARGE_INPUT == 1) && (LOCK == 1) && (CHAR_C != 2))   //���ϵ�
//					{
//				CHAR_B = 1;
//				CHAR_C = 1;
//				LOCK = 0;   //1
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_C != 2)) {
//				CHAR_B = 1;
//				CHAR_C = 1;
//				LOCK = 0;   //2
//			}
//			if ((CHARGE_INPUT == 1) && (CHAR_B == 1) && (LOCK == 1)) {
//				LOCK = 1;
//				CHAR_C = 2;   //3
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_B == 1))   //
//					{
//				LOCK = 0;
//				CHAR_C = 2;  //����**************4
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 1) && (CHAR_C == 2))  //���ϵ�
//					{
//				LOCK = 1;  //5
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_C == 2)) {
//				LOCK = 0;  //6
//			}
//			if ((CHARGE_INPUT == 0) && (CHAR_B == 1) && (LOCK == 1)) {
//				LOCK = 1;
//				CHAR_B = 0;
//				CHAR_C = 0;  //7 JIA LOCK=1
//			}
//			if ((CHARGE_INPUT == 0) && (LOCK == 0) && (CHAR_B == 1))  //
//					{
//				LOCK = 0;
//				CHAR_B = 0;
//				CHAR_C = 0;  //����**************8
//			}
//
//			DELAY_02S++;
//			DIS_DELAY_500ms++;
//			delay1s++;
//			lock_display_time++;
//			dalay_time_10ms++;
//			if (POWER_ON_FLAG == 4)
//				display_time++;
//			//else display_time=300;
//
//			if (VOL_FLAG == 1)
//				dalay_time_1ms++;
//			else {
//				dalay_time_1ms = 0;
//			}
//
//			/*	if((BAT==3)&&(POWER_ON_FLAG!=0)) //����10%//////////**********************888
//			 {
//			 persent_10_time++;
//			 if(persent_10_time>=50)
//			 {
//			 persent_10_time=0;
//			 DIS2=~DIS2;
//			 }
//			 }*/
//
//			if (DELAY_02S >= 30)              //���300ms��
//					{
//				DELAY_02S = 0;			//˫����־λ
//				data_write();
//				if (POWER_ON_FLAG != 4)
//					ADC_Read();			//�鿴����ʱ��������
//			}
//
//			if ((POWER_ON_FLAG == 0) && (SW_OF == 1) && (SW_OF == 1)
//					&& (CHARGE_INPUT == 0))  //�ػ�״̬�£�2�벻���������������״̬��
//					{
//				SLEEP_TIME++;
//				if (SLEEP_TIME >= 300)
//					sleep_flag = 0;  //��������
//			} else
//				SLEEP_TIME = 0;
//
//			key_number = Key_scan_ON();
//		}
//
//		switch (POWER_ON_FLAG)      //���ݿ�����ʾִ�У��ϵ��������״̬
//		{
//		case 0:
//			if (sleep_flag == 1) {
//
//				if ((key_number == 6) && (LOCK == 0))      //����������
//						{
//					POWER_ON_FLAG = 4;     	//��ʾ2�����
//					break;
//				}
//				if ((key_number == 2) && (LOCK == 0))	//S1���� ����0.5S  ��������������ģʽ
//						{
//					POWER_ON_FLAG = 1;     	//ת��������
//					key_number = 0;
//					data_temp = 255;
//					temp2 = 0;
//					sleep_flag = 0;
//				} else if ((key_number == 7) && (LOCK == 0))    //�ػ�-->����
//						{
//					if (CHARGE_INPUT == 0) {
//						POWER_ON_FLAG = 1;     	//ת��������
//						key_number = 0;
//						data_temp = 255;
//						temp2 = 0;
//						sleep_flag = 0;
//					} else {
//						key_number = 0;
//						data_temp = 255;
//						POWER_ON_FLAG = 2;
//						temp2 = 0;
//						data2 = 0;
//						temp5 = 0;				//������������
//						sleep_flag = 0;
//						stop_run_flash = 0;
//					}
//					if (CHARGE_INPUT == 0) {
//						key_number = 0;
//						data_temp = 255;
//						POWER_ON_FLAG = 2;
//						temp2 = 0;
//						data2 = 0;
//						temp5 = 0;				//������������
//						sleep_flag = 0;
//						stop_run_flash = 0;
//					} else {
//						POWER_ON_FLAG = 1;     	//ת��������
//						key_number = 0;
//						data_temp = 255;
//						temp2 = 0;
//						sleep_flag = 0;
//					}
//				} else if (key_number == 8) {
//					key_number = 0;
//					lock_display_time = 0;
//					LOCK = !LOCK;		//����/����    LOCK==0;����������������LOCK==1������
//					if (LOCK == 1)
//						POWER_ON_FLAG = 3; //������ڹػ�״̬�½�������������˸���Σ�Ȼ��Ϩ����ʾ�Ѿ���������״̬��
//					else {
//						POWER_ON_FLAG = 1;		//��������������ģʽ
//						data_temp = 255;
//						temp2 = 1;  	 		//�����������������ֹ���ظ������β��ܹػ�
//						sleep_flag = 0;
//					}
//				} else if (((key_number == 2) && (LOCK == 1))
//						|| ((key_number == 1) && (LOCK == 1))
//						|| ((key_number == 6) && (LOCK == 1))
//						|| ((key_number == 7) && (LOCK == 1))) //�������״̬���������������ʾһ������״̬
//						{
//					lock_display_time = 0;
//					number1 = 1;
//					POWER_ON_FLAG = 3;    //������ڹػ�״̬�½�������������˸���Σ�Ȼ��Ϩ����ʾ�Ѿ���������״̬��
//				}
//
//				if (CHARGE_INPUT == 1) {   // POWER_ON_FLAG=1;
//
//					input_flag1 = 1;   //cuowu
//					if (CHARGE_STATE == 0) {
//						DIS1 = 0;
//						DIS2 = 1;
//					} else {
//						DIS1 = 1;
//						DIS2 = 0;
//					}
//				}  //�����ʾ
//				else {
//
//					if (input_flag1 == 1) {
//
//						input_flag1 = 0;
//						//RESET();
//						tsoftReset();
//						goto softReset;
//					}
//					if ((key_number == 2) && (LOCK == 0)) //S1���� ����0.5S  ��������������ģ
//
//							{
//						POWER_ON_FLAG = 1;     	//ת��������ģʽ1
//						key_number = 0;          //��������ֵ��
//						data_temp = 255;
//						temp2 = 0;  	              //����״̬Ϊ0��
//						sleep_flag = 0;             //���˯��
//					} else if ((key_number == 7) && (LOCK == 0))    //�ػ�-->����
//							{
//						if (CHARGE_INPUT != 1) {
//							key_number = 0;
//							data_temp = 255;
//							POWER_ON_FLAG = 2;
//							temp2 = 0;
//							data2 = 0;
//							temp5 = 0;		//������������
//							sleep_flag = 0;
//							stop_run_flash = 0;
//						} else {
//							POWER_ON_FLAG = 1;     	//ת��������
//							key_number = 0;
//							data_temp = 255;
//							temp2 = 0;
//							sleep_flag = 0;
//						}
//					}
//					DIS1 = 0;
//					DIS2 = 0;
//				}
//			} else {
//				AD_FLAG = 0;
//				mode_return_flag = 0;			//ģʽ���ر�־λ4������-->������
//				SLEEP_TIME = 0;
//
//				PWM_OUT = 0;            //PWM����͵�ƽ
//				CCPR1L = 0;
//				LATA0 = 0;
//
//				LED_ON1 = 0;           //�ر�������ʹ�ܶˣ�ʹ����������С
//				TMR1ON = 0;
//				temp2 = 0;
//				ADON = 0;           //�ر�AD
//				FVREN = 0;           //�ر��ڲ��ο���ѹ
//				FVRRDY = 0;  //��ʹ�ܲο���ѹ
//				ADFVR0 = 0;
//				ADFVR1 = 0;  //�̶��ο���ѹguanbi
//				TMR2ON = 0;
//				data_temp2 = 0;
//				WPUA = 0X06;
//				WPUC = 0X00;
//				TRISA = 0XFF;
//				TRISC = 0X00;  //FF*******
//				RA0 = 0;
//				CHARGE_PUTIN_FLAG = 0;				//�����־
//				CCPR1L = 0;
//				PORTA = 0x06;
//				PORTC = 0x00;
//				LATA = 0X06;
//				LATC = 0X00;
//				LATA1 = 1;
//				LATA2 = 1;
//
//				WPUA1 = 1;
//				WPUA2 = 1;
//				TRISA1 = 1;
//				TRISA2 = 1;
//
//				stop_run_flash = 0;
//				AD_SW = 0;
//
//				IOCAP = 0b00100000;     //���GP�����ػ���
//				IOCAN = 0b00000110;     //GP1���ŵ�ƽ�仯��������ʹ��
//				//IOCAF        = 0b00100000;     //
//				//IOCAF        = 0b00000000;     //
//				INTCON = 0b00001000;     //���ⲿ���ŵ�ƽ�仯���ѣ�ֻ���ѣ����ж�
//				if (CHARGE_INPUT != 1) {     //���ʱ����������
//					SLEEP();                 		//����
//					;                             //�ղ�����˯�ߺ�Ԥȡָ��Ϊ��
//				}
//				IOCAF = 0b00000000;   //�廽�ѱ�־λ
//				//IOCAP  			=	0b00000000;
//				//IOCAN  			=	0b00000000;   	//�廽�ѱ�־λ
//				CUP_Init();							//���Ѻ��λ��������
//				INTERRUPT_Init();
//				AD_Init();
//				data_temp = 255;
//				sleep_flag = 1;
//				DELAY_02S = 0;     // ���˫����־λ
//				for (i = 2000; i > 0; i--)
//					;     //��Ҫ��ʱ
//				ADC_Read();
//				if ((data1 == 0) && (adresult <= 872))	//<3.85V
//						{
//					data1 = 4;
//					DOWN_ELE = 1;
//				}   	//���ߺ���ص�ѹ���
//				else if ((data1 == 4) && (adresult <= 845))	//<3.7
//						{
//					data1 = 3;
//					DOWN_ELE = 2;
//				}
//				/*		else if((data1 == 3)&&(adresult<=859))
//				 {
//				 data1 = 2;
//				 DOWN_ELE=3;
//				 }
//				 else if((data1 == 2)&&(adresult<=859))
//				 {
//				 data1 = 2;
//				 DOWN_ELE=4;
//				 }
//				 */
//				switch (DOWN_ELE)	// �жϵ�ѹ
//				{
//				case 0:
//					break;
//				case 1:				//������ڼ����������½�һ��
//					if (data1 == 0) {
//						DOWN_FLAG = 1;		//��־һ
//						data1 = 4;
//					}
//					if (data_temp1 == 1)		// ����ģʽ�� pwmռ�ձȵı�־λ
//							{
//						data_temp1 = 2;
//						BAOSHAN_JIANDANG = 1;
//					}
//					break;
//				case 2:				//������ڸ����������½�������
//					if ((data1 == 4) || (data1 == 0)) {
//						DOWN_FLAG = 2;		//��־һ
//						data1 = 3;
//					}
//					if ((data_temp1 == 2) || (data_temp1 == 1))	// ����ģʽ�� pwmռ�ձȵı�־λ
//							{
//						data_temp1 = 3;
//						BAOSHAN_JIANDANG = 2;
//					}
//					break;
//				case 3:				//������������������½�������3.2
//					if ((data1 == 3) || (data1 == 4) || (data1 == 0)) {
//						DOWN_FLAG = 3;		//��־һ
//						data1 = 2;
//					}
//					if ((data_temp1 == 3) || (data_temp1 == 2)
//							|| (data_temp1 == 1))		// ����ģʽ�� pwmռ�ձȵı�־λ
//							{
//						data_temp1 = 4;
//						BAOSHAN_JIANDANG = 3;
//					}
//					break;
//				case 4:				//������ڵ����������½�������
//					if ((data1 == 2) || (data1 == 3) || (data1 == 4)
//							|| (data1 == 0)) {
//						DOWN_FLAG = 4;		//��־һ
//						data1 = 1;
//					}
//					if ((data_temp1 == 4) || (data_temp1 == 3)
//							|| (data_temp1 == 2) || (data_temp1 == 1))// ����ģʽ�� pwmռ�ձȵı�־λ
//							{
//						data_temp1 = 5;
//						BAOSHAN_JIANDANG = 4;
//					}
//					break;
//				default:
//					break;
//				}
//			}
//			break;
//		case 1:   //ģʽ1��������λ
//			switch (key_number) {
//			case 0:
//				break;
//
//			case 1:   //�����������������������������������������������������������������������������⣨����������
//				key_number = 0;         	//��������ֵ����
//				data_temp = 255;       			//ÿ���а����ı䣬��Ҫ���½��뿪�����
//				dalay_time_10ms = 0;
//				data1++;
//				if (data1 >= 5)
//					data1 = 0;
//				if (((data1 == 0) && (DOWN_FLAG == 1))
//						|| (DOWN_ELE == 1) && ((data1 == 0)))
//					data1 = 1;   //�����������������
//				if (((data1 == 4) && (DOWN_FLAG == 2))
//						|| (DOWN_ELE == 2) && ((data1 == 4)))
//					data1 = 1;   //�����������������
//				if (((data1 == 3) && (DOWN_FLAG == 3))
//						|| (DOWN_ELE == 3) && ((data1 == 3)))
//					data1 = 1;   //�����������������
//				if (((data1 == 2) && (DOWN_FLAG == 4))
//						|| (DOWN_ELE == 4) && ((data1 == 2)))
//					data1 = 1;   //�����������������
//				break;
//			case 6:   //������������������������������������������������������������������������������
//				key_number = 0;
//				POWER_ON_FLAG = 2;
//				data_temp2 = 0;
//				dalay_time_1ms = 0;
//				dalay_time_10ms = 0;
//				FC_FALSH_DATA = 0;
//				bright_time = 0;
//				SOS_DATA = 0;
//				data2 = 0;
//				SOS1 = 0;
//				SOS2 = 0;
//				temp5 = 0;						//������������
//				stop_run_flash = 0;   //����ģʽ���뱬��ģʽ����ʼ��һ������
//				break;
//			case 7:   //�����������������������������������������������������������������������ر�LED������0.5�룩
//				key_number = 0;         //��������ֵ����
//				CCPR1L = 0;
//				LED_ON1 = 0;
//				break;
//			case 9:         //�����������������������������������������������������������������������ػ�������0.5�룩
//				key_number = 0;
//				if (temp2 == 1) //��������ֵ����	//����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//						{
//					data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//					PWM_OUT = 0;                        //PWM����͵�ƽ
//					LED_ON1 = 0;
//					POWER_ON_FLAG = 0;
//					stop_run_flash = 0;
//					data_temp = 255;
//					temp2 = 0;
//					if (input_flag1 == 1) {
//						input_flag1 = 0;
//						tsoftReset();                        //            8.4
//						goto softReset;
//					}
//				}
//				if (temp2 == 0) 			 //�������0.5s�ػ���
//						{
//					temp2 = 1;
//				}
//				break;
//			case 8: 		//��������������������������������������������������������������������������״̬����������2����Ӧ��
//				key_number = 0;
//				number1 = 0;
//				data_temp = 255;
//				LOCK = !LOCK;
//				lock_display_time = 0;
//				POWER_ON_FLAG = 3;
//				break;
//			case 2: 		//�����������������������������������������������������������������������ر�LED������0.5�룩
//				key_number = 0;         //��������ֵ����
//				PWM_OUT = 0;                        //PWM����͵�ƽ
//				LED_ON1 = 0;
//				break;
//			case 4:              //�����������������������������������������������������������������������ػ�������0.5�룩
//				key_number = 0;
//				if (temp2 == 1) //��������ֵ����	//����ػ���temp2==1����ػ����㣬����temp2==0ʱ ��temp2��1
//						{
//					data_temp = 255;       //ÿ���а����ı䣬��Ҫ���½��뿪�����
//					PWM_OUT = 0;                        //PWM����͵�ƽ
//					LED_ON1 = 0;
//					POWER_ON_FLAG = 0;
//					data_temp = 255;
//					temp2 = 0;			 //temp2=0������
//					if (input_flag1 == 1) {
//						input_flag1 = 0;
//						tsoftReset();			 //         8.4
//						goto softReset;
//					}
//				}
//				if (temp2 == 0) 			//�������0.5s�ػ���tem
//						{
//					temp2 = 1;
//				}
//				break;
//			default:
//				key_number = 0;
//				break;   //�������������
//			}
//			lighting_MODE();
//			break;
//		case 2:   //������������ת�ӳ���
//			FLASH_BS_SOS_MODE();
//			if (CHARGE_INPUT == 1)	//******zzc������
//
//					{ // key_number=1;//
//				key_state = key_state_1;     //����ʱ������!
//				//	POWER_ON_FLAG    =  1 ;     //ǿ��ת��Ϊ�͵�
//				data1 = 2;
//				//����ģʽ
//				if (CHARGE_STATE == 0) {
//					DIS1 = 0;
//					DIS2 = 1;
//				} else {
//					DIS1 = 1;
//					DIS2 = 0;
//				}
//			}
//			break;
//		case 3:		//
//			LCOK_prompt();
//			break;
//		case 4:		//��ʾ����
//			//		if(CHARGE_INPUT!=1)//��粻�Ե�
//			DISPLAY_ELV();
//			if ((key_number == 2) && (LOCK == 0))		//S1���� ����0.5S  ��������������ģ
//					{
//				POWER_ON_FLAG = 1;     	//ת��������ģʽ1
//				key_number = 0;          //��������ֵ��
//				data_temp = 255;
//				temp2 = 0;  	              //����״̬Ϊ0��
//				sleep_flag = 0;             //���˯��
//			} else if ((key_number == 7) && (LOCK == 0))    //�ػ�-->����
//					{
//				if (CHARGE_INPUT != 1) {
//					key_number = 0;
//					data_temp = 255;
//					POWER_ON_FLAG = 2;
//					temp2 = 0;
//					data2 = 0;
//					temp5 = 0;		//������������
//					sleep_flag = 0;
//					stop_run_flash = 0;
//				} else {
//					POWER_ON_FLAG = 1;     	//ת��������
//					key_number = 0;
//					data_temp = 255;
//					temp2 = 0;
//					sleep_flag = 0;
//				}
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void tsoftReset() {
//	key_state = 0;
//	stime = 0;                        //2ms��ʱ������żĴ���
//	POWER_ON_FLAG = 0;                        //��Ƭ������״̬�Ĵ���
//	BAT = 0;				   						//������ʾ�Ĵ���
//	count_temp1 = 0;                        //  ������ʱ������Ĵ���
//	count_temp2 = 0;                        //  ������ʱ�����Ĵ���
//	count_temp3 = 0;                        //  ��������ͬʱ���£�ʱ�����Ĵ���
//	bright_time = 0;   					//  ������ʱ
//	address1 = 0x0010;                  //��λ�����ַ
//	address2 = 0x0020;                  //��λ�����ַ
//	temp1 = 0;                        //������λ������ʱ��żĴ���
//	temp2 = 0;                        //������λ������ʱ��żĴ���
//	DELAY_02S = 0;							//0.2ms delay
//	key_number = 0;                        //����ֵ��żĴ���
//	data_temp = 8;                        //PWM�����洢�Ĵ���
//	dalay_time_10ms = 0;                        //10mS��ɴ����Ĵ���
//	lock_display_time = 0;							//��������ʾ
//	sleep_flag = 0;
//	//	data1           =0; //�˲�����
//	data2 = 0;
//	adresult = 0;
//	VOL_FLAG = 0;
//	Double_flag = 0;
//	//	LOCK=0;				//Ĭ�ϲ�������ÿ�������ϵ��ʾ������������ʱ��LOCK==1
//	number1 = 0;		    //
//	PWM_DATA = 0;         //pwmռ�ձȳ�ֵ
//	AUTO_TEMP_TIME = 0;   //���ܽ���ʱ��
//	SOS1 = 0;
//	SOS2 = 0;
//	SOS_DATA = 0;                        //SOS�ڲ�����Ĵ���
//	FC_FALSH_DATA = 0;
//	data_temp1 = 1;                        //��˸��λPWM������ʱ�洢�Ĵ���
//	data_temp2 = 0;                        //��˸��λPWM������ʱ�洢�Ĵ���
//	temp3 = 0;                        //˫����ɱ�ʾ��ʱ��żĴ���
//	dalay_time_1ms = 0;                        //1mS��ɴ����Ĵ���
//	BAT_VO_CH = 0;
//	FC_FALSH_FLAG = 0;                        //��������״̬�Ĵ���
//	Input_voltage_low = 0;
//	temp5 = 254;                        //��λ��λ��ʱ��żĴ���
//	stop_run_flash = 0;				//�ر�LEDʱ��ֹͣ���б���
//	MIRCOR = 0;                       //��������
//	STOP_RUN_FLASH = 0;			//�ر�LEDʱ��ֹͣ���б���
//	CHARGE_PUTIN_FLAG = 0;			// //��������������
//	CHARGE_DONE_FLAG = 0;			//�����������״̬���
//	DISPLAY_ELE = 0;
//	display_time = 0;
//	time_2s = 0;
//	time_10s = 0;
//	time_2min = 0;
//	time_3min = 0;			//
//	time_2min_flag = 0;
//	time_3_min_flag = 0;
//
//	DOWN_FLAG = 0;				//����λ��������־λ
//	DOWN_ELE = 0;				//������ѹλ
//
//	REMENBER_data_temp1 = 0;		//
//	BAOSHAN_JIANDANG = 0;			//����������־
//	add_flag = 0;					//������ˣ��ͱ�־
//	RESET_LOCK = 0;				//
//	RESET_ELE = 0;				//�ݵ�ѹ��λ��־��λ���
//	CHAR_B = 0;
//	CHAR_C = 0;
//	persent_10_time = 0;
//
//	input_flag1 = 0;
//	Ji_down = 0;			//
//	AD_FLAG = 0;			//
//	AD_FLAG1 = 0;
//	mode_return_flag = 0;  //����ʱ��ģʽ���ر�־
//	DIS_FLAG = 0;			//
//	DIS_TIME = 0;			//
//	DIS_DELAY_500ms = 0;	//
//	//	delay1s=0;			//
//	SLEEP_TIME = 0;		//
//}
////*******************************
////                              *
////  2016.8.18����ok ûȫ����ʾ����         *
////    ���û���⣬                          *
////*******************************
