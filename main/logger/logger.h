// servo.h
#ifndef LOGGER_H
#define LOGGER_H


int initialize_socket();

void log_message(char* controller_name, char* message, char* message_type);

int initialize_time(void);

void accept_clients(void);
#endif // logger_H