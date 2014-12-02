



void USART_Config(void)
{
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // Configure USART2 TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Map USART2 to A02
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

    // Initialize USART
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    // Initialize USART2
    USART_Init(USART2, &USART_InitStructure);   
    // Enable USART2
    USART_Cmd(USART2, ENABLE);  
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
    while(*s){
        // Wait until status register is clear before sending next char
        while( !(USARTx->SR & 0x00000040) );
            USART_SendData(USARTx, *s);
            *s++;
    }
}