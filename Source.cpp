#include <string>
#include<iostream>
#include <bitset>
#include <time.h>
#define BYTE unsigned char
using namespace std;

void Key_init(BYTE Key[]);
BYTE XOR(BYTE target,BYTE Key[]);
BYTE S_Box[]={2,3,0,1};


void Encrypt_ECB(BYTE b[],int len,BYTE Key[]);
BYTE Encrypt(BYTE b,BYTE Key[]);
BYTE Encry_Shift(BYTE target);
BYTE Encry_Permutation(BYTE target);
BYTE Encry_Substitution(BYTE target);
void Encrypt_OFB(BYTE b[],int len,BYTE Key[],BYTE initkey[]);


void Decrypt_ECB(BYTE b[],int len,BYTE Key[]);
BYTE Decrypt(BYTE b,BYTE Key[]);
BYTE Decry_Shift(BYTE target);
BYTE Decry_Permutation(BYTE target);
BYTE Decry_Substitution(BYTE target);
void Decrypt_OFB(BYTE b[],int len,BYTE initkey[]);


void printBytes(BYTE b[], int len) {
	int i;
	for (i=0; i<len; i++)
		printf("%c", b[i]);
	printf("\n");
}
//BYTE Sbox[]={};0.
int main(void)
{
	BYTE KEY[2];
	Key_init(KEY);
	string plaintext;
	int initlen;

	double start,end;

	cout<<"輸入ecb明文:";
	getline(cin,plaintext);
	//BYTE A=(BYTE)malloc(sizeof(BYTE)*plaintext.length());

	BYTE A[10000],B[10000],T[10000];
	cout<<endl;

	for (int i = 0; i <plaintext.length(); ++i)
	{
		A[i]= (BYTE)plaintext.c_str()[i];
	}

	//start = clock();
	//for(int i=0;i<100000;i++)
	//{
	Encrypt_ECB(A,plaintext.length(),KEY);

	cout<<"ecb密文:";
	printBytes(A,plaintext.length());
	cout<<endl;

	/*for (int i = 0; i < plaintext.length(); i++)
	{
	cout<<(char)A[i];
	}*/

	Decrypt_ECB(A,plaintext.length(),KEY);
	//}
	//end = clock();

	//cout<<"ecb解密時間:";
	//cout<<(end-start)/CLOCKS_PER_SEC;
	//cout<<"秒";
	//cout<<endl;
	//cout<<endl;

	cout<<"ecb解密:";
	printBytes(A,plaintext.length());
	cout<<endl;
	cout<<endl;

	BYTE initkey[100000];
	cout<<"輸入ofb明文:";
	getline(cin,plaintext);
	//BYTE A=(BYTE)malloc(sizeof(BYTE)*plaintext.length());

	//BYTE A[10000],B[10000],T[10000];
	cout<<endl;

	for (int i = 0; i <plaintext.length(); ++i)
	{
		A[i]= (BYTE)plaintext.c_str()[i];
	}
	//start = clock();

	//for(int i=0;i<1000000;i++)
	//{
	Encrypt_OFB(A,plaintext.length(),KEY,initkey);

	cout<<"ofb密文:";
	printBytes(A,plaintext.length());
	cout<<endl;

	/*for (int i = 0; i < plaintext.length(); i++)
	{
	cout<<(char)A[i];
	}*/

	Decrypt_OFB(A,plaintext.length(),initkey);
	//}
	//end = clock();

	//cout<<"ofb解密時間:";
	//cout<<(end-start)/CLOCKS_PER_SEC;
	//cout<<"秒";
	//cout<<endl;
	//cout<<endl;

	cout<<"ofb解密:";
	printBytes(A,plaintext.length());
	cout<<endl;
	cout<<endl;
	system("pause");
	return 0;
}
void Key_init(BYTE Key[])
{
	Key[0]=rand()%256;
	Key[1]=rand()%256;
}
BYTE XOR(BYTE target,BYTE Key[])
{
	BYTE temp=target^Key[0];
	return temp;
}


