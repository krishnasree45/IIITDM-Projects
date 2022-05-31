//selective reject receiver
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
#include <math.h>
using namespace std;

class client
{
 private:
	int sockfd,portno,f_size,no_of_frames,sws,rws,seqno,f_drop[100],rwin[50],filled[50],frame_value,wait,iterator,frame_error;
 	struct sockaddr_in serv_addr;
 	struct hostent *server;
 	char buffer[256];//this is used to take values from server and send values to the server
	int sws_ptr,msg[200][200];
	

 public:
	client(int portno,char *ser);
	void simulate();
	int err_crc_12;
	void init_rwin(int seq_no);
	void handshaking();
	void recv_frame(int frame_value);
	void send_ack_slw(int frame_value);
	void send_ack_sw(int f,int fill_value,int frame_value);//stop & wait sending ack/nack
	int checking(int msg[200][200],int loop,int rpointer);
	void check_rwin();
	int crc12(int msg[200][200],int n,int pp);
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

void client::simulate()// for receiving number of frames and frame size
{
	int x,is_error;
	char buf[50];

	recv(sockfd,&x,sizeof x,0);
 	no_of_frames=ntohs(x);

	recv(sockfd,&x,sizeof x,0);
 	f_size=ntohs(x);

	recv(sockfd,&x,sizeof x,0);
 	sws=ntohs(x);

	recv(sockfd,&x,sizeof x,0);
 	rws=ntohs(x);

	for(int i=0;i<rws;i++)
	  rwin[i]=0;

	frame_value=0;
	iterator=0;
	init_rwin(-1);
	handshaking();
}

void client::init_rwin(int seq_no)//iniatilising receiver window 
{
	for(int i=0;i<rws;i++)
	  {
	      rwin[i]=(seq_no+i+1)%(sws+rws);
	      filled[i]=0;// for checking frame is accepted or not 
	  }
}


void client::handshaking()// for calculating rtt
{
	char buf[1000];
	int hand[200][200];
	long long int y;
	int x;

	cout<<"handsahking:"<<endl;
	cout << "H_frame "<<":";
	for(int index=1;index<=f_size+12;index++)
	   {
		recv(sockfd,&x,sizeof x,0);
		hand[1][index]=ntohs(x);
		cout << hand[1][index];
           }
	cout << endl;
	int is_error=crc12(hand,(f_size+12),1);
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
	cout<<"wait:"<<wait<<endl;

	f_size=f_size+3;
	recv_frame(0);
}

void client::recv_frame(int frame_value)//receiving frames 
{
	iterator++;
	cout<<"\niterator:"<<iterator<<" send_ack_go back n"<<endl;
	char buf[50];
	int is_error,f;
	int check=0,x;
	
	sleep(1);
	cout << "framedrop:";
	for(int i=1;i<=sws+1;i++)   //receiving frame drop array
	  {
	     recv(sockfd,&x,sizeof x,0);
	     f_drop[i]=ntohs(x);
	     cout << f_drop[i] << " "; 
	  }
	cout << endl;
	sleep(1);
	for(int i=1;i<=sws;i++)	  //receiving frames
	    { 
		if(f_drop[i]==0 && (i)<=f_drop[sws+1])
		{
		cout<< "frame "<<i<<":";
		for(int index=1;index<=f_size+12;index++)
	   	 {
		     recv(sockfd,&x,sizeof x,0);
			msg[i][index]=ntohs(x);
			cout << msg[i][index];
		 }
		cout << endl;
		}
	    }
	sws_ptr=-1;
	send_ack_slw(frame_value);
}

void client::send_ack_sw(int f,int fill_value,int frame_value)//stop & wait sending ack/nack
{
	char buf[50];
	int msg_sw[200][200],check=0,x,is_error;
	int is_sw=0;
	iterator++;
	cout<<"\niterator:"<<iterator<<" send_ack stop & wait"<<endl;
	frame_error=0;
   	
	for(int index=1;index<=f_size+12;index++)
	   	 {
		     recv(sockfd,&x,sizeof x,0);
		     msg_sw[1][index]=ntohs(x);
		     cout << msg_sw[1][index];
		 }

		//check_rwin();
		if(filled[fill_value]==0)
		{
		
		/*if(sws_ptr<sws)
		{
		if(f_drop[((sws_ptr+1)%sws)]==1)
		  {
		      filled[fill_value]=0;
		      frame_error=1;
		  }
		else if(f_drop[((sws_ptr+1)%sws)]==0)
		  {*/
		     is_error=crc12(msg_sw,f_size,1);
		      if(is_error>=1)//if there is an error frame is not accepted
			{
			   filled[fill_value]=0;
			   frame_error=1;
			}
		      else if(is_error==0)// no error 
			{
			   int check=checking(msg_sw,1,fill_value);//whether it received the expecting frame 
		   	   cout<<fill_value+1<<" :check:"<<check<<endl;
			   if(check==1)//received expecting frame 
			     {
				filled[fill_value]=1;
				frame_value++;
			        frame_error=0;
			     }
			   else if(check==0)// not received expecting frame 
			     {
				filled[fill_value]=0;
				frame_error=1;
			     }
			}
		  }
		//sws_ptr=(sws_ptr+1);
		//}
		/*else if((frame_value%sws)==0)
		{
			goto label;
		   //recv_frame();
		}
		else
		  break;
		}*/

	
	/*int is_error=crc12(msg_sw,f_size,1);
	if(is_error==0)
	{
	  check=checking(msg_sw,1,fill_value);
	  if(check==0)
		frame_error=1;
	  else if(check==1)
		frame_error=0;
	}
	else if(is_error>=1)
	  frame_error=1;*/	
		
	cout << "fe_sw:" << frame_error << endl;
	int choice=rand()%2;
	//int f=frame_value%2;
	ostringstream oss;
	oss << f;
	if(choice==0)//sending ack or delayed ack 
	{
	 cout<<"normal/delayed..."<<endl;
	 int wait_time=rand()%((wait+2)-wait)+wait;
	 if(frame_error==0)//sending ack
	  {
	        for(int index=0;index<rws;index++)
	          {
			if(filled[index]==0)
			  {
			     fill_value=index;
			     f=rwin[index];
			     is_sw=1;
			     break;
			  }
		  }
		if(is_sw==0)
		  {
			check_rwin();
			f=rwin[0];
			fill_value=0;
		  }
		//int f1=frame_value%(sws+rws);
		ostringstream oss;
		oss << f;
	      int ch=rand()%2;
	      
   	      if(ch==0)//sending ack
	      {
		string st1="ack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending ack" << endl;
	    	cout<<"buf:"<<buf<<endl;
		/*if(filled[fill_value]==0)
		{
	    	frame_value++;
		filled[fill_value]=1;
		}*/
		send(sockfd,buf,sizeof buf,0);
		usleep((wait-10000));
		cout << "frame " << frame_value<<":"<<endl;
		recv_frame(frame_value);
	     }
	    else if(ch==1)//sending delayed ack
	    {
	        usleep(wait_time);
	    
		string st1="dack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending delayed ack" << endl;
	        cout<<"buf:"<<buf<<endl;
		/*if(filled[fill_value]==0)
		{
		frame_value++;
		filled[fill_value]=1;
		}*/
		send(sockfd,buf,sizeof buf,0);
	        send_ack_sw(f,fill_value,frame_value);
	   }
	  }
	if(frame_error>=1)//sending nack
	  {
		ostringstream oss;
		oss << f;
		int ch=rand()%2;
		if(ch==0)//nack 
		{
	      //oss << f;
		string st1="nack";
	        st1 += oss.str();
		strcpy(buf,st1.c_str());
		cout << "sending nack" << endl;
	        cout<<"buf:"<<buf<<endl;
		usleep((wait-10000));
		send(sockfd,buf,sizeof buf,0);
		}
		else if(ch==1)//delayed nack
		{
	         usleep(wait_time);
		 string st1="nack";
	         st1 += oss.str();
		 strcpy(buf,st1.c_str());
	        cout << "sending delayed nack" << endl;
		cout<<"buf:"<<buf<<endl;
		send(sockfd,buf,sizeof buf,0);
	     }
	   
	     
		send_ack_sw(f,fill_value,frame_value);//receiving frames  by stop and wait 
	  }
	}
	
	else if(choice==1)//ack or nack drop
	{
	   if(frame_error==0)//ack drop
	    {
	      for(int index=0;index<rws;index++)
	          {
			if(filled[index]==0)
			  {
			     fill_value=index;
			     f=rwin[index];
			     is_sw=1;
			     break;
			  }
		  }
		if(is_sw==0)//updating rwin
		  {
			check_rwin();
			f=rwin[0];
			fill_value=0;
		  }
	      cout<<"ack"<< f << " drop..."<<endl;
	    } 
	   else if(frame_error>=1)//nack drop
	     cout<<"nack"<< f << " drop..."<<endl;
	  usleep((wait-10000));
	 send_ack_sw(f,fill_value,frame_value);
	}
}



void client::send_ack_slw(int frame_value)//sending ack by go back n
{
	//iterator++;
	//cout<<"\niterator:"<<iterator<<" send_ack_go back n"<<endl;
	
	char buf[50];
	int is_error,f;
	int check=0,x;
	
	frame_error=0;
	for(int i=0;i<rws;i++)
	    {
		check_rwin();
		sws_ptr=(sws_ptr+1);//maintaing ptr for sender window 
		if(filled[i]==0)
		{
		
		if(sws_ptr<sws)
		{
		if(f_drop[((sws_ptr+1)%(sws+rws))]==1)
		  {
		      filled[i]=0;
		      frame_error=1;
		  }
		else if(f_drop[((sws_ptr+1)%(sws+rws))]==0)
		  {
		     is_error=crc12(msg,f_size,((sws_ptr+1)%(sws+rws)));//crc check
		      if(is_error>=1)//there exists an error 
			{
			   filled[i]=0;
			   frame_error=1;
			}
		      else if(is_error==0)// no error 
			{
			   int check=checking(msg,((sws_ptr+1)%(sws+rws)),i);// checking whether it received expecting frame
		   	   cout<<i+1<<" :check:"<<check<<endl;
			   if(check==1)
			     {
				filled[i]=1;
				frame_value++;
				frame_error=0;
			     }
			   else if(check==0)
			     {
				filled[i]=0;
				frame_error=1;
			     }
			}
		  }
		//sws_ptr=(sws_ptr+1);
		}
		else if((frame_value%sws)==0)
		{
			goto label;
		   //recv_frame();
		}
		else
		  break;
		}
		
	    }

	cout << "frame value:" << frame_value<< endl;

	/*int sum=0;
	 for(int i=0;i<sws;i++)
         { 
	    sum=sum+is_error[i];
         }
	if(sum>=1)	
	    frame_error=1;
        else
	   frame_error=0;*/


	cout<<"frame_error :"<<frame_error<<endl;


	label:
	int choice=rand()%2;
	cout<<"choice:"<<choice<<endl;
	if(choice==0)		//normal ack,nack or delayed ack,nack
	{
	  int wait_time=rand()%((wait+5)-wait)+wait;
	
	  if(frame_error==0)
	  {
		cout<<"normal/delayed..."<<endl;
	 	int dn=rand()%2;
		ostringstream oss;
		    //cout << "rfv:"<< frame_value<<endl;
		f=frame_value%(sws+rws);
		if((frame_value%sws)==0)
		{
		  oss<<f;
		 if(dn==0)//ack 
		  {
			string st1="ack";
			st1 += oss.str();
			strcpy(buf,st1.c_str());
			cout << "sending ack..." << endl;
			cout << "buf" << buf << endl;
		        send(sockfd,buf,sizeof buf,0);
			usleep((wait-10000));
			recv_frame(frame_value);
	          }
	    	else if(dn==1)//delayed ack 
	   	  {
			usleep(wait_time);
			string st1="dack";
			st1 += oss.str();
			strcpy(buf,st1.c_str());
			cout << "sending delayed ack..." << endl;
			cout << "buf" << buf << endl;
		        send(sockfd,buf,sizeof buf,0);
			recv_frame(frame_value);
	  	  }
	       }
	      else
		{
		cout << "back..."<<endl;
		send_ack_slw(frame_value);
		return;
		}
  
	  //cout<<"frame_vali,no_fram"<<frame_value<<" "<<no_of_frames<<endl;
	    //if(frame_value<no_of_frames)
	      
	    //else
	    {
	    	//return;
	    }
	  }
	if(frame_error>=1)//there exists an error 
	  {
		int fill_value;
		int dn=rand()%2;
		for(int i=0;i<rws;i++)
		  {
		      if(filled[i]==0)
			{
			   f=rwin[i];
			   fill_value=i;
			   break;
			}
		  }
		ostringstream oss;	
		oss<<f;
		if(dn==0)//sending nack  and then moves to stop and wait 
		{
			string st1="nack";
			st1 += oss.str();
			strcpy(buf,st1.c_str());
			cout << "sending nack..." << endl;
			cout << "buf:"<< buf << endl;
		        send(sockfd,buf,sizeof buf,0);
			usleep((wait-10000));
			send_ack_sw(f,fill_value,frame_value);
		}
		else if(dn==1)//sending dnack and then moves to go back n
		{
		    	usleep(wait_time);
		     	string st1="dnack";
			st1 += oss.str();
			strcpy(buf,st1.c_str());
			cout << "sending delayed nack..." << endl;
			cout << "buf:"<< buf << endl;
		        send(sockfd,buf,sizeof buf,0);
			recv_frame(frame_value);
		}
	       
	      
	   }
	}
	else if(choice==1)//for ack/nack drops
	{
	   f=frame_value%(sws+rws);
	   if(frame_error==0)
	    {
		if(frame_value%sws==0)
	        {
	          cout<<"ack"<< f << " drop..."<<endl;
		  usleep((wait-10000));
		  recv_frame(frame_value);
		}
		else
		{
		    cout << "back..."<<endl;
		    send_ack_slw(frame_value);
			return;
		}
	   }
	   else
	   {
	     cout<<"nack"<< f << " drop..."<<endl;
	     usleep((wait-10000));
	     recv_frame(frame_value);
	  }
	   
	}
	
}

int client::checking(int msg[200][200],int loop,int rpointer)//fro checking it received expecting frame 
  {
	int sum=0;
	int ch[10];
	int check;
		
		int index=(f_size-2);
		for( int j=1,i=1;j<=3;j++)
		{
		   ch[j]=msg[loop][index];
		   i++;
		   index++;
		}

		 //for(int i=1;i<=3;i++)
		 //cout << "Ch[i]:   " << ch[i]<<endl;
	
		for(int i=3,j=0;i>=1;i--,j++)
		{
			sum+=ch[i]*pow(2,j);
		}
		//cout<<"sum:"<<sum<<endl;
		cout<<"rwin:"<<rwin[rpointer]<<endl;

		if(sum==rwin[rpointer])
		  check=1;
		else
		   check=0;
		//cout<<"check:"<<check<<endl;
		return check;
  }

void client::check_rwin()//updating rwin
{
	int sum=0;
	for(int i=0;i<rws;i++)
	  {
	      sum=sum+filled[i];
	  }
	if(sum==rws)
	  init_rwin(rwin[rws-1]);
	else
	  return;
}

int client::crc12(int msg[200][200],int n,int pp)//crc check 
{
	 int r[200],divisor[200],add=0,k=0,errmsg[200],i,t,t1;
	for(i=1;i<=n+12;i++)
	{
		errmsg[i]=msg[pp][i];
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
