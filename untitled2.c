void PWM_Config(int period)
{
  uint16_t PrescalerValue = 0;
  /* -----------------------------------------------------------------------
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM4 counter clock at 24 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
                                              
    To get TIM4 output clock at 30 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1

    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
  ----------------------------------------------------------------------- */  

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock/(2 * 24000000)) - 1;

  // Time base configuration
  TIM_TimeBaseInitTypeDef &TIM_TimerInitStructure;
  TIM_TimeBaseStructure.TIM_Period = period;                    
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimerInitStructure);

  // PWM channel configuration
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  // Enable TIM4 counter
  TIM_Cmd(TIM4, ENABLE);
}