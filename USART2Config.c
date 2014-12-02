#include "USART2Config.h"

static void USART2_Config();
static void USART_puts();


static void USART2_Config(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                // Structure for output pin
    USART_InitTypeDef USART2_InitStructure;                             // Structure for USART2

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);         // Enable periph clock for TX pin
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    // Enable periph clock for USART2

    // Configure USART2 TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       // TX on pin A02
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      // Alternating function
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // Output type is pushpull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      // Internal pullup
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Map USART2 to A02
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

    // Initialize USART
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;                     // Configured for TX only
    
    // Initialize USART2
    USART_Init(USART2, &USART2_InitStructure);   
    
    // Enable USART2
    USART_Cmd(USART2, ENABLE);  
}

void USART_puts(USART_TypeDef* USARTx, const volatile char *s)
{
    while(*s){
        // Wait until status register is clear before sending next char
        while( !(USARTx->SR & 0x00000040) );
            USART_SendData(USARTx, *s);
            *s++;
    }
}

