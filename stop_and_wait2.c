#include <stdio.h>
#include <stdlib.h>

struct Frame {
    int info;
    int seq;
};

struct Frame packet;
int ack, max_packets = 5;
char turn = 's';  // 's' -> Sender's turn, 'r' -> Receiver's turn
int frame_error = 0, ack_error = 0;
int disconnect = 0;

void sender();
void receiver();

int main() {
    packet.info = 0;
    packet.seq = 0;
    
    while (!disconnect) {
        sender();
        receiver();
    }
    
    return 0;
}

void sender() {
    static int first_packet = 1;
    static int retransmit = 0; // Flag to check if we need to resend the same packet

    if (turn == 's') {
        if (!ack_error) {
            if (!first_packet && !retransmit) {
                printf("SENDER: Received ACK for packet %d\n", ack);
            }

            if (!retransmit) { // Move to the next packet only if no retransmission is needed
                if (packet.seq == max_packets) {
                    printf("SENDER: All packets sent successfully. Terminating...\n");
                    disconnect = 1;
                    return;
                }
                packet.info++;
                packet.seq++;
            }

            printf("SENDER: Sent packet with seq NO: %d\n", packet.seq);
            frame_error = rand() % 4; // 1 in 4 chance of frame error

            if (frame_error == 0) {
                printf("SENDER: Error while sending Packet!\n");
                retransmit = 1; // Mark for retransmission
            } else {
                retransmit = 0; // Reset retransmission flag
            }

            turn = 'r';
            first_packet = 0;
        } else {
            printf("SENDER: Retransmitting packet %d (Previous ACK was lost)\n", packet.seq);
            frame_error = rand() % 4;
            if (frame_error == 0) {
                printf("SENDER: Error while resending Packet!\n");
            }
            turn = 'r';
        }
    }
}

void receiver() {
    static int expected_seq = 1;

    if (turn == 'r') {
        if (frame_error == 0) {
            printf("RECEIVER: Packet %d lost. Requesting retransmission...\n", expected_seq);
            turn = 's'; // Allow sender to retransmit
            return;
        }

        if (packet.seq == expected_seq) {
            printf("RECEIVER: Received packet with seq NO: %d\n", packet.seq);
            ack = packet.seq;
            expected_seq++;
        } else {
            printf("RECEIVER: Duplicate packet received! Resending ACK for seq NO: %d\n", expected_seq - 1);
            ack = expected_seq - 1;
        }

        ack_error = rand() % 4; // 1 in 4 chance of ACK error
        if (ack_error == 0) {
            printf("RECEIVER: Error while sending ACK!\n");
        } else {
            printf("RECEIVER: ACK sent for seq NO: %d\n", ack);
        }
        turn = 's';
    }
}

