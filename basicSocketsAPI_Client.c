#include <sys/socket.h> // UNIX socket
#include <netinet/in.h>
// #include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

int main()
{
	int sfd, rc;
	char buff[100] = "";
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( sfd < 0 )
	{
		perror("socket call failed\n");
		exit(1);
	}
	// two domains AF_LOCAL for interprocess communication for the same mch
	// AF_INET (IPv4) internet communication
	// type is for the type of socket to be created // SOCK_STREAM = TCP
	// SOCK_DGRAM = UCP and SOCK_RAW these sockets allow acess to some datagrams at the IP layer.
	// protocol field is for which protocol should be used (implicitly specified with the type and set to 0.
	// In some cases (RAW) there are several possible protocols to be sepecified.
	struct sockaddr_in peer; //(AF_INET)struct needed to store info of the peer
	peer.sin_family = AF_INET;
	peer.sin_port = htons(8000);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1");
	rc = connect(sfd, (struct sockaddr *)&peer, sizeof(peer)); // open connection with peer (peer adress and port sotred in sockaddr)
	if (rc)
	{
		perror("connect failed\n");
		exit(1);
	}
	rc = send(sfd, buff, 100, 0); // like write in UNIX write info to the socket fd
	if (rc <= 0)
	{
		perror( "send call failed");
		exit( 1 );
	}
	rc = recv( sfd, buff, 100, 0 );
	if ( rc <=0 )
		perror( "recv call failed" );
	else
		printf( "%s\n", buff );
	exit( 0 );
}
