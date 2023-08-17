

class UART {
private:
	//a physical address, look at ic datasheet
	//assign it will trigger certain hardware action to send it. After finishing the sending, uart will trigger UartISR;
	//also store the receive_date in this
	uint8_t *uart_data_; 
	
	//it will be assign 1 by hardware, beacause of there is now data comein. Clear to 0 by user.
	uint8_t *uart_receive_interrupt_flag_;
	
	//it will be assign 1 by hardware when finished the transmit. Clear to 0 by user.
	uint8_t *uart_transmit_finish_intrrupt_flag_;
	
	int baud_rate_;
	
	queue<char> receive_buffer_;
	void (*callback_receive_)();
	
	int transmit_state_; //0<ready> 1<transmit>
	queue<char> transmit_buffer_;
	void (*callback_transmit_finish_)();
	
public:
	UART(int baud_rate);
	bool CheckParityError(); //by hardware
	bool CheckOverflowError(); //byhardware

	void UartISR();

	bool HasRecieveData();
	char GetRecieveChar();

	bool IsTransmitIdle();
	void SendCharBlocking(const char c);
	void SendCharNonBlocking(const char c);
	void FlushTransmitBuffer();
	
};

UART::UART(int baud_rate) {
	uart_data_ = 0x01;
	uart_receive_interrupt_flag_ = 0x02;
	uart_transmit_finish_intrrupt_flag_ = 0x03;
	
	baud_rate_ = baud_rate;
	
	receive_buffer_.clear();
	callback_receive_ = nullptr;
	
	transmit_state_ = 0;
	transmit_buffer_.clear();
	callback_transmit_finish_ = nullptr;
	
	
	//set tx pin
	//set rx pin
	//set uart mode
	//set enable for receive
	//set interrupt enable for uart
}

void UART::UartISR() {
	if (*uart_receive_interrupt_flag_ == 1) {
		*uart_receive_interrupt_flag_ = 0;
		receive_buffer_.push(*uart_data_);
		if (callback_receive_) {
			callback_receive_();
		}
	}
	if (*uart_transmit_finish_intrrupt_flag_ == 1) {
		*uart_transmit_finish_intrrupt_flag_ = 0;
		if (transmit_buffer_.size() > 0) {
			const char c = transmit_buffer_.front();
			transmit_buffer_.pop();
			*uart_data_ = c;
		} else {
			transmit_state_ = 0;
			if (callback_transmit_finish_) {
				callback_transmit_finish_();
			}
		}
	}
}

bool UART::HasRecieveData() {
	return receive_buffer_.size() > 0;
}
char UART::GetRecieveChar() {
	if (receive_buffer_.size() == 0) {
		return 0;
	}
	const char c = receive_buffer_.front();
	receive_buffer_.pop();
	return c;
}

bool UART::IsTransmitIdle() {
	return transmit_state_ == 0;
}
void UART::SendCharBlocking(const char c) {
	while (!IsTransmitIdle()) {
		;
	}
	transmit_state_ = 1;
	*uart_data_ = c;
	while (!IsTransmitIdle()) {
		;
	}	
}
void UART::SendCharNonBlocking(const char c) {
	if (IsTransmitIdle()) {
		transmit_buffer_.push(c);
	} else {
		transmit_state_ = 1;
		*uart_data_ = c;
	}
}
void UART::FlushTransmitBuffer() {
	transmit_buffer_.clear();
}



