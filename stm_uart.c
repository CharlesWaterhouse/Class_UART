
UART_HandleTypeDef g_uart1_handle;

void IinitalUart() {
  
  
  __HAL_RCC_GPIOA_CLK_ENABLE();  //enable the clock for this port
  GPIO_InitTypeDef switch_tx_init;
  switch_tx_init.Pin = <pin_id>;           //which pin for output, it is tx or rx base on the pysical pin position(datasheet will present this pin is for tx or rx)
  switch_tx_init.Pull = GPIO_PULLUP;            // set the mode about the pysical design.
  switch_tx_init.Alternate = GPIO_AF7_USART1;    //set this pin how to use. 
  switch_tx_init.Mode = GPIO_MODE_AF_PP; // set the mode about this.
  switch_tx_init.Speed = GPIO_SPEED_FREQ_LOW:   //
  HAL_GPIO_Init(<port_id>, &switch_tx_init);
  
  __HAL_RCC_GPIOB_CLK_ENABLE();  //enable the clock for this port
  GPIO_InitTypeDef switch_rx_init;
  switch_rx_init.Pin = <pin_id>; 
  switch_rx_init.Pull = GPIO_PULLUP;
  switch_rx_init.Alternate = GPIO_AF7_USART1;
  switch_rx_init.Mode = GPIO_AF_PP;
  switch_rx_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(<port_id>, &switch_rx_init);
  
  
  __HAL_RCC_USART1_CLK_ENABLE(); //enable the clock for this uart module
  UART_HandleTypeDef uart1_handle;
  uart1_handle.Instance = USART1;
  uart1_handle.Init.BaudRate = 9600;
  uart1_handle.Init.WordLength = UART_WORDLENGTH_B8;
  uart1_handle.Init.StopBits = UART_STOPBITS_1;
  uart1_handle.Init.Parity = UART_PARTITY_NONE;
  uart1_handle.Init.Mode = UART_MODE_TX_RX;   // do you only use rx or tx or both.
  uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  uart1_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  uart1_handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;   //frequency of uart clock (to be certain multiple of the main clock)
  uart1_handle.Init.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&uart1_handle);
  
  //set the FIFO size in the UART module, or not use FIFO.
  HAL_UARTEx_SetTxFifoThreshold(&uart1_handle, UART_TXFIFO_THRESHOLD_1_8);
  HAL_UARTEx_SetRxFifoThreshold(&uart1_handle, UART_RXFIFO_THRESHOLD_1_8);
  HAL_UARTEx_DisableFifoMode(&uart1_handle);
  
  
  __HAL_UART_ENABLE_IT(&uart1_handle, UART_IT_RXNE); //connect this uart module to interrupt(INT_ID_USART1, USART1_IRQHandler), UART_IT_RXNE is the trigger condiction.
  __HAL_UART_ENABLE(&uart1_handle); //after setting all parameter, enable this uart module.
  

}





void USART1_IRQHandler() {
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();

	if (__HAL_UART_GET_IT(&UartHandleCAN, UART_IT_RXNE) != RESET) {
		u16 data;
		data = LL_LPUART_ReceiveData8(UartHandleCAN.Instance);
		//do something.
		
	} else {
		//finish transmitting or there is some error occurs in uart module.
		LL_USART_ClearFlag_ORE(UartHandleCAN.Instance);
		LL_LPUART_ReceiveData8(UartHandleCAN.Instance);	
	}


	OS_EXIT_CRITICAL();
	OSIntExit();             /* Tell uC/OS-II that we are leaving the ISR */
}



