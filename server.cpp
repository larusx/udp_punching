//udp服务器
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<vector>
#include<string>
#include<iostream>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
#define SERVER_PORT 10000
class server{
public:
  server(io_service &ios):_ios(ios),
    sep(udp::v4(),SERVER_PORT),sock(ios,sep){
      sock.async_receive_from(buffer(data,length),cep,
        boost::bind(&server::handle_receive_from,this,placeholders::error,
        placeholders::bytes_transferred));
  }
  //添加连接客户端
  void add_client(udp::endpoint ep){
    nep.push_back(ep);
  }
  void handle_receive_from(const system::error_code &err,size_t bytes_recvd)
  {
    //接收一个字符，说明是控制信息，0为用户下线，1为用户登录，2为用户请求P2P通信
    if(!err && bytes_recvd == 1)
    {
      switch (data[0])
      {
      case 1:
        add_client(cep);
        data[0]=0;
        sock.send_to(buffer(data,1),cep);
        break;
      case 2:
        p2p_addr=nep.front().address().to_string();
        if(p2p_addr == cep.address().to_string())
          p2p_addr=nep.back().address().to_string();
        sock.send_to(buffer(p2p_addr),cep);
        std::cout<<p2p_addr<<std::endl;
        std::cout<<"发送给"<<cep.address()<<":"<<cep.port()<<std::endl;
        break;
      default:
        break;
      } 
    }
    sock.async_receive_from(buffer(data,length),cep,
      boost::bind(&server::handle_receive_from,this,placeholders::error,
      placeholders::bytes_transferred));
  }
private:
  io_service &_ios;
  //客户地址
  udp::endpoint cep;
  //存储客户地址
  std::vector<udp::endpoint> nep;
  //服务器地址
  udp::endpoint sep;
  udp::socket sock;
  enum{length=1};
  char data[length];
  //P2P地址
  std::string p2p_addr;
};
int main()
{
  io_service ios;
  try{
    server app(ios);
    ios.run();
  }
  catch (std::exception& e)
  {
    std::cout<<e.what()<<std::endl;
  }
  return 0; 
}
