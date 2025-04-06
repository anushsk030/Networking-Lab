#include<stdio.h>
#include<time.h>
#include<stdlib.h>
void sender();
void reciever();
int ack_error = 0,frame_error = 0,disconnect = 0;
char turn = 's';
int max_packets = 3;
int seq = 0;
void main(){
    srand(time(NULL));
    
    while(disconnect == 0){
        sender();
        reciever();


    }
}
void sender(){
    static int retransmit = 0;
    if(turn == 's'){
        if(seq == max_packets){
            printf("SENDER: All packets sent successfully. Terminating...\n");
            disconnect = 1;
            return;
        }
        if(retransmit == 0){
            printf("SENDER : Sending packet %d  \n",seq);

        }else{
            printf("SENDER : Resending Packet %d\n",seq);
        }
        frame_error = rand() % 4;
        if(frame_error==0){
            printf("SENDER : Packet %d lost while sending \n",seq);
            retransmit = 1;

        }else{
            turn = 'r';
            retransmit = 0;
        }

    }
}

void reciever(){
    static int expected_packet = 0;
    if(turn == 'r'){
        if(frame_error == 0){
            turn = 's';
            return;
        }
        if(seq == expected_packet){
            printf("RECEIVER : Packet %d recieved \n",seq);
            expected_packet++;
        }else{
            printf("RECIEVER : Duplicate packet of %d recieved\n",seq);

        }
        ack_error = rand()%4;
        if(ack_error == 0){
            printf("RECIEVER : Acknowledge lost for packet %d\n",seq);
            turn = 's';
        }else{
            printf("RECIEVER : Acknowledge send for packet  %d\n ",seq);
            seq++;
            turn = 's';
        }

    }
}
