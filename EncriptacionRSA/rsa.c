#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

 long	phi	,
		M		,
		n		,
		expClavePublica,
		d		,
		C		;

int mcd (int a, int b)
{
	int c;

	while (a != b)
	{
		if (a > b)
			a = a - b;
		else
			b = b - a;
	}

	c = a;
	return c;
}

int sonCoprimos(int a, int b)
{
	return mcd(a, b) == 1;
}

// Comprobamos si un número es primo o no
int esPrimo(int numero)
{
  int res = 0;
  int divs = 2;
  int mitad;
  
  if (numero > 2)    // Si el número es mayor a 2, comprobamos si es primo.
    {
      /* Para ahorrar sólo comprobamos si un número es divisible hasta su mitad, ya que 7/2 = 3.xxx, 7/3=2.xxx, 7/4=0.xxx */
      /* cuando empezamos a obtener valores menores que 0, ya no encontraremos más divisores enteros */
      mitad = floor( (double) numero / 2);
      do
    {
      res = (numero % divs != 0);  /* ¿ El resto es 0 ? */
      //      printf("Divido %d entre %d. Resto %d. Res: %d\n", numero, divs, numero%divs, res);
      divs++;

    } while ((res != 0) && (divs <= mitad));
    }
  else 
	  if (numero <= 2 && numero > 0) 
		res = 1;

  return res;
}

void genera_primos (int cantidad, int resultado[])
{
  int primos = 0;
  int i;
  time_t seconds;

  seconds = time (NULL); 
  srand((int) seconds);

  i = (int) rand() / 100;

  while (primos < cantidad)
    {
      // Vamos comprobando uno a uno si es primo
      if (esPrimo(i))
    {
      resultado[primos] = i;
      primos++;
    }
      i++;
    }
}

int generaClavePublica(int phi)
{
	time_t seconds;
	int expClavePublica;

	seconds = time (NULL); 
	srand((int) seconds);

	do 
	{
		expClavePublica = (int) rand() / 100;

		if(expClavePublica >= phi )
			expClavePublica /= 7;

	}	while ( sonCoprimos(phi, expClavePublica) != 1 || expClavePublica >= phi );

	return expClavePublica;
}

int calculaClavePrivada(int phi)
{
	int s;
	int d = 1;
	
	do
	{
		s = (d * expClavePublica ) % phi;
		d++;
	} while(s != 1);
	
	d = d - 1;

	return d;
}

int generaPhi(int p, int q)
{
	return (p-1) * (q-1);
}

void obtenerPhi_y_N()
{
	int dosPrimos[2];
	int p, q;
	genera_primos(2, dosPrimos);

	p = dosPrimos[0];
	q = dosPrimos[1];

	printf("Primos: %d %d\n", p, q);
	phi = generaPhi(p, q);

	n = (p * q);
}

int encrypt(char M)
{
	int i;
	C = 1;
	
	//printf("\n\tNot encrypted keyword : %d",C);
	for(i = 0; i < expClavePublica; i++)
		C = C * M % n;
	
	C = C % n;
	//printf("\n\tEncrypted keyword : %d",C);
	return C;
}

int decrypt(int C)
{
	int i;
	M = 1;
	
	for(i = 0; i < d; i++)
		M = M * C % n;
	M = M % n;
	//printf("\n\tDecrypted keyword : %d",M);
	return M;
}

void encryptString(char cad[], long result[])
{
	int i = 0;
	int length = strlen(cad);

	while(i < length)
	{
		result[i] = encrypt(cad[i]);
		i++;
	}

}

void decryptString(char cad[], long result[])
{
	int i = 0;
	int length = strlen(cad);

	while(i < length)
	{
		cad[i] = (char) decrypt(result[i]);
		i++;
	}

}

void aplicarRSA(char cad[])
{
	long result[5000];
	unsigned int i;

	obtenerPhi_y_N();
	printf("\n\tF(n)\t= %d",phi);

	expClavePublica = generaClavePublica(phi);

	printf("\ne: %ld", expClavePublica);
	
	d = calculaClavePrivada(phi);
	
	printf("\n\tPublic Key\t: {%d,%d}",expClavePublica,n);
	printf("\n\tPrivate Key\t: {%d,%d}",d,n);

	printf("\nOrig: %s", cad);
	encryptString( cad, result);
	
	printf("\nCifrado:\n");
	for(i = 0; i < strlen(cad); i++)
		printf("%d", result[i]);
	printf("\n");

	decryptString( cad, result);
	printf("\nDescifrado: %s", cad);
}

void main()
{	
	char hola[5000];
	

	puts("Introduzca la frase a cifrar: ");
	gets(hola);

	aplicarRSA(hola);

	system("pause");
}
