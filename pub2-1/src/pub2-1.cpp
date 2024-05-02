#include "rclcpp/rclcpp.hpp"//ros2 client libarary 헤더파일 추가
#include "std_msgs/msg/string.hpp"//사용할 메세지 인터페이스 헤더파일 추가
#include "std_msgs/msg/int32.hpp"//사용할 메세지 인터페이스 헤더파일 추가
#include <memory>//스마트 포인터 사용하기 위한 헤더파일 추가
#include <chrono>//사용자 정의 리터럴을 사용하기 위한 헤더파일 추가
#include <functional>//function 객체 사용하기 위한 헤더파일 추가
//#include <string>
using namespace std::chrono_literals;//사용자 정의 리터럴 이름공간 사용
void callback(rclcpp::Node::SharedPtr node, rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub)//콜백함수 선언. 맴버변수로 노드와 퍼블리시 객체 받음
{
    static int count;//정적 정수형 변수 선언
    auto cnt = std_msgs::msg::Int32();//std_msgs::msg::Int32()인터페이스 사용하는 변수 선언
    cnt.data = count++;//생성한 메세지 인터페이스의 멤버변수 초기화
    RCLCPP_INFO(node->get_logger(), "Publish: %d", cnt.data);//RCLCPP_INFO를 이용해 printf나 cout과 같이 cnt 객체의 맴버변수 data 출력
    pub->publish(cnt);//메세지 인터페이스 객체 퍼블리시
    
}
int main(int argc, char* argv[])//메인 루틴 시작
{
    rclcpp::init(argc, argv);//ros2 초기화
    auto node = std::make_shared<rclcpp::Node>("mynode");//mynode 이름으로 노드 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));//전송 품질 설정
    auto pub = node->create_publisher<std_msgs::msg::Int32>("mytopic", qos_profile); 
    //노드 객체의 멤버함수 create_publisher 를 통해 퍼블리셔 생성. 퍼블리시 할 메세지 인터페이스는 std_msgs::msg::Int32. 퍼블리시 할 토픽 이름은 mytopic. 객체로 설정해둔 qos도 인자로 넣음
    std::function<void()> fn = std::bind(callback, node, pub);
    //create_wall_timer 함수의 2번째 매개변수는 반환형 void, 매개변수 없는 함수만 받을 수 있어 callback 함수를 bind 함수를 이용해 매개변수를 node와 pub로 묶어줌
    auto timer = node->create_wall_timer(1s, fn);//1초 주기로 바인드 된 콜백함수 fn을 호출
    rclcpp::spin(node);//node실행을 무한히 반복하고 이벤트를 체크하며 이벤트 발생시 콜백함수 호출. Ctrl+c 와 같은 종료 조건 발생하면 리턴
    rclcpp::shutdown();//ros2 작업 종료
    return 0;//0 반환
}//메인루틴 끝