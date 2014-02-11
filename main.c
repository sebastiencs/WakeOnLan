#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int						main()
{
  unsigned char			PaquetMagique[102];
  char					adresse[6];
  int					descripteur;
  int					i = 0;
  struct sockaddr_in	serveur = { 0 };

  printf("- Entrer l'adresse mac: ");
  fgets(adresse, 13, stdin);
  adresse[13] = 0;

  for (i = 0; i < 13; i++)
  {
	  if ((adresse[i] >= 'a') && (adresse[i] <= 'z'))
		  adresse[i] -= 'a' - 10;
	  else if ((adresse[i] >= 'A') && (adresse[i] <= 'Z'))
		  adresse[i] -= 'A' - 10;
	  else if ((adresse[i] >= '0') && (adresse[i] <= '9'))
		  adresse[i] -= '0';
	  else
		  adresse[i] = 0;
  }

  for (i = 0; i < 12; i += 2)
	  adresse[i / 2] = adresse[i] * 16 + adresse[i + 1];

  //Creation du paquet magique
  for (i = 0; i < 6; i++)
	  PaquetMagique[i] = 0xff;
  for (i = 0; i < 96; i++)
	  PaquetMagique[i + 6] = adresse[i % 6];

  descripteur = socket(AF_INET, SOCK_DGRAM, 0);
  if (descripteur == -1) {
	  perror("socket()");
	  return 0;
  }
  if (setsockopt (descripteur, SOL_SOCKET, SO_BROADCAST, (char *)&PaquetMagique, sizeof (PaquetMagique)) < 0)
  {
	  perror("setsockopt()");
	  return 0;
  }
  serveur.sin_family		= AF_INET;
  serveur.sin_port		= htons(9);
  serveur.sin_addr.s_addr	= htonl(0xffffffff); //Adresse broadcast
  if (sendto(descripteur, PaquetMagique, 102, 0, (struct sockaddr *)&serveur, sizeof(serveur)) < 0)
  {
	  perror("sendto()");
	  return 0;
  }

  close(descripteur);
  printf("Paquet envoye !\n");

  return 0;
}
