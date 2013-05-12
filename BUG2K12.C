//***********************************************************************************
#include <16F877A.H> // configurar para o controlador a ser usado e o seu caminho
#device ADC=10 // numero de bits do conversor AD - max de 10 bits e min de 8 bits
#use delay(clock=20000000) // informa qual e a frequencia do cristal  que esta sendo utilizado
#fuses HS, NOWDT,PUT,NOBROWNOUT,NOLVP
//***********************************************************************************
// Área de definições de entrada
//***********************************************************************************
#define SensorFrente pin_b0
#define SensorTraseiro pin_b1
#define SensorDireita pin_b2
#define SensorEsquerda pin_b3
//***********************************************************************************
// Área de definições de saídas
//***********************************************************************************
#define FreioDireita pin_d0
#define FreioEsquerda pin_d1
//***********************************************************************************
// Área de definições de valores constantes
//***********************************************************************************
#define Pot1 0
#define Pot2 1
#define SensorLuz 3
#define Iluminacao 300
//***********************************************************************************
// Área de definições de variáveis globais
//***********************************************************************************
long ValorLuz;
//***********************************************************************************
// Área de protótipos de funções
//***********************************************************************************
void Anda();
void Para ();
void Virar();
void Re();
//***********************************************************************************
// Área do programa princial
//***********************************************************************************
void main()
{  
  output_high(FreioDireita);
	output_high(FreioEsquerda);
	setup_adc_ports(RA0_RA1_RA3_ANALOG); // PORT A0 Analogica 
	setup_adc(ADC_CLOCK_INTERNAL); // seleciona o clock interno para o conversor AD
	port_b_pullups(true); // coloca o port b em pullup
	disable_interrupts(INT_EXT); // desabilita a interrupção indicada 	
	setup_timer_2 (T2_DIV_BY_1, 255,1); // timer 2 = 19,53KHz
	setup_ccp1(CCP_PWM); // habilita o PWM 1 a operar
	setup_ccp2(CCP_PWM); // habilita o PWM 1 a operar
	set_adc_channel(SensorLuz); // Leitura do sensor de iluminação 
	delay_us(15); // aguarda o circuito interno do pic se preparar para realizar a conversao
	set_pwm1_duty (512);
	set_pwm2_duty (512);
	while (true)
	{	
		set_adc_channel(SensorLuz); // Leitura do sensor de iluminação 
		delay_us(15); // aguarda o circuito interno do pic se preparar para realizar a conversao
		ValorLuz = read_adc();
		if(ValorLuz > Iluminacao)
		{
			Anda();			
		}
		else
		{
			Para();
			delay_ms(100);
		}
	}
}
void Para ()
{
	set_pwm1_duty(512);
	set_pwm2_duty(512);
	output_high(FreioDireita);
	output_high(FreioEsquerda);
}

void Anda ()
{
	if( input(SensorFrente) == 1 )
	{
		output_low(FreioDireita);
		output_low(FreioEsquerda);
		set_pwm1_duty(800);
		set_pwm2_duty(800);
	}
	else
	{
		Para();
		delay_ms(200);
		Re();
		delay_ms(1100);
		Para();
		delay_ms(700);
		Virar();
		delay_ms(700);
	}		
}

void Re()
{
	output_low(FreioDireita);
	output_low(FreioEsquerda);
	set_pwm1_duty(30);
	set_pwm2_duty(30);
}

void Virar()
{
	while(( !input(SensorDireita) & !input(SensorEsquerda)) == 1)
	{
		Re();
		delay_ms(200);
		Para();				
	}

	if( input(SensorDireita) == 1)
	{
		output_low(FreioDireita);
		output_low(FreioEsquerda);
		set_pwm1_duty(30);
		set_pwm2_duty(1000);
	}
	else
		if( input(SensorEsquerda) == 1)
		{
			output_low(FreioDireita);
			output_low(FreioEsquerda);
			set_pwm1_duty(1000);
			set_pwm2_duty(30);
		}
		delay_ms(900);
		Para();				
}
//***********************************************************************************
// Fim do código principal
//***********************************************************************************