void Encrypt_ECB(BYTE b[],int len,BYTE Key[])
{

	for (int i = 0; i < len; i++)
	{
		b[i]=Encrypt(b[i],Key);
	}
}
BYTE Encrypt(BYTE b,BYTE Key[])
{
	BYTE te[2];
	te[0]=Encry_Shift(b);
	te[0]=Encry_Permutation(te[0]);
	te[0]=XOR(te[0],Key);
	te[0]=Encry_Substitution(te[0]);
	te[0]=Encry_Permutation(te[0]);


	return te[0];
}
BYTE Encry_Shift(BYTE target)
{
	BYTE L=target<<5;
	BYTE R=target>>3;
	BYTE merge=0x0;
	merge=merge|L|R;
	return merge;
}
BYTE Encry_Permutation(BYTE target)
{
	BYTE A=target;
	BYTE L=0x00,R=0x00;
	for (int i = 0; i < 8; i++)
	{
		A=target;
		A=A<<i;
		A=A>>7;
		if (i%2==0)
		{
			R=R<<1;
			R=R|A;
		}
		else
		{
			L=L<<1;
			L=L|A;
		}
	}
	L=L<<4;
	A=R|L;
	return A;
}
BYTE Encry_Substitution(BYTE target)
{
	BYTE A=target,temp,value=0x00;
	for (int i = 0; i < 4; i++)
	{
		temp=A<<i*2;
		temp=temp>>6;
		value=value<<2;
		value=value|S_Box[temp];
	}
	return value;
}



void Decrypt_ECB(BYTE b[],int len,BYTE Key[])
{
	int keyLen = 32;
	for (int i = 0; i < len; i++)
	{
		b[i]=Decrypt(b[i],Key);
	}
}
BYTE Decrypt(BYTE b,BYTE Key[])
{
	BYTE te[2];
	te[0]=Decry_Permutation(b);
	te[0]=Decry_Substitution(te[0]);
	te[0]=XOR(te[0],Key);
	te[0]=Decry_Permutation(te[0]);
	te[0]=Decry_Shift(te[0]);

	return te[0];
}
BYTE Decry_Shift(BYTE target)
{
	BYTE L=target>>5;
	BYTE R=target<<3;
	BYTE merge=0x0;
	merge=merge|L|R;
	return merge;
}
BYTE Decry_Permutation(BYTE target)
{
	BYTE A=0x00;
	BYTE L=target>>4,R=target<<4;
	R=R>>4;
	for (int i = 0; i < 4; i++)
	{
		BYTE temp;
		temp=R<<4+i;
		temp=temp>>7;
		A=A<<1;
		A=A|temp;
		temp=L<<4+i;
		temp=temp>>7;
		A=A<<1;
		A=A|temp;
	}
	return A;
}
BYTE Decry_Substitution(BYTE target)
{
	BYTE A=target,temp,value=0x00;
	for (int i = 0; i < 4; i++)
	{
		temp=A<<i*2;
		temp=temp>>6;
		value=value<<2;
		value=value|S_Box[temp];
	}
	return value;
}

// OFB
void Encrypt_OFB(BYTE b[],int len,BYTE Key[],BYTE initkey[])
{
	string init = "h";
	BYTE A[10000];

	for (int i = 0; i <init.length(); ++i)
	{
		A[i]= (BYTE)init.c_str()[i];
	}

	for (int i = 0; i < len; i++)
	{
		if(i==0)
			initkey[i]=Encrypt(A[i],Key);
		else
			initkey[i]=Encrypt(initkey[i-1],Key);
	}

	for (int i = 0; i < len; i++)
	{
		b[i]=b[i]^initkey[i];
	}
}
void Decrypt_OFB(BYTE b[],int len,BYTE initkey[])
{
	for (int i = 0; i < len; i++)
	{
		b[i]=b[i]^initkey[i];
	}
}