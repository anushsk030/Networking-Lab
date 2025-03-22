#include <stdio.h>  // Standard I/O functions
#include <stdlib.h> // For rand() function

// Global variables
int info = 0, seq = 0;  
int ack, max_packets = 5;  
char turn = 's';  // 's' -> Sender's turn, 'r' -> Receiver's turn
int frame_error = 0, ack_error = 0;
int disconnect = 0;
int timeout = 3;  // Timeout counter to force retransmission

// Function prototypes
void sender();
void receiver();

int main() {
    while (!disconnect) {
        sender();
        receiver();
    }
    return 0;
}

void sender() {
    static int first_packet = 1;  
    static int wait_count = 0;  // Counter for retransmission

    if (turn == 's') {  
        if (!ack_error) {  // If last acknowledgment was received correctly
            if (!first_packet) {  
                printf("SENDER: Received ACK for packet %d\n", ack);
            }

            if (seq == max_packets) {  
                printf("SENDER: All packets sent successfully. Terminating...\n");
                disconnect = 1;  
                return;
            }

            // Send new packet
            info++;
            seq++;
            printf("SENDER: Sent packet with seq NO: %d\n", seq);

            // Simulate packet loss
            frame_error = rand() % 4;
            if (frame_error == 0) {
                printf("SENDER: Error while sending Packet!\n");
            }

            turn = 'r';  
            first_packet = 0;  
            wait_count = 0;  // Reset timeout counter
        } 
        else {  
            // If previous ACK was lost, retransmit packet
            printf("SENDER: Retransmitting packet %d (Previous ACK was lost)\n", seq);
            frame_error = rand() % 4;
            if (frame_error == 0) {
                printf("SENDER: Error while resending Packet!\n");
            }
            turn = 'r';  
        }
    } 
    else {  
        // Timeout mechanism to force retransmission
        wait_count++;
        if (wait_count >= timeout) {  
            printf("SENDER: Timeout! Retransmitting packet %d\n", seq);
            wait_count = 0;  
            turn = 's';  // Force retransmission
        }
    }
}

void receiver() {
    static int expected_seq = 1;  

    if (turn == 'r') {  
        if (frame_error == 0) {  
            printf("RECEIVER: Packet %d lost. Waiting for retransmission...\n", expected_seq);
            turn = 's';  // Let sender retransmit
            return;
        }

        if (seq == expected_seq) {  
            printf("RECEIVER: Received packet with seq NO: %d\n", seq);
            ack = seq;  
            expected_seq++;  
        } 
        else {  
            printf("RECEIVER: Duplicate packet received! Resending ACK for seq NO: %d\n", expected_seq - 1);
            ack = expected_seq - 1;  
        }

        // Simulate ACK loss
        ack_error = rand() % 4;  
        if (ack_error == 0) {
            printf("RECEIVER: Error while sending ACK!\n");
        } 
        else {
            printf("RECEIVER: ACK sent for seq NO: %d\n", ack);
        }

        turn = 's';  
    }
}
