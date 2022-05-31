//stop and wait simulating all 7 models

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <ctype.h>
# include <ctime> //clock()
#include<pthread.h>
#include<math.h>
using namespace std;

class sock
{
 	private:
		int no_of_clients,arr[1000],count,no_of_frames,f_size,sws,rws;
		long long int wait;	
		int sockfd,client_fd[10],portno;
 		char buffer[1024];//this is used to take values from server and send values to the server
		struct sockaddr_in serv_addr,cli_addr[10];//to store address of server and client sockets
		int n,err_bits;
 		socklen_t clilen[10];//length of address of sockets
		
		int array[1000][1000],dividend[1000];

		int exp_mean;
		int iterator;

	public:
		sock(int portno);//to create socket
		void connect_client(int no_of_clients);//to connect with clients
		void handshaking();
		void random_gen(); // to generate random numbers
		int * error(int arr[1000],int size); // to currupt required number of bits
		void initialization();
		int * crc12_gen(int crc12_arr[1000][1000],int n,int loop);
		void recv_ack(int frame_value);
		void send_frame(int frame_value);
		int calc(long double num);
};


sock::sock(int portno)
{
 	sockfd=socket(AF_INET,SOCK_STREAM,0);//creating a socket

 	if(sockfd<0)
 	{
		cout<<"\nerror:opening socket\n";
		exit(1);
 	}

 	bzero((char*)&serv_addr,sizeof(serv_addr));//bzero helps in clearing the values already present in serv_addr

 	serv_addr.sin_family=AF_INET;//IP address format
 	serv_addr.sin_addr.s_addr=INADDR_ANY;//get your address on your own ie this pc address itself
 	serv_addr.sin_port=htons(portno);
  
 	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
 	{
 		cout<<"error on binding\n";
		exit(1);
 	}

	connect_client(1);
} 

void sock::connect_client(int no_of_clients)  // to connect to client 
{
	 int no,port;
	 listen(sockfd,no_of_clients);//at a time this server can handle given no_of_clients
	 no=0;
 	
            for(int i=1;i<=no_of_clients;i++)
 	{
		clilen[i]=sizeof(cli_addr[i]);
		client_fd[i]=accept(sockfd,(struct sockaddr*)&cli_addr[i],&clilen[i]);
 		
               if(client_fd[i]<0)
 		{
	 	 cout<<"error:accepting client"<<i<<endl;
	 	 exit(1);
 		}
 	       else
 		{
	 	 char *ip=inet_ntoa(cli_addr[i].sin_addr);//to get clients ip address it is saved in client address
	 	 int port=ntohs(cli_addr[i].sin_port);//to get port no with which client got connected to server
 	 	 cout<<"\nconnected successfully with client"<<i<<". IP of client:"<<ip<<". port:"<<port<<endl;
	 	 no++;
 		}
 	}
 	
            if(no==no_of_clients)
 		cout<<"\nall the clients are connected\n";
 	    else
 	   {
		cout<<"error:some clients are not connected\n";
		exit(1);
 	   }
}

void sock::initialization()
{
	cout << "Enter total no: of frames to be sent:";
	cin >> no_of_frames;//total number of frames 
	int x=htons(no_of_frames);
	send(client_fd[1],&x,sizeof x,0);
	cout<<"enter the each frame size: ";
	cin>> f_size;//frame size
	x=htons(f_size);
	send(client_fd[1],&x,sizeof x,0);
	cout<<"enter #of bits to corrupt"<<endl;
	cin>>err_bits;// number of bits to corrupt
	iterator =0;
	handshaking();
}

