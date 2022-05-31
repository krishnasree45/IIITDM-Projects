#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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



int * four_bit_addition(int a[5],int b[5])    // performs 4-bit addition 
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

void  error(int arr[1000],int size)
{
	int err_pos[1000],i;
	int err_bits=rand()%4+1;
	for(i=1;i<=err_bits;i++)
	{
		err_pos[i]=(rand()%size)+1;
	}
	for(i=1;i<=size;i++)
	{
		for(int j=1;j<=err_bits;j++)
		{
			if(err_pos[j]==i)
			{
				if(arr[i]==1)
					arr[i]=0;
				else
					arr[i]=1;
				
			}
		}
	}	
	
	
}




void checksum_gen(int chk_arr[1000],int n)    // performs checksum
{
	
	int array[50],i,a[5],b[5];
	int *ans;
	bzero(msg_to_be_transmitted,1000);
	/*int t;
	t=htonl(n);
	send(client_fd[1],&t,sizeof t,0); */  // sends array size to client
 
	a[1]=0;
	a[2]=0;
	a[3]=0;
	a[4]=0;
	
        for(int index=1;index<=n;index=index+4)
	  {
	     i=1;
	    
	     for(int index1=index;index1<=index+3;index1++)
		{
		    b[i]=chk_arr[index1];
		    i++;
		}
	     
	     ans=four_bit_addition(a,b);
	     for(int index1=1;index1<=4;index1++)
	        a[index1]=*(ans+index1);
	  }
	
	for(int index=1;index<=n;index++)
	  {
	      msg_to_be_transmitted[index]=chk_arr[index];
	     
	  }
	for(int index=1;index<=4;index++)
	  {
	      if(*(ans+index)==1)
	        msg_to_be_transmitted[n+index]=0;
		//cout<<"in chksum apending bits.."<<msg_to_be_transmitted[n+index];}
	      else
		 msg_to_be_transmitted[n+index]=1;
	        // cout<<"in chksum apending bits.."<<msg_to_be_transmitted[n+index];}
	  }
	int error_rand;
	
	error_rand=rand()%2;
	if(error_rand==0)
	{
		cout<<"corrupted "<<endl;
 		error(msg_to_be_transmitted,n+4);     // corrupts generated bits
			
	}
	else 
	  cout<<"not corrutped "<<endl;
	
	
	
	/*for(int index=1;index<=n+4;index++)
	{
		p=msg_to_be_transmitted[index];
		
		send(client_fd,&p,1,0);       // sends corrupted array to client
	}*/
	

}




int main(int argc,char* argv[])
{
  if(argc !=3)
    {
      cerr << "Usage syntax: ./server <port_no> <file_name>" << endl;
      return 0;
    }
  int sockfd;
  struct sockaddr_in my_addr;

  sockfd=socket(PF_INET,SOCK_STREAM,0);  //Creating the socket

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(atoi(argv[1]));
  my_addr.sin_addr.s_addr=INADDR_ANY;
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=bind(sockfd, (struct sockaddr *)&my_addr, (sizeof my_addr)); //Binding the socket to the address

  if(temp==0)
    cout << "Server started running at "<< inet_ntoa(my_addr.sin_addr)  << ":" << argv[1] << endl;
  else
    {
      cout << "Failed to bind socket" << endl;
      return -1;
    }

  listen(sockfd,10);
  
  struct sockaddr_in client_addr;
  socklen_t client_size=sizeof client_addr;
  int client_fd=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);

  cout << "Accepted incoming connection" << endl;
  
  char buf[256];
  
  //temp=recv(client_fd,buf,sizeof buf,0);
  //cout /*<< "Bytes received=" << temp << endl*/ << "Received data from client=" << buf << endl;
  
  //time_t now=time(0);
  //char serv_msg[1024];
  /*strcpy(serv_msg,ctime(&now));
  cout << "Current Server Time:" << serv_msg << endl;*/
  int nread;
  int no_of_bytes;
  int a=0;
  FILE *fp;
  FILE *fp1;
  int k=0;
  int op[24];
  fp=fopen(argv[2],"r");
  fp1=fopen(argv[2],"r");
  char reading[10000];
  char c;
  srand(time(NULL));
  while(c=(getc(fp1)!=EOF))
  {
	nread=fread(reading ,1,24,fp1);
	no_of_bytes+=nread;
  }
  cout<<"number of bytes:"<<no_of_bytes<<endl;
  int p=htonl(no_of_bytes+1);
   send(client_fd,&p,sizeof p,0);
   nread=0;
   no_of_bytes=0;
   int number=0;
    while(1)
        {
            /* First read file in chunks of 256 bytes */
           bzero(buf,24);
             nread = fread(buf,1,24,fp);
           cout<<"Bytes read  \n"<< nread;  
	   int g=htonl(nread);
	   send(client_fd,&g,sizeof g, 0);
           if(a==0)
           no_of_bytes=nread;
           else      
		   no_of_bytes=nread+no_of_bytes;
		   a++;
	    
            /* If read was success, send data. */
	   cout<<"iterator: "<<number <<endl;
            if(nread > 0)
            {
                
		for(k=1;k<=nread;k++)	
		{
		  op[k]=(buf[k]>>k)&1;
		  //mid_op[k]=atoi(op[k]);
		 // cout<<"op value :"<<op[k]<<" ";
		}
		
		checksum_gen(op,nread) ;
		
		cout<<"sending the frame  "<<number <<" with checksum bits appended"<<endl;
		//cout<<"value ..."<<msg_to_be_transmitted[nread]<<endl;
		for(int m=1;m<=(nread+4);m++)
		{
		  cout<<msg_to_be_transmitted[m]<<" ";
		  int o=htonl(msg_to_be_transmitted[m]);
                  send(client_fd,&o,sizeof o ,0);
		}
		cout<<endl;
            }
	
            /*
             * There is something tricky going on with read .. 
             * Either there was error, or we reached end of file.
             */
            if (nread < 24)
            {
                if (feof(fp))
                    cout<<"End of file"<<endl;
                if (ferror(fp))
                    cout<<"Error reading"<<endl;
                break;
            }

	number++;
        }

  //strcpy(serv_msg,&fp); 
//  temp=send(client_fd,serv_msg,sizeof serv_msg,0);
  cout << "Sent " << no_of_bytes<<endl;
  
  close(client_fd);
   
  close(sockfd);

  return 0;
}
