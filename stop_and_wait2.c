#include <stdio.h>
#include <stdlib.h>

struct Frame {
    int seq;  // Sequence number of the packet
};

struct Frame packet;
int ack, max_packets = 5;
char turn = 's';      // 's' -> Sender's turn, 'r' -> Receiver's turn
int frame_error = 0;  // Simulate packet loss
int ack_error = 0;    // Simulate ACK loss
int disconnect = 0;   // Stop condition

void sender();
void receiver();

int main() {
    packet.seq = 0; // Initialize sequence number
    
    while (disconnect == 0) {
        sender();
        receiver();
    }
    
    return 0;
}

void sender() {
    static int retransmit = 0; // Flag for retransmission

    if (turn == 's') {  
        if (packet.seq == max_packets) { // If all packets are sent, terminate
            printf("SENDER: All packets sent successfully. Terminating...\n");
            disconnect = 1;
            return;
        }

        if (retransmit == 0) { // Normal packet sending
            printf("SENDER: Sending packet %d\n", packet.seq);
        } else { // Retransmitting last packet due to ACK loss
            printf("SENDER: Retransmitting packet %d (ACK was lost)\n", packet.seq);
        }

        frame_error = rand() % 4; // 1 in 4 chance of packet loss

        if (frame_error == 0) {  
            printf("SENDER: Packet %d lost. Retransmitting...\n", packet.seq);
            retransmit = 1; // Set retransmit flag
        } else { 
            turn = 'r'; // Hand over control to receiver
        }
    }
}

void receiver() {
    static int expected_seq = 0;

    if (turn == 'r') {
        if (frame_error == 0) { // If packet was lost
            turn = 's'; // Ask sender to retransmit
            return;
        }

        if (packet.seq == expected_seq) { // If expected packet received
            printf("RECEIVER: Received packet %d\n", packet.seq);
            ack = packet.seq;
            expected_seq++; // Move to next expected packet
        } else { // If duplicate packet received
            printf("RECEIVER: Duplicate packet! Resending ACK for %d\n", expected_seq - 1);
            ack = expected_seq - 1;
        }

        ack_error = rand() % 4; // Simulate ACK loss
        if (ack_error == 0) {
            printf("RECEIVER: ACK lost for packet %d\n", ack);
            turn = 's'; // Sender should retransmit the last packet
        } else {
            printf("RECEIVER: ACK sent for packet %d\n", ack);
            packet.seq++;   // Move to the next packet only if ACK was received
            turn = 's'; // Switch turn back to sender
        }
    }
}


