#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <sstream>
using namespace std;

class client
{
 private:
	int sockfd,portno,f_size,no_of_frames;
	long long int wait;
 	struct sockaddr_in serv_addr;
 	struct hostent *server;
 	char buffer[256];//this is used to take values from server and send values to the server
	float prob;	
	int delayed_ack[2];	
	int max_dack;
	int iterator;

 public:
	client(int portno,char *ser);
	void handshaking();
	void simulate();
	int crc12(int n);
	int err_crc_12;
	void send_ack(int frame_value);
};

client::client(int portno,char *ser)
{
 	int option;

 	sockfd=socket(AF_INET,SOCK_STREAM,0);//creating a socket

 	if(sockfd<0)
 	{
		cout<<"error:opening socket\n";
		exit(1);
 	}
 
 	server=gethostbyname(ser);//servers ip address
 	if (server==NULL)
 	{
 		fprintf(stderr,"error no such host\n");
   		exit(0);
 	}

 	bzero((char*)&serv_addr,sizeof(serv_addr));//clear values in serv_addr

 	serv_addr.sin_family=AF_INET;
 	bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);//copying server address
 	serv_addr.sin_port=htons(portno);
 
 	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
 	{
 		cout<<"error connecting\n";
        	exit(1);
 	}
	
	cout<<"\nconnection established..."<<endl;
}

void client::simulate()
{
	int x,is_error;
	char buf[50];

	prob=drand48()*10000000000000.0;//generating number b/w 0 to 1
	cout<<"probability ..."<<prob<<endl;
	for(int i=0;i<2;i++)
	delayed_ack[i]=0;
	max_dack=0;
	recv(sockfd,&x,sizeof x,0);
 	no_of_frames=ntohs(x);//number of frames 
	recv(sockfd,&x,sizeof x,0);
 	f_size=ntohs(x);//frame size
	cout<<"frames_size"<<f_size<<endl;
	max_dack=(1/prob);//number of maximum number of dacks/dnacks for frame 1 and 2
	cout<<"number of dacks ..."<<max_dack<<endl;
	handshaking();
	iterator =0;
}

void client::handshaking()//to calculate rtt
{
	char buf[1000];
	long long int y;
	int x;

	cout<<"handsahking:"<<endl;
	cout << "H_frame "<<":";
	int is_error=crc12(f_size);
	if(is_error==0)
	{
		string st1="ack";
	        //st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending ack" << endl;
	    	cout<<"buf:"<<buf<<endl;
		send(sockfd,buf,sizeof buf,0);
	}
	recv(sockfd,&y,sizeof y,0);
 	wait=ntohl(y);
	cout<<"wait:"<<wait;
	send_ack(1);
}

