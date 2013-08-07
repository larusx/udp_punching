//udp����ʾ���Զ���½������
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<iostream>
#include<vector>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
#define SERVER_IP "192.168.0.38"
#define SERVER_PORT 10000
//P2P��ַ
udp::endpoint p2p_cep;
//�ȴ����������ʹ���Ϣ
void punching(udp::socket &cssock/*�����˵�sock*/,udp::socket &ccsock/*P2Pͨ�ŵ�sock*/)
{
  //�ȴ�����������P2P�Զ˵�ַ
  char addr[20]={0};
  //����p2p����
  char p2p_buf[100];
  //�ӷ���������
  udp::endpoint sep;
  cssock.receive_from(buffer(addr),sep);
  //���յ�����Ϣ
  //Զ�˿ͻ���ַ
  udp::endpoint cep(address::from_string(addr),9999);
  p2p_cep=cep;
  //����Ϣ���ض����Է�NAT����
  ccsock.send_to(buffer(addr),cep);
  std::cout<<"�򶴽�������ʼ����P2P����"<<std::endl;
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
  //cssock�ͻ��������ͨ��
  udp::socket cssock(ios,udp::endpoint(udp::v4(),8888));
  //ccsock�ͻ�֮��P2Pͨ��
  udp::socket ccsock(ios,udp::endpoint(udp::v4(),9999));
  //server��ַ
  udp::endpoint sep(address::from_string(SERVER_IP),SERVER_PORT);
  //P2Pͨ�ŵĶԶ˵�
  udp::endpoint cep;
  //������
  system::error_code ec;
  //�ͻ��������ͨ�Ż�����
  //1Ϊ��½��0Ϊ���ߣ�2Ϊ�����
  char cs_buf[1];
  //������Ϣ
  char send_buf[100];
  //�Զ���½
  try{
  while(1)
  {
    std::cout<<"��¼�С�����"<<std::endl;
    cs_buf[0]=1;
    cssock.send_to(buffer(cs_buf,1),sep);
    cssock.receive_from(buffer(cs_buf),sep);
    //server����0��ʾ��½�ɹ�
    if(cs_buf[0] == 0)
      break;
  }
  std::cout<<"��½�ɹ�!"<<std::endl;
  thread thread_punching(bind(punching,ref(cssock),ref(ccsock)));
  thread_punching.detach();
  while(1)
  {
    std::cout<<"�����ӿͻ��˰�����������һ���ͻ��˴�"<<std::endl;
    getchar();
    cs_buf[0]=2;
    cssock.send_to(buffer(cs_buf,1),sep);
    while(1)
    {
      
      std::cout<<"���뷢����Ϣ"<<std::endl;
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