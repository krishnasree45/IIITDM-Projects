//selective reject sender
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
		int no_of_clients,arr[1000],count,no_of_frames,f_size,f_drop[200],iterator;	
		long long int wait;
		int sockfd,client_fd[10],portno;
 		char buffer[1024];//this is used to take values from server and send values to the server
		struct sockaddr_in serv_addr,cli_addr[10];//to store address of server and client sockets
		int n,err_bits;
 		socklen_t clilen[10];//length of address of sockets
		
		int array[200][200],dividend[1000];
		int rws,sws,seqno,swin[1000];

		int msg[200][200];
		int msg1[200][200];	
		
	public:
		sock(int portno);//to create socket
		void connect_client(int no_of_clients);//to connect with clients
		void random_gen(); // to generate random numbers
		void error(int arr[100][200],int index,int size); // to currupt required number of bits
		void initialization();
		void handshaking();
		void crc12_gen(int array[200][200],int n,int loop);  
		void send_frame_slw(int frame_value);//sending frames 
		void send_frame_sw(int frame_value);
		void recv_ack_slw(int frame_value);//receiving ack
		void recv_ack_sw(int frame_value);//receiving ack
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

void sock::initialization()//for sending no of frames and frame size
{
	cout << "Enter total no: of frames to be sent:";
	cin >> no_of_frames;
	int x=htons(no_of_frames);
	send(client_fd[1],&x,sizeof x,0);

	cout<<"enter the each frame size: ";
	cin>> f_size;
	x=htons(f_size);
	send(client_fd[1],&x,sizeof x,0);

	//cout<<"enter #of bits to corrupt"<<endl;
	//cin>>err_bits;
	err_bits=rand()%f_size;	
	
	cout<<"enter the sws: ";
	cin>> sws;
	x=htons(sws);
	send(client_fd[1],&x,sizeof x,0);

	cout<<"enter the rws: ";
	cin>> rws;
	x=htons(rws);
	send(client_fd[1],&x,sizeof x,0);

	/*seqno=sws+rws;
	x=htons(seqno);
	send(client_fd[1],&x,sizeof x,0);*/
	
	handshaking();
}

void sock::handshaking()//for calculating rtt
{
	int hand[200][200];
	int x;
	long long int y;
	int frame_to_transmit[1000];
	char buf2[1000];
	

	clock_t start_time,end_time;
	start_time=clock();
	for(int loop_index=1;loop_index<=f_size;loop_index++)
	{
		hand[1][loop_index]=rand()%2;
		cout << hand[1][loop_index];
	}
	cout<<endl;
        crc12_gen(hand,f_size,1);
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
	wait=((temp_time*sws)*3); 
	cout<<"wait:"<<wait<<endl;
	y=htonl(wait);
	send(client_fd[1],&y,sizeof y,0);

}

void sock::random_gen()     //  generates  random numbers
{
	int i,x;
	
	
	count=0;
	srand(time(NULL));
		int index=1;
	   for(int index=1;index<=no_of_frames;index++)
	    {
		cout << "index " << index << ":"; 
		for(int loop_index=1;loop_index<=f_size;loop_index++)
		{
			array[index][loop_index]=rand()%2;
			cout << array[index][loop_index];
		}
	        cout << endl;
	    }

	for(int index=1;index<=no_of_frames;index++)//appending seq numbers
	    {
		int z=((index-1)%(sws+rws));
		int t[10];
		for(int i=1;i<=3;i++)
		t[i]=0;
		int p=3;
		while(z>0)
		{
			t[p]=z%2;
			z=z/2;
			p--;
		}
		//cout << "index " << index << ":"; 
		for(int loop_index=f_size+1,i=1;loop_index<=f_size+3;loop_index++,i++)
		{
			array[index][loop_index]=t[i];
			cout << array[index][loop_index];
		}
	        cout << endl;
	    }
 	f_size=f_size+3;

	send_frame_slw(0);//go back n 
}


