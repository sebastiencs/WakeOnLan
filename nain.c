#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int	main() {
  unsigned char		PaquetMagique[102];
  int			i = 0;
  char			adresse[6] = {0};
  FILE			*fichier;

  gets(adresse);

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

  fichier = fopen("test2.bin", "wb");
  fwrite(PaquetMagique, 1, sizeof(PaquetMagique), fichier);
  fclose(fichier);

  return 0;
}
