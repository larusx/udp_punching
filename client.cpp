//udp打洞演示，自动登陆服务器
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<iostream>
#include<vector>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
#define SERVER_IP "192.168.0.38"
#define SERVER_PORT 10000
//P2P地址
udp::endpoint p2p_cep;
//等待服务器发送打洞信息
void punching(udp::socket &cssock/*与服务端的sock*/,udp::socket &ccsock/*P2P通信的sock*/)
{
  //等待服务器发来P2P对端地址
  char addr[20]={0};
  //接收p2p数据
  char p2p_buf[100];
  //从服务器接收
  udp::endpoint sep;
  cssock.receive_from(buffer(addr),sep);
  //接收到打洞信息
  //远端客户地址
  udp::endpoint cep(address::from_string(addr),9999);
  p2p_cep=cep;
  //打洞信息，必定被对方NAT拦截
  ccsock.send_to(buffer(addr),cep);
  std::cout<<"打洞结束，开始接收P2P数据"<<std::endl;
  while (1)
  {
    ccsock.receive_from(buffer(p2p_buf),cep);
    std::cout<<p2p_buf<<std::endl;
  }
  
}
int main()
{
  std::cout<<"udp server start."<<std::endl;
  io_service ios;
  //cssock客户与服务器通信
  udp::socket cssock(ios,udp::endpoint(udp::v4(),8888));
  //ccsock客户之间P2P通信
  udp::socket ccsock(ios,udp::endpoint(udp::v4(),9999));
  //server地址
  udp::endpoint sep(address::from_string(SERVER_IP),SERVER_PORT);
  //P2P通信的对端点
  udp::endpoint cep;
  //错误码
  system::error_code ec;
  //客户与服务器通信缓冲区
  //1为登陆，0为下线，2为请求打洞
  char cs_buf[1];
  //发送信息
  char send_buf[100];
  //自动登陆
  try{
  while(1)
  {
    std::cout<<"登录中。。。"<<std::endl;
    cs_buf[0]=1;
    cssock.send_to(buffer(cs_buf,1),sep);
    cssock.receive_from(buffer(cs_buf),sep);
    //server返回0表示登陆成功
    if(cs_buf[0] == 0)
      break;
  }
  std::cout<<"登陆成功!"<<std::endl;
  thread thread_punching(bind(punching,ref(cssock),ref(ccsock)));
  thread_punching.detach();
  while(1)
  {
    std::cout<<"后连接客户端按任意键请求第一个客户端打洞"<<std::endl;
    getchar();
    cs_buf[0]=2;
    cssock.send_to(buffer(cs_buf,1),sep);
    while(1)
    {
      
      std::cout<<"输入发送信息"<<std::endl;
      std::cin>>send_buf;
      ccsock.send_to(buffer(send_buf,100),p2p_cep);
    }

  }
  }//end try
  catch(std::exception &e)
  {
    std::cout<<e.what()<<std::endl;
  }//end catch

}