void sock::send_frame_slw(int frame_value)//sending frames  using go back n 
{
	iterator++;
	cout<<"\nITERATOR:"<<iterator<<" send_frame sliding window"<<endl;
	cout<<"frame value"<<frame_value<<endl; 
 	int arr1[100];
	int x;
	for(int i=1;i<=sws;i++)
	   {
	      if((frame_value+i)<=no_of_frames)
	      {
	       crc12_gen(array,f_size,(frame_value+i));//crc computation
	       cout<<"frame"<<i<<":";
	       for(int j=1;j<=f_size+12;j++)
		{
		 msg[i][j]=dividend[j];
		 cout<<msg[i][j];
		 msg1[i][j]=msg[i][j];
		}
	       cout<<endl;
	      }
	   }
	
	for(int i=0;i<sws;i++)//calculating sender window
	  swin[i]=(frame_value+i)%(sws+rws);
	int choice=rand()%3;   
          
	if(choice==1)//sending corrupted bits
	   {
		cout << "Corrupting..." << endl;
               int no_of_frames_corrupt=rand()%sws+1;//corrutpting no of frames 
		
		for(int i=1;i<=sws;i++)
		arr1[i]=i;
		
		for(int i=1;i<=no_of_frames_corrupt;i++)//which frames to corrupt
		{
		  int k=(rand()%sws)+1;
		  cout<<"k:"<<k<<" ";
			 for(int j=1;j<=sws;j++)
			 {
			   if(k==arr1[j])
			   {
			    arr1[j]=0;
			    error(msg,k,(f_size+12));
			    for(int z=1;z<=f_size+12;z++)
			     msg1[k][z]=msg[k][z];
			    break;
			   }
			   else if(j==sws)
			    i--;
			 }
		}
		cout<<endl;
		sleep(1);
	    cout << "frame drop:";
		for(int i=1;i<=sws;i++)//frame drop
	         {
		   f_drop[i]=0;
		   cout << f_drop[i];
		   x=htons(f_drop[i]);
		   send(client_fd[1],&x,sizeof x,0);
		 }
			int ite;//number of frames to be sent 
			if((no_of_frames-frame_value) >= (sws))
			ite= (sws);
			else if((no_of_frames-frame_value) <sws)
			ite= (no_of_frames-frame_value);
		   f_drop[sws+1]=(ite);
		   cout << f_drop[sws+1];
		   x=htons(f_drop[sws+1]);
		   send(client_fd[1],&x,sizeof x,0);
		cout << endl;

		sleep(1);
	    for(int i=1;i<=sws;i++)
	    { 
		if(f_drop[i]==0 && (frame_value+i)<=no_of_frames)
		{ 
		cout<<"frame "<<i<<": ";
		for(int index=1;index<=f_size+12;index++)//sending corrupted frame
	   	 {
		     cout << msg1[i][index];
		     x=htons(msg1[i][index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		}
	   }
	   cout << endl;
	   recv_ack_slw(frame_value);
	   return;
	 } 
	   
	 else if(choice==0)//not corrupted bits
	     {
		sleep(1);
	        cout << "Not corrupted" << endl;
	        cout << "framedrop:";
		for(int i=1;i<=sws;i++)
	         {
		   f_drop[i]=0;
		   cout << f_drop[i];
		   x=htons(f_drop[i]);
		   send(client_fd[1],&x,sizeof x,0);
		 }
			
			int ite;//number of frames to be sent 
			if((no_of_frames-frame_value) >= (sws))
			ite= (sws);
			else if((no_of_frames-frame_value) <sws)
			ite= (no_of_frames-frame_value);
		f_drop[sws+1]=(ite);
		   cout << f_drop[sws+1];
		   x=htons(f_drop[sws+1]);
		   send(client_fd[1],&x,sizeof x,0);
		cout << endl;

		sleep(1);
               for(int i=1;i<=sws;i++)//sending frames 
	       { 
		if(f_drop[i]==0 && (frame_value+i)<=no_of_frames)
		{
		cout<<"frame not corrupt"<<i<<": ";
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     cout << msg[i][index];
		     x=htons(msg[i][index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		}
	      }
		
	      recv_ack_slw(frame_value);//receiving ack
     	      return;
		
		
	 }
	 if(choice==2)//frame drops 
	     {
		sleep(1);
		cout << "Frame drop" << endl;
		for(int i=1;i<=sws;i++)
	         {
		   f_drop[i]=rand()%2;
		   cout << f_drop[i];
		   x=htons(f_drop[i]);
		   send(client_fd[1],&x,sizeof x,0);
		 }
			
			int ite;
			if((no_of_frames-frame_value) >= (sws))
			ite= (sws);
			else if((no_of_frames-frame_value) <sws)
			ite= (no_of_frames-frame_value);
		   f_drop[sws+1]=(ite);
		   cout << f_drop[sws+1];
		   x=htons(f_drop[sws+1]);
		   send(client_fd[1],&x,sizeof x,0);
		cout << endl;
		sleep(1);
		for(int i=1;i<=sws;i++)
	          { 
		      cout<<"frame "<<i<<": ";
		      if(f_drop[i]==0 && (frame_value+i)<=no_of_frames)
			{
			  for(int index=1;index<=f_size+12;index++)
	   	 	   {
		     	     cout << msg[i][index];
		    	     x=htons(msg[i][index]);
		    	     send(client_fd[1],&x,sizeof x,0);
		 	   }
			  cout << endl;
		       }
	         }
		recv_ack_slw(frame_value);
		return;
	     }

	  
}


void sock::send_frame_sw(int frame_value)//using stop and wait sending frames 
{
	iterator++;
	cout<<"\nITERATOR:"<<iterator<<" send_frame stop and wait"<<endl;
 	int frame_to_transmit[200][200];	
	int x;
		
	  // cout << "entering...." << endl;
	
	   crc12_gen(array,f_size,(frame_value+1));
	   for(int index=1;index<=f_size+12;index++)
	    {
	       frame_to_transmit[1][index]=dividend[index];
	    }
	   int choice=rand()%3;
	   
           
	   if(choice==1)//corrupting and sending
	     {
		cout << "Corrupting..." << endl;
		error(frame_to_transmit,1,f_size+12);
		cout << "Frame " <<frame_value  << ":";
		sleep(1);
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     //frame_to_transmit[index]=msg;
		     cout << frame_to_transmit[1][index];
		     x=htons(frame_to_transmit[1][index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		recv_ack_sw(frame_value);//receive ack by stop and wait 
		return;
	     }
	   else if(choice==0)//not corrupted bits 
	     {
	        cout << "Not corrupted" << endl;
		cout << "Frame " << frame_value << ":";
		sleep(1);
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     cout << frame_to_transmit[1][index];
		     x=htons(frame_to_transmit[1][index]);
		     send(client_fd[1],&x,sizeof x,0);
		 }
		cout << endl;
		recv_ack_sw(frame_value);//receive ack by stop and wait 
		return;
	     }
	  else if(choice==2)
	     {
		cout << "Frame drop" << endl;
		recv_ack_sw(frame_value);
		return;
	     }

	  
}


void sock::recv_ack_slw(int frame_value)//receiving ack
{
	  int x;
	  char buf[100],buf2[100];
	  bzero(buf,100);
	  bzero(buf2,100);
    	  usleep(wait);
	  int temp=recv(client_fd[1],buf2,sizeof buf2,MSG_DONTWAIT);

	  cout << "buf:" << buf2 << endl;
	  if(buf2[0]=='d')//if it is delayed receive another ack/nack
	    {
		temp=recv(client_fd[1],buf,sizeof buf,MSG_DONTWAIT);
		
		cout << "buf2:" << buf << endl;
		
	    }
	  else
	   strcpy(buf,buf2);

	  if(temp==-1)
	   {
	       
		cout<<"timeout..."<<endl;
	       send_frame_slw(frame_value);
	       return;
	   }
	  
	  int pos=-1;
	 
	  if(strncmp(buf,"nack",4)==0)//checking whether the received nack value is in sender window or not 
	   {
		int value=buf[4]-48;
		cout << "Value:" << value << endl;

		for(int i=0;i<sws;i++)
		{
			if(value==swin[i])
			{
			pos=i;
			break;
			}
		}
		if(pos!=-1)//nack value is not there in sender window 
		  {
                     frame_value+=pos;
		     cout << "nack frame Value:" << frame_value <<endl;
		     if(frame_value<no_of_frames)
			  send_frame_sw(frame_value);//as it is nack sending by stop and wait 
			else
			{
			   cout<<endl;		
			   return;
			}
		  }
		else//sending new frames 
		  {
		     frame_value+=sws;

			if(frame_value<no_of_frames)
			  send_frame_sw(frame_value);
			else
			{
			   cout<<endl;		
			   return;
			}
		  }
	   }
	  else if(strncmp(buf,"ack",3)==0)//as it is ack sending by go back n new frames 
	   {
	        frame_value+=sws;
		cout << "frame Value:" << frame_value <<endl;
		if(frame_value<no_of_frames)
		send_frame_slw(frame_value);
		else
		{
			cout<<endl;		
			return;
		}
	   }
	 
	  
}


void sock::recv_ack_sw(int frame_value)//receiving ack by stop and wait 
{
	  clock_t start_time,end_time;
	  //start_time=clock();
	  usleep(wait);
	  //end_time=clock();
	  //cout << "\nEstimated time=" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
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
	       send_frame_sw(frame_value);
	       return;
	   }
	  
	  
	 
	  if(strncmp(buf,"nack",4)==0)//receiving nack
	   {
		int value=ntohl(buf[4]);
		if(value != (frame_value %(sws+rws)))
		frame_value++;
		//else
		if(frame_value<no_of_frames)//transmitting remaining frame
	         send_frame_sw(frame_value);
	       else
		{
			cout<<endl;		
			return;
		}
		 return;
	   }
	  else if(strncmp(buf,"ack",3)==0)//receivng ack 
	   {
	       int value=ntohl(buf[3]);
	       frame_value++;
	       cout << "Value:" << frame_value << endl;

	       if(frame_value<no_of_frames)//transmitting remaining frames as it is ack  by go back n
	         send_frame_slw(frame_value);
	       else
		{
			cout<<endl;		
			return;
		}
	   }
	 
	  
}




void  sock:: error(int arr[100][200],int index,int size)// for  corrupting frames 
{
	int err_pos[200],i;

	
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
				if(arr[index][i]==1)
					arr[index][i]=0;
				else
					arr[index][i]=1;
				
			}
		}
	}
	//cout<<"returning err msg.."<<endl;
	return ;	

}

void sock:: crc12_gen(int array[200][200],int n,int loop)         // performs crc 12
{
	int a[200],errmsg[200],k=0,add=0,r[200],divisor[200],divid[200],temp=0;
	//static int dividend[200];
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
		//cout << "k:" << k << endl;p
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
 	/*cout <<"divid";
	for(int i=1;i<=n+12;i++)
	{
	 cout<<dividend[i];
	}
	cout<<endl;*/
	return ;
	
  
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