void sock::handshaking()//to calculate rtt 
{
	int hand[1000][1000];
	int *msg;
	int x;
	long long int y;
	int frame_to_transmit[1000];
	char buf2[1000];
	sws=1;

	clock_t start_time,end_time;
	  start_time=clock();
	cout<<"handshaking to calculate timeout:"<<endl;
	cout<<"h_frame:";

	srand(time(NULL));
	for(int loop_index=1;loop_index<=f_size;loop_index++)
	{
		hand[1][loop_index]=rand()%2;
		cout << hand[1][loop_index];
	}
	cout<<endl;

	msg=crc12_gen(hand,f_size,1);
	for(int index=1;index<=f_size+12;index++)
         {
          frame_to_transmit[index]=dividend[index];
         }

	cout << "H_Frame "<< ":";
	sleep(1);
	for(int index=1;index<=f_size+12;index++)
	    {
	     cout << frame_to_transmit[index];
	     x=htons(frame_to_transmit[index]);
	     send(client_fd[1],&x,sizeof x,0);
            }
	cout << endl;

	int temp=recv(client_fd[1],buf2,sizeof buf2,0);
         cout << "buf2:" << buf2 << endl;

	end_time=clock();
	double temp_time= (double)(end_time-start_time)/CLOCKS_PER_SEC ;
	temp_time=temp_time * pow(10,9);
	cout << "\nEstimated time=" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
	//cout<<"size:"<<sizeof(temp_time);
	wait=((temp_time*sws)*2); 
	cout<<"wait:"<<wait<<endl;
	y=htonl(wait);
	send(client_fd[1],&y,sizeof y,0);

}

void sock::random_gen()     //  generates  random numbers
{
	int i,x;
	exp_mean=0.5;//exponential distribution mean value
	
	count=0;
	srand(time(NULL));
	int index=1;
	long double exp;
	int m;
        for(int index=1;index<=no_of_frames;index++)
	    {
		exp=drand48();
		m=calc(exp);
		usleep(m);//making the frame to wait
		cout<<"generating frame :"<< index <<endl;
		cout << "index " << index << ":"; 
		for(int loop_index=1;loop_index<=f_size;loop_index++)
		{
			array[index][loop_index]=rand()%2;
			cout << array[index][loop_index];
		}
	        cout << endl;
	    }
	cout<<endl;
	cout<<endl;
	send_frame(1);
}

int sock::calc(long double num)//calculating the time for a frame to wait
{
	int op;
	int dummy=(1/(1-num));
	op=(exp_mean)*log(dummy);
	return op;
}

