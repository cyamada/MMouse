#include "SensorConfig.h"

static void PWM_Config();
static void ADC_Config();
static u16 ReadADC1();

static void PWM_Config()
{
  uint16_t PrescalerValue = 0;
  /* -----------------------------------------------------------------------
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
      
      Where, by default, SystemCoreClock = 72 MHz
          
    To get TIM4 counter clock at 24 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
                                              
    To get TIM4 output clock at 21 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM4 counter clock / TIM4 output clock) - 1
           = 1141

    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
  ----------------------------------------------------------------------- */  

  // Compute the prescaler value 
  PrescalerValue = (uint16_t) (SystemCoreClock/(2 * 24000000)) - 1;

  // Time base configuration
  TIM_TimeBaseInitTypeDef &TIM_TimerInitStructure;
  TIM_TimeBaseStructure.TIM_Period = 1141;                                    // Calculated period for 21 KHz
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimerInitStructure);

  // PWM channel 1 configuration 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                   
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;                                   // The duty cycle of the channel
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  // Enable TIM4 counter
  TIM_Cmd(TIM4, ENABLE);
}

static void ADC_Config(void)
{
  ADC_InitTypeDef ADC1_InitStructure;
  ADC_DeInit();

  // Set peripheral clocks
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // Set the ADC for one conversion
  ADC_Struct.ADC_Mode                 = ADC_Mode_Independent            // Keep ADC1 and ADC2 independent
  ADC_Struct.ADC_Resolution           = ADC_Resolution_12b;             // Get 12 bits of data
  ADC_Struct.ADC_DataAlign            = ADC_DataAlign_Right;            // Data stored in lowest 12 bits
  ADC_Struct.ADC_ContinuousConvMode   = DISABLE;                        
  ADC_Struct.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T1_CC1;    // Triggered by Timer 1's CC1
  ADC_Struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Struct.ADC_NbrOfConversion      = 1;
  ADC_Struct.ADC_ScanConvMode         = DISABLE;

  // Setup
  ADC_Init(ADC1, &ADC1_InitStructure);
  ADC_Cmd(ADC1, ENABLE);

  // Calibration
  ADC_ResetCalibration(ADC1);         
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}

u16 ReadADC1(u8 channel)
{
  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);        // Minimum read time
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                     // Start Conversion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);                      // Wait until conversion is finished
  return ADC_GetConversionValue(ADC1);
}