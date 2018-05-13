#ifndef CLIENT_H_
#define CLIENT_H_

void *get_in_addr(struct sockaddr *sa);
int createSocket_client();
void bind_client();
void connect_client();

#endif