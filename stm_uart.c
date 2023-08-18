
UART_HandleTypeDef g_uart1_handle;

void IinitalUart() {
  
  GPIO_InitTypeDef switch_tx_init;
  switch_tx_init.Pin = SWITCH_TX_Pin;           //which pin for output, it is tx or rx base on the pysical pin position(datasheet will present this pin is for tx or rx)
  switch_tx_init.Pull = GPIO_PULLUP;            // set the mode about the pysical design.
  switch_tx_init.Alternate = GPIO_AF7_USART1;    //set this pin how to use. 
  switch_tx_init.Mode = GPIO_MODE_AF_PP; // set the mode about this.
  switch_tx_init.Speed = GPIO_SPEED_FREQ_LOW:   //
  HAL_GPIO_Init(SWITCH_TX_GPIO_Port, &switch_tx_init);
  
  GPIO_InitTypeDef switch_rx_init;
  switch_rx_init.Pin = SWITCH_RX_Pin; 
  switch_rx_init.Pull = GPIO_PULLUP;
  switch_rx_init.Alternate = GPIO_AF7_USART1;
  switch_rx_init.Mode = GPIO_AF_PP;
  switch_rx_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SWITCH_RX_GPIO_Port, &switch_rx_init);
  
  
  
  g_uart1_handle.Instance = USART1;
  g_uart1_handle.Init.BaudRate = 9600;
  g_uart1_handle.Init.WordLength = UART_WORDLENGTH_B8;
  g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
  g_uart1_handle.Init.Parity = UART_PARTITY_NONE;
  g_uart1_handle.Init.Mode = UART_MODE_TX_RX;   // do you only use rx or tx or both.
  g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  g_uart1_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  g_uart1_handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;   //frequency of uart clock (to be certain multiple of the main clock)
  g_uart1_handle.Init.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&uart1_handle);
  
}


