



class UART {
public:
	UART(const int baud_rate, const int buffer_size);
	bool CheckParityError(); //by specificify register (look at datasheet)
	bool CheckOverflowError(); //by specificify register (look at datasheet)		
	
	// run after receieve any data(include the date beause of transmit)
	void UartISR();
	
	
	bool IsReadyToSend();
	void SendCharBlocking(const char c);
	void SendCharNonblocking(const char c);
	void FlushTransmitBuffer();
	bool hasRecieveData();
	char GetRecieveData();
	
private:

	//need a physics address(look at datasheet), assign it will trigger someting hardware action.
	uint8_t *uart_data_register_;  
	
	//it will be set 1 by hardware beacuse of there are some data in uart_data_register_
	uint8_t *uart_recieve_interrupt_flag_;   
	
	//it will be set to 1 by hardware when uart_data_register_ is written by user
	uint8_t *uart_transmit_interrupt_flag_;  
	
	
	int baud_rate_;
	
	string receieve_buffer_;
	int next_buffer_index_;
	int unprocess_recieve_data_num_ = 0;
	void (*callback_receive_) ();
	
	queue<char> transmit_buffer_;
	void (*callback_transmitFinish_) ();
}


UART::UART(const int baud_rate, const int buffer_size) {
	uart_data_register_ = 0x01; //need a physics address(look at datasheet)
	uart_recieve_interrupt_flag_ = 0x02; //need a physics address(look at datasheet)
	uart_transmit_interrupt_flag_ = 0x03; //need a physics address(look at datasheet)
	
	baud_rate_ = baud_rate;	
	
	receieve_buffer_.resize(buffer_size);
	next_buffer_index_ = 0;
	has_recieve_data_ = 0;
	callback_receive_ = nullptr;
	
	callback_transmitFinish_ = nullptr;
}

void UART::UartISR() {
	if (*uart_recieve_interrupt_flag_) {
		*uart_recieve_interrupt_flag_ = 0;            
		
		receieve_buffer_[next_buffer_index_] = *uart_data_register_;
		next_buffer_index_++;
		
		if (next_buffer_index_ >= receieve_buffer_.size()) {
			next_buffer_index_ = 0;
		}
		unprocess_recieve_data_num_++;
		if (callback_receive_) {
			callback_receive_();
		}
	}

	if (*uart_transmit_interrupt_flag_) {
		*uart_transmit_interrupt_flag_ = 0;

		if (!transmitBuffer_.empty()) {
			const char nextChar = transmitBuffer_.front();
			transmitBuffer_.pop();
			*uart_data_register_ = nextChar;
		} else {
			if (transmitCompleteCallback) {
				transmitCompleteCallback();
			}
		}
	}
}
	
bool UART::IsReadyToSend() {
	return (*uart_transmit_interrupt_flag) > 0;
}

void UART::SendCharBlocking(const char c) {
	while (!IsReadyToSend()) {
		;
	}
	
	*uart_data_register_ = c;
	
	while (!IsReadyToSend()) {
		;
	}		
}

void UART::SendCharNonblocking(const char c) {
	if (transmitBuffer_.empty() && IsReadyToSend()) {
		*uart_data_register_ = c;
	} else {
		transmit_buffer_.push(c);
	}
}

void UART::FlushTransmitBuffer() {
    while (!transmit_buffer_.empty()) {
        transmit_buffer_.pop();
    }
}

bool UART::hasRecieveData() {
	return unprocess_recieve_data_num_ > 0;
}

char UART::GetRecieveData() {
	unprocess_recieve_data_num_ = 0;
	const int last_index = next_buffer_index_ - 1; 
	if (last_index < 0) {
		return receieve_buffer_.back();
	}
	return receieve_buffer_[last_index];
}





UART g_uart(9600, 20);

//-----------------------------------------------------------------------------
//  UART0 ISR
//-----------------------------------------------------------------------------
void UART0_ISR(void) interrupt ISRUart  // Vector @  0x23
{
    g_uart.UartISR();
}


// enable UART0_ISR
#define _UART0_INTERRUPT_ENABLE				IEN0 |= 0x10;