void client::send_ack(int frame_value)//sending ack/nack
{
	char buf[50];
	//int option=rand()%7 + 1;
	cout<<endl;
	cout<<endl;
	cout<<"iterator:" <<iterator<<endl;
   	cout << "frame " << frame_value<<":";
	int is_error=crc12(f_size);

	int choice=rand()%2;
	int f=frame_value%2;
	ostringstream oss;
	oss << f;
        iterator++;
	//int choice=0;
	//ostringstream oss;
	if(choice==0)//sending ack or delayed ack 
	{
	 cout<<"normal/delayed..."<<endl;
	 int wait_time=rand()%((wait+2)-wait)+wait;
	 if(is_error==0)//sending ack
	  {
	      int ch=rand()%2;
	      if(frame_value==1&&delayed_ack[0]<=max_dack)//if it is frame 1 send dack/nack
		{
		ch=1;
		delayed_ack[0]++;
		}
	      if(frame_value==2&&delayed_ack[1]<=max_dack)
		{
		ch=1;	
		delayed_ack[1]++;
		}
   	      if(ch==0)
	      {
		string st1="ack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending ack" << endl;
	    	cout<<"buf:"<<buf<<endl;
	    	frame_value++;
		send(sockfd,buf,sizeof buf,0);
	    /* send(sockfd,buf,sizeof buf,0);
		//string st="ack";
	      buf[0]='a';
	      buf[1]='c';
	      buf[2]='k';
		//st += oss.str();
		//strcpy(buf,st.c_str());
	      buf[3]=htonl(f);
	      buf[4]='\0';
	     //sleep(1);
	    // int temp=htons(0);*/
	     }
	    else if(ch==1)//sending delayed ack
	    {
	        usleep(wait_time);
	    //if(wait_time>50000)
	    
	       //oss << f;
		string st1="dack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending delayed ack" << endl;
	        cout<<"buf:"<<buf<<endl;
		send(sockfd,buf,sizeof buf,0);
	      /* buf[0]='d';
	      buf[1]='a';
	      buf[2]='c';
	      buf[3]='k';
	      buf[4]=htonl(f);
		buf[5]='\0';*/
	      
	   
	   }
	      //send(sockfd,buf,sizeof buf,0);

	    
	    if(frame_value<=no_of_frames)//transmitting ack for remaining frames
	    send_ack(frame_value);
	    else
	    {
		//sleep(10);
	    	return;
	    }
	  }
	if(is_error>=1)//sending nack
	  {
		int ch=rand()%2;
		if(frame_value==1&&delayed_ack[0]<=max_dack)//if it is frame 1 send dack/nack
		{
		ch=1;
		delayed_ack[0]++;
		}
	      if(frame_value==2&&delayed_ack[1]<=max_dack)
		{
		ch=1;	
		delayed_ack[1]++;
		}
		if(ch==0)//nack 
		{
	      //oss << f;
		string st1="nack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending nack" << endl;
	        cout<<"buf:"<<buf<<endl;
		send(sockfd,buf,sizeof buf,0);
	     /* buf[0]='n';
	      buf[1]='a';
	      buf[2]='c';
	      buf[3]='k';
	      buf[4]='\0';
  
	      //string st="nack";
	      //int temp=htons(1);*/
		}
		else if(ch==1)//delayed nack
		{
	         usleep(wait_time);
	     //if(wait_time>500000)
	     
	       //oss << f;
		 string st1="dnack";
	         st1 += oss.str();
		 strcpy(buf,st1.c_str());
	       /*buf[0]='d';
	      buf[1]='n';
	      buf[2]='a';
	      buf[3]='c';
	      buf[4]='k';
	      buf[5]='\0';*/
	        cout << "sending delayed nack" << endl;
		cout<<"buf:"<<buf<<endl;
		send(sockfd,buf,sizeof buf,0);
	     }
	   
	     // send(sockfd,buf,sizeof buf,0);
		
	        send_ack(frame_value);
	  }
	}
	
	else if(choice==1)//ack or nack drop
	{
	 if(is_error==0)
	   cout<<"ack drop..."<<endl;
	 else
	   cout<<"nack drop..."<<endl;
	   send_ack(frame_value);
	}
}

int client::crc12(int n)//crc calculation
{
	 int r[1000],divisor[1000],add=0,k=0,errmsg[1000],i,t,t1;
	//int n1;
	//recv( sockfd,&n1,sizeof n1,0 );
	//n=ntohs(n1);
	sleep(1);
	for(i=1;i<=n+12;i++)
	{
		
		recv(sockfd,&t,sizeof t,0);
		t1=ntohs(t);
		errmsg[i]=t1;
		cout << errmsg[i];
	}
	cout << endl;
	do
	{
	add=0;
		
		for(int i=1;i<=n+12;i++)
		divisor[i]=0;

		for(int i=1;i<=n+12;i++)
		{
		if(errmsg[i]==1)
		{
		k=i;
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
		if(divisor[i]==errmsg[i])
		r[i]=0;
		else
		r[i]=1;
		}
		

		for(int i=1;i<=n+12;i++)
		{
		errmsg[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}

		for(int i=1;i<=n+12;i++)
		divisor[i]=0;
	}while(add!=n);

	
	int w=0;
	for(int i=1;i<=n+12;i++)
	{
		
		if(r[i]!=0)
			w++;
	}
        //cout << endl;
	if(w==0)
		cout <<"no error in crc 12"<<endl;
	else
	 {
		cout<<"error in crc 12"<<endl;
		err_crc_12++;
	 }
	return w;



}




int main(int argc,char*argv[])
{
 int portno,choice;
 if(argc<3)//ip addr and port number will be passed during run time
 {
   	fprintf(stderr,"usage %s hostname port\n",argv[0]);
   	exit(0);
 }

 portno=atoi(argv[2]);
 
 int count=0,is_error;
 char buf[100];
 client client1(portno,argv[1]);
 client1.simulate();
  
return 0;
}
