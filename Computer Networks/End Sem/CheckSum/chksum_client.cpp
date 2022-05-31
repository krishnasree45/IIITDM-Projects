#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdio.h>
#include<fstream>

using namespace std;

int msg_to_be_transmitted[1000];

int full_adder_sum(int a,int b,int c)
  {
	return a ^ b ^ c;
  }

int full_adder_carry(int a,int b,int c)
  {
	return (a & b) | (b&c) | (c&a);
  }

int * four_bit_addition(int a[5],int b[5])
  {
	int c[5],temp[5];
	static int result[5];
	 
	result[4]=full_adder_sum(a[4],b[4],0);
	c[1]=full_adder_carry(a[4],b[4],0);
	result[3]=full_adder_sum(a[3],b[3],c[1]);
	c[2]=full_adder_carry(a[3],b[3],c[1]);
	result[2]=full_adder_sum(a[2],b[2],c[2]);
	c[3]=full_adder_carry(a[2],b[2],c[2]);
	result[1]=full_adder_sum(a[1],b[1],c[3]);
	c[4]=full_adder_carry(a[1],b[1],c[3]);
	if(c[4]==1)
	 {
	   b[1]=0;
	   b[2]=0;
	   b[3]=0;
	   b[4]=1;
	  for(int index=1;index<=4;index++)
	   a[index]=result[index];
	  result[4]=full_adder_sum(a[4],b[4],0);
	c[1]=full_adder_carry(a[4],b[4],0);
	result[3]=full_adder_sum(a[3],b[3],c[1]);
	c[2]=full_adder_carry(a[3],b[3],c[1]);
	result[2]=full_adder_sum(a[2],b[2],c[2]);
	c[3]=full_adder_carry(a[2],b[2],c[2]);
	result[1]=full_adder_sum(a[1],b[1],c[3]);
	c[4]=full_adder_carry(a[1],b[1],c[3]);
	
	return result;
	}
	else
	 return result;
	
  }

void chk_sum(int array[1000],int n)
{
	int t,a[5],b[5],t1;
	int *ans;
	//recv(sockfd,&t,sizeof t,0);
	//n=ntohl(t);
	
	a[1]=0;
	a[2]=0;
	a[3]=0;
	a[4]=0;
	int msg_to_be_transmitted[1000],i;
	
	for(int index=1;index<=n+4;index++)	
	{
		
		msg_to_be_transmitted[index]=array[index];
	
	}
	
	for(int index=1;index<=n+4;index=index+4)
	  {
	     i=1;
	     for(int index1=index;index1<=index+3;index1++)
		{
		    b[i]=msg_to_be_transmitted[index1];
		    i++;
		}
	     ans=four_bit_addition(a,b);
	     for(int index1=1;index1<=4;index1++)
	        a[index1]=*(ans+index1);
	  }
	int sum=0;
	
	for(int index=1;index<=4;index++)
	{
	 
	 sum=sum+(*(ans+index));
	}
	cout << endl;
	
         if(sum==4)
	 	 cout <<"no error" << endl;	 
	else
	 {
	  cout << "Error " << endl;
	  //err_chksum++;
	 }
}



int main(int argc,char* argv[])
{
  if(argc != 3)
    {
      cerr << "Usage syntax: ./client <ip_address> <port_no>" << endl;
      return 0;
    }
  int sockfd;
  struct sockaddr_in my_addr;

  sockfd=socket(PF_INET,SOCK_STREAM,0);

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(atoi(argv[2]));
  my_addr.sin_addr.s_addr=inet_addr(argv[1]);
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=connect(sockfd,(struct sockaddr*)&my_addr, sizeof my_addr);

  if(temp==0)
    cout << "Connection established with server" << endl;
  else
    {
      cout << "Could not establish connection!" << endl;
      return -1;
    }

  int buf[28];
  bzero(buf,28);
  int bytes_received;
  char rec_data[50];
   //FILE *fp;
    ofstream myfile("module_1.txt");
    //fp = fopen("module_1.txt", "w"); 
    if(!(myfile.is_open()))
    {
        cout<<"Error opening file"<<endl;
        return 1;
    }

    /* Receive data in chunks of 28 bytes */ 
    int no_of_bytes,p;
    recv(sockfd,&p,sizeof p,0);
    no_of_bytes=ntohl(p);
    int no=no_of_bytes,nread;
    cout<<"number of bytes.."<<no_of_bytes<<endl;
    int z=0;
    while(no>0)
    {
       //int k=0;
	cout<<"iterator :"<<z<<endl;
	bzero(buf,24);
	int g;
	nread=0;
	recv(sockfd,&g,sizeof g,0);	
	nread=ntohl(g);
	
       for(int m=1;m<=nread+4;m++)
	{
	  int o;	
         int temp= recv(sockfd,&o,sizeof o ,0);
	  if(temp!=-1)
	  {
	    buf[m]=ntohl(o);
	  //  k++;
	    cout<<buf[m]<<" ";
	    no--;
	  }
	  if(no<0)
	    break;	
	  if(temp==-1)
		 break;
	  
	}
	cout<<"received frame "<< z <<endl;
	chk_sum(buf,nread);
	z++;
    }
   /* if(bytes_received < 0)
    {
        printf("\n Read Error \n");
    }*/

  //recv(sockfd,rec_data,sizeof rec_data,0);
  //cout << "Server's time:" << rec_data << endl;
  close(sockfd);
  return 0;
}