void sock::send_frame(int frame_value)//sending frame
{
 	int frame_to_transmit[1000];
	int *msg;	
	int x;
	cout<<endl;
	cout<<endl;
		cout<<"iterator..:"<<iterator<<endl;
	  // cout << "entering...." << endl;
	
	msg=crc12_gen(array,f_size,frame_value);
        for(int index=1;index<=f_size+12;index++)
	    {
	       frame_to_transmit[index]=dividend[index];
	    }
        int choice=rand()%3;//generating choices i.e corrutption ,no corruption ,timeouts
	   
           iterator++;
        if(choice==1)//sending corrupted bits
	     {
		cout << "Corrupting..." << endl;
		msg=error(frame_to_transmit,f_size+12);
		cout << "Frame " <<frame_value  << ":";
		sleep(1);
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     frame_to_transmit[index]=*(msg+index);
		     cout << frame_to_transmit[index];
		     x=htons(frame_to_transmit[index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		recv_ack(frame_value);//receiving ack
		return;
	     }
	 else if(choice==0)//no corruption
	     {
	        cout << "Not corrupted" << endl;
		cout << "Frame " << frame_value << ":";
		sleep(1);
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     cout << frame_to_transmit[index];
		     x=htons(frame_to_transmit[index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		recv_ack(frame_value);//receiving ack
		return;
	     }
	 else if(choice==2)//frame drops
	     {
		cout << "Frame drop" << endl;
		recv_ack(frame_value);
		return;
	     }

	  
}

void sock::recv_ack(int frame_value)//receiving ack
{
	  clock_t start_time,end_time;
	  start_time=clock();
	  usleep(wait);
	  end_time=clock();
	  cout << "\nEstimated time=" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
	  char buf[100],buf2[100];
	  bzero(buf,100);
	  bzero(buf2,100);
	  int temp=recv(client_fd[1],buf2,sizeof buf2,MSG_DONTWAIT);
	  cout << "buf2:" << buf2 << endl;
	  if(buf2[0]=='d')//if it is delayed 
	    {
		temp=recv(client_fd[1],buf,sizeof buf,MSG_DONTWAIT);
		cout << "buf:" << buf << endl;
		//send_frame(frame_value);
	    }
	  else
	   strcpy(buf,buf2);

	  if(temp==-1)//not received so timeout
	   {
	       
               cout<<"timeout..."<<endl;
	       send_frame(frame_value);
	       return;
	   }
	  
	  
	 
	  if(strncmp(buf,"nack",4)==0)//receiving nack
	   {
		 send_frame(frame_value);
		 return;
		//loop--;
	   }
	  else if(strncmp(buf,"ack",3)==0)//receivng ack 
	   {
	       int value=ntohl(buf[3]);
	       frame_value++;
	       cout << "Value:" << frame_value << endl;

	       if(frame_value<=no_of_frames)//transmitting remaining frames
	         send_frame(frame_value);
	       else
		{
			cout<<endl;		
			return;
		}
	   }
	 
	  
}


int * sock:: error(int arr[1000],int size)//for corrupting bits
{
	int err_pos[1000],i;
	
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
	cout<<"returning err msg.."<<endl;
	return arr;	

}


int * sock:: crc12_gen(int array[1000][1000],int n,int loop)         // performs crc 12
{
	int a[1000],errmsg[1000],k=0,add=0,r[1000],divisor[1000],divid[1000],temp=0;
	//static int dividend[1000];
	//int n1;
	//n1=htons(n);
	//send(client_fd[1],&n1,sizeof n1,0);//sending the value of n
	
	
	for(int i=1;i<=n;i++)
	  dividend[i]=array[loop][i];
	
	for(int i=n+1;i<=n+12;i++)       // appending with 12 zeroes
	 dividend[i]=0;
	
	for(int i=1;i<=n+12;i++)
	divid[i]=dividend[i];
	/*cout << "divid ourside do:";
		for(int j=1;j<=n+12;j++)
		cout << divid[j];
		cout << endl;*/
	do
	{                                   // dividing with divisor polynomial
	add=0;
		temp++;

		for(int z=1;z<=n+12;z++)
		divisor[z]=0;

		for(int i=1;i<=n+12;i++)
		{
		  /*cout << "divid:";
		for(int j=1;j<=n+12;j++)
		cout << divid[j];
		cout << endl;*/
		if(divid[i]==1)
		{
		k=i;
		//cout << "k:" << k << endl;
		divisor[k]=1;
		divisor[k+1]=1;
		divisor[k+2]=0;
		divisor[k+3]=0;
		divisor[k+4]=0;
		divisor[k+5]=0;
		divisor[k+6]=0;
		divisor[k+7]=0;
		divisor[k+8]=0;
		divisor[k+9]=1;
		divisor[k+10]=1;
		divisor[k+11]=1;
		divisor[k+12]=1;
		break;
		}
		}

		for(int i=1;i<=n+12;i++)
		{
		if(divisor[i]==divid[i])
		r[i]=0;
		else
		r[i]=1;
		}
	       /*cout << "rem:";
		for(int i=1;i<=n+12;i++)
		{
		cout << r[i];
		
		}
		cout << endl;*/
		for(int i=1;i<=n+12;i++)
		{
		divid[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)//n+7-2
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}
		for(int z=1;z<=n+12;z++)
		divisor[z]=0;

	}while(add!=n);
	

	for(int i=1;i<=n+12;i++)       
	{
	if(dividend[i]==r[i])
	dividend[i]=0;
	else
	dividend[i]=1;
	}

	/*for(int i=n+1;i<=n+12;i++)
	{
	 dividend[i]=r[i];
	}*/
	
	return dividend;
	/*error(dividend,n+12);



	int x;
	for(int i=1;i<=n+12;i++)
	{
	x=htons(dividend[i]);
	send(client_fd[1],&x,sizeof x,0);     // sending corrupted array to client
	}*/
  
}


int main(int argc,char *argv[])
{
 	int portno;
 	if(argc<2)//port number will be passed during run time
 	{
   		fprintf(stderr,"error: no port provided\n");
   		exit(1);
 	}

 	portno=atoi(argv[1]);
 	
 	sock sock1(portno);
	
	sock1.initialization();
	
	sock1.random_gen();
	
	return 0;
}
